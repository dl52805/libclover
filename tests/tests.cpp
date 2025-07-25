#include <stdio.h>
#include <stdlib.h>

#include "../clover/arena.h"
#include "../clover/array_list.h"

typedef struct point
{
  int x, y, z;
} point_t;

int main()
{
  point_t *point_arr = nullptr;

  for (int i = 1; i <= 10; i += 1)
  {
    point_t curr_point = { .x = i * 12, .y = i * 24, .z = i * 30 };
    array_list_append(point_arr, curr_point);
  }

  point_t curr_point = { .x = 4, .y = 5, .z = 8 };
  array_list_insert(point_arr, 3, curr_point);

  point_t curr_point_2 = { .x = 0, .y = 0, .z = 0 };
  array_list_insert(point_arr, 0, curr_point_2);

  point_t curr_point_3 = { .x = 9, .y = 9, .z = 9 };
  array_list_insert(point_arr, 0, curr_point_3);

  point_t curr_point_4 = { .x = 0, .y = 0, .z = 0 };
  array_list_insert(point_arr, 0, curr_point_4);

  array_list_remove(point_arr, 0);
  array_list_pop(point_arr);

  for (int i = 0; i < arr_count(point_arr); i += 1)
  {
    printf("point: x: %d, y: %d, z: %d\n",
           point_arr[i].x, point_arr[i].y, point_arr[i].z);
  }

  array_list_free(point_arr);
}

