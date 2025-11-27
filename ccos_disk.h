#ifndef CCOS_DISK_H
#define CCOS_DISK_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint8_t* data;
  size_t   size;
  uint16_t sector_size;
  uint16_t superblock_id;
  uint16_t bitmap_block_id;
} ccos_disk_t;

#define DISK_READ_BLOCK(disk, sector)  ((void*)&(disk)->data[(disk)->sector_size * (sector)])
#define DISK_READ_WORD(disk, offset)   (*((uint16_t*)&(disk)->data[(offset)]))

#endif  // CCOS_DISK_H
