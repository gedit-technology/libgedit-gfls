/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gtk/gtk.h>
#include <locale.h>

static void
activate_cb (GApplication *g_app,
	     gpointer      user_data)
{
	GtkWidget *window;
	GtkGrid *hgrid;
	GtkButton *open_file_button;
	GtkTextView *view;
	GtkWidget *scrolled_window;

	window = gtk_application_window_new (GTK_APPLICATION (g_app));
	gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

	hgrid = GTK_GRID (gtk_grid_new ());

	open_file_button = GTK_BUTTON (gtk_button_new_with_label ("Open File"));
	gtk_widget_set_valign (GTK_WIDGET (open_file_button), GTK_ALIGN_START);
	gtk_container_add (GTK_CONTAINER (hgrid), GTK_WIDGET (open_file_button));

	view = GTK_TEXT_VIEW (gtk_text_view_new ());
	gtk_text_view_set_monospace (view, TRUE);
	g_object_set (view,
		      "expand", TRUE,
		      NULL);

	scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_add (GTK_CONTAINER (scrolled_window), GTK_WIDGET (view));
	gtk_container_add (GTK_CONTAINER (hgrid), scrolled_window);
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
