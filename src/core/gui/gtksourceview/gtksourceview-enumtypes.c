/* This file is generated by glib-mkenums, do not modify it. This code is licensed under the same license as the containing project. Note that it links to GLib, so must comply with the LGPL linking clauses. */

#define GTK_SOURCE_H_INSIDE

#include "gtksourceview-enumtypes.h"
#include "gtksourceview.h"
#include "gtksourceautocleanups.h"
#include "gtksourcebuffer.h"
#include "gtksourcecompletion.h"
#include "gtksourcecompletioncontext.h"
#include "gtksourcecompletioninfo.h"
#include "gtksourcecompletionitem.h"
#include "gtksourcecompletionproposal.h"
#include "gtksourcecompletionprovider.h"
#include "gtksourceencoding.h"
#include "gtksourcefile.h"
#include "gtksourcefileloader.h"
#include "gtksourcefilesaver.h"
#include "gtksourcegutter.h"
#include "gtksourcegutterrenderer.h"
#include "gtksourcegutterrendererpixbuf.h"
#include "gtksourcegutterrenderertext.h"
#include "gtksourcelanguage.h"
#include "gtksourcelanguagemanager.h"
#include "gtksourcemap.h"
#include "gtksourcemark.h"
#include "gtksourcemarkattributes.h"
#include "gtksourceprintcompositor.h"
#include "gtksourceregion.h"
#include "gtksourcesearchcontext.h"
#include "gtksourcesearchsettings.h"
#include "gtksourcespacedrawer.h"
#include "gtksourcestyle.h"
#include "gtksourcestylescheme.h"
#include "gtksourcestyleschemechooser.h"
#include "gtksourcestyleschemechooserbutton.h"
#include "gtksourcestyleschemechooserwidget.h"
#include "gtksourcestyleschememanager.h"
#include "gtksourcetag.h"
#include "gtksourcetypes.h"
#include "gtksourceundomanager.h"
#include "gtksourceutils.h"
#include "gtksourceview.h"

#define C_ENUM(v) ((gint) v)
#define C_FLAGS(v) ((guint) v)

/* enumerations from "gtksourcebuffer.h" */

