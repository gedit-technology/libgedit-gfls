/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gfls/gfls.h>

static void
test_allocate_and_release_numbers (void)
{
	GflsUnsavedDocumentTitles *titles;
	gint num_a;
	gint num_b;
	gint num_c;

	titles = gfls_unsaved_document_titles_get_default ();

	num_a = gfls_unsaved_document_titles_allocate_number (titles);
	num_b = gfls_unsaved_document_titles_allocate_number (titles);
	g_assert_cmpint (num_a, ==, 1);
	g_assert_cmpint (num_b, ==, 2);

	gfls_unsaved_document_titles_release_number (titles, num_a);
	num_a = 0;

	num_c = gfls_unsaved_document_titles_allocate_number (titles);
	g_assert_cmpint (num_c, ==, 1);
}

static gchar *
my_title_cb (gint num)
{
	return g_strdup_printf ("My Dear New Document %d", num);
}

static void
test_title (void)
{
	GflsUnsavedDocumentTitles *titles;
	gchar *title_a;
	gchar *title_b;
	gchar *my_title;

	titles = gfls_unsaved_document_titles_new ();

	title_a = gfls_unsaved_document_titles_get_title (titles, 1);

	gfls_unsaved_document_titles_set_title_callback (titles, my_title_cb);
	title_b = gfls_unsaved_document_titles_get_title (titles, 1);
	my_title = my_title_cb (1);
	g_assert_cmpstr (title_b, ==, my_title);
	g_free (title_b);
	g_free (my_title);

	gfls_unsaved_document_titles_set_title_callback (titles, NULL);
	title_b = gfls_unsaved_document_titles_get_title (titles, 1);
	g_assert_cmpstr (title_a, ==, title_b);

	g_free (title_a);
	g_free (title_b);
	g_object_unref (titles);
}

int
main (int    argc,
      char **argv)
{
	int exit_status;

	gfls_init ();
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/UnsavedDocumentTitles/allocate_and_release_numbers", test_allocate_and_release_numbers);
	g_test_add_func ("/UnsavedDocumentTitles/title", test_title);

	exit_status = g_test_run ();
	gfls_finalize ();

	return exit_status;
}
