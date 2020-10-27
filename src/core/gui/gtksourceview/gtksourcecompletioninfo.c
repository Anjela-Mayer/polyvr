/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*- */
/* gtksourcecompletioninfo.c
 * This file is part of GtkSourceView
 *
 * Copyright (C) 2007 -2009 Jesús Barbero Rodríguez <chuchiperriman@gmail.com>
 * Copyright (C) 2009 - Jesse van den Kieboom <jessevdk@gnome.org>
 * Copyright (C) 2012 - Sébastien Wilmet <swilmet@gnome.org>
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

/**
 * SECTION:completioninfo
 * @title: GtkSourceCompletionInfo
 * @short_description: Calltips object
 *
 * This object can be used to show a calltip or help for the
 * current completion proposal.
 *
 * The info window has always the same size as the natural size of its child
 * widget, added with gtk_container_add().  If you want a fixed size instead, a
 * possibility is to use a scrolled window, as the following example
 * demonstrates.
 *
 * <example>
 *   <title>Fixed size with a scrolled window.</title>
 *   <programlisting>
 * GtkSourceCompletionInfo *info;
 * GtkWidget *your_widget;
 * GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
 *
 * gtk_widget_set_size_request (scrolled_window, 300, 200);
 * gtk_container_add (GTK_CONTAINER (scrolled_window), your_widget);
 * gtk_container_add (GTK_CONTAINER (info), scrolled_window);
 *   </programlisting>
 * </example>
 *
 * If the calltip is displayed on top of a certain widget, say a #GtkTextView,
 * you should hide it when the #GtkWidget::focus-out-event signal is emitted by
 * the #GtkTextView. You may also be interested by the
 * #GtkTextBuffer:cursor-position property (when its value is modified). If you
 * use the #GtkSourceCompletionInfo through the #GtkSourceCompletion machinery,
 * you don't need to worry about this.
 */

#include "gtksourcecompletioninfo.h"
#include "gtksourcecompletionutils.h"
#include "gtksourceview-i18n.h"

struct _GtkSourceCompletionInfoPrivate
{
	guint idle_resize;
};

