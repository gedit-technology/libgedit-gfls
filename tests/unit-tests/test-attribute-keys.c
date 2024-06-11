/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include <gfls/gfls.h>

static void
test_basics (void)
{
	GflsAttributeKeys *keys = gfls_attribute_keys_new ();
	gchar *result;

	result = gfls_attribute_keys_to_string (keys);
	g_assert_null (result);

	gfls_attribute_keys_add (keys, "standard::display-name");
	result = gfls_attribute_keys_to_string (keys);
	g_assert_cmpstr (result, ==, "standard::display-name");
	g_free (result);

	gfls_attribute_keys_add (keys, "access::*");
	result = gfls_attribute_keys_to_string (keys);
	g_assert_cmpstr (result, ==, "standard::display-name,access::*");
	g_free (result);

	g_object_unref (keys);
}

int
main (int    argc,
      char **argv)
{
	int exit_status;

	gfls_init ();
	g_test_init (&argc, &argv, NULL);

	g_test_add_func ("/AttributeKeys/basics", test_basics);

	exit_status = g_test_run ();
	gfls_finalize ();

	return exit_status;
}
