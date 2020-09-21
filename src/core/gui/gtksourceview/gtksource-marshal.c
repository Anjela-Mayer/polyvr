/* This file is generated by glib-genmarshal, do not modify it. This code is licensed under the same license as the containing project. Note that it links to GLib, so must comply with the LGPL linking clauses. */
#include "gtksource-marshal.h"

#include <glib-object.h>

#ifdef G_ENABLE_DEBUG
#define g_marshal_value_peek_boolean(v)  g_value_get_boolean (v)
#define g_marshal_value_peek_char(v)     g_value_get_schar (v)
#define g_marshal_value_peek_uchar(v)    g_value_get_uchar (v)
#define g_marshal_value_peek_int(v)      g_value_get_int (v)
#define g_marshal_value_peek_uint(v)     g_value_get_uint (v)
#define g_marshal_value_peek_long(v)     g_value_get_long (v)
#define g_marshal_value_peek_ulong(v)    g_value_get_ulong (v)
#define g_marshal_value_peek_int64(v)    g_value_get_int64 (v)
#define g_marshal_value_peek_uint64(v)   g_value_get_uint64 (v)
#define g_marshal_value_peek_enum(v)     g_value_get_enum (v)
#define g_marshal_value_peek_flags(v)    g_value_get_flags (v)
#define g_marshal_value_peek_float(v)    g_value_get_float (v)
#define g_marshal_value_peek_double(v)   g_value_get_double (v)
#define g_marshal_value_peek_string(v)   (char*) g_value_get_string (v)
#define g_marshal_value_peek_param(v)    g_value_get_param (v)
#define g_marshal_value_peek_boxed(v)    g_value_get_boxed (v)
#define g_marshal_value_peek_pointer(v)  g_value_get_pointer (v)
#define g_marshal_value_peek_object(v)   g_value_get_object (v)
#define g_marshal_value_peek_variant(v)  g_value_get_variant (v)
#else /* !G_ENABLE_DEBUG */
/* WARNING: This code accesses GValues directly, which is UNSUPPORTED API.
 *          Do not access GValues directly in your code. Instead, use the
 *          g_value_get_*() functions
 */
#define g_marshal_value_peek_boolean(v)  (v)->data[0].v_int
#define g_marshal_value_peek_char(v)     (v)->data[0].v_int
#define g_marshal_value_peek_uchar(v)    (v)->data[0].v_uint
#define g_marshal_value_peek_int(v)      (v)->data[0].v_int
#define g_marshal_value_peek_uint(v)     (v)->data[0].v_uint
#define g_marshal_value_peek_long(v)     (v)->data[0].v_long
#define g_marshal_value_peek_ulong(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_int64(v)    (v)->data[0].v_int64
#define g_marshal_value_peek_uint64(v)   (v)->data[0].v_uint64
#define g_marshal_value_peek_enum(v)     (v)->data[0].v_long
#define g_marshal_value_peek_flags(v)    (v)->data[0].v_ulong
#define g_marshal_value_peek_float(v)    (v)->data[0].v_float
#define g_marshal_value_peek_double(v)   (v)->data[0].v_double
#define g_marshal_value_peek_string(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_param(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_boxed(v)    (v)->data[0].v_pointer
#define g_marshal_value_peek_pointer(v)  (v)->data[0].v_pointer
#define g_marshal_value_peek_object(v)   (v)->data[0].v_pointer
#define g_marshal_value_peek_variant(v)  (v)->data[0].v_pointer
#endif /* !G_ENABLE_DEBUG */

