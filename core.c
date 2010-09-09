#include "simplePad.h"
#include "buffers.h"

#define FILENAME_LENGTH 1024

int functionFlag = STANDBY, isFullscreen = false, isSaved = false;

char current_filename[FILENAME_LENGTH];

char *copyleft = "SimplePad 1.0\nCopyright (C) 2009, 2010 HANG Feifei.\n\nSimplePad is free software; you can redistribute it and/or\nmodify it under the terms of the GNU General Public License as \npublished by the Free Software Foundation; either version 2.1 \nof the License, or (at your option) any later version.\n\nSimplePad is distributed in the hope that it will be useful,\nbut WITHOUT ANY WARRANTY; without even the implied warranty of\nMERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU \nGeneral Public License for more details.\n\nYou should have received a copy of the GNU General Public\nLicense along with SimplePad; if not, write to the Free\nSoftware Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA\n02111-1307 USA.";

void update_current_filename(char *filename) {
  bzero(current_filename, FILENAME_LENGTH * sizeof(char));
  strcpy(current_filename, filename);
}

unsigned int get_dir_length(char *path) {
  DIR *dir;
  struct dirent *ptr;
  dir = opendir(path);
  unsigned int length = 0;
  length += strlen(path) + strlen("\n----------------"); // count the length of list head.
  while((ptr = readdir(dir)) != NULL) {
    length += strlen(ptr->d_name) + strlen("\n");
  }
  closedir(dir);
  return length;
}

void readDir(char *path) {
  DIR *dir;
  struct dirent *ptr;
  dir = opendir(path);
  int list_length = get_dir_length(path);
  char list[list_length];
  bzero(list, list_length * sizeof(char));
  sprintf(list, "%s\n----------------", path);
  while((ptr = readdir(dir)) != NULL) {
    sprintf(list, "%s\n%s", list, ptr->d_name);
  }
  closedir(dir);
  GtkTextBuffer *textBuffer;
  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_set_text(textBuffer, list, -1);
}

int openFile(char *filename, void *data) {
  struct stat buf;
  stat(filename, &buf);
  if(S_ISDIR(buf.st_mode)) {
    readDir(filename);
    sprintf(filename, "%s - SimplePad", filename);
    gtk_window_set_title(GTK_WINDOW(window), filename);
    return 0;
  }
  gchar *content;
  gsize bytes;
  GError *error = NULL;
  GtkTextBuffer *buffer;
  g_file_get_contents(filename, &content, &bytes, &error);
  if(error != NULL) {
    entry_update_select_all(error->message);
    g_clear_error(&error);
    return -1;
  }
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_set_text(buffer, content, -1);

  /* Update buffers. */
  insert_into_buffers(filename, content);

  /* Update current filename. */
  update_current_filename(filename);

  sprintf(filename, "%s - SimplePad", filename);
  gtk_window_set_title(GTK_WINDOW(window), filename);
  return 0;
}

int saveFile(char *filename, boolean isCurrentSaving) {
  FILE *fptr;
  if((fptr = fopen(filename, "w+")) == NULL) {
    return -1;
  }
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  GtkTextIter start, end;
  gtk_text_buffer_get_bounds(buffer, &start, &end);
  char *content = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
  if(fputs(content, fptr) == EOF) {
    return -1;
  }
  fclose(fptr);

  /* Update current filename. */
  update_current_filename(filename);

  if(!isCurrentSaving) {
    /* Update buffers. */
    insert_into_buffers(filename, content);
  }

  return 0;
}

void fullscreen(void) {
  if(isFullscreen == false) {
    isFullscreen = true;
    gtk_window_fullscreen(GTK_WINDOW(window));
  }
  else {
    isFullscreen = false;
    gtk_window_unfullscreen(GTK_WINDOW(window));
  }
}

void newFile(void) {
  GtkTextBuffer *buffer;
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_set_text(buffer, "", -1);
  gtk_window_set_title(GTK_WINDOW(window), "SimplePad");
}

void entry_update_select_all(char *info) {
  gtk_entry_set_text(GTK_ENTRY(entry), info);
  gtk_entry_select_region(GTK_ENTRY(entry), 0, strlen(gtk_entry_get_text(GTK_ENTRY(entry))));
}

