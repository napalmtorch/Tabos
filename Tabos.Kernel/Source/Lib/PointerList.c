#include <Lib/PointerList.h>
#include <Core/Common.h>

TOS_PtrList TOS_NewPtrList()
{
    TOS_PtrList list = { NULL, 0 };
    return list;
}

TOS_PtrList TOS_DuplicatePtrList(TOS_PtrList* list)
{
    if (list == NULL || list->count == 0 || list->entries == NULL) { return TOS_NewPtrList(); }
    TOS_PtrList out = (TOS_PtrList){ TOS_Alloc(sizeof(void*) * list->count), list->count };
    return out;
}

void TOS_FreePtrList(TOS_PtrList* list, bool free_entries)
{
    if (list == NULL) { TOS_Panic("TOS_FreePtrList(00000000) - Attempt to free null pointer list"); return; }
    if (list->entries == NULL) { return; }
    if (free_entries)
    {
        for (size_t i = 0; i < list->count; i++) { TOS_Free(list->entries[i]); }
    }
    TOS_Free(list->entries);
    *list = (TOS_PtrList){ NULL, 0 };
}

void TOS_PtrListClear(TOS_PtrList* list, bool free_entries) { TOS_FreePtrList(list, free_entries); }

void* TOS_PtrListAdd(TOS_PtrList* list, void* value)
{
    if (list == NULL) { TOS_Panic("TOS_PtrListAdd(00000000, %p) - Attempt to add entry to null pointer list", value); return NULL; }
    void** entries = TOS_Alloc(sizeof(void*) * (list->count + 1));

    if (list->entries != NULL) { TOS_MemCopy(entries, list->entries, sizeof(void*) * list->count); TOS_Free(list->entries); }
    list->entries = entries;
    list->entries[list->count++] = value;
    return list->entries[list->count - 1];
}

bool TOS_PtrListErase(TOS_PtrList* list, int index, bool free)
{
    if (list == NULL) { TOS_Panic("TOS_PtrListErase(00000000, %d, %d) - Attempt to erase entry from null pointer list", index, free); return false; }
    if (index == 0 && list->count == 1) { TOS_FreePtrList(list, free); return true; }

    void** entries = TOS_Alloc(sizeof(void*) * (list->count - 1));

    int j = 0;
    for (size_t i = 0; i < list->count; i++) { if (i != (size_t)index) { entries[j++] = list->entries[i]; } }
    TOS_Free(list->entries);
    list->entries = entries;
    list->count--;
    return true;
}

void* TOS_PtrListAt(TOS_PtrList* list, int index)
{
    if (list == NULL) { TOS_Panic("TOS_PtrListAt(00000000, %d) - Index out of bounds", index); return NULL; }
    return list->entries[index];
}
