#include <gtk/gtk.h>
#include <stdio.h>
#include "../battery.h"

gboolean updateBattery(gpointer label) {
  int capacity = getBatteryCapacity();
  char buffer[1024];
  snprintf(buffer, 1024, "%d%%", capacity);
  gtk_label_set_text(label, buffer);
  return TRUE;
}

GtkWidget* newBatteryDisplay() {
  int capacity = getBatteryCapacity(); //initial check if device has battery
  if (capacity == -1) return NULL;
  GtkWidget* label = gtk_label_new("00%");
  g_timeout_add(1000, updateBattery, label);

  gtk_widget_set_name(label, "battery-display-module");

  return label;
  
}
