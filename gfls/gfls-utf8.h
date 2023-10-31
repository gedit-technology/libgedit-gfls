/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_UTF8_H
#define GFLS_UTF8_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <gmodule.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
gboolean	gfls_utf8_has_very_long_line	(const gchar *str,
						 guint        max_n_bytes_per_line);

G_END_DECLS

#endif /* GFLS_UTF8_H */
