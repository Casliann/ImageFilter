#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "utils.h"
#include "core.h"

int starts_with(const char* word, const char* prefix) {
    if (!word || !prefix) {    // Wenn Zeiger Null ist
        return -1;
    }
    size_t prefixLen = strlen(prefix);
    size_t wordLen = strlen(word);

    // Wenn das Prefix länger als das Wort ist, kann es nicht am Anfang stehen
    if (prefixLen > wordLen) {
        return 0;
    }

    // Zeichenweise Überprüfung, ob das Wort mit dem Prefix beginnt
    for (size_t i = 0; i < prefixLen; i++) {
        if (word[i] != prefix[i]) {
            return 0;
        }
    }
    return 1;
}

int load_picture_from_path(const char* path, picture_t *target) {
    if (!path || !target) {
        return -1;
    }
    char line[500];

    FILE *file = fopen(path, "rb");  
    
    if (file == NULL) {
        perror("ERROR opening file!\n");
        return -1;
    }
    
    // Lese das Format (P3 oder P6)
    int formatValid = fscanf(file, "%2s", target->format);    // P3 oder P6
    if (formatValid != 1 || (target->format[0] != 'P') || ((target->format[1] != '3' && target->format[1] != '6'))) {
        printf("Unsopported image format.\n");
        fclose(file);
        return -1; 
    }
    
    // Lese Breite & Höhe 
    if (fscanf(file, "%u %u", &target->y, &target->x) != 2) {
        printf("Failed to read image dimensions.\n");
        fclose(file);
        return -1; 
    }

    // Lese den maximalen Farbwert 
    if (fscanf(file, "%u", &target->maxColorValue) != 1) {
        printf("Failed to read max color value.\n");

        fclose(file);
        return -1;
    }
    
    // Lese Header & überspringe Kommentare 
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] == '#') {
            continue;
        }
        break;
    }
    
    // Speicher für Pixel zuweisen
    uint32_t dataSegmentSize = target->x * target->y;
    if (dataSegmentSize == 0) {
        printf("Invalid image size.\n");
        fclose(file);
        return -1;
    }
    printf("Allocating %u kB\n", dataSegmentSize / 1024);
    target->pixels = malloc(dataSegmentSize * sizeof(color_t));
    if (target->pixels == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return -1;
    }

    // Pixeldaten lesen (für jedes Pixel werden RGB-Farbkomponente einzeln gelesen)
    if (target->format[1] == '3') {    // ASCII
       for (uint32_t i = 0; i < dataSegmentSize; i++) {
            int red, green, blue;
            if (fscanf(file, "%d %d %d", &red, &green, &blue) != 3) {
                printf("Failed to read pixel data.\n");
                free(target->pixels);
                target->pixels = NULL;
                fclose(file);
                return -1;
            }
            target->pixels[i].red = (uint8_t)red;
            target->pixels[i].green = (uint8_t)green; 
            target->pixels[i].blue = (uint8_t)blue;
        }  
    }
    else {    // Binärmodus
        for (uint32_t i = 0; i < dataSegmentSize; i++) {
            target->pixels[i].red = fgetc(file);
            target->pixels[i].green = fgetc(file);
            target->pixels[i].blue = fgetc(file);
        }
    }
    fclose(file);
    return 0;
}

int generate_file_from_picture(const char* path, picture_t *target) { 
    if (!path ||!target) {
        return -1;
    }
    uint32_t numPixels = target->x * target->y;
    FILE *file = fopen(path, "wb"); 
    if (file == NULL) {
        return -2;
    }

    // Header schreiben
    fprintf(file, "%s\n", target->format);             // Format (P3 oder P6)
    fprintf(file, "%u %u\n", target->y, target->x);    // Breite und Höhe
    fprintf(file, "%u\n", target->maxColorValue);      // Maximaler Farbwert
    
    // Pixel-Daten schreiben
    if (target->format[1] == '6') {    // Binär
        for (uint32_t i = 0; i < numPixels; i++) {
            fwrite(&target->pixels[i].red, sizeof(uint8_t), 1, file);
            fwrite(&target->pixels[i].green, sizeof(uint8_t), 1, file);
            fwrite(&target->pixels[i].blue, sizeof(uint8_t), 1, file);
        }  
    }
    else if (target->format[1] == '3') {    // ASCII
        for (uint32_t i = 0; i < numPixels; i++) {
            fprintf(file, "%d %d %d\n", 
                    target->pixels[i].red,
                    target->pixels[i].green,
                    target->pixels[i].blue);
        }
    }
    fclose(file);
    return 0;
}

int validate_output_path(const char *path) {
    const char *prefix = "new-";
    size_t prefixLen = strlen(prefix);

    if (strncmp(path, prefix, prefixLen) == 0) {
        if (strstr(path, prefix)) {
            return 0;
        }
    }
    return 1; 
}

color_t *get_pixel(const picture_t *target, uint32_t x, uint32_t y) {
    if (!target) {
        printf("Cannot get pixel from NULL target\n");
        return NULL;
    }
    
    if (target->y < 1 || target->x < 1) {
        printf("Cannot get pixel from 0 size image\n");
        return NULL;
    }

    if (x > target->x - 1 || y > target->y - 1) {
        return NULL;
    }
    return &target->pixels[x + y * target->y];    // Pixel spaltenweise berechnen
}



bool are_same(double a, double b) {
    return fabs(a - b) < EPSILON;
}


int compare(const void* a, const void* b) {
   return (*(uint8_t*)a - *(uint8_t*)b);
}


scale_t get_scale(picture_t *source, picture_t *target) {
    scale_t scale; 
    scale.x = (float)target->x / source->x;
    scale.y = (float)target->y / source->y;
    return scale; 
}

int scale_image(picture_t *source, scale_t scale) {
    if (!source) {
        return -1;
    }
    if (are_same(scale.x, 1.0) && are_same(scale.y, 1.0)) {
        return 0;
    }

    // Neue Bildgröße berechnen
    uint32_t newX = source->x * scale.x;
    uint32_t newY = source->y * scale.y;

    uint32_t dataSegmentSize = newX * newY;
    if (dataSegmentSize < 1) {
        return -2;
    }

    color_t *tempPixels = (color_t*)malloc(dataSegmentSize * sizeof(color_t));
    if (!tempPixels) {
        return -3;
    }

    for (uint32_t y = 0; y < newY; y++) {
        for (uint32_t x = 0; x < newX; x++) {
            int srcX = (int)(x / scale.x);  
            int srcY = (int)(y / scale.y);

            size_t offset = x + y * newX;
        
            color_t *currentTargetPixel = &tempPixels[offset];
            color_t *srcPixel = get_pixel(source, srcX, srcY);
            if(srcPixel) {
                *currentTargetPixel = *srcPixel;
            }
        }
    }

    // Pixeldaten ersetzen
    free(source->pixels);
    source->pixels = tempPixels;
    source->x = newX;
    source->y = newY;

    return 0; 
}