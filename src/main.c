#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "filters.h"
#include "utils.h"
#include "core.h"

/**
 * @brief Gibt die Hilfeoptionen für das Programm aus.
 * Zeigt eine Übersicht der verfügbaren Optionen und deren Beschreibung für das Programm an
 */
void print_help() {
    printf("\nOptions:\n");
    printf("  if=<filename>    Specify the input file (e.g., if=image.ppm)\n");
    printf("  of=<filename>    Specify the output file (e.g., of=newimage.ppm)\n");
    printf("  ff=<filename>    Specify the filter file (e.g., ff=image.ppm)\n");
    printf("  color=<filename> Specify the color of the filter (e.g., color=red)\n");
    printf("  filter=<option>  Apply a filter to the image:\n");
    printf("                   - overlay: overlays filter file to the input image\n");
    printf("                   - emboss: applies emboss filter\n");
    printf("                   - blur-median: applies median blur filter\n");
    printf("                   - blur-light: applies blur light filter\n");
    printf("                   - blur-medium: applies blur medium filter\n");
    printf("                   - blackframe: adds a black frame\n");
    printf("                   - whiteframe: adds a white frame\n");
    printf("                   - snowflakes: adds snowflakes\n");
    printf("                   - hearts: adds hearts\n");
    printf("                   - stars: adds stars\n");
    printf("  help             Show this help message\n");
    printf("\nExample:\n");
    printf("  ./imagefilter if=image.ppm of=newimage.ppm filter=emboss\n\n");
}

/**
 * @brief Hauptfunktion des Programms
 * 
 * Liest Eingabeargumente, lädt ein Bild, wendet Filter an & speichert das resultierende Bild ab. 
 * Zeigt eine Hilfe an, wenn keine Argumente übergeben werden oder gibt Fehler bei ungültigen Eingaben aus.
 *
 * @param argc argc Anzahl der Argumente, die beim Aufruf des Programms über die Kommandozeile übergeben werden
 * @param argv Array der Kommandozeilenargumente
 * @return int Rückgabewert des Programms: 0 bei Erfolg, nicht null bei Fehler
 */
int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help();
        return -1;
    }

    char outputPath[MAX_FILE_PATH_LEN] = {0};  
    char inputPath[MAX_FILE_PATH_LEN] = {0};
    filter_descriptor_t filter = {0};
    picture_t target = {0};   
    int status = 0;
    
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];   

        // Output Dateipfad setzen (z.b. of=output.ppm)
        if (starts_with(arg, "of=") == 1) {
            memcpy(outputPath, arg+3, MAX_FILE_PATH_LEN);
        } 
        // Input Dateipfad setzen (z.b. if=input.ppm)
        else if (starts_with(arg, "if=") == 1) {
            memcpy(inputPath, arg+3, MAX_FILE_PATH_LEN);
        }
        // Filter Dateipfad setzen
        else if (starts_with(arg, "ff=") == 1) {
            memcpy(filter.path, arg+3, MAX_FILE_PATH_LEN);
        }
        else if (starts_with(arg, "filter=") == 1) {
            set_filter_from_name(&filter, arg+7);
        }
        else if (starts_with(arg, "color=") == 1) {
            set_filter_color(&filter, arg+6);
            filter.useColor = true;
        }
        else if (starts_with(arg, "help") == 1) {
            print_help(); 
            return 0;
        }
        else {
            printf("Unknown argument: %s\n", arg);
        }
    }

    if (strlen(inputPath) < 1) {
        printf("No input file provided, exiting!\n");
        status = -1;
    }

    status = load_picture_from_path(inputPath, &target);
    if (status != 0) {
        printf("Error loading input file %d, exiting!\n", status);
        goto cleanup;
    }

    // Wenn output_file nicht angegeben wird, wird automatisch eine Datei erstellt
    if (strlen(outputPath) < 1) {
        const char *prefix = "new-";
        const char *lastSlash = strrchr(inputPath, '/');      

        if (lastSlash) {
            char temp[MAX_FILE_PATH_LEN];
            strcpy(temp, prefix);           // Prefix kopieren
            strcat(temp, lastSlash + 1);    // Dateinamen anhängen
            strcpy(outputPath, temp);       // outputPath wird gesetzt, inputPath bleibt unverändert
        }
        else {
            // Wenn kein Slash gefunden wird, das Präfix zu input_path anfügen
            strcpy(outputPath, prefix);
            strcat(outputPath, inputPath);
        }
        validate_output_path(outputPath);
    }  

    if (filter.preset == UNKNOWN) {
        printf("Unknown filter preset, exiting!\n");
        status = -1;
    }

    if (filter.preset == OVERLAY && strlen(filter.path) < 1) {   // Wenn der Filter auf OVERLAY gesetzt ist, aber kein Pfad zur Filterdatei angegeben wurde 
        printf("Frame overlay requires filter file ff=/path/to/filter.ppm, exiting!\n");
        status = -1;
    }

    printf("Picture size: x:%u, y:%u\n", target.x, target.y);

    status = apply_filter(&filter, &target);
    if (status != 0) {
        printf("Error applying filter: %d, exiting!\n", status);
        goto cleanup;
    }

    status = generate_file_from_picture(outputPath, &target);
    if (status != 0) {
        printf("Error generating output file: %d, exiting!\n", status);
        goto cleanup;
    }

    printf("File saved to %s\n", outputPath);

    cleanup:
    if (target.pixels) {
        free(target.pixels);
        target.pixels = NULL;
    }

    return 0;
}