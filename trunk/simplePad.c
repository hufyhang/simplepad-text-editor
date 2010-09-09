#include "simplePad.h"
#include "buffers.h"

int main(int argc, char **argv) {
  gtk_set_locale();
  gtk_init(&argc, &argv);
  GtkBuilder *builder = gtk_builder_new();
  gtk_builder_add_from_file(builder, argv[1], NULL);
  gtk_builder_connect_signals(builder, NULL);
  window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
  textview = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));
  entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry"));
  statusbar = GTK_WIDGET(gtk_builder_get_object(builder, "statusbar"));
  g_object_unref(builder);

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
  g_signal_connect(entry, "activate", G_CALLBACK(handle_entry), NULL);
  g_signal_connect(textview, "insert-at-cursor", G_CALLBACK(text_changed), NULL);
  g_signal_connect(textview, "delete-from-cursor", G_CALLBACK(text_changed), NULL);

  gtk_widget_show_all(window);
  gtk_window_maximize(GTK_WINDOW(window));

  showCopyleft(); // Show copyleft information.
  initial_buffers(); // Initialize buffers.
  show_usage_in_statusbar();

  gtk_main();

  return 0;
}
