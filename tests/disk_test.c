#include <criterion/criterion.h>

#include <ccos_disk.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
