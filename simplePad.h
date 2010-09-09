#ifndef SIMPLEPAD_H
#define SIMPLEPAD_H

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

#define STANDBY 0
#define OPEN 1
#define SAVE 2
#define BUFFER 3

#define true 1
#define false 0

typedef int boolean;

GtkWidget *window, *textview, *entry, *statusbar;

/* GtkEntry input handler. */
void handle_entry(void);

/* Get the length of dir objects. */
unsigned int get_dir_length(char *path);

/* Read contents in directories. */
void readDir(char *path);

/* Open and read a file. */
int openFile(char *filename, void *data);

/* Trigger for saveFile. */
void saveFile_trigger(char* filename, boolean isCurrentSaving);

/* Write the contents in text buffer into file. */
int saveFile(char *filename, boolean isCurrentSaving);

/* Clear the text view and its buffer. */
void newFile(void);

/* Fullscreen / unfullscreen SimplePad */
void fullscreen(void);

/* Select all the contents in entry after updating. */
void entry_update_select_all(char *info);

void text_changed(void);

/* Display copyleft information. */
void showCopyleft(void);

/* Change wrap mode of text view. */
void change_text_view_wrap_mode(void);

/* Update the current_filename. */
void update_current_filename(char *filename);

#endif
