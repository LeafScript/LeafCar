#ifndef _BASE_TYPE_H_
#define _BASE_TYPE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif