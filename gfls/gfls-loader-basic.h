/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_LOADER_BASIC_H
#define GFLS_LOADER_BASIC_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <gio/gio.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
void		gfls_loader_basic_load_async	(GFile               *file,
						 gsize                max_size,
						 guint                max_n_bytes_per_line,
						 gint                 io_priority,
						 GCancellable        *cancellable,
						 GAsyncReadyCallback  callback,
						 gpointer             user_data);

G_MODULE_EXPORT
GBytes *	gfls_loader_basic_load_finish	(GFile         *file,
						 GAsyncResult  *result,
						 GError       **error);

G_END_DECLS

#endif /* GFLS_LOADER_BASIC_H */
