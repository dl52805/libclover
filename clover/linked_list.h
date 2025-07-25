#pragma once

/*
 * (dl) Linked List Implementation
 */

#include <stdlib.h>

#include "def.h"
#include "arena.h"

#include "math.h"

// NOTE(dl): this header provides macros for common linked list operations,
// however all linked list should be user defined
//
// linked lists should include a node struct as well as a list struct
// containing the length `count` and pointers `head`, `tail`
//
// singly linked list node structs must include a `next` field and doubly
// linked list node structs must include a `prev` field

// singly linked list example

typedef struct vec3_list_node
{
  cl_vec3_t *data;
  struct vec3_list_node *next;
  struct vec3_list_node *prev;
} vec3_list_node_t;

typedef struct vec3_list
{
  size_t count;
  vec3_list_node_t *head;
  vec3_list_node_t *tail;
} vec3_list_t;

#define linked_list_append(list, node_type)                                    \
  do                                                                           \
  {                                                                            \
    node_type *new_elem = (node_type *) calloc(1, sizeof(node_type));          \
                                                                               \
    if (list->tail == NULL)                                                    \
    {                                                                          \
      list->tail = new_elem;                                                   \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      list->tail->next = new_elem;                                             \
      new_elem->next = NULL;                                                   \
      list->tail = new_elem;                                                   \
    }                                                                          \
  } while(0)