void
_gtk_source_marshal_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT (GClosure     *closure,
                                                         GValue       *return_value,
                                                         guint         n_param_values,
                                                         const GValue *param_values,
                                                         gpointer      invocation_hint G_GNUC_UNUSED,
                                                         gpointer      marshal_data)
{
  typedef gboolean (*GMarshalFunc_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT) (gpointer data1,
                                                                        gpointer arg1,
                                                                        gpointer arg2,
                                                                        gint arg3,
                                                                        gint arg4,
                                                                        gpointer arg5,
                                                                        gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT callback;
  gboolean v_return;

  g_return_if_fail (return_value != NULL);
  g_return_if_fail (n_param_values == 6);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       g_marshal_value_peek_boxed (param_values + 1),
                       g_marshal_value_peek_boxed (param_values + 2),
                       g_marshal_value_peek_int (param_values + 3),
                       g_marshal_value_peek_int (param_values + 4),
                       g_marshal_value_peek_object (param_values + 5),
                       data2);

  g_value_set_boolean (return_value, v_return);
}

void
_gtk_source_marshal_BOOLEAN__BOXED_BOXED_INT_INT_OBJECTv (GClosure *closure,
                                                          GValue   *return_value,
                                                          gpointer  instance,
                                                          va_list   args,
                                                          gpointer  marshal_data,
                                                          int       n_params,
                                                          GType    *param_types)
{
  typedef gboolean (*GMarshalFunc_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT) (gpointer data1,
                                                                        gpointer arg1,
                                                                        gpointer arg2,
                                                                        gint arg3,
                                                                        gint arg4,
                                                                        gpointer arg5,
                                                                        gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT callback;
  gboolean v_return;
  gpointer arg0;
  gpointer arg1;
  gint arg2;
  gint arg3;
  gpointer arg4;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    arg1 = g_boxed_copy (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  arg2 = (gint) va_arg (args_copy, gint);
  arg3 = (gint) va_arg (args_copy, gint);
  arg4 = (gpointer) va_arg (args_copy, gpointer);
  if (arg4 != NULL)
    arg4 = g_object_ref (arg4);
  va_end (args_copy);

  g_return_if_fail (return_value != NULL);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_BOOLEAN__BOXED_BOXED_INT_INT_OBJECT) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       arg0,
                       arg1,
                       arg2,
                       arg3,
                       arg4,
                       data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    g_boxed_free (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  if (arg4 != NULL)
    g_object_unref (arg4);

  g_value_set_boolean (return_value, v_return);
}

void
_gtk_source_marshal_BOOLEAN__BOXED_BOXED_BOXED (GClosure     *closure,
                                                GValue       *return_value,
                                                guint         n_param_values,
                                                const GValue *param_values,
                                                gpointer      invocation_hint G_GNUC_UNUSED,
                                                gpointer      marshal_data)
{
  typedef gboolean (*GMarshalFunc_BOOLEAN__BOXED_BOXED_BOXED) (gpointer data1,
                                                               gpointer arg1,
                                                               gpointer arg2,
                                                               gpointer arg3,
                                                               gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_BOOLEAN__BOXED_BOXED_BOXED callback;
  gboolean v_return;

  g_return_if_fail (return_value != NULL);
  g_return_if_fail (n_param_values == 4);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_BOOLEAN__BOXED_BOXED_BOXED) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       g_marshal_value_peek_boxed (param_values + 1),
                       g_marshal_value_peek_boxed (param_values + 2),
                       g_marshal_value_peek_boxed (param_values + 3),
                       data2);

  g_value_set_boolean (return_value, v_return);
}

void
_gtk_source_marshal_BOOLEAN__BOXED_BOXED_BOXEDv (GClosure *closure,
                                                 GValue   *return_value,
                                                 gpointer  instance,
                                                 va_list   args,
                                                 gpointer  marshal_data,
                                                 int       n_params,
                                                 GType    *param_types)
{
  typedef gboolean (*GMarshalFunc_BOOLEAN__BOXED_BOXED_BOXED) (gpointer data1,
                                                               gpointer arg1,
                                                               gpointer arg2,
                                                               gpointer arg3,
                                                               gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_BOOLEAN__BOXED_BOXED_BOXED callback;
  gboolean v_return;
  gpointer arg0;
  gpointer arg1;
  gpointer arg2;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    arg1 = g_boxed_copy (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  arg2 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[2] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg2 != NULL)
    arg2 = g_boxed_copy (param_types[2] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg2);
  va_end (args_copy);

  g_return_if_fail (return_value != NULL);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_BOOLEAN__BOXED_BOXED_BOXED) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       arg0,
                       arg1,
                       arg2,
                       data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    g_boxed_free (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  if ((param_types[2] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg2 != NULL)
    g_boxed_free (param_types[2] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg2);

  g_value_set_boolean (return_value, v_return);
}

void
_gtk_source_marshal_STRING__OBJECT (GClosure     *closure,
                                    GValue       *return_value,
                                    guint         n_param_values,
                                    const GValue *param_values,
                                    gpointer      invocation_hint G_GNUC_UNUSED,
                                    gpointer      marshal_data)
{
  typedef gchar* (*GMarshalFunc_STRING__OBJECT) (gpointer data1,
                                                 gpointer arg1,
                                                 gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_STRING__OBJECT callback;
  gchar* v_return;

  g_return_if_fail (return_value != NULL);
  g_return_if_fail (n_param_values == 2);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_STRING__OBJECT) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       g_marshal_value_peek_object (param_values + 1),
                       data2);

  g_value_take_string (return_value, v_return);
}

void
_gtk_source_marshal_STRING__OBJECTv (GClosure *closure,
                                     GValue   *return_value,
                                     gpointer  instance,
                                     va_list   args,
                                     gpointer  marshal_data,
                                     int       n_params,
                                     GType    *param_types)
{
  typedef gchar* (*GMarshalFunc_STRING__OBJECT) (gpointer data1,
                                                 gpointer arg1,
                                                 gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_STRING__OBJECT callback;
  gchar* v_return;
  gpointer arg0;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if (arg0 != NULL)
    arg0 = g_object_ref (arg0);
  va_end (args_copy);

  g_return_if_fail (return_value != NULL);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_STRING__OBJECT) (marshal_data ? marshal_data : cc->callback);

  v_return = callback (data1,
                       arg0,
                       data2);
  if (arg0 != NULL)
    g_object_unref (arg0);

  g_value_take_string (return_value, v_return);
}

void
_gtk_source_marshal_VOID__BOXED_BOXED (GClosure     *closure,
                                       GValue       *return_value G_GNUC_UNUSED,
                                       guint         n_param_values,
                                       const GValue *param_values,
                                       gpointer      invocation_hint G_GNUC_UNUSED,
                                       gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__BOXED_BOXED) (gpointer data1,
                                                  gpointer arg1,
                                                  gpointer arg2,
                                                  gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_BOXED callback;

  g_return_if_fail (n_param_values == 3);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_BOXED) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_boxed (param_values + 1),
            g_marshal_value_peek_boxed (param_values + 2),
            data2);
}

