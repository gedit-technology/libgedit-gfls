/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gfls/gfls.h>

static void
test_has_very_long_line (void)
{
	/* Without newline chars */
	g_assert_false (gfls_utf8_has_very_long_line ("", 0));
	g_assert_true (gfls_utf8_has_very_long_line ("a", 0));

	g_assert_false (gfls_utf8_has_very_long_line ("", 1));
	g_assert_false (gfls_utf8_has_very_long_line ("a", 1));
	g_assert_true (gfls_utf8_has_very_long_line ("ab", 1));

	g_assert_true (gfls_utf8_has_very_long_line ("é", 1));
	g_assert_false (gfls_utf8_has_very_long_line ("é", 2));

	/* With \n */
	g_assert_false (gfls_utf8_has_very_long_line ("\n\n\n", 0));
	g_assert_true (gfls_utf8_has_very_long_line ("\na\n", 0));
	g_assert_true (gfls_utf8_has_very_long_line ("\na", 0));

	g_assert_false (gfls_utf8_has_very_long_line ("a\nb\nc\n", 1));
	g_assert_false (gfls_utf8_has_very_long_line ("a\nb\nc", 1));
	g_assert_true (gfls_utf8_has_very_long_line ("a\nbc\n", 1));
	g_assert_true (gfls_utf8_has_very_long_line ("a\nbcd\n", 1));
	g_assert_true (gfls_utf8_has_very_long_line ("a\nbcd\n", 2));
	g_assert_true (gfls_utf8_has_very_long_line ("a\nbcd", 2));

	g_assert_true (gfls_utf8_has_very_long_line ("é\nSé", 2));
	g_assert_false (gfls_utf8_has_very_long_line ("é\nSé", 3));
	g_assert_false (gfls_utf8_has_very_long_line ("é\nSé\n", 3));

	/* Quick test with \r */
	g_assert_false (gfls_utf8_has_very_long_line ("\r\r\r", 0));
	g_assert_false (gfls_utf8_has_very_long_line ("\n\r\n\r\n\r", 0));
	g_assert_false (gfls_utf8_has_very_long_line ("\r\n\r\n\r\n", 0));

	g_assert_false (gfls_utf8_has_very_long_line ("a\rb\nc\rd", 1));

	g_assert_true (gfls_utf8_has_very_long_line ("a\ré", 1));
	g_assert_false (gfls_utf8_has_very_long_line ("a\ré", 2));
}

int
main (int    argc,
      char **argv)
{
	int exit_status;

	gfls_init ();
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/utf8/has_very_long_line", test_has_very_long_line);

	exit_status = g_test_run ();
	gfls_finalize ();

	return exit_status;
}
