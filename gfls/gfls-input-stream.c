/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-input-stream.h"

/**
 * SECTION:gfls-input-stream
 * @Title: GflsInputStream
 * @Short_description: Additional functions for #GInputStream
 *
 * Additional functions for #GInputStream.
 */

/* Problem with g_file_load_partial_contents_async/finish():
 *
 * From an API point of view, the maximum number of bytes to read is not
 * provided *initially*, the GFileReadMoreCallback is called at least once. For
 * example if I try to load a 100MB file with that function, nothing tells me
 * that I'll be able to restrict it to 20MB (the API doesn't guarantee it).
 *
 * gfls_input_stream_read_async/finish() solves this (and works on any
 * GInputStream, which we need for stdin support).
 */

typedef struct
{
	/* Buffer to store the bytes read. It needs to contain one more byte at
	 * the end to nul-terminate the string. Note that it can contain nul
	 * bytes inside the content too (whatever is read from the input
	 * stream).
	 */
	guint8 *buf;

	/* The number of bytes currently allocated to @buf. */
	gsize buf_size;

	/* The current position inside @buf where to write the next bytes.
	 * Invariant: buf_pos < buf_size
	 */
	gsize buf_pos;

	/* Max value for @buf_size. */
	gsize buf_max_size;

	/* Boolean to know if the content from the input stream needed to be
	 * truncated because the maximum number of bytes to read has been
	 * reached (and there *is* more content to read).
	 */
	guint truncated : 1;
} TaskData;

/* 8 KiB, a value recommended by g_input_stream_read_bytes(). */
#define CHUNK_SIZE (8192)

static void read_next_chunk (GTask *task);

static TaskData *
task_data_new (gsize expected_size,
	       gsize max_size)
{
	TaskData *task_data;
	gsize initial_buf_size;

	/* +1 to nul-terminate the buffer. */
	initial_buf_size = MIN (expected_size + 1, max_size + 1);

	task_data = g_new (TaskData, 1);
	task_data->buf = g_malloc (initial_buf_size);
	task_data->buf_size = initial_buf_size;
	task_data->buf_pos = 0;
	task_data->buf_max_size = max_size + 1;
	task_data->truncated = FALSE;

	return task_data;
}

static void
task_data_free (TaskData *task_data)
{
	if (task_data != NULL)
	{
		g_free (task_data->buf);
		g_free (task_data);
	}
}

static gboolean
task_data_max_reached (TaskData *task_data)
{
	g_assert (task_data->buf_pos < task_data->buf_max_size);

	/* When this is true, it's still possible to nul-terminate the buffer. */
	return task_data->buf_pos == task_data->buf_max_size - 1;
}

/* Returns: the new value for @n_bytes_to_write. */
static gsize
task_data_prepare_buffer (TaskData *task_data,
			  gsize     n_bytes_to_write)
{
	gsize required_buf_size = task_data->buf_pos + n_bytes_to_write + 1;

	if (required_buf_size <= task_data->buf_size)
	{
		return n_bytes_to_write;
	}

	/* Needs to realloc the buffer */

	if (required_buf_size <= task_data->buf_max_size)
	{
		task_data->buf = g_realloc (task_data->buf, required_buf_size);
		task_data->buf_size = required_buf_size;
		return n_bytes_to_write;
	}

	task_data->buf = g_realloc (task_data->buf, task_data->buf_max_size);
	task_data->buf_size = task_data->buf_max_size;
	return task_data->buf_size - task_data->buf_pos - 1;
}

static void
task_data_nul_terminate_buffer (TaskData *task_data)
{
	task_data->buf[task_data->buf_pos] = '\0';
}

static GBytes *
task_data_buffer_to_gbytes (TaskData *task_data)
{
	GBytes *bytes;

	bytes = g_bytes_new_take (task_data->buf, task_data->buf_pos);
	task_data->buf = NULL;

	return bytes;
}

static void
read_past_max_cb (GObject      *source_object,
		  GAsyncResult *result,
		  gpointer      user_data)
{
	GInputStream *input_stream = G_INPUT_STREAM (source_object);
	GTask *task = user_data;
	TaskData *task_data;
	GBytes *bytes;
	GError *error = NULL;

	bytes = g_input_stream_read_bytes_finish (input_stream, result, &error);

	if (error != NULL)
	{
		g_task_return_error (task, error);
		g_object_unref (task);
		return;
	}

	task_data = g_task_get_task_data (task);

	if (g_bytes_get_size (bytes) > 0)
	{
		task_data->truncated = TRUE;
	}

	g_bytes_unref (bytes);

	task_data_nul_terminate_buffer (task_data);
	g_task_return_boolean (task, TRUE);
	g_object_unref (task);
}

static void
read_past_max (GTask *task)
{
	GInputStream *input_stream;

	input_stream = g_task_get_source_object (task);

	g_input_stream_read_bytes_async (input_stream,
					 1,
					 g_task_get_priority (task),
					 g_task_get_cancellable (task),
					 read_past_max_cb,
					 task);
}