void
_gtk_source_marshal_VOID__BOXED_BOXEDv (GClosure *closure,
                                        GValue   *return_value G_GNUC_UNUSED,
                                        gpointer  instance,
                                        va_list   args,
                                        gpointer  marshal_data,
                                        int       n_params,
                                        GType    *param_types)
{
  typedef void (*GMarshalFunc_VOID__BOXED_BOXED) (gpointer data1,
                                                  gpointer arg1,
                                                  gpointer arg2,
                                                  gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_BOXED callback;
  gpointer arg0;
  gpointer arg1;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    arg1 = g_boxed_copy (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  va_end (args_copy);


  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_BOXED) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            arg0,
            arg1,
            data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    g_boxed_free (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
}

void
_gtk_source_marshal_VOID__BOXED_BOXED_BOXED (GClosure     *closure,
                                             GValue       *return_value G_GNUC_UNUSED,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint G_GNUC_UNUSED,
                                             gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__BOXED_BOXED_BOXED) (gpointer data1,
                                                        gpointer arg1,
                                                        gpointer arg2,
                                                        gpointer arg3,
                                                        gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_BOXED_BOXED callback;

  g_return_if_fail (n_param_values == 4);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_BOXED_BOXED) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_boxed (param_values + 1),
            g_marshal_value_peek_boxed (param_values + 2),
            g_marshal_value_peek_boxed (param_values + 3),
            data2);
}

