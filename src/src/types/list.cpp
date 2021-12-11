/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-11 18:29:30
 */

#include "list.h"

void list_add(list_node *node, list_node *list) {
    list->prev->next = node;
    node->prev = list->prev;
    list->prev = node;
    node->next = list;
}

void list_remove(list_node *node) {
    node->prev->next = node->next;
    node->next->prev = node->prev;
}