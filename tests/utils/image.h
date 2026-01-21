#ifndef TESTS_UTILS_IMAGE_H
#define TESTS_UTILS_IMAGE_H

#include <stdint.h>
#include <stddef.h>

uint8_t* load_image(const char* path, size_t expected_size);

#endif // TESTS_UTILS_IMAGE_H