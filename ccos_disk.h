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
 * @brief Opens a disk and initializes the disk structure. Disk parameters are determined heuristically.
 *
 * @param data     Pointer to the disk image data.
 * @param size     Size of the disk image, in bytes.
 * @param output   Pointer to the ccos_disk_t structure to be initialized.
 *
 * @return 0 on success, or a positive value on failure.
 */
int ccos_disk_open(uint8_t* data, size_t size, ccos_disk_t* output);

/**
 * @brief Validates a disk with the specified parameters.
 *
 * @param disk  Pointer to the ccos_disk_t structure representing the disk to validate.
 *
 * @return 0 if the disk params are valid, or a positive value if the disk params are invalid.
 */
int ccos_disk_params_valid(ccos_disk_t* disk);

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
