/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-init.h"
#include "gfls-unsaved-document-titles.h"

/**
 * SECTION:gfls-init
 * @Title: Gfls Initialization and Finalization
 */

/**
 * gfls_init:
 *
 * Initializes the Gfls library (e.g. for the internationalization).
 *
 * This function can be called several times, but is meant to be called at the
 * beginning of main(), before any other Gfls function call.
 *
 * Since: 0.1
 */
void
gfls_init (void)
{
}

/**
 * gfls_finalize:
 *
 * Free the resources allocated by Gfls. For example it unrefs the singleton
 * objects.
 *
 * It is not mandatory to call this function, it's just to be friendlier to
 * memory debugging tools. This function is meant to be called at the end of
 * main(). It can be called several times.
 *
 * Since: 0.1
 */
void
gfls_finalize (void)
{
	static gboolean done = FALSE;

	/* Unref the singleton only once, even if this function is called
	 * multiple times, to see if a reference is not released correctly.
	 * Normally the singleton have a ref count of 1. If for some reason the
	 * ref count is increased somewhere, it needs to be decreased
	 * accordingly, at the right place.
	 */
	if (!done)
	{
		_gfls_unsaved_document_titles_unref_default_instance ();
		done = TRUE;
	}
}
