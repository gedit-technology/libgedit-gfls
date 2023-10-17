/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gio/gio.h>
#include <locale.h>

/* Purpose of this test program
 * ============================
 *
 * To test the following race condition:
 * (1) Read a first block of bytes from a file (with GIO).
 * (2) Launch a timeout function (long enough to manually alter the file).
 * (3) With another program, alter the content of the file. E.g. with:
 *     $ echo foobar > file
 * (4) When the timeout callback function is called, continue to read the file.
 *
 * And see what happens, how the programs behave, if errors are reported, etc.
 *
 * Result
 * ------
 *
 * Absolutely no errors are reported. Even when closing the GFileInputStream.
 *
 * Solution
 * --------
 *
 * Rely on the etag values provided by GIO to know if the file has been
 * externally modified.
 */

/* Read a string of 80 chars and nul-terminate it.
 * This is for testing purposes, so a short value is chosen.
 */
#define BUFFER_SIZE (81)

typedef struct
{
	GFile *file;
	GFileInputStream *input_stream;
	guint8 buffer[BUFFER_SIZE];
} ProgramData;

static void read_next_chunk (ProgramData *program_data);

static ProgramData *
program_data_new (void)
{
	ProgramData *program_data;

	program_data = g_new (ProgramData, 1);
	program_data->file = NULL;
	program_data->input_stream = NULL;

	/* Don't zero the buffer, it will anyway be nul-terminated manually. */

	return program_data;
}

static void
program_data_free (ProgramData *program_data)
{
	if (program_data != NULL)
	{
		g_clear_object (&program_data->file);
		g_clear_object (&program_data->input_stream);
		g_free (program_data);
	}
}

static void
quit_program (void)
{
	g_application_release (g_application_get_default ());
}

static void
close_file_input_stream (ProgramData *program_data)
{
	GError *error = NULL;

	g_print ("Closing file input stream.\n");

	g_input_stream_close (G_INPUT_STREAM (program_data->input_stream), NULL, &error);

	if (error != NULL)
	{
		g_printerr ("Failed to close the file input stream: %s\n", error->message);
		g_clear_error (&error);
	}
}

static gboolean
timeout_cb (gpointer user_data)
{
	ProgramData *program_data = user_data;

	read_next_chunk (program_data);

	return G_SOURCE_REMOVE;
}

static void
read_next_chunk_cb (GObject      *source_object,
		    GAsyncResult *result,
		    gpointer      user_data)
{
	ProgramData *program_data = user_data;
	GError *error = NULL;
	gssize n_bytes_read;

	n_bytes_read = g_input_stream_read_finish (G_INPUT_STREAM (program_data->input_stream), result, &error);

	if (error != NULL)
	{
		g_printerr ("Failed to read the next chunk of content: %s\n", error->message);
		g_clear_error (&error);
		quit_program ();
		return;
	}

	if (n_bytes_read == 0)
	{
		g_print ("Finished reading.\n");
		close_file_input_stream (program_data);
		quit_program ();
		return;
	}

	g_assert (n_bytes_read > 0);
	g_assert (n_bytes_read < BUFFER_SIZE);

	program_data->buffer[n_bytes_read] = '\0';
	g_print ("Next chunk read:\n%s\n", program_data->buffer);

	g_timeout_add_seconds (15, timeout_cb, program_data);
}

static void
read_next_chunk (ProgramData *program_data)
{
	g_input_stream_read_async (G_INPUT_STREAM (program_data->input_stream),
				   program_data->buffer,
				   BUFFER_SIZE - 1, /* -1 to keep one byte to nul-terminate the string. */
				   G_PRIORITY_DEFAULT,
				   NULL,
				   read_next_chunk_cb,
				   program_data);
}

static void
file_read_cb (GObject      *source_object,
	      GAsyncResult *result,
	      gpointer      user_data)
{
	GFile *file = G_FILE (source_object);
	ProgramData *program_data = user_data;
	GError *error = NULL;

	g_assert (program_data->input_stream == NULL);
	program_data->input_stream = g_file_read_finish (file, result, &error);

	if (error != NULL)
	{
		g_printerr ("Failed to open file for reading: %s\n", error->message);
		g_clear_error (&error);
		quit_program ();
		return;
	}

	read_next_chunk (program_data);
}

static void
launch_program (ProgramData *program_data)
{
	g_application_hold (g_application_get_default ());

	g_file_read_async (program_data->file,
			   G_PRIORITY_DEFAULT,
			   NULL,
			   file_read_cb,
			   program_data);
}

static gint
app_command_line_cb (GApplication            *app,
		     GApplicationCommandLine *command_line,
		     gpointer                 user_data)
{
	ProgramData *program_data = user_data;
	gchar **argv;
	gint argc;
	gint exit_status = EXIT_SUCCESS;

	argv = g_application_command_line_get_arguments (command_line, &argc);

	if (argc != 2)
	{
		g_application_command_line_printerr (command_line,
						     "Usage: %s <input_file>\n",
						     argv[0]);
		exit_status = EXIT_FAILURE;
		goto out;
	}

	program_data->file = g_application_command_line_create_file_for_arg (command_line, argv[1]);

	launch_program (program_data);

out:
	g_strfreev (argv);
	return exit_status;
}

int
main (int    argc,
      char **argv)
{
	GApplication *app;
	ProgramData *program_data;
	int exit_status;

	setlocale (LC_ALL, "");

	app = g_application_new (NULL, G_APPLICATION_HANDLES_COMMAND_LINE);
	program_data = program_data_new ();

	g_signal_connect (app,
			  "command-line",
			  G_CALLBACK (app_command_line_cb),
			  program_data);

	exit_status = g_application_run (app, argc, argv);

	g_object_unref (app);
	program_data_free (program_data);

	return exit_status;
}
