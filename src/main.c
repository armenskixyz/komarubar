#include <fcntl.h>
#include <gtk-layer-shell/gtk-layer-shell.h>
#include <gtk/gtk.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "css.h"
#include "gdk/gdk.h"
#include "glib.h"

#include "modules/clock.h"
#include "modules/workspaces.h"
#include "modules/battery-display.h"

#define BAR_HEIGHT 30

#define STANDARD_SPACING 5

void hyprlandHandleEvent(const char *event_data) {
  // g_print("%s\n", event_data);
  const char *id;
  if (g_str_has_prefix(event_data, "workspace>>")) {
    id = event_data + 11;
    workspaceSetActive(atoi(id));
  }
  if (g_str_has_prefix(event_data, "focusedmon>>")) {
    id = strchr(event_data, ',') + 1;
    workspaceSetActive(atoi(id));
  }
}
gboolean onHyprlandEvent(GIOChannel *source, GIOCondition condition,
                         gpointer data) {
  GString *s = g_string_new(NULL);
  GError *error = NULL;

  if (g_io_channel_read_line_string(source, s, NULL, &error) ==
      G_IO_STATUS_NORMAL) {
    g_strchomp(s->str);
    hyprlandHandleEvent(s->str);
  }
  return TRUE;
}

GtkWidget *mainBoxInit(GtkContainer *container) {
  GtkWidget *main_box =
      gtk_box_new(GTK_ORIENTATION_HORIZONTAL, STANDARD_SPACING);
  gtk_container_add(container, main_box);
  return main_box;
}

void barInit(GdkMonitor *monitor) {

  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  gtk_widget_set_size_request(window, -1, BAR_HEIGHT);

  gtk_layer_init_for_window(GTK_WINDOW(window));
  gtk_layer_set_monitor(GTK_WINDOW(window), monitor);

  gtk_layer_set_layer(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP);

  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_TOP, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_LEFT, TRUE);
  gtk_layer_set_anchor(GTK_WINDOW(window), GTK_LAYER_SHELL_EDGE_RIGHT, TRUE);

  gtk_layer_auto_exclusive_zone_enable(GTK_WINDOW(window));

  GtkWidget *main_box = mainBoxInit(GTK_CONTAINER(window));

  GtkWidget *workspaces_box = workspacesInit();

  gtk_box_pack_start(GTK_BOX(main_box), workspaces_box, FALSE, FALSE, 10);

  GtkWidget *center_box =
      gtk_box_new(GTK_ORIENTATION_HORIZONTAL, STANDARD_SPACING);

  gtk_box_set_center_widget(GTK_BOX(main_box), center_box);

  gtk_widget_set_halign(center_box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(center_box, GTK_ALIGN_CENTER);

  GtkWidget *clock_center = clockNew();
  GtkWidget *battery_display = newBatteryDisplay();

  gtk_box_pack_start(GTK_BOX(center_box), clock_center, FALSE, TRUE, 10);
  gtk_box_pack_end(GTK_BOX(center_box), battery_display, FALSE, FALSE, 3);

  int sock = hyprlandConnect(EVENT_SOCKET);
  fcntl(sock, F_SETFL, O_NONBLOCK);

  GIOChannel *channel = g_io_channel_unix_new(sock);
  g_io_add_watch(channel, G_IO_IN, onHyprlandEvent, NULL);
  g_io_channel_unref(channel);

  gtk_widget_show_all(window);
}

void onMonitorAdded(GdkDisplay *display, GdkMonitor *monitor, gpointer data) {
  barInit(monitor);
}

int main(int argc, char **argv) {
  gtk_init(&argc, &argv);
  applyCss();
  GdkDisplay *display = gdk_display_get_default();
  int n_monitors = gdk_display_get_n_monitors(display);

  for (int i = 0; i < n_monitors; ++i) {
    GdkMonitor *monitor = gdk_display_get_monitor(display, i);
    barInit(monitor);
  }

  g_signal_connect(display, "monitor-added", G_CALLBACK(onMonitorAdded), NULL);
  gtk_main();

  return 0;
}
