/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-utf8.h"

/**
 * SECTION:gfls-utf8
 * @Title: GflsUtf8
 * @Short_description: Functions for UTF-8 strings
 *
 * Additional functions for UTF-8 strings, to complement what is provided by
 * GLib.
 */

/**
 * gfls_utf8_has_very_long_line:
 * @str: a UTF-8 nul-terminated string.
 * @max_n_bytes_per_line: the maximum number of bytes per line, without counting
 *   the newline character(s).
 *
 * Returns %TRUE if a line in @str exceeds @max_n_bytes_per_line.
 *
 * For performance reasons, @str is not checked whether it is a valid UTF-8
 * string. So you must call for example g_utf8_validate() beforehand.
 *
 * Returns: %TRUE if @str contains a very long line, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
gfls_utf8_has_very_long_line (const gchar *str,
			      guint        max_n_bytes_per_line)
{
	guint cur_pos;
	guint line_length = 0;

	g_return_val_if_fail (str != NULL, FALSE);

	for (cur_pos = 0; str[cur_pos] != '\0'; cur_pos++)
	{
		gchar ch = str[cur_pos];

		if (ch == '\n' || ch == '\r')
		{
			line_length = 0;
		}
		else
		{
			line_length++;

			if (line_length > max_n_bytes_per_line)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
