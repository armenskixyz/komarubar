#include "gdk/gdk.h"
#include "glib-object.h"
#include "gtk/gtkcssprovider.h"
#include <gtk/gtk.h>

void getCssPath(char* buffer, int size) {
  //TODO: Add custom css paths
  char* home_path = getenv("HOME");
  snprintf(buffer, size, "%s/.config/komarubar/style.css", home_path );
}

void applyCss() {
  GtkCssProvider *provider = gtk_css_provider_new();
  GdkDisplay *display = gdk_display_get_default();
  GdkScreen *screen = gdk_screen_get_default();

  char css_path[1024];
  getCssPath(css_path, 1024);

  g_print("%s", css_path);

  GError *error = NULL;
  gtk_css_provider_load_from_path(provider, css_path, &error);

  if (error) {
    g_warning("Could not load CSS: %s", error->message);
    g_clear_error(&error);
  }

  gtk_style_context_add_provider_for_screen(
      screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  
  g_object_unref(provider);
}
