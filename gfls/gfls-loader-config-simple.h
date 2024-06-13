/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_LOADER_CONFIG_SIMPLE_H
#define GFLS_LOADER_CONFIG_SIMPLE_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <gio/gio.h>

G_BEGIN_DECLS

#define GFLS_TYPE_LOADER_CONFIG_SIMPLE             (gfls_loader_config_simple_get_type ())
#define GFLS_LOADER_CONFIG_SIMPLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFLS_TYPE_LOADER_CONFIG_SIMPLE, GflsLoaderConfigSimple))
#define GFLS_LOADER_CONFIG_SIMPLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GFLS_TYPE_LOADER_CONFIG_SIMPLE, GflsLoaderConfigSimpleClass))
#define GFLS_IS_LOADER_CONFIG_SIMPLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFLS_TYPE_LOADER_CONFIG_SIMPLE))
#define GFLS_IS_LOADER_CONFIG_SIMPLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GFLS_TYPE_LOADER_CONFIG_SIMPLE))
#define GFLS_LOADER_CONFIG_SIMPLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GFLS_TYPE_LOADER_CONFIG_SIMPLE, GflsLoaderConfigSimpleClass))

typedef struct _GflsLoaderConfigSimple         GflsLoaderConfigSimple;
typedef struct _GflsLoaderConfigSimpleClass    GflsLoaderConfigSimpleClass;
typedef struct _GflsLoaderConfigSimplePrivate  GflsLoaderConfigSimplePrivate;

struct _GflsLoaderConfigSimple
{
	GObject parent;

	GflsLoaderConfigSimplePrivate *priv;
};

struct _GflsLoaderConfigSimpleClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			gfls_loader_config_simple_get_type		(void);

G_MODULE_EXPORT
GflsLoaderConfigSimple *gfls_loader_config_simple_new_from_file		(GFile *file);

G_MODULE_EXPORT
GflsLoaderConfigSimple *gfls_loader_config_simple_new_from_stream	(GInputStream *input_stream);

G_MODULE_EXPORT
GFile *			gfls_loader_config_simple_get_file		(GflsLoaderConfigSimple *config);

G_MODULE_EXPORT
GInputStream *		gfls_loader_config_simple_get_stream		(GflsLoaderConfigSimple *config);

G_END_DECLS

#endif /* GFLS_LOADER_CONFIG_SIMPLE_H */
