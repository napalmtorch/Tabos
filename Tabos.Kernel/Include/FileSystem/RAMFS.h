#pragma once
#include <Lib/Types.h>

typedef struct TOS_RAMFile TOS_RAMFile;
struct attr_pack TOS_RAMFile 
{
    char     name[64];
    bool     hidden : 8;
    uint32_t offset;
    size_t   size;
};

typedef struct
{
    uint32_t table_offset;
    uint32_t table_size;
    uint32_t data_offset;
    uint32_t data_size;
    uint32_t data_pos;
    uint32_t count;
    uint32_t max;
} attr_pack TOS_RAMFSHeader;

typedef struct
{
    TOS_RAMFSHeader* hdr;
    uint8_t*          data;
    size_t            sz;
    TOS_RAMFile*     files;
} attr_pack TOS_RAMFS;

#define RAMFS_ENTSZ (sizeof(TOS_RAMFile))
#define RAMFS_ROOT  0

void TOS_InitRAMFS(TOS_RAMFS* fs, void* data, size_t sz);
TOS_RAMFile* TOS_OpenRAMFile(TOS_RAMFS* fs, const char* name);
void* TOS_ReadRAMFileData(TOS_RAMFS* fs, TOS_RAMFile* file);
bool TOS_RAMFileExists(TOS_RAMFS* fs, const char* name);

TOS_RAMFile** TOS_GetRAMFiles(TOS_RAMFS* fs, size_t* sz);