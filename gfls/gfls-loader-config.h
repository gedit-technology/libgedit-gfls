/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_LOADER_CONFIG_H
#define GFLS_LOADER_CONFIG_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define GFLS_TYPE_LOADER_CONFIG               (gfls_loader_config_get_type ())
#define GFLS_LOADER_CONFIG(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFLS_TYPE_LOADER_CONFIG, GflsLoaderConfig))
#define GFLS_IS_LOADER_CONFIG(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFLS_TYPE_LOADER_CONFIG))
#define GFLS_LOADER_CONFIG_GET_INTERFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GFLS_TYPE_LOADER_CONFIG, GflsLoaderConfigInterface))

typedef struct _GflsLoaderConfig          GflsLoaderConfig;
typedef struct _GflsLoaderConfigInterface GflsLoaderConfigInterface;

/**
 * GflsLoaderConfigInterface:
 * @parent_interface: The parent interface.
 *
 * Since: 0.2
 */
struct _GflsLoaderConfigInterface
{
	GTypeInterface parent_interface;
};

G_MODULE_EXPORT
GType gfls_loader_config_get_type (void);

G_END_DECLS

#endif /* GFLS_LOADER_CONFIG_H */
