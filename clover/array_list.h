/*
 * (dl) Array List (Dynamic Array) Implementation
 */

#pragma once

#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "arena.h"

typedef struct list_header
{
  size_t count;
  size_t capacity;
} list_header_t;

#define arr_header(list)   ((list_header_t *) list - 1)
#define arr_count(list)    arr_header(list)->count
#define arr_capacity(list) arr_header(list)->capacity

#define array_list_append(list, item)                                          \
  do                                                                           \
  {                                                                            \
    if (list != NULL)                                                          \
    {                                                                          \
      if (arr_count(list) + 1 < arr_capacity(list))                            \
      {                                                                        \
        list[arr_count(list)++] = (item);                                      \
      }                                                                        \
      else                                                                     \
      {                                                                        \
        size_t new_capacity = arr_capacity(list) * 1.5;                        \
        void *temp = realloc(                                                  \
          arr_header(list),                                                    \
          sizeof(list_header_t) + sizeof(*list) * new_capacity                 \
        );                                                                     \
        list = __typeof(list) ((byte *) temp + sizeof(list_header_t));         \
        arr_capacity(list) = new_capacity;                                     \
        list[arr_count(list)++] = (item);                                      \
      }                                                                        \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      list = __typeof(list) calloc(1, sizeof(list_header_t)                    \
                    + sizeof(*list) * ARRAY_LIST_DEFAULT_CAPACITY);            \
      list = __typeof(list) ((byte *) list + sizeof(list_header_t));           \
      arr_count(list) = 0;                                                     \
      arr_capacity(list) = ARRAY_LIST_DEFAULT_CAPACITY;                        \
      list[arr_count(list)++] = (item);                                        \
    }                                                                          \
  } while(0)

#define array_list_insert(list, idx, item)                                     \
  do {                                                                         \
    if (list != NULL)                                                          \
    {                                                                          \
      if ((idx > arr_count(list)) || (idx < 0))                                \
      {                                                                        \
        break;                                                                 \
      }                                                                        \
                                                                               \
      if (arr_count(list) + 1 >= arr_capacity(list))                           \
      {                                                                        \
        size_t new_capacity = arr_capacity(list) * 1.5;                        \
        void *temp = realloc(                                                  \
          arr_header(list),                                                    \
          sizeof(list_header_t) + sizeof(*list) * new_capacity                 \
        );                                                                     \
        list = __typeof(list) ((byte *) temp + sizeof(list_header_t));         \
        arr_capacity(list) = new_capacity;                                     \
      }                                                                        \
                                                                               \
      if (idx == arr_count(list))                                              \
      {                                                                        \
        list[arr_count(list)++] = (item);                                      \
      }                                                                        \
      else                                                                     \
      {                                                                        \
        memmove(&list[idx + 1], &list[idx],                                    \
                (arr_count(list) - idx) * sizeof(*list));                      \
        list[idx] = (item);                                                    \
        arr_count(list) += 1;                                                  \
      }                                                                        \
    }                                                                          \
  } while(0)

#define array_list_pop(list)                                                   \
  do                                                                           \
  {                                                                            \
    if (list != NULL)                                                          \
    {                                                                          \
      arr_count(list) -= 1;                                                    \
    }                                                                          \
  } while(0)

#define array_list_remove(list, idx)                                           \
  do                                                                           \
  {                                                                            \
    if (list != NULL)                                                          \
    {                                                                          \
      if ((idx >= arr_count(list)) || (idx < 0))                               \
      {                                                                        \
        break;                                                                 \
      }                                                                        \
                                                                               \
      if (idx != arr_count(list) - 1)                                          \
      {                                                                        \
        memmove(&list[idx], &list[idx + 1],                                    \
                (arr_count(list) - (idx + 1)) * sizeof(*list));                \
      }                                                                        \
      arr_count(list) -= 1;                                                    \
    }                                                                          \
  } while(0)

#define array_list_free(list)                                                  \
  do                                                                           \
  {                                                                            \
    free(arr_header(list));                                                    \
    list = NULL;                                                               \
  } while(0)

