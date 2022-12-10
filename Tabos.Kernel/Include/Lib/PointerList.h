#pragma once
#include <Lib/Types.h>

typedef struct
{
    void** entries;
    size_t count;
} TOS_PtrList;

TOS_PtrList TOS_NewPtrList();
TOS_PtrList TOS_DuplicatePtrList(TOS_PtrList* list);
void TOS_FreePtrList(TOS_PtrList* list, bool free_entries);
void TOS_PtrListClear(TOS_PtrList* list, bool free_entries);
void* TOS_PtrListAdd(TOS_PtrList* list, void* value);
bool  TOS_PtrListErase(TOS_PtrList* list, int index, bool free);
void* TOS_PtrListAt(TOS_PtrList* list, int index);
