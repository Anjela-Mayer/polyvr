/* GdkGLExt - OpenGL Extension to GDK
 * Copyright (C) 2012  Thomas Zimmermann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA.
 */

#ifndef __GDK_GLEXT_CONTEXT_IMPL_H__
#define __GDK_GLEXT_CONTEXT_IMPL_H__

#include "gdkgl.h"

G_BEGIN_DECLS

#define GDK_TYPE_GLEXT_CONTEXT_IMPL            (gdk_glext_context_impl_get_type ())
#define GDK_GLEXT_CONTEXT_IMPL(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_GLEXT_CONTEXT_IMPL, GdkGLExtContextImpl))
#define GDK_GLEXT_CONTEXT_IMPL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_GLEXT_CONTEXT_IMPL, GdkGLExtContextImplClass))
#define GDK_IS_GLEXT_CONTEXT_IMPL(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_GLEXT_CONTEXT_IMPL))
#define GDK_IS_GLEXT_CONTEXT_IMPL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_GLEXT_CONTEXT_IMPL))
#define GDK_GLEXT_CONTEXT_IMPL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_GLEXT_CONTEXT_IMPL, GdkGLExtContextImplClass))

typedef struct _GdkGLExtContextImpl
{
  GObject parent;
} GdkGLExtContextImpl;

typedef struct _GdkGLExtContextImplClass
{
  GObjectClass parent_class;

  gboolean       (*copy_glext_context_impl) (GdkGLExtContext  *glextcontext,
                                          GdkGLExtContext  *src,
                                          unsigned long  mask);
  GdkGLDrawable* (*get_gl_drawable) (GdkGLExtContext *glextcontext);
  GdkGLConfig*   (*get_gl_config) (GdkGLExtContext *glextcontext);
  GdkGLExtContext*  (*get_share_list) (GdkGLExtContext *glextcontext);
  gboolean       (*is_direct) (GdkGLExtContext *glextcontext);
  int            (*get_render_type) (GdkGLExtContext *glextcontext);
  gboolean       (*make_current)(GdkGLExtContext  *glextcontext,
                                 GdkGLDrawable *draw,
                                 GdkGLDrawable *read);
  void           (*make_uncurrent)(GdkGLExtContext *glextcontext);

} GdkGLExtContextImplClass;

GType gdk_glext_context_impl_get_type (void);

G_END_DECLS

#endif /* __GDK_GLEXT_CONTEXT_IMPL_H__ */
