#include "gdk/gdk.h"
#include "glib-object.h"
#include "gtk/gtkcssprovider.h"
#include <gtk/gtk.h>

#define CSS_PATH "src/style.css"

void applyCss() {
  GtkCssProvider *provider = gtk_css_provider_new();
  GdkDisplay *display = gdk_display_get_default();
  GdkScreen *screen = gdk_screen_get_default();

  GError *error = NULL;
  gtk_css_provider_load_from_path(provider, CSS_PATH, &error);

  if (error) {
    g_warning("Could not load CSS: %s", error->message);
    g_clear_error(&error);
  }

  gtk_style_context_add_provider_for_screen(
      screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  
  g_object_unref(provider);
}
