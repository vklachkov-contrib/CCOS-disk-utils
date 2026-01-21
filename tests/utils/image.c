#include "image.h"

#include <criterion/criterion.h>

#include <stdio.h>
#include <stdlib.h>

uint8_t* load_image(const char* path, size_t expected_size) {
  FILE* f = fopen(path, "rb");
  if (f == NULL) {
    cr_log_error("Failed to open file '%s'", path);
    return NULL;
  }

  uint8_t* data = malloc(expected_size);
  if (data == NULL) {
    cr_log_error("Failed to allocate %zu bytes for loading '%s'", expected_size, path);
    fclose(f);
    return NULL;
  }

  size_t read = fread(data, 1, expected_size, f);
  fclose(f);

  if (read != expected_size) {
    cr_log_error("Read %zu bytes, expected %zu from '%s'", read, expected_size, path);
    free(data);
    return NULL;
  }

  return data;
}
