// Copyright (c) 2019 Hongzheng Chen
// chenhzh37@mail2.sysu.edu.cn
// Ubuntu 18.04 + gcc 7.3.0

/****** FAT12.h ******/

#ifndef FAT12_H
#define FAT12_H

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

#include "ide.h"
#include "stdio.h"
#include <stdint.h>

/*
 * FAT12
 *
 * 2 * 18 * 80 = 2880 sectors
 *
 * LogSec | NumSec   | Context
 * 0      | 1 (512)  | BootSec
 * 1      | 9 (4608) | FAT1
 * 10     | 9 (4608) | FAT2
 * 19     | 14 (9728)| root
 * 33     | 14 (9828)| data
 *
 * Reference:
 * MinirighiOS, http://minirighi.sourceforge.net/html/fat_8h-source.html
 * FAT12, https://github.com/imtypist/fat12
 */

#define FAT_SECTOR_SIZE 512
#define FAT_BOOTSECTOR_SIZE 1
#define FAT_PHYS_SIZE 9
#define FAT_ROOT_SIZE 14
#define FAT_NUM_ROOT_ENTRY 224
#define FAT_NUM_DIR_ENTRY 16
#define FAT_NUM_ENTRY 3072

#define FAT_BOOTSECTOR_START 0
#define FAT_ENTRY_START 1
#define FAT_ROOT_REGION_START 19
#define FAT_DATA_REGION_START 33

#define FAT_ATTR_READ_ONLY 0x01
#define FAT_ATTR_HIDDEN 0x02
#define FAT_ATTR_SYSTEM 0x04
#define FAT_ATTR_VOLUME_ID 0x08
#define FAT_ATTR_SUBDIRECTORY 0x10
#define FAT_ATTR_ARCHIVE 0x20
#define FAT_ATTR_DEVICE 0x40
#define FAT_ATTR_UNUSED 0x80

#define EOF_FAT12	   0xFF8


// Boot Sector (BS) & BPB (BIOS Parameter Block)
typedef struct bootsect
{
	uint8_t	      Jump[3]; // 3
	char          BS_OEMName[8]; // 11
	uint16_t      BPB_BytesPerSector; // 13
	uint8_t       BPB_SectorsPerCluster; // 14
	uint16_t      BPB_ReservedSectors; // 16
	uint8_t       BPB_NumFATs; // 17
	uint16_t      BPB_RootDirectoryEntries; // 19
	uint16_t      BPB_LogicalSectors; // 21
	uint8_t       BPB_MediumDescriptorByte; // 22
	uint16_t      BPB_SectorsPerFat; // 24
	uint16_t      BPB_SectorsPerTrack; // 26
	uint16_t      BPB_NumHeads; // 28
	uint32_t      BPB_HiddenSectors; // 32
	uint8_t       code[480]; // the last 2B is signature "AA55h"

} __attribute__ ((packed)) bootsect_t;
static bootsect_t bootsector;

/*
 * File Allocation Table (FAT)
 * which is a linked list, with 12 bits of each entry
 * 0FF0 - 0FFF are bad clusters
 *
 * 9 sectors * 512B/sector = 4608
 * 4608B * 8bits / 12bits = 3072 entries
 *
 */
typedef struct phys_fat
{
	uint8_t entry [FAT_PHYS_SIZE * FAT_SECTOR_SIZE]; // 4608　FAT_PHYS_SIZE=9 * FAT_SECTOR_SIZE=512

} __attribute__ ((packed)) phys_fat12_t;
static phys_fat12_t phys_fat;

typedef struct logic_fat
{
	// only the first 3B are used (24bits)
	// the first two clusters (0,1) are useless
	uint32_t entry [FAT_NUM_ENTRY]; // 3072 //FAT_NUM_ENTRY=3072 uint32_t=4Bytes

} __attribute__ ((packed)) fat12_t;
static fat12_t fat;

typedef struct fat12_file_attr
{
	unsigned long read_only : 1;
	unsigned long hidden    : 1;
	unsigned long system    : 1;
	unsigned long label	    : 1;
	unsigned long directory : 1;
	unsigned long archive   : 1;
	unsigned long __res     : 2;
} __attribute__ ((packed)) fat12_file_attr_t;

// Root Directory
// 16 directory entries per sector = 512B / 32B = 16
// 14 sectors * 16 = 224 entries
typedef struct FileEntry // 32B
{
	char name[8];  //ok
	char extension[3];//ok
	fat12_file_attr_t attribute; //ok
	uint8_t reserved[10];
	uint16_t time;
	uint16_t date;
	uint16_t startCluster;
	uint32_t fileLength;

} __attribute__ ((packed)) FileEntry_t;

typedef struct fat_date
{
	int year;
	int month;
	int day;

} __attribute__ ((packed)) fat_date_t;

typedef struct fat_time
{
	int hour;
	int minute;
	int second;

} __attribute__ ((packed)) fat_time_t;

typedef struct root_directory
{
	FileEntry_t entry[FAT_SECTOR_SIZE / sizeof(FileEntry_t) * FAT_ROOT_SIZE]; // 224 FAT_SECTOR_SIZE=512/sizeof(FileEntry_t) =32Bytes * 14 = 512 / 32 * 14 = 16 * 14 = 224

} __attribute__ ((packed)) fat_root_t;

static fat_root_t root_dir;

typedef struct sub_directory
{
	FileEntry_t entry[FAT_SECTOR_SIZE / sizeof(FileEntry_t)]; // 16

} __attribute__ ((packed)) fat_subdir_t;

extern void set_curr_dir(int dir);
extern int get_curr_dir();

extern void set_curr_path(char* path);

extern void get_curr_path(char* new_path);

// bool Read_FAT();
// bool load_file(char *stringa, uint8_t *buffer);
// int get_file_size(char *file_name);
// char *pwd();
// void ls();
// bool cd(char *new_path);
// bool cat(char *stringa);
// bool rm(char *filename);

extern bool fat12_init();

extern void int_to_date(fat_date_t* d, uint16_t date);
extern void int_to_time(fat_time_t* time, uint16_t t);
extern int date_to_int(int year, int month, int day);
extern int time_to_int(int hour, int minute, int second);

extern char* fat12_construct_file_name(char* name, FileEntry_t *f);

extern bool fat12_next_sector(uint32_t* next, uint32_t actual);
extern int fat12_read_bytes(char* buf, uint32_t cstart, uint32_t numBytes);

// start from the @cstart-th cluster, read the whole file to @buf
// return the number of bytes read
extern int fat12_read_clusters(char* buf, uint32_t cstart);

extern int fat12_write_bytes(char* buf, uint32_t cstart, uint32_t numBytes);
extern int fat12_write_clusters(char* buf, uint32_t cstart);

extern void fat12_write_back_fat();

extern void fat12_del_fat_entry(uint32_t cstart);
extern bool fat12_read_file(char* filename, char* addr, int* size);

extern void fat12_show_file_attrib(fat12_file_attr_t attr);

extern int fat12_get_num_cluster(uint32_t start);

extern bool fat12_show_file_entry(FileEntry_t *f);

extern void fat12_set_dir(int new_dir, char* action);

extern FileEntry_t* fat12_find_entry(char* folder);

extern void fat12_ls();
extern bool fat12_cd(char* folder);

extern bool fat12_rm(char* filename);

extern void fat12_create_file(uintptr_t addr, int size, char* name);

extern bool fat12_cp(char* src, char* dst);

#ifdef __cplusplus
}
#endif

#endif // FAT12_H
