#ifndef CCOS_CONTEXT_H
#define CCOS_CONTEXT_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
  uint8_t* data;
  size_t   size;
  uint16_t sector_size;
  uint16_t superblock_id;
  uint16_t bitmap_block_id;
} ccos_disk_t;

#endif  // CCOS_CONTEXT_H
