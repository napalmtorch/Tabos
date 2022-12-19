#include <atd/list.h>
#include <atd/mem.h>
#include <stdio.h>

ATD_LINK_NODE(uint32_t);
ATD_LIST_CREATE(uint32_t, 0);

int main() {
    ATD_node_uint32_t_t *head = ATD_LIST_uint32_t_MAKE();
    
    *ATD_LIST_uint32_t_AT(head, 0) = 1;
    *ATD_LIST_uint32_t_PUSH(head) = 2;
    *ATD_LIST_uint32_t_PUSH(head) = 3;
    
    
}