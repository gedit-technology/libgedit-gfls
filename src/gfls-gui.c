/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gtk/gtk.h>
#include <locale.h>

/* 100 MiB */
#define FILE_SIZE_HARD_LIMIT (100 * 1024 * 1024)

typedef struct
{
	GtkApplicationWindow *window;
	GtkTextView *view;
	GtkSpinButton *file_size_limit_spin_button;
} ProgramData;

static ProgramData *
program_data_new (void)
{
	return g_new0 (ProgramData, 1);
}

static void
program_data_free (ProgramData *program_data)
{
	g_free (program_data);
}

static GtkWidget *
create_file_size_limit_spin_button (ProgramData *program_data)
{
	GtkGrid *hgrid;
	GtkWidget *label;

	g_assert (program_data->file_size_limit_spin_button == NULL);

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 6);

	label = gtk_label_new ("File size limit:");
	gtk_container_add (GTK_CONTAINER (hgrid), label);

	program_data->file_size_limit_spin_button =
		GTK_SPIN_BUTTON (gtk_spin_button_new_with_range (0.0, FILE_SIZE_HARD_LIMIT, 1.0));

	gtk_container_add (GTK_CONTAINER (hgrid), GTK_WIDGET (program_data->file_size_limit_spin_button));

	return GTK_WIDGET (hgrid);
}

static void
open_file_chooser_response_cb (GtkFileChooserNative *open_file_chooser,
			       gint                  response_id,
			       ProgramData          *program_data)
{
	if (response_id == GTK_RESPONSE_ACCEPT)
	{
		g_print ("Open file.\n");
	}

	g_object_unref (open_file_chooser);
}

static void
open_file_button_clicked_cb (GtkButton   *open_file_button,
			     ProgramData *program_data)
{
	GtkFileChooserNative *file_chooser;

	file_chooser = gtk_file_chooser_native_new ("Open File",
						    GTK_WINDOW (program_data->window),
						    GTK_FILE_CHOOSER_ACTION_OPEN,
						    NULL, NULL);

	g_signal_connect (file_chooser,
			  "response",
			  G_CALLBACK (open_file_chooser_response_cb),
			  program_data);

	gtk_native_dialog_set_modal (GTK_NATIVE_DIALOG (file_chooser), TRUE);
	gtk_native_dialog_show (GTK_NATIVE_DIALOG (file_chooser));
}

static GtkWidget *
create_open_file_button (ProgramData *program_data)
{
	GtkButton *open_file_button;

	open_file_button = GTK_BUTTON (gtk_button_new_with_label ("Open File"));

	g_signal_connect (open_file_button,
			  "clicked",
			  G_CALLBACK (open_file_button_clicked_cb),
			  program_data);

	return GTK_WIDGET (open_file_button);
}

static GtkWidget *
create_side_panel (ProgramData *program_data)
{
	GtkGrid *vgrid;

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	gtk_container_add (GTK_CONTAINER (vgrid), create_file_size_limit_spin_button (program_data));
	gtk_container_add (GTK_CONTAINER (vgrid), create_open_file_button (program_data));

	return GTK_WIDGET (vgrid);
}

static GtkWidget *
create_view (ProgramData *program_data)
{
	GtkWidget *scrolled_window;

	g_assert (program_data->view == NULL);

	program_data->view = GTK_TEXT_VIEW (gtk_text_view_new ());
	gtk_text_view_set_monospace (program_data->view, TRUE);
	g_object_set (program_data->view,
		      "expand", TRUE,
		      NULL);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (program_data->view));

	return scrolled_window;
}

static void
activate_cb (GApplication *g_app,
	     ProgramData  *program_data)
{
	GtkGrid *hgrid;

	g_assert (program_data->window == NULL);

	program_data->window = GTK_APPLICATION_WINDOW (gtk_application_window_new (GTK_APPLICATION (g_app)));
	gtk_window_set_default_size (GTK_WINDOW (program_data->window), 800, 600);

	hgrid = GTK_GRID (gtk_grid_new ());

	gtk_container_add (GTK_CONTAINER (hgrid), create_side_panel (program_data));
	gtk_container_add (GTK_CONTAINER (hgrid), create_view (program_data));

	gtk_container_add (GTK_CONTAINER (program_data->window), GTK_WIDGET (hgrid));

	gtk_widget_show_all (GTK_WIDGET (program_data->window));
}

int
main (int    argc,
      char **argv)
{
	GtkApplication *app;
	ProgramData *program_data;
	int exit_status;

	setlocale (LC_ALL, "");

	app = gtk_application_new (NULL, G_APPLICATION_DEFAULT_FLAGS);
	program_data = program_data_new ();

	g_signal_connect (app,
			  "activate",
			  G_CALLBACK (activate_cb),
			  program_data);

	exit_status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);
	program_data_free (program_data);
	return exit_status;
}
