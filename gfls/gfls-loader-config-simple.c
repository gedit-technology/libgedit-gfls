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
	GFile *file;
	GInputStream *input_stream;
};

G_DEFINE_TYPE_WITH_PRIVATE (GflsLoaderConfigSimple, gfls_loader_config_simple, G_TYPE_OBJECT)

static void
gfls_loader_config_simple_dispose (GObject *object)
{
	GflsLoaderConfigSimple *config = GFLS_LOADER_CONFIG_SIMPLE (object);

	g_clear_object (&config->priv->file);
	g_clear_object (&config->priv->input_stream);

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

/**
 * gfls_loader_config_simple_new_from_file:
 * @file: a #GFile.
 *
 * Creates a new #GflsLoaderConfigSimple object with @file for the input
 * content.
 *
 * Returns: (transfer full): a new #GflsLoaderConfigSimple object.
 * Since: 0.2
 */
GflsLoaderConfigSimple *
gfls_loader_config_simple_new_from_file (GFile *file)
{
	GflsLoaderConfigSimple *config;

	g_return_val_if_fail (G_IS_FILE (file), NULL);

	config = g_object_new (GFLS_TYPE_LOADER_CONFIG_SIMPLE, NULL);
	config->priv->file = g_object_ref (file);

	return config;
}

/**
 * gfls_loader_config_simple_new_from_stream:
 * @input_stream: a #GInputStream.
 *
 * Creates a new #GflsLoaderConfigSimple object with @input_stream for the input
 * content.
 *
 * To load from stdin, a useful function is
 * g_application_command_line_get_stdin().
 *
 * Returns: (transfer full): a new #GflsLoaderConfigSimple object.
 * Since: 0.2
 */
GflsLoaderConfigSimple *
gfls_loader_config_simple_new_from_stream (GInputStream *input_stream)
{
	GflsLoaderConfigSimple *config;

	g_return_val_if_fail (G_IS_INPUT_STREAM (input_stream), NULL);

	config = g_object_new (GFLS_TYPE_LOADER_CONFIG_SIMPLE, NULL);
	config->priv->input_stream = g_object_ref (input_stream);

	return config;
}

/**
 * gfls_loader_config_simple_get_file:
 * @config: a #GflsLoaderConfigSimple.
 *
 * Returns: (transfer none) (nullable): the #GFile of @config, or %NULL.
 * Since: 0.2
 */
GFile *
gfls_loader_config_simple_get_file (GflsLoaderConfigSimple *config)
{
	g_return_val_if_fail (GFLS_IS_LOADER_CONFIG_SIMPLE (config), NULL);
	return config->priv->file;
}

/**
 * gfls_loader_config_simple_get_stream:
 * @config: a #GflsLoaderConfigSimple.
 *
 * Returns: (transfer none) (nullable): the #GInputStream of @config, or %NULL.
 * Since: 0.2
 */
GInputStream *
gfls_loader_config_simple_get_stream (GflsLoaderConfigSimple *config)
{
	g_return_val_if_fail (GFLS_IS_LOADER_CONFIG_SIMPLE (config), NULL);
	return config->priv->input_stream;
}
