/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gtk/gtk.h>
#include <locale.h>

/* 100 MiB */
#define FILE_SIZE_HARD_LIMIT (100 * 1024 * 1024)

static GtkWidget *
create_file_size_limit_spin_button (void)
{
	GtkGrid *hgrid;
	GtkWidget *label;
	GtkWidget *spin_button;

	hgrid = GTK_GRID (gtk_grid_new ());
	gtk_grid_set_column_spacing (hgrid, 6);

	label = gtk_label_new ("File size limit:");
	gtk_container_add (GTK_CONTAINER (hgrid), label);

	spin_button = gtk_spin_button_new_with_range (0.0, FILE_SIZE_HARD_LIMIT, 1.0);
	gtk_container_add (GTK_CONTAINER (hgrid), spin_button);

	return GTK_WIDGET (hgrid);
}

static GtkWidget *
create_side_panel (void)
{
	GtkGrid *vgrid;
	GtkButton *open_file_button;

	vgrid = GTK_GRID (gtk_grid_new ());
	gtk_orientable_set_orientation (GTK_ORIENTABLE (vgrid), GTK_ORIENTATION_VERTICAL);
	gtk_grid_set_row_spacing (vgrid, 6);

	gtk_container_add (GTK_CONTAINER (vgrid), create_file_size_limit_spin_button ());

	open_file_button = GTK_BUTTON (gtk_button_new_with_label ("Open File"));
	gtk_container_add (GTK_CONTAINER (vgrid), GTK_WIDGET (open_file_button));

	return GTK_WIDGET (vgrid);
}

static GtkWidget *
create_view (void)
{
	GtkTextView *view;
	GtkWidget *scrolled_window;

	view = GTK_TEXT_VIEW (gtk_text_view_new ());
	gtk_text_view_set_monospace (view, TRUE);
	g_object_set (view,
		      "expand", TRUE,
		      NULL);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (view));

	return scrolled_window;
}

static void
activate_cb (GApplication *g_app,
	     gpointer      user_data)
{
	GtkWidget *window;
	GtkGrid *hgrid;

	window = gtk_application_window_new (GTK_APPLICATION (g_app));
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

	hgrid = GTK_GRID (gtk_grid_new ());

	gtk_container_add (GTK_CONTAINER (hgrid), create_side_panel ());
	gtk_container_add (GTK_CONTAINER (hgrid), create_view ());

	gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (hgrid));

	gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
	GtkApplication *app;
	int exit_status;

	setlocale (LC_ALL, "");

	app = gtk_application_new (NULL, G_APPLICATION_DEFAULT_FLAGS);

	g_signal_connect (app,
			  "activate",
			  G_CALLBACK (activate_cb),
			  NULL);

	exit_status = g_application_run (G_APPLICATION (app), argc, argv);

	g_object_unref (app);
	return exit_status;
}
