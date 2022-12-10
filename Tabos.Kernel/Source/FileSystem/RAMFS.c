#include <FileSystem/RAMFS.h>
#include <Core/Common.h>

void TOS_InitRAMFS(TOS_RAMFS* fs, void* data, size_t sz)
{
    fs->data  = (uint8_t*)data;
    fs->sz    = sz;
    fs->hdr   = (TOS_RAMFSHeader*)fs->data;
    fs->files = (TOS_RAMFile*)((uintptr_t)fs->data + sizeof(TOS_RAMFSHeader));
    TOS_Log("%s Initialized RAMFS - Ptr:%p Count:%d/%d Size:%a\n", DEBUG_OK, fs->data, fs->hdr->count, fs->hdr->max, fs->hdr->data_size);
}

TOS_RAMFile* TOS_OpenRAMFile(TOS_RAMFS* fs, const char* name)
{
    if (name == NULL || TOS_StrLen(name) == 0) { return NULL; }
    for (size_t i = 0; i < fs->hdr->max; i++)
    {
        if (fs->files[i].name[0] == 0) { continue; }
        if (TOS_StrEquals(fs->files[i].name, name)) { return &fs->files[i]; }
    }
    return NULL;
}

void* TOS_ReadRAMFileData(TOS_RAMFS* fs, TOS_RAMFile* file)
{
    void* filedata = (void*)((uintptr_t)fs->data + fs->hdr->data_offset + file->offset);
    return filedata;
}

bool TOS_RAMFileExists(TOS_RAMFS* fs, const char* name)
{
    TOS_RAMFile* file = TOS_OpenRAMFile(fs, name);
    return (file != NULL);
}

TOS_RAMFile** TOS_GetRAMFiles(TOS_RAMFS* fs, size_t* sz)
{
    size_t c = 0;
    for (size_t i = 0; i < fs->hdr->max; i++) { if (fs->files[i].name[0] == 0) { continue; } c++; }
    if (c == 0) { return NULL; }
    
    TOS_RAMFile** files = TOS_Alloc(sizeof(TOS_RAMFile*) * c);
    size_t j = 0;
    for (size_t i = 0; i < fs->hdr->max; i++)
    {
        if (fs->files[i].name[0] == 0) { continue; }
        files[j++] = &fs->files[i];
    }
    *sz = c;
    return files;
}