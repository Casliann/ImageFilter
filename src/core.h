#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_FILE_PATH_LEN 512
#define EPSILON 0.0001

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} color_t; 

typedef struct {
    uint32_t maxColorValue; 
    uint8_t format[3];
    uint32_t x;         // Breite 
    uint32_t y;         // Höhe
    color_t *pixels;    
} picture_t; 

typedef struct {
    float x; 
    float y; 
} scale_t;

#endif      /* CORE_H */