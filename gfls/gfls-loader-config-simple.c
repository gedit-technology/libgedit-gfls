/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-loader-config-simple.h"

/**
 * SECTION:gfls-loader-config-simple
 * @Title: GflsLoaderConfigSimple
 * @Short_description: Loader configuration
 */

struct _GflsLoaderConfigSimplePrivate
{
	gint something;
};

G_DEFINE_TYPE_WITH_PRIVATE (GflsLoaderConfigSimple, gfls_loader_config_simple, G_TYPE_OBJECT)

static void
gfls_loader_config_simple_dispose (GObject *object)
{

	G_OBJECT_CLASS (gfls_loader_config_simple_parent_class)->dispose (object);
}

static void
gfls_loader_config_simple_class_init (GflsLoaderConfigSimpleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->dispose = gfls_loader_config_simple_dispose;
}

static void
gfls_loader_config_simple_init (GflsLoaderConfigSimple *config)
{
	config->priv = gfls_loader_config_simple_get_instance_private (config);
}
