/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_ATTRIBUTE_KEYS_H
#define GFLS_ATTRIBUTE_KEYS_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define GFLS_TYPE_ATTRIBUTE_KEYS             (gfls_attribute_keys_get_type ())
#define GFLS_ATTRIBUTE_KEYS(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFLS_TYPE_ATTRIBUTE_KEYS, GflsAttributeKeys))
#define GFLS_ATTRIBUTE_KEYS_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GFLS_TYPE_ATTRIBUTE_KEYS, GflsAttributeKeysClass))
#define GFLS_IS_ATTRIBUTE_KEYS(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFLS_TYPE_ATTRIBUTE_KEYS))
#define GFLS_IS_ATTRIBUTE_KEYS_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GFLS_TYPE_ATTRIBUTE_KEYS))
#define GFLS_ATTRIBUTE_KEYS_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GFLS_TYPE_ATTRIBUTE_KEYS, GflsAttributeKeysClass))

typedef struct _GflsAttributeKeys         GflsAttributeKeys;
typedef struct _GflsAttributeKeysClass    GflsAttributeKeysClass;
typedef struct _GflsAttributeKeysPrivate  GflsAttributeKeysPrivate;

struct _GflsAttributeKeys
{
	GObject parent;

	GflsAttributeKeysPrivate *priv;
};

struct _GflsAttributeKeysClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

G_MODULE_EXPORT
GType			gfls_attribute_keys_get_type	(void);

G_MODULE_EXPORT
GflsAttributeKeys *	gfls_attribute_keys_new		(void);

G_MODULE_EXPORT
void			gfls_attribute_keys_add		(GflsAttributeKeys *keys,
							 const gchar       *str);

G_MODULE_EXPORT
gchar *			gfls_attribute_keys_to_string	(GflsAttributeKeys *keys);

G_END_DECLS

#endif /* GFLS_ATTRIBUTE_KEYS_H */
