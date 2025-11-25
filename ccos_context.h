#ifndef CCOS_CONTEXT_H
#define CCOS_CONTEXT_H

#include <stdint.h>

typedef struct {
  uint8_t* disk;
  size_t disk_size;
  uint16_t sector_size;
  uint16_t superblock_id;
  uint16_t bitmap_block_id;
} ccfs_context_t;

typedef ccfs_context_t* ccfs_handle;

#endif  // CCOS_CONTEXT_H