GType
gtk_source_bracket_match_type_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_BRACKET_MATCH_NONE), "GTK_SOURCE_BRACKET_MATCH_NONE", "none" },
    { C_ENUM(GTK_SOURCE_BRACKET_MATCH_OUT_OF_RANGE), "GTK_SOURCE_BRACKET_MATCH_OUT_OF_RANGE", "out-of-range" },
    { C_ENUM(GTK_SOURCE_BRACKET_MATCH_NOT_FOUND), "GTK_SOURCE_BRACKET_MATCH_NOT_FOUND", "not-found" },
    { C_ENUM(GTK_SOURCE_BRACKET_MATCH_FOUND), "GTK_SOURCE_BRACKET_MATCH_FOUND", "found" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceBracketMatchType", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_change_case_type_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_CHANGE_CASE_LOWER), "GTK_SOURCE_CHANGE_CASE_LOWER", "lower" },
    { C_ENUM(GTK_SOURCE_CHANGE_CASE_UPPER), "GTK_SOURCE_CHANGE_CASE_UPPER", "upper" },
    { C_ENUM(GTK_SOURCE_CHANGE_CASE_TOGGLE), "GTK_SOURCE_CHANGE_CASE_TOGGLE", "toggle" },
    { C_ENUM(GTK_SOURCE_CHANGE_CASE_TITLE), "GTK_SOURCE_CHANGE_CASE_TITLE", "title" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceChangeCaseType", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_sort_flags_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GFlagsValue values[] = {
    { C_FLAGS(GTK_SOURCE_SORT_FLAGS_NONE), "GTK_SOURCE_SORT_FLAGS_NONE", "none" },
    { C_FLAGS(GTK_SOURCE_SORT_FLAGS_CASE_SENSITIVE), "GTK_SOURCE_SORT_FLAGS_CASE_SENSITIVE", "case-sensitive" },
    { C_FLAGS(GTK_SOURCE_SORT_FLAGS_REVERSE_ORDER), "GTK_SOURCE_SORT_FLAGS_REVERSE_ORDER", "reverse-order" },
    { C_FLAGS(GTK_SOURCE_SORT_FLAGS_REMOVE_DUPLICATES), "GTK_SOURCE_SORT_FLAGS_REMOVE_DUPLICATES", "remove-duplicates" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_flags_register_static ("GtkSourceSortFlags", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcecompletion.h" */

GType
gtk_source_completion_error_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_COMPLETION_ERROR_ALREADY_BOUND), "GTK_SOURCE_COMPLETION_ERROR_ALREADY_BOUND", "already-bound" },
    { C_ENUM(GTK_SOURCE_COMPLETION_ERROR_NOT_BOUND), "GTK_SOURCE_COMPLETION_ERROR_NOT_BOUND", "not-bound" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceCompletionError", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcecompletioncontext.h" */

GType
gtk_source_completion_activation_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GFlagsValue values[] = {
    { C_FLAGS(GTK_SOURCE_COMPLETION_ACTIVATION_NONE), "GTK_SOURCE_COMPLETION_ACTIVATION_NONE", "none" },
    { C_FLAGS(GTK_SOURCE_COMPLETION_ACTIVATION_INTERACTIVE), "GTK_SOURCE_COMPLETION_ACTIVATION_INTERACTIVE", "interactive" },
    { C_FLAGS(GTK_SOURCE_COMPLETION_ACTIVATION_USER_REQUESTED), "GTK_SOURCE_COMPLETION_ACTIVATION_USER_REQUESTED", "user-requested" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_flags_register_static ("GtkSourceCompletionActivation", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcefile.h" */

GType
gtk_source_newline_type_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_NEWLINE_TYPE_LF), "GTK_SOURCE_NEWLINE_TYPE_LF", "lf" },
    { C_ENUM(GTK_SOURCE_NEWLINE_TYPE_CR), "GTK_SOURCE_NEWLINE_TYPE_CR", "cr" },
    { C_ENUM(GTK_SOURCE_NEWLINE_TYPE_CR_LF), "GTK_SOURCE_NEWLINE_TYPE_CR_LF", "cr-lf" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceNewlineType", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_compression_type_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_COMPRESSION_TYPE_NONE), "GTK_SOURCE_COMPRESSION_TYPE_NONE", "none" },
    { C_ENUM(GTK_SOURCE_COMPRESSION_TYPE_GZIP), "GTK_SOURCE_COMPRESSION_TYPE_GZIP", "gzip" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceCompressionType", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcefileloader.h" */

GType
gtk_source_file_loader_error_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_FILE_LOADER_ERROR_TOO_BIG), "GTK_SOURCE_FILE_LOADER_ERROR_TOO_BIG", "too-big" },
    { C_ENUM(GTK_SOURCE_FILE_LOADER_ERROR_ENCODING_AUTO_DETECTION_FAILED), "GTK_SOURCE_FILE_LOADER_ERROR_ENCODING_AUTO_DETECTION_FAILED", "encoding-auto-detection-failed" },
    { C_ENUM(GTK_SOURCE_FILE_LOADER_ERROR_CONVERSION_FALLBACK), "GTK_SOURCE_FILE_LOADER_ERROR_CONVERSION_FALLBACK", "conversion-fallback" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceFileLoaderError", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcefilesaver.h" */

GType
gtk_source_file_saver_error_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_FILE_SAVER_ERROR_INVALID_CHARS), "GTK_SOURCE_FILE_SAVER_ERROR_INVALID_CHARS", "invalid-chars" },
    { C_ENUM(GTK_SOURCE_FILE_SAVER_ERROR_EXTERNALLY_MODIFIED), "GTK_SOURCE_FILE_SAVER_ERROR_EXTERNALLY_MODIFIED", "externally-modified" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceFileSaverError", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_file_saver_flags_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GFlagsValue values[] = {
    { C_FLAGS(GTK_SOURCE_FILE_SAVER_FLAGS_NONE), "GTK_SOURCE_FILE_SAVER_FLAGS_NONE", "none" },
    { C_FLAGS(GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_INVALID_CHARS), "GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_INVALID_CHARS", "ignore-invalid-chars" },
    { C_FLAGS(GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_MODIFICATION_TIME), "GTK_SOURCE_FILE_SAVER_FLAGS_IGNORE_MODIFICATION_TIME", "ignore-modification-time" },
    { C_FLAGS(GTK_SOURCE_FILE_SAVER_FLAGS_CREATE_BACKUP), "GTK_SOURCE_FILE_SAVER_FLAGS_CREATE_BACKUP", "create-backup" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_flags_register_static ("GtkSourceFileSaverFlags", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcegutterrenderer.h" */

GType
gtk_source_gutter_renderer_state_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GFlagsValue values[] = {
    { C_FLAGS(GTK_SOURCE_GUTTER_RENDERER_STATE_NORMAL), "GTK_SOURCE_GUTTER_RENDERER_STATE_NORMAL", "normal" },
    { C_FLAGS(GTK_SOURCE_GUTTER_RENDERER_STATE_CURSOR), "GTK_SOURCE_GUTTER_RENDERER_STATE_CURSOR", "cursor" },
    { C_FLAGS(GTK_SOURCE_GUTTER_RENDERER_STATE_PRELIT), "GTK_SOURCE_GUTTER_RENDERER_STATE_PRELIT", "prelit" },
    { C_FLAGS(GTK_SOURCE_GUTTER_RENDERER_STATE_SELECTED), "GTK_SOURCE_GUTTER_RENDERER_STATE_SELECTED", "selected" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_flags_register_static ("GtkSourceGutterRendererState", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_gutter_renderer_alignment_mode_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_GUTTER_RENDERER_ALIGNMENT_MODE_CELL), "GTK_SOURCE_GUTTER_RENDERER_ALIGNMENT_MODE_CELL", "cell" },
    { C_ENUM(GTK_SOURCE_GUTTER_RENDERER_ALIGNMENT_MODE_FIRST), "GTK_SOURCE_GUTTER_RENDERER_ALIGNMENT_MODE_FIRST", "first" },
    { C_ENUM(GTK_SOURCE_GUTTER_RENDERER_ALIGNMENT_MODE_LAST), "GTK_SOURCE_GUTTER_RENDERER_ALIGNMENT_MODE_LAST", "last" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceGutterRendererAlignmentMode", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourcespacedrawer.h" */

GType
gtk_source_space_type_flags_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GFlagsValue values[] = {
    { C_FLAGS(GTK_SOURCE_SPACE_TYPE_NONE), "GTK_SOURCE_SPACE_TYPE_NONE", "none" },
    { C_FLAGS(GTK_SOURCE_SPACE_TYPE_SPACE), "GTK_SOURCE_SPACE_TYPE_SPACE", "space" },
    { C_FLAGS(GTK_SOURCE_SPACE_TYPE_TAB), "GTK_SOURCE_SPACE_TYPE_TAB", "tab" },
    { C_FLAGS(GTK_SOURCE_SPACE_TYPE_NEWLINE), "GTK_SOURCE_SPACE_TYPE_NEWLINE", "newline" },
    { C_FLAGS(GTK_SOURCE_SPACE_TYPE_NBSP), "GTK_SOURCE_SPACE_TYPE_NBSP", "nbsp" },
    { C_FLAGS(GTK_SOURCE_SPACE_TYPE_ALL), "GTK_SOURCE_SPACE_TYPE_ALL", "all" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_flags_register_static ("GtkSourceSpaceTypeFlags", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_space_location_flags_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GFlagsValue values[] = {
    { C_FLAGS(GTK_SOURCE_SPACE_LOCATION_NONE), "GTK_SOURCE_SPACE_LOCATION_NONE", "none" },
    { C_FLAGS(GTK_SOURCE_SPACE_LOCATION_LEADING), "GTK_SOURCE_SPACE_LOCATION_LEADING", "leading" },
    { C_FLAGS(GTK_SOURCE_SPACE_LOCATION_INSIDE_TEXT), "GTK_SOURCE_SPACE_LOCATION_INSIDE_TEXT", "inside-text" },
    { C_FLAGS(GTK_SOURCE_SPACE_LOCATION_TRAILING), "GTK_SOURCE_SPACE_LOCATION_TRAILING", "trailing" },
    { C_FLAGS(GTK_SOURCE_SPACE_LOCATION_ALL), "GTK_SOURCE_SPACE_LOCATION_ALL", "all" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_flags_register_static ("GtkSourceSpaceLocationFlags", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

/* enumerations from "gtksourceview.h" */

GType
gtk_source_view_gutter_position_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_VIEW_GUTTER_POSITION_LINES), "GTK_SOURCE_VIEW_GUTTER_POSITION_LINES", "lines" },
    { C_ENUM(GTK_SOURCE_VIEW_GUTTER_POSITION_MARKS), "GTK_SOURCE_VIEW_GUTTER_POSITION_MARKS", "marks" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceViewGutterPosition", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_smart_home_end_type_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_SMART_HOME_END_DISABLED), "GTK_SOURCE_SMART_HOME_END_DISABLED", "disabled" },
    { C_ENUM(GTK_SOURCE_SMART_HOME_END_BEFORE), "GTK_SOURCE_SMART_HOME_END_BEFORE", "before" },
    { C_ENUM(GTK_SOURCE_SMART_HOME_END_AFTER), "GTK_SOURCE_SMART_HOME_END_AFTER", "after" },
    { C_ENUM(GTK_SOURCE_SMART_HOME_END_ALWAYS), "GTK_SOURCE_SMART_HOME_END_ALWAYS", "always" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceSmartHomeEndType", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

GType
gtk_source_background_pattern_type_get_type (void)
{
  static volatile gsize gtype_id = 0;
  static const GEnumValue values[] = {
    { C_ENUM(GTK_SOURCE_BACKGROUND_PATTERN_TYPE_NONE), "GTK_SOURCE_BACKGROUND_PATTERN_TYPE_NONE", "none" },
    { C_ENUM(GTK_SOURCE_BACKGROUND_PATTERN_TYPE_GRID), "GTK_SOURCE_BACKGROUND_PATTERN_TYPE_GRID", "grid" },
    { 0, NULL, NULL }
  };
  if (g_once_init_enter (&gtype_id)) {
    GType new_type = g_enum_register_static ("GtkSourceBackgroundPatternType", values);
    g_once_init_leave (&gtype_id, new_type);
  }
  return (GType) gtype_id;
}

static const GFlagsValue _gtk_source_draw_spaces_flags_values[] = {
  { GTK_SOURCE_DRAW_SPACES_SPACE, "GTK_SOURCE_DRAW_SPACES_SPACE", "space" },
  { GTK_SOURCE_DRAW_SPACES_TAB, "GTK_SOURCE_DRAW_SPACES_TAB", "tab" },
  { GTK_SOURCE_DRAW_SPACES_NEWLINE, "GTK_SOURCE_DRAW_SPACES_NEWLINE", "newline" },
  { GTK_SOURCE_DRAW_SPACES_NBSP, "GTK_SOURCE_DRAW_SPACES_NBSP", "nbsp" },
  { GTK_SOURCE_DRAW_SPACES_LEADING, "GTK_SOURCE_DRAW_SPACES_LEADING", "leading" },
  { GTK_SOURCE_DRAW_SPACES_TEXT, "GTK_SOURCE_DRAW_SPACES_TEXT", "text" },
  { GTK_SOURCE_DRAW_SPACES_TRAILING, "GTK_SOURCE_DRAW_SPACES_TRAILING", "trailing" },
  { GTK_SOURCE_DRAW_SPACES_ALL, "GTK_SOURCE_DRAW_SPACES_ALL", "all" },
  { 0, NULL, NULL }
};

GType
gtk_source_draw_spaces_flags_get_type (void)
{
  static GType type = 0;

  if (!type)
    type = g_flags_register_static ("GtkSourceDrawSpacesFlags", _gtk_source_draw_spaces_flags_values);

  return type;
}

/* Generated data ends here */
