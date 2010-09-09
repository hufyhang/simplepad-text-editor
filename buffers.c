#include "buffers.h"

int buffers_usage = 0;

void initial_buffers(void) {
  int index = 0;
  for(; index != NUM_OF_BUFFER; ++index) {
    buffers[index].isUsed = false;
    bzero(buffers[index].buffer_name, BUFFER_NAME_LENGTH * sizeof(char));
  }
}

void show_usage_in_statusbar(void) {
  int info_length = 16 + strlen(" buffers in use.");
  char info[info_length]; 
  bzero(info, 16 * sizeof(char));
  sprintf(info, " %d / %d buffers in use.", buffers_usage, NUM_OF_BUFFER);
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), USAGE_DESCRIPTOR, info);
}

void insert_into_buffers(char *name, char *content) {
  if(buffers_usage == NUM_OF_BUFFER) { // if buffers have already arrived at its top size, do nothing.
    entry_update_select_all("Maximum buffers.");
    return;
  }

  buffers[buffers_usage].isUsed = true;
  strcpy(buffers[buffers_usage].buffer_name, name);
  buffers[buffers_usage++].buffer_content = content;

  show_usage_in_statusbar();
}

void show_buffer_content(char *name) {
  int index = 0;
  for(; index != buffers_usage; ++index) {
    if(strcmp(buffers[index].buffer_name, name) == 0) {
      GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
      gtk_text_buffer_set_text(buffer, buffers[index].buffer_content, -1);
      int title_length = strlen(name) + strlen(" - SimplePad");
      char title[title_length];
      sprintf(title, "%s - SimplePad", name);
      update_current_filename(name);
      gtk_window_set_title(GTK_WINDOW(window), title);
      break;
    }
  }
}

void show_buffers(void) {
  if(buffers_usage == 0) { // if there is no buffers in used.
    entry_update_select_all("Buffers empty.");
    return;
  }

  char displays[1024];
  bzero(displays, 1024 * sizeof(char));
  int index = 0;
  while(buffers[index].isUsed != false) {
    sprintf(displays, "%s\n%s", displays, buffers[index++].buffer_name);
  }
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_set_text(buffer, displays, -1);
  gtk_window_set_title(GTK_WINDOW(window), "Buffers : SimplePad");
}

void update_current_buffer(char *name) {
  int index = 0;
  for(; index != buffers_usage; ++index) {
    if(strcmp(buffers[index].buffer_name, name) == 0) {
      GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
      GtkTextIter start, end;
      gtk_text_buffer_get_bounds(buffer, &start, &end);
      buffers[index].buffer_content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
      break;
    }
  }
}
