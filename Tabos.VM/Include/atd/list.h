#ifndef ATD_LIST_Htype
#define ATD_LIST_H 1

#ifdef TABOS_KERNEL
    #include <Lib/Types.h>
#else
    #include <stdint.h>
    #include <stddef.h>
#endif

#define ATD_LINK_NODE(type) \
    typedef struct ATD_node_ ## type { \
        type content;\
        void * next;\
        void * prev;\
    } ATD_node_ ## type ## _t

#define ATD_LIST_CREATE(type, initData) \
    ATD_node_ ## type ## _t * ATD_LIST_ ## type ## _MAKE() \
    { \
        ATD_node_ ## type ## _t *head = (ATD_node_ ## type ## _t *) ATD_malloc(sizeof(ATD_node_ ## type ## _t)); \
        head->prev = NULL; \
        head->next = NULL; \
        head->content = initData; \
        return head; \
    } \
    type * ATD_LIST_ ## type ## _AT(ATD_node_ ## type ## _t * head, size_t ind) \
    { \
        size_t i; \
        ATD_node_ ## type ## _t * current = head; \
        for (i = 0; i < ind; i++) { \
            if (!current->next) return NULL; \
            current = (ATD_node_ ## type ## _t *) current->next; \
        } \
        return & (current->content); \
    } \
    type * ATD_LIST_ ## type ## _PUSH(ATD_node_ ## type ## _t * head) \
    { \
        ATD_node_ ## type ## _t * current = head; \
        while(current->next) { \
            if (!current->next) return NULL; \
            current = (ATD_node_ ## type ## _t *) current->next; \
        } \
        ATD_node_ ## type ## _t * node = ATD_LIST_ ## type ## _MAKE(); \
        current->next = node; \
        node->prev = current; \
        node->next = NULL; \
        node->content = initData; \
        return & (node->content); \
    } \
    type ATD_LIST_ ## type ## _POP(ATD_node_ ## type ## _t * head) \
    { \
        ATD_node_ ## type ## _t * current = head; \
        while(current->next) { \
            if (!current->next) return initData; \
            current = (ATD_node_ ## type ## _t *) current->next; \
        } \
        ((ATD_node_ ## type ## _t *) current->prev)->next = NULL; \
        type r = current->content; \
        ATD_free(current); \
        return r; \
    } \
    type * ATD_LIST_ ## type ## _INSERT(ATD_node_ ## type ## _t * head, type data, size_t ind) \
    { \
        \
        size_t i; \
        ATD_node_ ## type ## _t * current = head; \
        for (i = 0; i < ind; i++) { \
            if (!current->next) return NULL; \
            current = (ATD_node_ ## type ## _t *) current->next; \
        } \
        ATD_node_ ## type ## _t * node = ATD_LIST_ ## type ## _MAKE(); \
        current->next = node; \
        node->prev = current; \
        node->next = current->next; \
        node->content = data; \
        return & (node->content); \
    } \
    type ATD_LIST_ ## type ## _REMOVEAT(ATD_node_ ## type ## _t * head, size_t ind) \
    { \
        size_t i; \
        ATD_node_ ## type ## _t * current = head; \
        for (i = 0; i < ind; i++) { \
            if (!current->next) return initData; \
            current = (ATD_node_ ## type ## _t *) current->next; \
        } \
        ((ATD_node_ ## type ## _t *) current->prev)->next = current->next; \
        ((ATD_node_ ## type ## _t *) current->next)->prev = current->prev; \
        type r = current->content; \
        ATD_free(current); \
        return r; \
    } \
    size_t ATD_LIST_ ## type ## _LENGTH(ATD_node_ ## type ## _t * head) \
    { \
        size_t i = 1; \
        while (head->next) head = (ATD_node_ ## type ## _t *)head->next, i++; \
        return i; \
    } \
    void ATD_LIST_ ## type ## _FOREACH(ATD_node_ ## type ## _t * head, void (*callback) (type *value, size_t index)) \
    { \
        size_t i = 0; \
        size_t l = ATD_LIST_ ## type ## _LENGTH(head);\
        while (i < l) \
        { \
            callback(& (head->content), i); \
            head = head->next; \
            i++; \
        } \
    }

#endif