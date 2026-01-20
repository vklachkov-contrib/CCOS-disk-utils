#include "ccos_disk.h"

void* ccos_disk_read_sector(ccos_disk_t* disk, uint16_t sector) {
    const size_t offset = (size_t)disk->sector_size * (size_t)sector;
    if (offset < disk->size) {
        return (void*)&disk->data[offset];
    } else {
        return NULL;
    }
}
