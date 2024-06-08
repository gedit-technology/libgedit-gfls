/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#include "gfls-attribute-keys.h"

/**
 * SECTION:gfls-attribute-keys
 * @Title: GflsAttributeKeys
 * @Short_description: A list of #GFile attribute keys
 *
 * #GflsAttributeKeys is a small utility to build a list of #GFile attribute
 * keys. The result is a single string to pass as an argument to a function like
 * g_file_query_info().
 *
 * It becomes useful to do only a single query with a list of keys coming from
 * different parts of the program. To take an example, Gfls might have its own
 * list of keys to query, but it is desirable to ask the application for
 * additional keys.
 */

/* Rationale
 * =========
 *
 * The goal is to get desired #GFileInfo attributes in one query.
 *
 * Reasons to do it in one query:
 * 1. To avoid unnecessary round-trips (?) (especially for remote files).
 *    Actually I don't know how it is implemented in GIO and GVfs. In case of
 *    doubt a single query is better since the implementation has the
 *    *possibility* to optimize network usage.
 * 2. For convenience in the implementation, to just call one pair of
 *    async()/finish() functions for that task.
 */

struct _GflsAttributeKeysPrivate
{
	/* Element-type: "owned gchar *" */
	GPtrArray *array;
};

G_DEFINE_TYPE_WITH_PRIVATE (GflsAttributeKeys, gfls_attribute_keys, G_TYPE_OBJECT)

static void
gfls_attribute_keys_finalize (GObject *object)
{
	GflsAttributeKeys *keys = GFLS_ATTRIBUTE_KEYS (object);

	g_clear_pointer (&keys->priv->array, g_ptr_array_unref);

	G_OBJECT_CLASS (gfls_attribute_keys_parent_class)->finalize (object);
}

static void
gfls_attribute_keys_class_init (GflsAttributeKeysClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gfls_attribute_keys_finalize;
}

static void
gfls_attribute_keys_init (GflsAttributeKeys *keys)
{
	keys->priv = gfls_attribute_keys_get_instance_private (keys);

	keys->priv->array = g_ptr_array_new_null_terminated (0, g_free, TRUE);
}

/**
 * gfls_attribute_keys_new:
 *
 * Returns: (transfer full): a new #GflsAttributeKeys object.
 * Since: 0.2
 */
GflsAttributeKeys *
gfls_attribute_keys_new (void)
{
	return g_object_new (GFLS_TYPE_ATTRIBUTE_KEYS, NULL);
}

/**
 * gfls_attribute_keys_add:
 * @keys: a #GflsAttributeKeys.
 * @str: the value to add.
 *
 * Adds @str to @keys.
 *
 * @str has the same semantics as the corresponding parameter of
 * g_file_query_info().
 *
 * Since: 0.2
 */
void
gfls_attribute_keys_add (GflsAttributeKeys *keys,
			 const gchar       *str)
{
	g_return_if_fail (GFLS_IS_ATTRIBUTE_KEYS (keys));
	g_return_if_fail (str != NULL);

	g_ptr_array_add (keys->priv->array, g_strdup (str));
}

/**
 * gfls_attribute_keys_to_string:
 * @keys: a #GflsAttributeKeys.
 *
 * Returns: (transfer full) (nullable): the complete string to pass to
 *   g_file_query_info() (for example), or %NULL if the list is empty.
 * Since: 0.2
 */
gchar *
gfls_attribute_keys_to_string (GflsAttributeKeys *keys)
{
	g_return_val_if_fail (GFLS_IS_ATTRIBUTE_KEYS (keys), NULL);

	if (keys->priv->array->len == 0)
	{
		return NULL;
	}

	/* The list is *not* optimized here (to remove duplicates, simplify when
	 * there are wildcards, etc). It is already optimized in GIO, but as an
	 * implementation detail: by g_file_attribute_matcher_new(), which is
	 * called by g_file_query_info().
	 */
	return g_strjoinv (",", (gchar **) keys->priv->array->pdata);
}
