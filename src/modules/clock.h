#include <gtk/gtk.h>
#include <time.h>

gboolean updateClock(gpointer label) {
  time_t rawtime;
  struct tm *timeinfo;
  char buffer[80];

  time(&rawtime);
  timeinfo = localtime(&rawtime);

  strftime(buffer, sizeof(buffer), "%H:%M:%S", timeinfo);
  gtk_label_set_text(GTK_LABEL(label), buffer);

  return TRUE;
}

GtkWidget* clockNew(void) {
  GtkWidget* label = gtk_label_new("00:00:00");
  g_timeout_add(1000, updateClock, label);

  gtk_widget_set_name(label, "clock-module");

  return label;
}
