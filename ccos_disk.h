#ifndef CCOS_DISK_H
#define CCOS_DISK_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint16_t sector_size;
  uint16_t superblock_fid;
  uint16_t bitmap_fid;
  size_t   size;
  uint8_t* data;
} ccos_disk_t;

/**
 * @brief Reads and returns a pointer to the data of the specified sector.
 *
 * @param disk    Pointer to the disk structure.
 * @param sector  The sector number to read.
 *
 * @return Pointer to the start of the sector's data within the disk image, or NULL if the sector is out of bounds.
 */
void* ccos_disk_read_sector(ccos_disk_t* disk, uint16_t sector);

#endif  // CCOS_DISK_H
