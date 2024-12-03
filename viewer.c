#include <stdio.h>
#include <ctype.h>

#define BYTES 16

typedef struct {
  char buffer[4096];
  char *filename;
  FILE *file;
  long offset;
} VIEWER;

int initViewer(VIEWER *viewer, char *filename) {
  viewer->filename = filename;
  viewer->offset = 0;

  viewer->file = fopen(filename, "rb");

  if (!viewer->file) {
    perror("Error");
    return -1;
  }
  return 0;

}

int closeViewer(VIEWER *viewer) {

}

void printBytes(VIEWER *viewer, int bytesRead) {
  printf("%08lx ", viewer->offset);
  printf("| ");
  for (int i = 0; i < BYTES; i++) {
    if (i < bytesRead)
      printf("%02X ", viewer->buffer[i]);
    else
      printf(" ");
    if (i == 7)
      printf(" ");
  }
  printf(" | ");

  for (int i = 0; i < bytesRead; i++) {
    if (isprint(viewer->buffer[i]))
      putchar(viewer->buffer[i]);
    else 
      putchar('.');
  }

  printf("\n");
  
}


void readFile(VIEWER *viewer) {
  int bytesRead;
  while ((bytesRead = fread(viewer->buffer, 1, BYTES, viewer->file)) > 0) {
    printBytes(viewer, bytesRead);
    viewer->offset += bytesRead;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Error: No filename given\n");
    return 1;
  }

  VIEWER viewer;
  if (initViewer(&viewer, argv[1]) != 0) {
    return 1;
  }

  readFile(&viewer);

  return 0;
} 
