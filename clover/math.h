#pragma once

/*
 * (dl) Math Library
 */

#include "def.h"

typedef union cl_vec3
{
  struct
  {
    float x, y, z;
  };

  struct
  {
    float u, v, w;
  };

  struct
  {
    float r, g, b;
  };
} cl_vec3_t;

typedef union cl_vec2
{
  struct
  {
    float x, y;
  };

  struct
  {
    float u, v;
  };

  struct
  {
    float left;
    float right;
  };

  struct
  {
    float width;
    float height;
  };

  struct
  {
    float w, h;
  };
} cl_vec2_t;

typedef union cl_vec4
{
  struct
  {
    float x, y, z, w;
  };

  struct
  {
    float r, g, b, a;
  };
} cl_vec4_t;

