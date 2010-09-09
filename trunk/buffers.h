#ifndef BUFFERS_H
#define BUFFERS_H

#include "simplePad.h"

#define NUM_OF_BUFFER 20
#define BUFFER_NAME_LENGTH 1024
#define USAGE_DESCRIPTOR 101

struct __buffer {
  boolean isUsed;
  char buffer_name[BUFFER_NAME_LENGTH];
  char *buffer_content;
} typedef sys_buffer;

sys_buffer buffers[NUM_OF_BUFFER];

/* Initialize buffers. */
void initial_buffers(void);

void insert_into_buffers(char *name, char *content);

void show_buffers(void);

void show_buffer_content(char *name);

void update_current_buffer(char *name);

void show_usage_in_statusbar(void);

#endif
