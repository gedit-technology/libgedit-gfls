/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-config.h"
#include "gfls-unsaved-document-titles.h"
#include <glib/gi18n-lib.h>

/**
 * SECTION:gfls-unsaved-document-titles
 * @Title: GflsUnsavedDocumentTitles
 * @Short_description: To give titles to unsaved documents
 *
 * #GflsUnsavedDocumentTitles is used to give titles to unsaved documents, such
 * as "Unsaved Document N" with 'N' replaced by a number.
 *
 * This is for new documents that have never been saved before and reside in
 * memory only. These documents have not yet an associated #GFile.
 *
 * Do not confuse it with documents with unsaved <emphasis>changes</emphasis>.
 */

#define FIRST_NUMBER (1)

struct _GflsUnsavedDocumentTitlesPrivate
{
	/* Sorted list of owned 'gint*'. */
	GList *allocated_numbers;

	GflsUnsavedDocumentTitleCallback title_callback;
};

static GflsUnsavedDocumentTitles *default_instance;

G_DEFINE_TYPE_WITH_PRIVATE (GflsUnsavedDocumentTitles, gfls_unsaved_document_titles, G_TYPE_OBJECT)

static gint
compare_numbers (gconstpointer a,
		 gconstpointer b)
{
	const gint *ptr_num_a;
	const gint *ptr_num_b;
	gint num_a;
	gint num_b;

	g_return_val_if_fail (a != NULL, 0);
	g_return_val_if_fail (b != NULL, 0);

	ptr_num_a = a;
	ptr_num_b = b;

	num_a = *ptr_num_a;
	num_b = *ptr_num_b;

	return num_a - num_b;
}

static gchar *
default_title_cb (gint num)
{
	return g_strdup_printf (_("Unsaved Document %d"), num);
}

static void
gfls_unsaved_document_titles_finalize (GObject *object)
{
	GflsUnsavedDocumentTitles *titles = GFLS_UNSAVED_DOCUMENT_TITLES (object);

	if (default_instance == titles)
	{
		default_instance = NULL;
	}

	g_list_free_full (titles->priv->allocated_numbers, g_free);

	G_OBJECT_CLASS (gfls_unsaved_document_titles_parent_class)->finalize (object);
}

static void
gfls_unsaved_document_titles_class_init (GflsUnsavedDocumentTitlesClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gfls_unsaved_document_titles_finalize;
}

static void
gfls_unsaved_document_titles_init (GflsUnsavedDocumentTitles *titles)
{
	titles->priv = gfls_unsaved_document_titles_get_instance_private (titles);

	titles->priv->title_callback = default_title_cb;
}

/**
 * gfls_unsaved_document_titles_new:
 *
 * Returns: (transfer full): a new #GflsUnsavedDocumentTitles object.
 * Since: 0.1
 */
GflsUnsavedDocumentTitles *
gfls_unsaved_document_titles_new (void)
{
	return g_object_new (GFLS_TYPE_UNSAVED_DOCUMENT_TITLES, NULL);
}

/**
 * gfls_unsaved_document_titles_get_default:
 *
 * Returns: (transfer none): the default instance of #GflsUnsavedDocumentTitles.
 * Since: 0.1
 */
GflsUnsavedDocumentTitles *
gfls_unsaved_document_titles_get_default (void)
{
	if (default_instance == NULL)
	{
		default_instance = gfls_unsaved_document_titles_new ();
	}

	return default_instance;
}

/**
 * gfls_unsaved_document_titles_unref_default_instance:
 *
 * Can be called at the end of main() to release the memory used by the default
 * instance, to be friendly to memory-debugging tools.
 *
 * Since: 0.1
 */
void
gfls_unsaved_document_titles_unref_default_instance (void)
{
	if (default_instance != NULL)
	{
		g_object_unref (default_instance);
	}

	/* default_instance is not set to NULL here, it is set to NULL in
	 * gfls_unsaved_document_titles_finalize() (i.e. when we are sure that
	 * the ref count reaches 0).
	 */
}