void
_gtk_source_marshal_VOID__BOXED_BOXED_BOXEDv (GClosure *closure,
                                              GValue   *return_value G_GNUC_UNUSED,
                                              gpointer  instance,
                                              va_list   args,
                                              gpointer  marshal_data,
                                              int       n_params,
                                              GType    *param_types)
{
  typedef void (*GMarshalFunc_VOID__BOXED_BOXED_BOXED) (gpointer data1,
                                                        gpointer arg1,
                                                        gpointer arg2,
                                                        gpointer arg3,
                                                        gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_BOXED_BOXED callback;
  gpointer arg0;
  gpointer arg1;
  gpointer arg2;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    arg1 = g_boxed_copy (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  arg2 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[2] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg2 != NULL)
    arg2 = g_boxed_copy (param_types[2] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg2);
  va_end (args_copy);


  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_BOXED_BOXED) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            arg0,
            arg1,
            arg2,
            data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    g_boxed_free (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  if ((param_types[2] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg2 != NULL)
    g_boxed_free (param_types[2] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg2);
}

void
_gtk_source_marshal_VOID__BOXED_ENUM (GClosure     *closure,
                                      GValue       *return_value G_GNUC_UNUSED,
                                      guint         n_param_values,
                                      const GValue *param_values,
                                      gpointer      invocation_hint G_GNUC_UNUSED,
                                      gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__BOXED_ENUM) (gpointer data1,
                                                 gpointer arg1,
                                                 gint arg2,
                                                 gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_ENUM callback;

  g_return_if_fail (n_param_values == 3);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_ENUM) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_boxed (param_values + 1),
            g_marshal_value_peek_enum (param_values + 2),
            data2);
}

void
_gtk_source_marshal_VOID__BOXED_ENUMv (GClosure *closure,
                                       GValue   *return_value G_GNUC_UNUSED,
                                       gpointer  instance,
                                       va_list   args,
                                       gpointer  marshal_data,
                                       int       n_params,
                                       GType    *param_types)
{
  typedef void (*GMarshalFunc_VOID__BOXED_ENUM) (gpointer data1,
                                                 gpointer arg1,
                                                 gint arg2,
                                                 gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_ENUM callback;
  gpointer arg0;
  gint arg1;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gint) va_arg (args_copy, gint);
  va_end (args_copy);


  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_ENUM) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            arg0,
            arg1,
            data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
}

void
_gtk_source_marshal_VOID__BOXED_INT (GClosure     *closure,
                                     GValue       *return_value G_GNUC_UNUSED,
                                     guint         n_param_values,
                                     const GValue *param_values,
                                     gpointer      invocation_hint G_GNUC_UNUSED,
                                     gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__BOXED_INT) (gpointer data1,
                                                gpointer arg1,
                                                gint arg2,
                                                gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_INT callback;

  g_return_if_fail (n_param_values == 3);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_INT) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_boxed (param_values + 1),
            g_marshal_value_peek_int (param_values + 2),
            data2);
}

void
_gtk_source_marshal_VOID__BOXED_INTv (GClosure *closure,
                                      GValue   *return_value G_GNUC_UNUSED,
                                      gpointer  instance,
                                      va_list   args,
                                      gpointer  marshal_data,
                                      int       n_params,
                                      GType    *param_types)
{
  typedef void (*GMarshalFunc_VOID__BOXED_INT) (gpointer data1,
                                                gpointer arg1,
                                                gint arg2,
                                                gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_INT callback;
  gpointer arg0;
  gint arg1;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gint) va_arg (args_copy, gint);
  va_end (args_copy);


  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_INT) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            arg0,
            arg1,
            data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
}

