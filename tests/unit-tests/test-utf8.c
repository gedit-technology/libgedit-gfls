/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gfls/gfls.h>

static void
test_has_very_long_line (void)
{
	/* TODO */
}

int
main (int    argc,
      char **argv)
{
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/utf8/has_very_long_line", test_has_very_long_line);

	return g_test_run ();
}
