#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "filters.h"
#include "utils.h"
#include "core.h"

int apply_emboss(const filter_descriptor_t *filter, picture_t *target) {    
    if (!filter || !target) {
        return -1;   
    }

    for (uint32_t i = 1; i < target->x * target->y - 1; i++) {
        color_t pixel = target->pixels[i];
        color_t pixelPrev = target->pixels[i - 1];
        color_t pixelNext = target->pixels[i + 1];

        int sumRed = pixelNext.red - pixelPrev.red + 128;
        int sumGreen = pixelNext.green - pixelPrev.green + 128;
        int sumBlue = pixelNext.blue - pixelPrev.blue + 128;

        pixel.red = (uint8_t)sumRed;
        pixel.green = (uint8_t)sumGreen;
        pixel.blue = (uint8_t)sumBlue;

        target->pixels[i] = pixel; 
    }

    return 0;
}  

void apply_median_blur(color_t *neighbor[8], color_t *currentPixel) {
    uint8_t red[8];
    uint8_t green[8];
    uint8_t blue[8];

    size_t redCount = 0;
    size_t greenCount = 0;
    size_t blueCount = 0;

    for (size_t i = 0; i < 8; i++) {
        if (neighbor[i] != NULL) {
            red[redCount++] = neighbor[i]->red;
            green[greenCount++] = neighbor[i]->green;
            blue[blueCount++] = neighbor[i]->blue;
        }
    }

    // Sortieren der Farbkanäle
    qsort(red, redCount, sizeof(uint8_t), compare);
    qsort(green, greenCount, sizeof(uint8_t), compare);
    qsort(blue, blueCount, sizeof(uint8_t), compare);

    // Median anwenden
    currentPixel->red = red[redCount / 2];    // redCount enthält die Anzahl der gültigen Pixel für rot 
    currentPixel->green = green[greenCount / 2];
    currentPixel->blue = blue[blueCount / 2];
}

int median_blur_filter(picture_t *target) {
    if (!target) {
        return -1; 
    }
    
    for (uint32_t y = 0; y < target->y; y++) {
        for (uint32_t x = 0; x < target->x; x++) {
            color_t *neighbor[8];

            // Nachbarn speichern
            neighbor[0] = get_pixel(target, x, y+1);    
            neighbor[1] = get_pixel(target, x+1, y+1);
            neighbor[2] = get_pixel(target, x+1, y);
            neighbor[3] = get_pixel(target, x+1, y-1);
            neighbor[4] = get_pixel(target, x, y-1);
            neighbor[5] = get_pixel(target, x-1, y-1);
            neighbor[6] = get_pixel(target, x-1, y);
            neighbor[7] = get_pixel(target, x-1, y+1);

            color_t *currentPixel = get_pixel(target, x, y);    // Zentrum
            apply_median_blur(neighbor, currentPixel);    
        }  
    }   
    return 0;
}

