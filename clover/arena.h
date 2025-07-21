/*
 * (dl) Arena Allocator Implementation
 *
 * adapted from https://github.com/tsoding/arena
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "def.h"

#ifndef ARENA_REGION_DEFAULT_CAPACITY
#define ARENA_REGION_DEFAULT_CAPACITY (8 * 1024)
#endif

#ifndef ARRAY_LIST_DEFAULT_CAPACITY
#define ARRAY_LIST_DEFAULT_CAPACITY (256)
#endif

// NOTE(dl): `capacity` and `count` are stored as the number of bytes, not the
// direct size of the memory

typedef struct region
{
  size_t capacity;
  size_t count;
  struct region *next;
  uintptr_t buffer[];
} region_t;

typedef struct arena
{
  region_t *begin;
  region_t *end;
} arena_t;

region_t *region_init(size_t capacity)
{
  size_t region_byte_size = sizeof(region_t) + sizeof(uintptr_t) * capacity;
  region_t *r = (region_t *) calloc(1, region_byte_size);
  assert(r != NULL);

  r->next = NULL;
  r->count = 0;
  r->capacity = capacity;

  return r;
}

void region_free(region_t *region)
{
  free(region);
}

// NOTE(dl): parameter `size` is the actual size of the memory to be alloacted,
// not the number of bytes
void *arena_alloc(arena_t *arena, size_t size)
{
  // NOTE(dl): the number of bytes, not the actual size of memory. this is so
  // that memory within the arena regions are aligned by bytes
  size_t byte_count = (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);

  if (arena->end == NULL)
  {
    assert(arena->begin == NULL);
    size_t capacity = ARENA_REGION_DEFAULT_CAPACITY;
    if (capacity < byte_count) capacity = byte_count;
    arena->begin = region_init(capacity);
    arena->end = arena->begin;
  }

  while ((arena->end->count + byte_count > arena->end->capacity)
         && (arena->end->next != NULL))
  {
    arena->end = arena->end->next;
  }

  if (arena->end->count + byte_count > arena->end->capacity)
  {
    assert(arena->end == NULL);
    size_t capacity = ARENA_REGION_DEFAULT_CAPACITY;
    if (capacity < byte_count) capacity = byte_count;
    arena->end->next = region_init(capacity);
    arena->end = arena->end->next;
  }

  void *result = &arena->end->buffer[arena->end->count];
  arena->end->count += size;
  return result;
}

void *arena_realloc(arena_t *arena, void *old_ptr,
                    size_t old_size, size_t new_size)
{
  if (new_size <= old_size) return old_ptr;

  void *new_ptr = arena_alloc(arena, new_size);

  // convert to char (byte) pointers so that we can copy the data byte by byte
  byte *new_ptr_byte = (byte *) new_ptr;
  byte *old_ptr_byte = (byte *) old_ptr;

  for (size_t i = 0; i < old_size; i++)
  {
    new_ptr_byte[i] = old_ptr_byte[i];
  }

  return new_ptr;
}

void *arena_memcpy(void *dest, const void *src, size_t n)
{
  byte *dest_ptr_byte = (byte *) dest;
  const byte *src_ptr_byte = (byte *) src;

  for (; n; n -= 1) *dest_ptr_byte++ = *src_ptr_byte++;

  return dest;
}

void *arena_memdup(arena_t *arena, void *data, size_t size)
{
  return arena_memcpy(arena_alloc(arena, size), data, size);
}

void arena_reset(arena_t *arena)
{
  for (region_t *region = arena->begin; region != NULL; region = region->next)
  {
    region->count = 0;
  }
}

void arena_free(arena_t *arena)
{
  region_t *region = arena->begin;

  while (region != NULL)
  {
    region_t *curr = region;
    region = region->next;
    region_free(curr);
  }

  arena->end->next = NULL;
}

void arena_trim(arena_t *arena)
{
  region_t *region = arena->end->next;

  while (region != NULL)
  {
    region_t *curr = region;
    region = region->next;
    region_free(curr);
  }

  arena->end->next = NULL;
}

#ifdef __cplusplus
  #define __typeof(v) (decltype(v))
#else
  #define __typeof(v) typeof(v)
#endif

#define arena_array_list_append(a, list, item)                                 \
  do                                                                           \
  {                                                                            \
    if ((list)->count >= (list)->capacity)                                     \
    {                                                                          \
      size_t new_capacity = (list)->capacity == 0                              \
        ? ARRAY_LIST_DEFAULT_CAPACITY : (list)->capacity * 2;                  \
      (list)->items = __typeof((list)->items) arena_realloc(                   \
        (a), (list)->items,                                                    \
        (list)->capacity * sizeof(*((list)->items)),                           \
        new_capacity * sizeof(*((list)->items))                                \
      );                                                                       \
      (list)->capacity = new_capacity;                                         \
    }                                                                          \
    (list)->items[(list)->count++] = (item);                                   \
  } while(0)

#define arena_array_list_append_many(a, list, new_items, new_items_count)      \
  do                                                                           \
  {                                                                            \
    if ((list)->count + (new_items_count) > (list)->capacity)                  \
    {                                                                          \
      size_t new_capacity = (list)->capacity;                                  \
      if (new_capacity == 0) new_capacity = ARRAY_LIST_DEFAULT_CAPACITY;       \
      while ((list)->count + (new_items_count) > new_capacity)                 \
      {                                                                        \
        new_capacity *= 2;                                                     \
      }                                                                        \
      (list)->items = __typeof((list)->items) arena_realloc(                   \
        (a), (list)->items,                                                    \
        (list)->capacity * sizeof(*((list)->items)),                           \
        new_capacity * sizeof(*((list)->items))                                \
      );                                                                       \
      (list)->capacity = new_capacity;                                         \
    }                                                                          \
    arena_memcpy((list)->items + (list)->count, (new_items),                   \
                 (new_items_count) * sizeof(*((list)->items)));                \
    (list)->count += (new_items_count);                                        \
  } while(0)

