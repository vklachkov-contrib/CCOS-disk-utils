#include "ccos_disk.h"

#include "ccos_structure.h"

static int is_valid_superblock(ccos_disk_t* disk) {
    ccos_inode_t* root_dir = ccos_disk_read_sector(disk, disk->superblock_fid);
    if (root_dir == NULL) {
        return 0;
    }

    int is_valid_header =
        (root_dir->header.file_id == disk->superblock_fid) &&
        (root_dir->header.file_fragment_index == 0);

    int is_valid_parent =
        root_dir->desc.dir_file_id == root_dir->header.file_id;

    return is_valid_header && is_valid_parent;
}

static int is_valid_bitmap_block(ccos_disk_t* disk) {
    ccos_bitmask_t* bitmap = ccos_disk_read_sector(disk, disk->bitmap_fid);
    if (bitmap == NULL) {
        return 0;
    }

    int is_valid_header =
        (bitmap->header.file_id == disk->bitmap_fid) &&
        (bitmap->header.file_fragment_index == 0);

    // TODO: Check is system blocks (boot sector, root, bitmap) marked as busy.
    int is_system_blocks_busy = 1;

    return is_valid_header && is_system_blocks_busy;
}

int ccos_disk_open(uint8_t* data, size_t size, ccos_disk_t* output) {
    // A sector size of 256 bytes was only found in the file system on bubble memory in the Compass 1101.
    // The laptop itself always has 3 modules of 128kb each installed (384kb total).
    const uint16_t sector_size = (size == 384 * 1024) ? 256 : 512;

    if (size % sector_size != 0 || size < 128 * 1024) {
        return 1;
    }

    ccos_disk_t disk = {
        .sector_size = sector_size,
        .superblock_fid = 0,
        .bitmap_fid = 0,
        .data = data,
        .size = size,
    };

    ccos_boot_sector_t* boot_sector = ccos_disk_read_sector(&disk, 0);
    if (boot_sector->superblock_fid && boot_sector->bitmap_fid) {
        disk.superblock_fid = boot_sector->superblock_fid;
        disk.bitmap_fid = boot_sector->bitmap_fid;
    } else if (size == 384 * 1024) {
        disk.superblock_fid = 0x3fe;   // From bubbles image.
        disk.bitmap_fid = 0x3fd;
    } else if (size == 360 * 1024) {
        disk.superblock_fid = 0x121;   // From 2102 firmware.
        disk.bitmap_fid = 0x120;
    } else if (size == 720 * 1024) {
        disk.superblock_fid = 0x6;     // From GRiD-OS 3.2.0 image.
        disk.bitmap_fid = 0x5;
    } else if (size >= 10 * 1024 * 1024) {
        disk.superblock_fid = 0x2420;  // From 2101 firmware.
        disk.bitmap_fid = 0x2400;
    }

    if (!disk.superblock_fid || !disk.bitmap_fid) {
        // TODO: try to find superblock using some heuristic.
        return 1;
    }

    if (!ccos_disk_params_valid(&disk)) {
        // TODO: try to find superblock using some heuristic.
        return 1;
    }

done:
    *output = disk;
    return 0;
}

int ccos_disk_params_valid(ccos_disk_t* disk) {
    return is_valid_superblock(disk) && is_valid_bitmap_block(disk);
}

void* ccos_disk_read_sector(ccos_disk_t* disk, uint16_t sector) {
    const size_t offset = (size_t)disk->sector_size * (size_t)sector;
    if (offset < disk->size) {
        return (void*)&disk->data[offset];
    } else {
        return NULL;
    }
}