int apply_overlay(filter_descriptor_t *filter, picture_t *target) {
    if (!filter || !target) {
        return -1; 
    }

    // Filterpfad basierend auf Preset setzen
    picture_t filterImage;
    char *path; 
    switch(filter->preset) {
        case SNOWFLAKES:
            path = "assets/snowflakes.ppm";
            break; 
        case HEARTS: 
            path = "assets/hearts.ppm";
            break; 
        case STARS: 
            path = "assets/stars.ppm";
            break; 
        case BLACKFRAME:
            path = "assets/blackframe.ppm";
            break; 
        case WHITEFRAME:
            path = "assets/whiteframe.ppm";
            break;
        case OVERLAY:
            path = filter->path; 
            break; 
        default: 
            return -2; 
    }

    // Filterbild laden
    int status = load_picture_from_path(path, &filterImage);
    if (status) {
        return status;
    } 

    // Filterbild skalieren
    scale_t scale = get_scale(&filterImage, target);
    status = scale_image(&filterImage, scale);
    if (status) {
        if (filterImage.pixels) {
            free (filterImage.pixels);
            filterImage.pixels = NULL;
        }
        return status;
    }
    
    // Overlay anwenden
    for (uint32_t x = 0; x < target->x; x++) {
        for (uint32_t y = 0; y < target->y; y++) {
            color_t *currentPixel = get_pixel(target, x, y);
            color_t *currentPixelOfTheFrame = get_pixel(&filterImage, x, y);

            if (!currentPixel || !currentPixelOfTheFrame) {
                continue;
            }
                
            // Schwarzer Rahmen: weiße Stellen ignorieren
            if (filter->preset == BLACKFRAME) {
                if (currentPixelOfTheFrame->red == 0xff && currentPixelOfTheFrame->green == 0xff && currentPixelOfTheFrame->blue == 0xff) {
                    continue;  
                }
                *currentPixel = *currentPixelOfTheFrame;
            }

            // Weißer Rahmen: schwarze Stellen ignorieren
            else if (filter->preset == WHITEFRAME) {
                if (currentPixelOfTheFrame->red == 0x00 && currentPixelOfTheFrame->green == 0x00 && currentPixelOfTheFrame->blue == 0x00) {
                    continue;  
                }
                *currentPixel = *currentPixelOfTheFrame;
            }

            // Durchschnitt für RGB für Overlay-Effekt
            uint32_t sumRed = currentPixel->red;
            uint32_t sumGreen = currentPixel->green;
            uint32_t sumBlue = currentPixel->blue;

            // filter->color.blue = 0xFF;
            // filter->color.red = 0xFF;
            // filter->color.green = 0xFF;

            if (filter->useColor) {
                    if (!!filter->color.red) {
                    sumRed += currentPixelOfTheFrame->red * !!filter->color.red;
                    currentPixel->red = (uint8_t)(sumRed / 2);
                }

                if (!!filter->color.green) {
                    sumGreen += currentPixelOfTheFrame->green * !!filter->color.green;
                    currentPixel->green = (uint8_t)(sumGreen / 2);
                }

                if (!!filter->color.blue) {
                    sumBlue += currentPixelOfTheFrame->blue * !!filter->color.blue;
                    currentPixel->blue = (uint8_t)(sumBlue / 2);
                }
            } 
            
            else {
                sumRed += currentPixelOfTheFrame->red;
                sumGreen += currentPixelOfTheFrame->green;
                sumBlue += currentPixelOfTheFrame->blue;
                currentPixel->red = (uint8_t)(sumRed / 2);
                currentPixel->green = (uint8_t)(sumGreen / 2);
                currentPixel->blue = (uint8_t)(sumBlue / 2);
            }
        }    
    } 

    if (filterImage.pixels) {
        free(filterImage.pixels);
        filterImage.pixels = NULL;
    }
    return 0;
}

int blur_filter_light(picture_t *target) {
    if (!target) {
        return -1;  
    }

    for (uint32_t y = 1; y < target->y - 1; y++) {
        for (uint32_t x = 1; x < target->x - 1; x++) {
            
            color_t *currentPixel = get_pixel(target, x, y);
            color_t *abovePixel = get_pixel(target, x, y-1);
            color_t *belowPixel = get_pixel(target, x, y+1);
            color_t *leftPixel = get_pixel(target, x-1, y);
            color_t *rightPixel = get_pixel(target, x+1, y);
            
            // Berechnung der Summen der Farbwerte der benachbarten Pixel
            uint32_t sumRed = currentPixel->red + abovePixel->red + belowPixel->red + leftPixel->red + rightPixel->red;
            uint32_t sumGreen = currentPixel->green + abovePixel->green + belowPixel->green + leftPixel->green + rightPixel->green;
            uint32_t sumBlue = currentPixel->blue + abovePixel->blue + belowPixel->blue + leftPixel->blue + rightPixel->blue;

            // Durchschnitt der Nachbarn berechnen
            currentPixel->red  = (uint8_t)(sumRed / 5); 
            currentPixel->green = (uint8_t)(sumGreen / 5); 
            currentPixel->blue = (uint8_t)(sumBlue / 5); 
        }
    }
    return 0;
}

