/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 * gtksourcecompletion-utils.h
 * This file is part of GtkSourceView
 *
 * Copyright (C) 2007 - 2009 Jesús Barbero Rodríguez <chuchiperriman@gmail.com>
 * Copyright (C) 2009 - Jesse van den Kieboom <jessevdk@gnome.org>
 *
 * GtkSourceView is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * GtkSourceView is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __GTK_SOURCE_COMPLETION_UTILS_H__
#define __GTK_SOURCE_COMPLETION_UTILS_H__

#include "gtksourceview.h"

G_BEGIN_DECLS

G_GNUC_INTERNAL
void		 gtk_source_completion_utils_get_word_iter		(GtkTextBuffer   *buffer,
									 GtkTextIter     *start_word,
									 GtkTextIter     *end_word);

G_GNUC_INTERNAL
void		 gtk_source_completion_utils_replace_current_word	(GtkTextBuffer   *buffer,
									 const gchar     *text);

G_GNUC_INTERNAL
void		 gtk_source_completion_utils_move_to_iter		(GtkWindow       *window,
									 GtkSourceView   *view,
									 GtkTextIter     *iter);

G_GNUC_INTERNAL
void		 gtk_source_completion_utils_move_to_cursor		(GtkWindow       *window,
									 GtkSourceView   *view);

G_END_DECLS

#endif /* __GTK_SOURCE_COMPLETION_ITEM_H__ */
