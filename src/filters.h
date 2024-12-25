#ifndef FILTERS_H
#define FILTERS_H

#include "core.h"

enum filter_preset_t {
    UNKNOWN,
    BLUR,
    BLURLIGHT,
    BLURMEDIUM,
    EMBOSS,
    OVERLAY,
    SNOWFLAKES,
    HEARTS,
    STARS,
    WHITEFRAME, 
    BLACKFRAME,
};

typedef struct { 
    enum filter_preset_t preset;
    char path[MAX_FILE_PATH_LEN];
    bool useColor;
    color_t color; 
} filter_descriptor_t;

/**
 * @brief Wendet einen Emboss-Filter auf ein Bild an
 * 
 * vergleicht benachbarte Pixel & berechnet einen neuen Wert für das aktuelle Pixel, um einen Emboss-Effekt zu erzeugen. 
 *
 * @param filter Das Filter, das angewendet werden soll
 * @param target Wo der Filter angewendet wird. Es enthält die Pixel, die verändert werden
 * @return int Gibt 0 bei Erfolg zurück, oder -1, wenn eines der Eingabeargumente ungültig ist
 */
int apply_emboss(const filter_descriptor_t *filter, picture_t *target);

/**
 * @brief  Wendet einen Median-Blur auf das aktuelle Pixel unter Verwendung seiner benachbarten Pixel an.
 * 
 * Berechnet den Medianwert für jeden Farbkanal (RGB) der benachbarten Pixel 
 * & wendet den resultierenden Median auf das aktuelle Pixel an. 
 * Der Median wird nach dem Sortieren der Farbwerte der Nachbarpixel berechnet.
 *
 * @param neighbor Ein Array von Zeigern auf benachbarte Pixel
 * @param currentPixel  Ein Zeiger auf das Pixel, auf das der Medianwert angewendet wird
 *
 */
void apply_median_blur(color_t *neighbor[8], color_t *currentPixel);

/**
 * @brief Wendet einen Median-Blur-Filter auf ein Bild an
 * 
 * Iteriert über jedes Pixel & aktualisiert dessen Farbwerte (RGB) anhand des Medians der benachbarten Pixel.
 * Nutzt `get_pixel()` zum Abrufen der Nachbarn und `apply_median_blur()` zur Berechnung des Medianwerts.
 *
 * @param target Ein Zeiger auf das Bild, das gefiltert werden soll. 
 * @return int Gibt 0 zurück, wenn der Filter erfolgreich angewendet wurde. Gibt -1 zurück, wenn das Bild ungültig ist
 */
int median_blur_filter(picture_t *target);

/**
 * @brief Wendet Overlay-Filter auf ein Bild an
 * 
 * Lädt einen vordefinierten Overlay-Filter (z.B. Hearts, Frames) 
 * oder verwendet ein benutzerdefiniertes Bild. Der Filter wird skaliert & auf das Zielbild 
 * angewendet, indem die Farbwerte gemittelt oder bestimmte Farben ausgelassen werden. 
 *
 * Nutzt `load_picture_from_path()` zum Laden des Filters.  
 * Nutzt `scale_image()` zum Skalieren des Filters auf die Zielgröße.  
 * Nutzt `get_pixel()` für den Zugriff auf einzelne Pixel. 
 *
 * @param filter Zeiger auf die Filterbeschreibung
 * @param target Zeiger auf das Zielbild, auf das der Filter angewendet werden soll
 * @return int Gibt 0 bei Erfolg zurück.  
 *             Rückgabewerte bei Fehlern:
 *             - -1: Ungültige Eingaben  
 *             - -2: Unbekannter Filtertyp
 *             - >0: Fehlercode von `load_picture_from_path()` oder `scale_image()`
 */
int apply_overlay(filter_descriptor_t *filter, picture_t *target);

/**
 * @brief Wendet einen einfachen Blur-Filter auf ein Bild an
 * 
 * Berechnet für jedes Pixel im Bild den Durchschnitt der benachbarten Pixel 
 * (oberhalb, unterhalb, links und rechts) & setzt diesen Wert als neuen Farbwert für 
 * das Pixel im Zielbild. Der Effekt ist eine leichte Unschärfe.
 *
 * @param target Zeiger auf das Zielbild, auf das der Unschärfefilter angewendet wird
 * @return int Gibt 0 bei Erfolg zurück, oder `-1` bei ungültigen Eingaben
 */
int blur_filter_light(picture_t *target);

/**
 * @brief Wendet Median-Blur auf ein Bild an
 *
 * berechnet für jedes Pixel im Bild den Durchschnitt der benachbarten Pixel 
 * (einschließlich der Pixel in einer größeren Umgebung, benachbarte Pixel in einem Umkreis von 2).
 * Der Effekt ist eine stärkere Unschärfe als bei Light-Blur
 * 
 * @param target Zeiger auf das Zielbild, auf das der Filter angewendet wird
 * @return int Gibt 0 bei Erfolg zurück, oder `-1` bei ungültigen Eingaben
 */
int blur_filter_medium(picture_t *target);

/**
 * @brief Setzt die Farbe des Filters basierend auf der übergebenen Farbeingabe
 * 
 * nimmt einen Farbstring und setzt die RGB-Werte des Filters entsprechend
 *
 * @param filter 
 * @param colorString 
 */
void set_filter_color(filter_descriptor_t *filter, const char *color_string);

/**
 * @brief Setzt den Filter basierend auf dem übergebenen Namen
 * 
 * Nimmt einen Filternamen als Eingabe & ordnet diesen einem Filtertyp (preset) zu.
 *
 * @param filter Zeiger auf die Filterbeschreibung, die aktualisiert wird
 * @param name Der Name des Filters, der zu einem vordefinierten Filtertyp zugeordnet wird
 */
void set_filter_from_name(filter_descriptor_t *filter, const char *name);

/**
 * @brief  Wendet einen angegebenen Filter auf ein Bild an
 * 
 * entscheidet basierend auf dem Filtertyp (Preset) welcher spezifische Filter angewendet werden soll. 
 *
 * @param filter Zeiger auf die Filterbeschreibung
 * @param target Zeiger auf das Zielbild, auf das der Filter angewendet wird
 * @return int  Gibt 0 bei Erfolg zurück, oder einen negativen Fehlercode:
 *              -1: Ungültige Eingabewerte
 *              -2: Unbekannter Filtertyp
 */
int apply_filter(filter_descriptor_t *filter, picture_t *target);

#endif      /* FILTERS_H */