/* Signals */
enum
{
	BEFORE_SHOW,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

G_DEFINE_TYPE(GtkSourceCompletionInfo, gtk_source_completion_info, GTK_TYPE_WINDOW);

#define GTK_SOURCE_COMPLETION_INFO_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTK_SOURCE_TYPE_COMPLETION_INFO, GtkSourceCompletionInfoPrivate))

/* Resize the window */

static gboolean
idle_resize (GtkSourceCompletionInfo *info)
{
	GtkWidget *child = gtk_bin_get_child (GTK_BIN (info));
	GtkRequisition nat_size;
	guint border_width;
	gint window_width;
	gint window_height;
	gint cur_window_width;
	gint cur_window_height;

	g_assert (child != NULL);

	info->priv->idle_resize = 0;

	gtk_widget_get_preferred_size (child, NULL, &nat_size);

	border_width = gtk_container_get_border_width (GTK_CONTAINER (info));

	window_width = nat_size.width + 2 * border_width;
	window_height = nat_size.height + 2 * border_width;

	gtk_window_get_size (GTK_WINDOW (info), &cur_window_width, &cur_window_height);

	/* Avoid an infinite loop */
	if (cur_window_width != window_width || cur_window_height != window_height)
	{
		gtk_window_resize (GTK_WINDOW (info),
				   MAX (1, window_width),
				   MAX (1, window_height));
	}

	return FALSE;
}

static void
queue_resize (GtkSourceCompletionInfo *info)
{
	if (info->priv->idle_resize == 0)
	{
		info->priv->idle_resize = g_idle_add ((GSourceFunc)idle_resize, info);
	}
}

static void
gtk_source_completion_info_check_resize (GtkContainer *container)
{
	GtkSourceCompletionInfo *info = GTK_SOURCE_COMPLETION_INFO (container);
	queue_resize (info);

	GTK_CONTAINER_CLASS (gtk_source_completion_info_parent_class)->check_resize (container);
}

/* Geometry management */

static GtkSizeRequestMode
gtk_source_completion_info_get_request_mode (GtkWidget *widget)
{
	return GTK_SIZE_REQUEST_CONSTANT_SIZE;
}

static void
gtk_source_completion_info_get_preferred_width (GtkWidget *widget,
						gint	  *min_width,
						gint	  *nat_width)
{
	GtkWidget *child = gtk_bin_get_child (GTK_BIN (widget));
	GtkRequisition nat_size;

	gtk_widget_get_preferred_size (child, NULL, &nat_size);

	if (min_width != NULL)
	{
		*min_width = nat_size.width;
	}

	if (nat_width != NULL)
	{
		*nat_width = nat_size.width;
	}
}

static void
gtk_source_completion_info_get_preferred_height (GtkWidget *widget,
						 gint	   *min_height,
						 gint	   *nat_height)
{
	GtkWidget *child = gtk_bin_get_child (GTK_BIN (widget));
	GtkRequisition nat_size;

	gtk_widget_get_preferred_size (child, NULL, &nat_size);

	if (min_height != NULL)
	{
		*min_height = nat_size.height;
	}

	if (nat_height != NULL)
	{
		*nat_height = nat_size.height;
	}
}

/* Init, dispose, finalize, ... */

static void
gtk_source_completion_info_init (GtkSourceCompletionInfo *info)
{
	info->priv = GTK_SOURCE_COMPLETION_INFO_GET_PRIVATE (info);

	/* Tooltip style */
	gtk_window_set_title (GTK_WINDOW (info), _("Completion Info"));
	gtk_widget_set_name (GTK_WIDGET (info), "gtk-tooltip");

	gtk_window_set_type_hint (GTK_WINDOW (info),
	                          GDK_WINDOW_TYPE_HINT_COMBO);

	gtk_container_set_border_width (GTK_CONTAINER (info), 1);
}

static void
gtk_source_completion_info_finalize (GObject *object)
{
	GtkSourceCompletionInfo *info = GTK_SOURCE_COMPLETION_INFO (object);

	if (info->priv->idle_resize != 0)
	{
		g_source_remove (info->priv->idle_resize);
	}

	G_OBJECT_CLASS (gtk_source_completion_info_parent_class)->finalize (object);
}

static void
gtk_source_completion_info_show (GtkWidget *widget)
{
	/* First emit BEFORE_SHOW and then chain up */
	g_signal_emit (widget, signals[BEFORE_SHOW], 0);

	GTK_WIDGET_CLASS (gtk_source_completion_info_parent_class)->show (widget);
}

static gboolean
gtk_source_completion_info_draw (GtkWidget *widget,
                                 cairo_t   *cr)
{
	GTK_WIDGET_CLASS (gtk_source_completion_info_parent_class)->draw (widget, cr);

	gtk_render_frame (gtk_widget_get_style_context (widget),
	                  cr,
	                  0, 0,
	                  gtk_widget_get_allocated_width (widget),
	                  gtk_widget_get_allocated_height (widget));

	return FALSE;
}

static void
gtk_source_completion_info_class_init (GtkSourceCompletionInfoClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	GtkContainerClass *container_class = GTK_CONTAINER_CLASS (klass);

	object_class->finalize = gtk_source_completion_info_finalize;

	widget_class->show = gtk_source_completion_info_show;
	widget_class->draw = gtk_source_completion_info_draw;
	widget_class->get_request_mode = gtk_source_completion_info_get_request_mode;
	widget_class->get_preferred_width = gtk_source_completion_info_get_preferred_width;
	widget_class->get_preferred_height = gtk_source_completion_info_get_preferred_height;

	container_class->check_resize = gtk_source_completion_info_check_resize;

	/**
	 * GtkSourceCompletionInfo::before-show:
	 * @info: The #GtkSourceCompletionInfo who emits the signal
	 *
	 * This signal is emitted before any "show" management. You can connect
	 * to this signal if you want to change some properties or position
	 * before the real "show".
	 */
	signals[BEFORE_SHOW] =
		g_signal_new ("before-show",
		              G_TYPE_FROM_CLASS (klass),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              0,
		              NULL,
		              NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE,
		              0);

	g_type_class_add_private (object_class, sizeof (GtkSourceCompletionInfoPrivate));
}

/* Public functions */

/**
 * gtk_source_completion_info_new:
 *
 * Returns: a new GtkSourceCompletionInfo.
 */
GtkSourceCompletionInfo *
gtk_source_completion_info_new (void)
{
	return g_object_new (GTK_SOURCE_TYPE_COMPLETION_INFO,
	                     "type", GTK_WINDOW_POPUP,
	                     NULL);
}

/**
 * gtk_source_completion_info_move_to_iter:
 * @info: a #GtkSourceCompletionInfo.
 * @view: a #GtkTextView on which the info window should be positioned.
 * @iter: (allow-none): a #GtkTextIter.
 *
 * Moves the #GtkSourceCompletionInfo to @iter. If @iter is %NULL @info is
 * moved to the cursor position. Moving will respect the #GdkGravity setting
 * of the info window and will ensure the line at @iter is not occluded by
 * the window.
 */
void
gtk_source_completion_info_move_to_iter (GtkSourceCompletionInfo *info,
                                         GtkTextView             *view,
                                         GtkTextIter             *iter)
{
	g_return_if_fail (GTK_SOURCE_IS_COMPLETION_INFO (info));
	g_return_if_fail (GTK_SOURCE_IS_VIEW (view));

	if (iter == NULL)
	{
		gtk_source_completion_utils_move_to_cursor (GTK_WINDOW (info),
							    GTK_SOURCE_VIEW (view));
	}
	else
	{
		gtk_source_completion_utils_move_to_iter (GTK_WINDOW (info),
							  GTK_SOURCE_VIEW (view),
							  iter);
	}
}

/**
 * gtk_source_completion_info_set_widget:
 * @info: a #GtkSourceCompletionInfo.
 * @widget: (allow-none): a #GtkWidget.
 *
 * Sets the content widget of the info window. See that the previous widget will
 * lose a reference and it can be destroyed, so if you do not want this to
 * happen you must use g_object_ref() before calling this method.
 *
 * Deprecated: 3.8: Use gtk_container_add() instead. If there is already a child
 * widget, remove it with gtk_container_remove().
 */
void
gtk_source_completion_info_set_widget (GtkSourceCompletionInfo *info,
                                       GtkWidget               *widget)
{
	GtkWidget *cur_child = NULL;

	g_return_if_fail (GTK_SOURCE_IS_COMPLETION_INFO (info));
	g_return_if_fail (widget == NULL || GTK_IS_WIDGET (widget));

	cur_child = gtk_bin_get_child (GTK_BIN (info));

	if (cur_child == widget)
	{
		return;
	}

	if (cur_child != NULL)
	{
		gtk_container_remove (GTK_CONTAINER (info), cur_child);
	}

	if (widget != NULL)
	{
		gtk_container_add (GTK_CONTAINER (info), widget);
	}
}

/**
 * gtk_source_completion_info_get_widget:
 * @info: a #GtkSourceCompletionInfo.
 *
 * Get the current content widget.
 *
 * Returns: (transfer none): The current content widget.
 *
 * Deprecated: 3.8: Use gtk_bin_get_child() instead.
 */
GtkWidget *
gtk_source_completion_info_get_widget (GtkSourceCompletionInfo* info)
{
	g_return_val_if_fail (GTK_SOURCE_IS_COMPLETION_INFO (info), NULL);

	return gtk_bin_get_child (GTK_BIN (info));
}
