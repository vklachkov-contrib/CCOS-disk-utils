#include <criterion/criterion.h>

#include "utils/image.h"

#include <ccos_disk.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Test(disk, open_bubbles) {
  size_t image_size = 3 * 128 * 1024;

  uint8_t* image = load_image("files/bubbles/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  ccos_disk_t disk;

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 0);

  cr_assert_eq(disk.sector_size, 256);
  cr_assert_eq(disk.superblock_fid, 0x3fe);
  cr_assert_eq(disk.bitmap_fid, 0x3fd);
  cr_assert_eq(disk.data, image);
  cr_assert_eq(disk.size, image_size);

  free(image);
}

Test(disk, open_bubbles_without_boot_sector) {
  size_t image_size = 3 * 128 * 1024;

  uint8_t* image = load_image("files/bubbles/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  // Fill boot sector with zeros to delete info about sector numbers.
  memset(image, 0x00, 256);

  ccos_disk_t disk;

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 0);

  cr_assert_eq(disk.sector_size, 256);
  cr_assert_eq(disk.superblock_fid, 0x3fe);
  cr_assert_eq(disk.bitmap_fid, 0x3fd);
  cr_assert_eq(disk.data, image);
  cr_assert_eq(disk.size, image_size);

  free(image);
}

Test(disk, open_floppy_360kb) {
  size_t image_size = 360 * 1024;

  uint8_t* image = load_image("files/floppy 360k/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  ccos_disk_t disk;

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 0);

  cr_assert_eq(disk.sector_size, 512);
  cr_assert_eq(disk.superblock_fid, 0x121);
  cr_assert_eq(disk.bitmap_fid, 0x120);
  cr_assert_eq(disk.data, image);
  cr_assert_eq(disk.size, image_size);

  free(image);
}

Test(disk, open_floppy_360kb_without_boot_sector) {
  size_t image_size = 360 * 1024;

  uint8_t* image = load_image("files/floppy 360k/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  // Fill boot sector with zeros to delete info about sector numbers.
  memset(image, 0x00, 512);

  ccos_disk_t disk;

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 0);

  cr_assert_eq(disk.sector_size, 512);
  cr_assert_eq(disk.superblock_fid, 0x121);
  cr_assert_eq(disk.bitmap_fid, 0x120);
  cr_assert_eq(disk.data, image);
  cr_assert_eq(disk.size, image_size);

  free(image);
}

Test(disk, failed_to_open_bad_floppy_360kb) {
  size_t image_size = 360 * 1024;

  uint8_t* image = load_image("files/floppy 360k/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  // Write zeros to superblock. ccos_disk_open must fail.
  memset(image + (0x121 * 512), 0x00, 512);

  ccos_disk_t disk = {0};

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 1);

  cr_assert_eq(disk.sector_size, 0);
  cr_assert_eq(disk.superblock_fid, 0);
  cr_assert_eq(disk.bitmap_fid, 0);
  cr_assert_eq(disk.data, NULL);
  cr_assert_eq(disk.size, 0);

  free(image);
}

Test(disk, open_floppy_720kb) {
  size_t image_size = 360 * 1024;

  uint8_t* image = load_image("files/floppy 720k/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  ccos_disk_t disk;

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 0);

  cr_assert_eq(disk.sector_size, 512);
  cr_assert_eq(disk.superblock_fid, 0x121);
  cr_assert_eq(disk.bitmap_fid, 0x120);
  cr_assert_eq(disk.data, image);
  cr_assert_eq(disk.size, image_size);

  free(image);
}

Test(disk, open_hdd_10mb) {
  size_t image_size = 10 * 1024 * 1024;

  uint8_t* image = load_image("files/hdd 10mb/empty.img", image_size);
  cr_assert_not_null(image, "Failed to load expected image");

  ccos_disk_t disk;

  int res = ccos_disk_open(image, image_size, &disk);
  cr_assert_eq(res, 0);

  cr_assert_eq(disk.sector_size, 512);
  cr_assert_eq(disk.superblock_fid, 0x2420);
  cr_assert_eq(disk.bitmap_fid, 0x241A);
  cr_assert_eq(disk.data, image);
  cr_assert_eq(disk.size, image_size);

  free(image);
}

Test(disk, read_sector_positive) {
  ccos_disk_t disk = {
    .sector_size = 512,
    .size = 512 * 4,
    .data = malloc(disk.size),
  };

  cr_assert_not_null(disk.data);

  // Initialize disk and set the first byte of each sector to its sector number.
  memset(disk.data, 0, disk.size);
  for (uint8_t sector = 0; sector < 4; sector++) {
    disk.data[sector * disk.sector_size] = (uint8_t)sector;
  }

  uint16_t sector_number = 2;
  void* sector = ccos_disk_read_sector(&disk, sector_number);
  cr_assert_not_null(sector);
  cr_assert_eq(sector, disk.data + (sector_number * disk.sector_size));
  cr_assert_eq(*((uint8_t*)sector), sector_number);

  free(disk.data);
}

Test(disk, read_sector_out_of_bounds) {
  ccos_disk_t disk = {
    .sector_size = 512,
    .size = 512 * 4,
    .data = malloc(disk.size),
  };

  cr_assert_not_null(disk.data);

  cr_assert_null(ccos_disk_read_sector(&disk, 100));

  free(disk.data);
}