int blur_filter_medium(picture_t *target) {
    if (!target) {
        return -1;     
    }
    
    for (uint32_t y = 1; y < target->y - 1; y++) {
        for (uint32_t x = 1; x < target->x - 1; x++) {
            color_t *neighbor[12];

            color_t *currentPixel = get_pixel(target, x, y);
            neighbor[0] = get_pixel(target, x+1, y+1);
            neighbor[1]= get_pixel(target, x, y+1);
            neighbor[2] = get_pixel(target, x-1, y+1);
            neighbor[3] = get_pixel(target, x-1, y);
            neighbor[4] = get_pixel(target, x+1, y);
            neighbor[5] = get_pixel(target, x-1, y-1);
            neighbor[6] = get_pixel(target, x, y-1);
            neighbor[7] = get_pixel(target, x+1, y-1);
            neighbor[8] = get_pixel(target, x, y-2);
            neighbor[9] = get_pixel(target, x, y+2);
            neighbor[10] = get_pixel(target, x+2,y); 
            neighbor[11] = get_pixel(target, x-2, y);

            uint32_t sumRed = currentPixel->red;
            uint32_t sumGreen = currentPixel->green;
            uint32_t sumBlue = currentPixel->blue;
               
            for (size_t i = 0; i < 12; i++) {
                if (neighbor[i] != NULL) {
                    sumRed += neighbor[i]->red;
                    sumGreen += neighbor[i]->green;
                    sumBlue += neighbor[i]->blue;
                }
            }
            currentPixel->red = sumRed / 13;
            currentPixel->green = sumGreen / 13;
            currentPixel->blue = sumBlue / 13;  
        }
    }
    return  0;
}

int apply_filter(filter_descriptor_t *filter, picture_t *target) { 
    if (!filter || !target) {
        return -1;
    }
    switch (filter->preset) {
        case EMBOSS:
            return apply_emboss(filter, target);
        case BLUR:
            return median_blur_filter(target);
        case BLURLIGHT:
            return blur_filter_light(target);
        case BLURMEDIUM:
            return blur_filter_medium(target);
        case OVERLAY:
        case WHITEFRAME:
        case BLACKFRAME:
        case HEARTS:
        case STARS:
        case SNOWFLAKES:
            return apply_overlay(filter, target);
        default:
            return -2;
    }
}

void set_filter_from_name(filter_descriptor_t *filter, const char *name) {
    if (!filter) {
        return;
    }
    if (!name) {
        filter->preset = UNKNOWN;
        return;
    }
    if (strcmp(name, "emboss") == 0) {
        filter->preset = EMBOSS;
    }
    else if (strcmp(name, "snowflakes") == 0) {
        filter->preset = SNOWFLAKES;
    }
    else if (strcmp(name, "hearts") == 0) {
        filter->preset = HEARTS;
    }
    else if (strcmp(name, "stars") == 0) {
        filter->preset = STARS;
    }
    else if (strcmp(name, "overlay") == 0) {
        filter->preset = OVERLAY;
    }
    else if (strcmp(name, "blur-median") == 0) {
        filter->preset = BLUR;
    }
    else if (strcmp(name, "blur-light") == 0) {
        filter->preset = BLURLIGHT;
    }
    else if (strcmp(name, "blur-medium") == 0) {
        filter->preset = BLURMEDIUM;
    }
    else if (strcmp(name, "whiteframe") == 0) {
        filter->preset = WHITEFRAME; 
    }
    else if (strcmp(name, "blackframe") == 0) {
        filter->preset = BLACKFRAME; 
    }
    else {
        filter->preset = UNKNOWN;
    }
}

void set_filter_color(filter_descriptor_t *filter, const char *colorString) {
    if (!filter) {
        return;
    }

    // Wenn der colorString NULL ist, wird Filter auf weiße Farbe gesetzt. RGB(255, 255, 255) 
    if (!colorString) {    
        filter->color.red = 0xff;
        filter->color.green = 0xff;
        filter->color.blue = 0xff;
        return;
    }
    // Wenn die Eingabe "red" ist, wird Filter auf pure rote Farbe gesetz
    if (strcmp(colorString, "red") == 0) {    
        filter->color.red = 0xff;
        filter->color.green = 0x00;
        filter->color.blue = 0x00;
    }
    else if (strcmp(colorString, "green") == 0) {
        filter->color.red = 0x00;
        filter->color.green = 0xff;
        filter->color.blue = 0x00;
    }
    if (strcmp(colorString, "blue") == 0) {
        filter->color.red = 0x00;
        filter->color.green = 0x00;
        filter->color.blue = 0xff;
    }
}