void
_gtk_source_marshal_VOID__ENUM_INT (GClosure     *closure,
                                    GValue       *return_value G_GNUC_UNUSED,
                                    guint         n_param_values,
                                    const GValue *param_values,
                                    gpointer      invocation_hint G_GNUC_UNUSED,
                                    gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__ENUM_INT) (gpointer data1,
                                               gint arg1,
                                               gint arg2,
                                               gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__ENUM_INT callback;

  g_return_if_fail (n_param_values == 3);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__ENUM_INT) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_enum (param_values + 1),
            g_marshal_value_peek_int (param_values + 2),
            data2);
}

void
_gtk_source_marshal_VOID__ENUM_INTv (GClosure *closure,
                                     GValue   *return_value G_GNUC_UNUSED,
                                     gpointer  instance,
                                     va_list   args,
                                     gpointer  marshal_data,
                                     int       n_params,
                                     GType    *param_types)
{
  typedef void (*GMarshalFunc_VOID__ENUM_INT) (gpointer data1,
                                               gint arg1,
                                               gint arg2,
                                               gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__ENUM_INT callback;
  gint arg0;
  gint arg1;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gint) va_arg (args_copy, gint);
  arg1 = (gint) va_arg (args_copy, gint);
  va_end (args_copy);


  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__ENUM_INT) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            arg0,
            arg1,
            data2);

}

void
_gtk_source_marshal_VOID__BOXED_BOXED_FLAGS (GClosure     *closure,
                                             GValue       *return_value G_GNUC_UNUSED,
                                             guint         n_param_values,
                                             const GValue *param_values,
                                             gpointer      invocation_hint G_GNUC_UNUSED,
                                             gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__BOXED_BOXED_FLAGS) (gpointer data1,
                                                        gpointer arg1,
                                                        gpointer arg2,
                                                        guint arg3,
                                                        gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_BOXED_FLAGS callback;

  g_return_if_fail (n_param_values == 4);

  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_peek_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_peek_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_BOXED_FLAGS) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            g_marshal_value_peek_boxed (param_values + 1),
            g_marshal_value_peek_boxed (param_values + 2),
            g_marshal_value_peek_flags (param_values + 3),
            data2);
}

void
_gtk_source_marshal_VOID__BOXED_BOXED_FLAGSv (GClosure *closure,
                                              GValue   *return_value G_GNUC_UNUSED,
                                              gpointer  instance,
                                              va_list   args,
                                              gpointer  marshal_data,
                                              int       n_params,
                                              GType    *param_types)
{
  typedef void (*GMarshalFunc_VOID__BOXED_BOXED_FLAGS) (gpointer data1,
                                                        gpointer arg1,
                                                        gpointer arg2,
                                                        guint arg3,
                                                        gpointer data2);
  GCClosure *cc = (GCClosure *) closure;
  gpointer data1, data2;
  GMarshalFunc_VOID__BOXED_BOXED_FLAGS callback;
  gpointer arg0;
  gpointer arg1;
  guint arg2;
  va_list args_copy;

  G_VA_COPY (args_copy, args);
  arg0 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    arg0 = g_boxed_copy (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  arg1 = (gpointer) va_arg (args_copy, gpointer);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    arg1 = g_boxed_copy (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
  arg2 = (guint) va_arg (args_copy, guint);
  va_end (args_copy);


  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = instance;
    }
  else
    {
      data1 = instance;
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__BOXED_BOXED_FLAGS) (marshal_data ? marshal_data : cc->callback);

  callback (data1,
            arg0,
            arg1,
            arg2,
            data2);
  if ((param_types[0] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg0 != NULL)
    g_boxed_free (param_types[0] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg0);
  if ((param_types[1] & G_SIGNAL_TYPE_STATIC_SCOPE) == 0 && arg1 != NULL)
    g_boxed_free (param_types[1] & ~G_SIGNAL_TYPE_STATIC_SCOPE, arg1);
}

