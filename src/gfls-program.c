/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gio/gio.h>
#include <locale.h>

typedef struct
{
	GFile *file;
} ProgramData;

static ProgramData *
program_data_new (void)
{
	return g_new0 (ProgramData, 1);
}

static void
program_data_free (ProgramData *program_data)
{
	if (program_data != NULL)
	{
		g_clear_object (&program_data->file);
		g_free (program_data);
	}
}

static void
quit_program (void)
{
	g_application_release (g_application_get_default ());
}

static void
query_info_cb (GObject      *source_object,
	       GAsyncResult *result,
	       gpointer      user_data)
{
	GFile *file = G_FILE (source_object);
	GFileInfo *info;
	GError *error = NULL;

	info = g_file_query_info_finish (file, result, &error);

	if (error != NULL)
	{
		g_printerr ("Failed to query file informations: %s\n", error->message);
		g_clear_error (&error);
	}

	/* TODO: print infos */

	g_clear_object (&info);

	quit_program ();
}

static void
launch_program (ProgramData *program_data)
{
	g_application_hold (g_application_get_default ());

	g_file_query_info_async (program_data->file,
				 G_FILE_ATTRIBUTE_STANDARD_TYPE ","
				 G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME ","
				 G_FILE_ATTRIBUTE_STANDARD_SIZE,
				 G_FILE_QUERY_INFO_NONE,
				 G_PRIORITY_DEFAULT,
				 NULL,
				 query_info_cb,
				 NULL);
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
