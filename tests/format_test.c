#include <criterion/criterion.h>

#include "utils/image.h"

#include <ccos_disk.h>
#include <ccos_format.h>
#include <ccos_private.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void display_bad_sector(const uint8_t* actual, size_t sector_size) {
  for (size_t i = 0; i < sector_size; i += 16) {
    char line[128];
    snprintf(line, sizeof(line), "%03zu | ", i);

    for (size_t j = 0; j < 16; ++j) {
      char hex[5];
      snprintf(hex, sizeof(hex), " %02X", actual[i + j]);
      strncat(line, hex, sizeof(line) - strlen(line) - 1);
    }

    cr_log_error("%s", line);
  }
}

static void compare_disk_with_ref(const ccos_disk_t* disk, const uint8_t* expected) {
  const size_t sector_count = disk->size / disk->sector_size;

  for (size_t i = 0; i < sector_count; i++) {
    const uint8_t* actual_sector = disk->data + i * disk->sector_size;
    const uint8_t* expected_sector = expected + i * disk->sector_size;
  
    if (memcmp(actual_sector, expected_sector, disk->sector_size)) {
      cr_log_error("Sector %zu mismatch", i);

      cr_log_error("Actual sector:");
      display_bad_sector(actual_sector, disk->sector_size);

      cr_log_error("Expected sector:");
      display_bad_sector(expected_sector, disk->sector_size);

      cr_assert(!"Bad sector");
    }
  }
}

Test(format, bubbles) {
  size_t image_size = 3 * 128 * 1024;

  ccos_disk_t disk;

  int ret = ccos_new_disk_image(CCOS_DISK_FORMAT_BUBMEM, image_size, &disk);
  cr_assert_eq(ret, 0, "ccos_new_disk_image failed");

  uint8_t* expected = load_image("files/bubbles/empty.img", image_size);
  cr_assert_not_null(expected, "Failed to load expected image");

  compare_disk_with_ref(&disk, expected);

  free(disk.data);
  free(expected);
}

Test(format, floppy_360k) {
  size_t image_size = 360 * 1024;

  ccos_disk_t disk;

  int ret = ccos_new_disk_image(CCOS_DISK_FORMAT_COMPASS, image_size, &disk);
  cr_assert_eq(ret, 0, "ccos_new_disk_image failed");

  uint8_t* expected = load_image("files/floppy 360k/empty.img", image_size);
  cr_assert_not_null(expected, "Failed to load expected image");

  compare_disk_with_ref(&disk, expected);

  free(disk.data);
  free(expected);
}

Test(format, floppy_720k) {
  size_t image_size = 720 * 1024;

  ccos_disk_t disk;

  int ret = ccos_new_disk_image(CCOS_DISK_FORMAT_COMPASS, image_size, &disk);
  cr_assert_eq(ret, 0, "ccos_new_disk_image failed");

  uint8_t* expected = load_image("files/floppy 720k/empty.img", image_size);
  cr_assert_not_null(expected, "Failed to load expected image");

  compare_disk_with_ref(&disk, expected);

  free(disk.data);
  free(expected);
}

Test(format, hdd_10mb) {
  size_t image_size = 10 * 1024 * 1024;

  ccos_disk_t disk;

  int ret = ccos_new_disk_image(CCOS_DISK_FORMAT_COMPASS, image_size, &disk);
  cr_assert_eq(ret, 0, "ccos_new_disk_image failed");

  uint8_t* expected = load_image("files/hdd 10mb/empty.img", image_size);
  cr_assert_not_null(expected, "Failed to load expected image");

  compare_disk_with_ref(&disk, expected);

  free(disk.data);
  free(expected);
}
