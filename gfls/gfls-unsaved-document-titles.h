/* SPDX-FileCopyrightText: 2024 - Sébastien Wilmet
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */

#ifndef GFLS_UNSAVED_DOCUMENT_TITLES_H
#define GFLS_UNSAVED_DOCUMENT_TITLES_H

#if !defined (GFLS_H_INSIDE) && !defined (GFLS_COMPILATION)
#error "Only <gfls/gfls.h> can be included directly."
#endif

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define GFLS_TYPE_UNSAVED_DOCUMENT_TITLES             (gfls_unsaved_document_titles_get_type ())
#define GFLS_UNSAVED_DOCUMENT_TITLES(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GFLS_TYPE_UNSAVED_DOCUMENT_TITLES, GflsUnsavedDocumentTitles))
#define GFLS_UNSAVED_DOCUMENT_TITLES_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GFLS_TYPE_UNSAVED_DOCUMENT_TITLES, GflsUnsavedDocumentTitlesClass))
#define GFLS_IS_UNSAVED_DOCUMENT_TITLES(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GFLS_TYPE_UNSAVED_DOCUMENT_TITLES))
#define GFLS_IS_UNSAVED_DOCUMENT_TITLES_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GFLS_TYPE_UNSAVED_DOCUMENT_TITLES))
#define GFLS_UNSAVED_DOCUMENT_TITLES_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GFLS_TYPE_UNSAVED_DOCUMENT_TITLES, GflsUnsavedDocumentTitlesClass))

typedef struct _GflsUnsavedDocumentTitles         GflsUnsavedDocumentTitles;
typedef struct _GflsUnsavedDocumentTitlesClass    GflsUnsavedDocumentTitlesClass;
typedef struct _GflsUnsavedDocumentTitlesPrivate  GflsUnsavedDocumentTitlesPrivate;

struct _GflsUnsavedDocumentTitles
{
	GObject parent;

	GflsUnsavedDocumentTitlesPrivate *priv;
};

struct _GflsUnsavedDocumentTitlesClass
{
	GObjectClass parent_class;

	gpointer padding[12];
};

/**
 * GflsUnsavedDocumentTitleCallback:
 * @num: the number.
 *
 * Type definition for a function that will be called to create a string
 * containing @num, to give a name to a document not yet present on disk.
 *
 * Since: 0.1
 */
typedef gchar *(*GflsUnsavedDocumentTitleCallback) (gint num);

G_MODULE_EXPORT
GType		gfls_unsaved_document_titles_get_type			(void);

G_MODULE_EXPORT
GflsUnsavedDocumentTitles *
		gfls_unsaved_document_titles_new			(void);

G_MODULE_EXPORT
GflsUnsavedDocumentTitles *
		gfls_unsaved_document_titles_get_default		(void);

G_MODULE_EXPORT
gint		gfls_unsaved_document_titles_allocate_number		(GflsUnsavedDocumentTitles *titles);

G_MODULE_EXPORT
void		gfls_unsaved_document_titles_release_number		(GflsUnsavedDocumentTitles *titles,
									 gint                       number);

G_MODULE_EXPORT
gchar *		gfls_unsaved_document_titles_get_title			(GflsUnsavedDocumentTitles *titles,
									 gint                       number);

G_MODULE_EXPORT
void		gfls_unsaved_document_titles_set_title_callback		(GflsUnsavedDocumentTitles        *titles,
									 GflsUnsavedDocumentTitleCallback  title_callback);

G_GNUC_INTERNAL
void		_gfls_unsaved_document_titles_unref_default_instance	(void);

G_END_DECLS

#endif /* GFLS_UNSAVED_DOCUMENT_TITLES_H */
