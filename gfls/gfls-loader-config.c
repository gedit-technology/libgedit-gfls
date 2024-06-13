/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-loader-config.h"

/**
 * SECTION:gfls-loader-config
 * @Title: GflsLoaderConfig
 * @Short_description: Loader configuration interface
 *
 * #GflsLoaderConfig is an interface that classes implement to store a loader
 * configuration.
 *
 * Currently it is an empty interface, also called a “marker interface”.
 */

G_DEFINE_INTERFACE (GflsLoaderConfig, gfls_loader_config, G_TYPE_OBJECT)

static void
gfls_loader_config_default_init (GflsLoaderConfigInterface *interface)
{
}
