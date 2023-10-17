/* SPDX-FileCopyrightText: 2023 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_INPUT_STREAM_H
#define GFLS_INPUT_STREAM_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <gio/gio.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
void		gfls_input_stream_read_async	(GInputStream        *input_stream,
						 gsize                expected_size,
						 gsize                max_size,
						 gint                 io_priority,
						 GCancellable        *cancellable,
						 GAsyncReadyCallback  callback,
						 gpointer             user_data);

G_MODULE_EXPORT
GBytes *	gfls_input_stream_read_finish	(GInputStream  *input_stream,
						 GAsyncResult  *result,
						 gboolean      *is_truncated,
						 GError       **error);

G_END_DECLS

#endif /* GFLS_INPUT_STREAM_H */