void text_changed(void) {
  if(isSaved == true) {
    char *title = strcat("**--", gtk_window_get_title(GTK_WINDOW(window)));
    gtk_window_set_title(GTK_WINDOW(window), title);
    isSaved = false;
  }
}

void showCopyleft(void) {
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
  gtk_text_buffer_set_text(buffer, copyleft, -1);
  gtk_window_set_title(GTK_WINDOW(window), "Copyleft : SimplePad");
}

void change_text_view_wrap_mode(void) {
  gtk_text_view_get_wrap_mode(GTK_TEXT_VIEW(textview)) == TRUE ? gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_NONE) : gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textview), GTK_WRAP_WORD_CHAR);

  entry_update_select_all("Wrap mode changed.");
}

void handle_entry(void) {
  char *input = gtk_entry_get_text(GTK_ENTRY(entry));
  if(strcmp(input, "cancel") == 0) {
    functionFlag = STANDBY;
    entry_update_select_all("Cancelled.");
    return;
  }

  if(functionFlag == STANDBY) {
    /* Quit SimplePad. */
    if(strcmp(input, "quit") == 0) {
      gtk_main_quit();
    }

    /* Open a file/directory. */
    else if(strcmp(input, "open") == 0 || strcmp(input, "read") == 0) {
      functionFlag = OPEN;
      char *info = "Enter file name to read...";
      entry_update_select_all(info);
    }

    /* Save/write a file. */
    else if(strcmp(input, "save") == 0) {
      functionFlag = SAVE;
      char *info = "Enter file name to save...";
      entry_update_select_all(info);
    }

    /* Load/view contents of a buffer. */
    else if(strcmp(input, "buffer") == 0) {
      functionFlag = BUFFER;
      char *info = "Enter buffer name to view...";
      entry_update_select_all(info);
    }

    /* Save the current file without asking filename. */
    else if(strcmp(input, "write") == 0 || strcmp(input, "save-current") == 0 || strcmp(input, "write-current") == 0) {
      update_current_buffer(current_filename);
      saveFile_trigger(current_filename, true);
      functionFlag = STANDBY;
    }

    /* Show up copyleft information.
    else if(strcmp(input, "about") == 0 || strcmp(input, "copyright") == 0 || strcmp(input, "copyleft") == 0) {
      update_current_buffer(current_filename);
      showCopyleft();
    }
    */

    /* Refresh workbrench. */
    else if(strcmp(input, "new") == 0) {
      update_current_buffer(current_filename);
      newFile();
      char *info = "Buffer flushed.";
      entry_update_select_all(info);
    }

    /* Fullscreen/unfullscreen SimplePad. */
    else if(strcmp(input, "fullscreen") == 0 || strcmp(input, "screen") == 0 || strcmp(input, "unfullscreen") == 0) {
      fullscreen();
    }

    /* Show a list of working buffers. */
    else if(strcmp(input, "buffers") == 0 || strcmp(input, "buffer-list") == 0) {
      update_current_buffer(current_filename);
      show_buffers();
    }

    /* Activate line wrapping. */
    else if(strcmp(input, "wrap") == 0 || strcmp(input, "line-wrap") == 0) {
      change_text_view_wrap_mode();
    }

    /* Oops, unknown command. */
    else {
      int info_length = strlen(input) + strlen("\"\" is undefined.");
      char info[info_length];
      sprintf(info, "\"%s\" is undefined.", input);
      entry_update_select_all(info);
    }
  }

  /* Open a file/directory. */
  else if(functionFlag == OPEN) {
    update_current_buffer(current_filename);
    openFile(input, NULL);
    functionFlag = STANDBY;
  }

  /* Save/write a file. */
  else if(functionFlag == SAVE) {
    saveFile_trigger(input, false);
  }

  /* View a buffer. */
  else if(functionFlag == BUFFER) {
    show_buffer_content(input);
    functionFlag = STANDBY;
  }
}

void saveFile_trigger(char *filename, boolean isCurrentSaving) {
  if(saveFile(filename, isCurrentSaving) == -1) {
    int info_length = strlen("Cannot write into ") + strlen(filename);
    char info[info_length];
    sprintf(info, "Cannot write into %s", filename);
    entry_update_select_all(info);
  }
  else {
    int info_length = strlen("Wrote ") + strlen(filename);
    char info[info_length];
    sprintf(info, "Wrote %s", filename);
    entry_update_select_all(info);
  }
  functionFlag = STANDBY;
}
