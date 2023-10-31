/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-config.h"
#include "gfls-loader-basic.h"
#include <glib/gi18n-lib.h>
#include "gfls-input-stream.h"
#include "gfls-utf8.h"

/**
 * SECTION:gfls-loader-basic
 * @Title: GflsLoaderBasic
 * @Short_description: Basic file loader
 *
 * Basic file loader.
 */

/* When the file size is unknown, start with 8 KiB. */
#define DEFAULT_FILE_SIZE (8192)

typedef struct
{
	/* Config */
	gsize max_size;
	guint max_n_bytes_per_line;

	/* Intermediate data */
	gsize expected_file_size;
	// TODO: close explicitly the stream? to get a GError if any.
	// With g_input_stream_close_async/finish().
	// Do it here? Or in gfls_input_stream_read_async/finish(), since it is
	// meant as the only read operation?
	GInputStream *input_stream;

	/* Result */
	GBytes *bytes;
} TaskData;

GQuark
gfls_loader_error_quark (void)
{
	static GQuark quark = 0;

	if (G_UNLIKELY (quark == 0))
	{
		quark = g_quark_from_static_string ("gfls-loader-error");
	}

	return quark;
}

static TaskData *
task_data_new (gsize max_size,
	       guint max_n_bytes_per_line)
{
	TaskData *task_data;

	task_data = g_new0 (TaskData, 1);
	task_data->max_size = max_size;
	task_data->max_n_bytes_per_line = max_n_bytes_per_line;

	return task_data;
}

static void
task_data_free (TaskData *task_data)
{
	if (task_data != NULL)
	{
		g_clear_object (&task_data->input_stream);
		g_clear_pointer (&task_data->bytes, g_bytes_unref);

		g_free (task_data);
	}
}

static void
check_bytes (GTask *task)
{
	TaskData *task_data;
	const gchar *text;
	gsize n_bytes;

	task_data = g_task_get_task_data (task);

	text = g_bytes_get_data (task_data->bytes, &n_bytes);

	if (!g_utf8_validate_len (text, n_bytes, NULL))
	{
		/* TODO: have our own GError domain. */
		g_task_return_new_error (task,
					 G_IO_ERROR,
					 G_IO_ERROR_FAILED,
					 _("The file is not encoded in UTF-8."));
		g_object_unref (task);
		return;
	}

	if (gfls_utf8_has_very_long_line (text, task_data->max_n_bytes_per_line))
	{
		/* TODO: have our own GError domain. */
		g_task_return_new_error (task,
					 G_IO_ERROR,
					 G_IO_ERROR_FAILED,
					 _("The file contains a very long line."));
		g_object_unref (task);
		return;
	}

	/* Everything OK, finally done. */
	g_task_return_boolean (task, TRUE);
	g_object_unref (task);
}

static void
read_input_stream_cb (GObject      *source_object,
		      GAsyncResult *result,
		      gpointer      user_data)
{
	GInputStream *input_stream = G_INPUT_STREAM (source_object);
	GTask *task = G_TASK (user_data);
	TaskData *task_data;
	gboolean is_truncated = FALSE;
	GError *error = NULL;

	task_data = g_task_get_task_data (task);

	g_clear_pointer (&task_data->bytes, g_bytes_unref);
	task_data->bytes = gfls_input_stream_read_finish (input_stream,
							  result,
							  &is_truncated,
							  &error);

	if (error != NULL)
	{
		g_task_return_error (task, error);
		g_object_unref (task);
		return;
	}

	if (is_truncated)
	{
		/* Free memory as early as possible, especially because it
		 * reached the maximum.
		 */
		g_clear_pointer (&task_data->bytes, g_bytes_unref);

		/* TODO: have our own GError domain. */
		g_task_return_new_error (task,
					 G_IO_ERROR,
					 G_IO_ERROR_FAILED,
					 _("Limit on the number of bytes to read reached."));
		g_object_unref (task);
		return;
	}

	check_bytes (task);
}

static void
read_input_stream (GTask *task)
{
	TaskData *task_data = g_task_get_task_data (task);

	gfls_input_stream_read_async (task_data->input_stream,
				      task_data->expected_file_size,
				      task_data->max_size,
				      g_task_get_priority (task),
				      g_task_get_cancellable (task),
				      read_input_stream_cb,
				      task);
}

static void
open_file_cb (GObject      *source_object,
	      GAsyncResult *result,
	      gpointer      user_data)
{
	GFile *file = G_FILE (source_object);
	GTask *task = G_TASK (user_data);
	GFileInputStream *file_input_stream;
	TaskData *task_data;
	GError *error = NULL;

	file_input_stream = g_file_read_finish (file, result, &error);

	if (error != NULL)
	{
		g_task_return_error (task, error);
		g_object_unref (task);
		g_clear_object (&file_input_stream);
		return;
	}

	task_data = g_task_get_task_data (task);

	g_clear_object (&task_data->input_stream);
	task_data->input_stream = G_INPUT_STREAM (file_input_stream);

	read_input_stream (task);
}

