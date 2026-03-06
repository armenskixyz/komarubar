#include "glib-object.h"
#include <gtk/gtk.h>

#include "../hyprland.h"
#include "glib.h"

GtkWidget *workspaces_box;

GList *workspace_buttons_groups[11];
int current_active_workspace = 1;

void workspaceButtonCallback(GtkWidget *widget, gpointer data) {
  g_print("Button clicked %s\n", (char *)data);
  char command[64];
  snprintf(command, sizeof(command), "/dispatch workspace %s", (char *)data);
  hyprlandDispatch(command);
}

GtkWidget *workspaceButtonNew(int n) {
  char label[16];
  sprintf(label, "%d", n);
  char *data = g_malloc(16);
  strncpy(data, label, 15);
  GtkWidget *btn = gtk_button_new_with_label(label);
  g_signal_connect(btn, "clicked", G_CALLBACK(workspaceButtonCallback), data);
  gtk_widget_set_name(btn, "workspace-button");
  gtk_widget_set_size_request(btn, 15, 15);
  return btn;
}

GtkWidget *workspacesInit(void) {
  workspaces_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  for (int i = 0; i < 10; ++i) {
    GtkWidget *btn = workspaceButtonNew(i + 1);
    workspace_buttons_groups[i] =
        g_list_append(workspace_buttons_groups[i], btn);
    gtk_box_pack_start(GTK_BOX(workspaces_box), btn, FALSE, FALSE, 2);
  }
  gtk_widget_set_name(workspaces_box, "workspaces-box");
  gtk_widget_set_valign(workspaces_box, GTK_ALIGN_CENTER);
  return workspaces_box;
}

void workspaceSetActive(int n) {
  if (n < 0 || n > 11) {
    return;
  }

  for (int i = 0;
       i < g_list_length(workspace_buttons_groups[current_active_workspace]);
       ++i) {

    GtkStyleContext *old_context =
        gtk_widget_get_style_context(GTK_WIDGET(g_list_nth_data(
            workspace_buttons_groups[current_active_workspace], i)));
    gtk_style_context_remove_class(old_context, "active");
  }

  for (int i = 0; i < g_list_length(workspace_buttons_groups[n-1]); ++i) {

    GtkStyleContext *new_context =
        gtk_widget_get_style_context(GTK_WIDGET(g_list_nth_data(
            workspace_buttons_groups[n-1], i)));
    gtk_style_context_add_class(new_context, "active");
  }

  current_active_workspace = n - 1;
}