/**
 * gfls_unsaved_document_titles_allocate_number:
 * @titles: a #GflsUnsavedDocumentTitles.
 *
 * Allocates a number for an unsaved document. When the document is saved on
 * disk, you need to give back the number with
 * gfls_unsaved_document_titles_release_number().
 *
 * The returned number is the lowest available value, starting at 1.
 *
 * Returns: the allocated number.
 * Since: 0.1
 */
gint
gfls_unsaved_document_titles_allocate_number (GflsUnsavedDocumentTitles *titles)
{
	gint num = FIRST_NUMBER;
	GList *node;
	gint *num_data;

	g_return_val_if_fail (GFLS_IS_UNSAVED_DOCUMENT_TITLES (titles), 0);

	node = titles->priv->allocated_numbers;
	while (node != NULL)
	{
		const gint *node_data = node->data;
		const gint allocated_num = *node_data;

		if (num != allocated_num)
		{
			break;
		}

		num++;
		node = node->next;
	}

	num_data = g_new (gint, 1);
	*num_data = num;

	titles->priv->allocated_numbers = g_list_insert_sorted (titles->priv->allocated_numbers,
								num_data,
								compare_numbers);

	return num;
}

/**
 * gfls_unsaved_document_titles_release_number:
 * @titles: a #GflsUnsavedDocumentTitles.
 * @number: the number to release.
 *
 * Call this function to give back @number to @titles, so that it becomes
 * available for a next unsaved document. This is usually done when the document
 * is saved on disk.
 *
 * Since: 0.1
 */
void
gfls_unsaved_document_titles_release_number (GflsUnsavedDocumentTitles *titles,
					     gint                       number)
{
	GList *node;

	g_return_if_fail (GFLS_IS_UNSAVED_DOCUMENT_TITLES (titles));

	for (node = titles->priv->allocated_numbers; node != NULL; node = node->next)
	{
		const gint *node_data = node->data;
		const gint allocated_num = *node_data;

		if (allocated_num == number)
		{
			break;
		}
	}

	if (node != NULL)
	{
		titles->priv->allocated_numbers = g_list_remove_link (titles->priv->allocated_numbers, node);
		g_list_free_full (node, g_free);
	}
}

/**
 * gfls_unsaved_document_titles_get_title:
 * @titles: a #GflsUnsavedDocumentTitles.
 * @number: a number.
 *
 * Generates the title of an unsaved document. To customize the returned string,
 * you can use gfls_unsaved_document_titles_set_title_callback().
 *
 * Returns: (transfer full): a suitable title for the unsaved document with the
 *   given @number.
 * Since: 0.1
 */
gchar *
gfls_unsaved_document_titles_get_title (GflsUnsavedDocumentTitles *titles,
					gint                       number)
{
	g_return_val_if_fail (GFLS_IS_UNSAVED_DOCUMENT_TITLES (titles), NULL);

	return titles->priv->title_callback (number);
}

/**
 * gfls_unsaved_document_titles_set_title_callback: (skip)
 * @titles: a #GflsUnsavedDocumentTitles.
 * @title_callback: (nullable): a #GflsUnsavedDocumentTitleCallback.
 *
 * Sets a #GflsUnsavedDocumentTitleCallback function. To reset to the default
 * setting, pass %NULL to @title_callback.
 *
 * The callback will be used by gfls_unsaved_document_titles_get_title().
 *
 * Since: 0.1
 */
void
gfls_unsaved_document_titles_set_title_callback (GflsUnsavedDocumentTitles        *titles,
						 GflsUnsavedDocumentTitleCallback  title_callback)
{
	g_return_if_fail (GFLS_IS_UNSAVED_DOCUMENT_TITLES (titles));

	if (title_callback != NULL)
	{
		titles->priv->title_callback = title_callback;
	}
	else
	{
		titles->priv->title_callback = default_title_cb;
	}
}