static void
read_next_chunk_cb (GObject      *source_object,
		    GAsyncResult *result,
		    gpointer      user_data)
{
	GInputStream *input_stream = G_INPUT_STREAM (source_object);
	GTask *task = user_data;
	GError *error = NULL;
	gssize n_bytes_read;
	TaskData *task_data;

	n_bytes_read = g_input_stream_read_finish (input_stream, result, &error);

	if (error != NULL)
	{
		g_task_return_error (task, error);
		g_object_unref (task);
		return;
	}

	if (n_bytes_read < 0)
	{
		/* Should not happen, already handled with error != NULL, but
		 * have robust code.
		 */
		g_task_return_boolean (task, FALSE);
		g_object_unref (task);
		return;
	}

	task_data = g_task_get_task_data (task);

	if (n_bytes_read == 0)
	{
		task_data_nul_terminate_buffer (task_data);
		g_task_return_boolean (task, TRUE);
		g_object_unref (task);
		return;
	}

	task_data->buf_pos += n_bytes_read;

	read_next_chunk (task);
}

static void
read_next_chunk (GTask *task)
{
	GInputStream *input_stream;
	TaskData *task_data;
	gsize actual_size_to_read;

	input_stream = g_task_get_source_object (task);
	task_data = g_task_get_task_data (task);

	if (task_data_max_reached (task_data))
	{
		read_past_max (task);
		return;
	}

	actual_size_to_read = task_data_prepare_buffer (task_data, CHUNK_SIZE);

	g_input_stream_read_async (input_stream,
				   task_data->buf + task_data->buf_pos,
				   actual_size_to_read,
				   g_task_get_priority (task),
				   g_task_get_cancellable (task),
				   read_next_chunk_cb,
				   task);
}

/**
 * gfls_input_stream_read_async:
 * @input_stream: a #GInputStream.
 * @expected_size: the expected number of bytes contained in @input_stream.
 * @max_size: the maximum number of bytes to read.
 * @io_priority: the I/O priority of the request. E.g. %G_PRIORITY_LOW,
 *   %G_PRIORITY_DEFAULT or %G_PRIORITY_HIGH.
 * @cancellable: (nullable): optional #GCancellable object, %NULL to ignore.
 * @callback: (scope async): a #GAsyncReadyCallback to call when the operation
 *   is finished.
 * @user_data: user data to pass to @callback.
 *
 * This function starts a read operation on @input_stream. It is meant to be
 * used as the only read operation on @input_stream, to get a #GBytes as a
 * result, with @max_size as the provided maximum number of bytes to read.
 *
 * @expected_size is typically a #GFile size as returned by
 * g_file_info_get_size(). But note that in that case, the returned #GBytes may
 * contain a different number of bytes than what was expected (the
 * TOC/TOU problem: time of check to time of use). @expected_size is used as an
 * indication to how much memory to allocate initially.
 *
 * See the #GAsyncResult documentation to know how to use this function.
 *
 * Since: 0.1
 */
void
gfls_input_stream_read_async (GInputStream        *input_stream,
			      gsize                expected_size,
			      gsize                max_size,
			      gint                 io_priority,
			      GCancellable        *cancellable,
			      GAsyncReadyCallback  callback,
			      gpointer             user_data)
{
	GTask *task;
	TaskData *task_data;

	g_return_if_fail (G_IS_INPUT_STREAM (input_stream));
	g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));

	task = g_task_new (input_stream, cancellable, callback, user_data);
	g_task_set_priority (task, io_priority);

	task_data = task_data_new (expected_size, max_size);
	g_task_set_task_data (task, task_data, (GDestroyNotify) task_data_free);

	read_next_chunk (task);
}

/**
 * gfls_input_stream_read_finish:
 * @input_stream: a #GInputStream.
 * @result: a #GAsyncResult.
 * @is_truncated: will be set to %TRUE if the @input_stream contains more data
 *   to be read, but the maximum number of bytes to read has been reached.
 * @error: a #GError, or %NULL.
 *
 * Finishes an operation started with gfls_input_stream_read_async().
 *
 * The data contained in the resulting #GBytes is always zero-terminated, but
 * this is not included in the #GBytes length. The resulting #GBytes should be
 * freed with g_bytes_unref() when no longer in use.
 *
 * Returns: a #GBytes, or %NULL on error.
 * Since: 0.1
 */
GBytes *
gfls_input_stream_read_finish (GInputStream  *input_stream,
			       GAsyncResult  *result,
			       gboolean      *is_truncated,
			       GError       **error)
{
	gboolean ok;

	g_return_val_if_fail (G_IS_INPUT_STREAM (input_stream), NULL);
	g_return_val_if_fail (g_task_is_valid (result, input_stream), NULL);
	g_return_val_if_fail (is_truncated != NULL, NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ok = g_task_propagate_boolean (G_TASK (result), error);

	if (ok)
	{
		TaskData *task_data;

		task_data = g_task_get_task_data (G_TASK (result));
		*is_truncated = task_data->truncated;

		return task_data_buffer_to_gbytes (task_data);
	}

	*is_truncated = FALSE;
	return NULL;
}
