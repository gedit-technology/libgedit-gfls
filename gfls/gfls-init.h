/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_INIT_H
#define GFLS_INIT_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <gmodule.h>

G_BEGIN_DECLS

G_MODULE_EXPORT
void	gfls_init	(void);

G_MODULE_EXPORT
void	gfls_finalize	(void);

G_END_DECLS

#endif /* GFLS_INIT_H */
