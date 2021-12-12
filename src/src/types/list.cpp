/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-12 19:37:16
 */

#include "list.h"
// void list_add(list_node *node, list_node *list) {
//     list->prev->next = node;
//     node->prev = list->prev;
//     list->prev = node;
//     node->next = list;
// }

// void list_remove(list_node *node) {
//     node->prev->next = node->next;
//     node->next->prev = node->prev;
// }

// MARKER list allocator
template <typename T> __list_node<T> *__list_allocator() { return (__list_node<T> *)kmalloc(sizeof(__list_node<T>)); }