static void
open_file (GTask *task)
{
	GFile *file = g_task_get_source_object (task);

	g_file_read_async (file,
			   g_task_get_priority (task),
			   g_task_get_cancellable (task),
			   open_file_cb,
			   task);
}

static void
query_file_info_cb (GObject      *source_object,
		    GAsyncResult *result,
		    gpointer      user_data)
{
	GFile *file = G_FILE (source_object);
	GTask *task = G_TASK (user_data);
	TaskData *task_data;
	GFileInfo *info;
	GError *error = NULL;

	info = g_file_query_info_finish (file, result, &error);

	if (error != NULL)
	{
		g_task_return_error (task, error);
		g_object_unref (task);
		g_clear_object (&info);
		return;
	}

	task_data = g_task_get_task_data (task);

	if (g_file_info_has_attribute (info, G_FILE_ATTRIBUTE_STANDARD_SIZE))
	{
		task_data->expected_file_size = g_file_info_get_size (info);

		if (task_data->expected_file_size > task_data->max_size)
		{
			/* TODO: have our own GError domain. */
			g_task_return_new_error (task,
						 G_IO_ERROR,
						 G_IO_ERROR_FAILED,
						 _("The size of the file is too big."));
			g_object_unref (task);
			g_clear_object (&info);
			return;
		}
	}
	else
	{
		task_data->expected_file_size = DEFAULT_FILE_SIZE;
	}

	open_file (task);

	g_clear_object (&info);
}

static void
query_file_info (GTask *task)
{
	GFile *file = g_task_get_source_object (task);

	g_file_query_info_async (file,
				 G_FILE_ATTRIBUTE_STANDARD_SIZE,
				 G_FILE_QUERY_INFO_NONE,
				 g_task_get_priority (task),
				 g_task_get_cancellable (task),
				 query_file_info_cb,
				 task);
}

/**
 * gfls_loader_basic_load_async:
 * @file: a #GFile.
 * @max_size: the maximum allowed number of bytes in total.
 * @max_n_bytes_per_line: the maximum allowed number of bytes per line, as per
 *   gfls_utf8_has_very_long_line().
 * @io_priority: the I/O priority of the request. E.g. %G_PRIORITY_LOW,
 *   %G_PRIORITY_DEFAULT or %G_PRIORITY_HIGH.
 * @cancellable: (nullable): optional #GCancellable object, %NULL to ignore.
 * @callback: (scope async): a #GAsyncReadyCallback to call when the operation
 *   is finished.
 * @user_data: user data to pass to @callback.
 *
 * Starts a basic file loading operation.
 *
 * If the @file content is not a valid UTF-8 string, or if the @max_size or
 * @max_n_bytes_per_line conditions are not satisfied, an error will be returned
 * without the file content.
 *
 * See the #GAsyncResult documentation to know how to use this function.
 *
 * Since: 0.1
 */
void
gfls_loader_basic_load_async (GFile               *file,
			      gsize                max_size,
			      guint                max_n_bytes_per_line,
			      gint                 io_priority,
			      GCancellable        *cancellable,
			      GAsyncReadyCallback  callback,
			      gpointer             user_data)
{
	GTask *task;
	TaskData *task_data;

	g_return_if_fail (G_IS_FILE (file));
	g_return_if_fail (cancellable == NULL || G_IS_CANCELLABLE (cancellable));

	task = g_task_new (file, cancellable, callback, user_data);
	g_task_set_priority (task, io_priority);

	task_data = task_data_new (max_size, max_n_bytes_per_line);
	g_task_set_task_data (task, task_data, (GDestroyNotify) task_data_free);

	query_file_info (task);
}

/**
 * gfls_loader_basic_load_finish:
 * @file: a #GFile.
 * @result: a #GAsyncResult.
 * @error: a #GError, or %NULL.
 *
 * Finishes an operation started with gfls_loader_basic_load_async().
 *
 * If everything went well, a #GBytes with the #GFile content (unmodified) is
 * returned. It is guaranteed to be a valid UTF-8 string.
 *
 * The data contained in the resulting #GBytes is always zero-terminated, but
 * this is not included in the #GBytes length. The resulting #GBytes should be
 * freed with g_bytes_unref() when no longer in use.
 *
 * Returns: a #GBytes, or %NULL on error.
 * Since: 0.1
 */
GBytes *
gfls_loader_basic_load_finish (GFile         *file,
			       GAsyncResult  *result,
			       GError       **error)
{
	gboolean ok;

	g_return_val_if_fail (G_IS_FILE (file), NULL);
	g_return_val_if_fail (g_task_is_valid (result, file), NULL);
	g_return_val_if_fail (error == NULL || *error == NULL, NULL);

	ok = g_task_propagate_boolean (G_TASK (result), error);

	if (ok)
	{
		TaskData *task_data;
		GBytes *bytes;

		task_data = g_task_get_task_data (G_TASK (result));

		bytes = task_data->bytes;
		task_data->bytes = NULL;

		return bytes;
	}

	return NULL;
}
