#ifndef UTILS_H
#define UTILS_H

#include "core.h"

/**
 * @brief Gibt das Pixel an der angegebenen Position im Bild zurück
 * 
 * Sortiert Pixel für das Median-Filter
 *
 * @param target  Das Bild, aus dem das Pixel abgerufen werden soll
 * @param x Die x-Koordinate des Pixels im Bild (Breite)
 * @param y Die y-Koordinate des Pixels im Bild (Höhe)
 * @return color_t* Ein Zeiger auf das Pixel an der angegebenen Position oder NULL, 
 * wenn das Bild ungültig ist oder die Koordinaten außerhalb des Bildes liegen.
 */
color_t *get_pixel(const picture_t *target, uint32_t x, uint32_t y);

/**
 * @brief Generiert eine Bilddatei im PPM-Format (P3 oder P6) aus den Bilddaten und speichert sie unter dem angegebenen Pfad.
 * 
 * Schreibt die Bilddaten aus der target Struktur in eine Datei im PPM-Format. 
 * Je nach dem Bildformat (P3/P6) werden die Pixel entweder im ASCII- oder im Binärformat gespeichert.
 *
 * @param path Der Dateipfad, unter dem das Bild gespeichert werden soll
 * @param target Die Struktur, die das Bild enthält und die in die Datei geschrieben wird
 * @return int 0 bei Erfolg, andernfalls ein Fehlercode:
 *            -1: Ungültige Eingabedaten 
 *            -2: Fehler beim Öffnen der Datei
 */
int generate_file_from_picture(const char *path, picture_t *target);

/**
 * @brief Lädt ein Bild von einem angegebenen Dateipfad und speichert es im target
 *
 * lädt ein Bild im PPM-Format (P3 oder P6) aus einer Datei. 
 Es werden die Bilddimensionen, der maximal mögliche Farbwert und die Pixeldaten aus der Datei gelesen.
 *
 * @param path Der Dateipfad zum Bild, das geladen werden soll.
 * @param target Die Struktur, in der das geladene Bild gespeichert wird. 
 * @return int 0 bei Erfolg, andernfalls -1 bei einem Fehler
 */
int load_picture_from_path(const char *path, picture_t *target);

/**
 * @brief Berechnet das Skalierungsverhältnis zwischen zwei Bildern
 * 
 * Ermittelt die Skalierungsfaktoren für die x- und y-Achse, basierend auf 
 * den Abmessungen der Quell- und Zielbilder.
 *
 * @param source Zeiger auf das Quellbild
 * @param target Zeiger auf das Zielbild
 * @return scale_t Struktur mit den berechneten Skalierungsfaktoren
 */
scale_t get_scale(picture_t *source, picture_t *target);

/**
 * @brief Überprüft, ob ein Wort mit einem angegebenen Präfix beginnt
 * 
 * @param word Das Wort, das auf das Präfix überprüft wird.
 * @param prefix Das Präfix, mit dem das Wort verglichen wird.
 * @return int 1 wenn das Wort mit dem Präfix beginnt, 0 wenn nicht, oder -1 bei Fehler
 */
int starts_with(const char *word, const char *prefix);

/**
 * @brief Skaliert ein Bild anhand eines angegebenen Skalierungsfaktors
 * 
 * Ändert die Größe eines Bildes, indem die Pixel entsprechend den 
 * Skalierungsfaktoren `scale.x` und `scale.y` neu berechnet werden. 
 * Jeder neue Pixel wird durch den entsprechenden Pixel aus dem Quellbild ersetzt 
 *
 * @param source Zeiger auf das Quellbild, das skaliert werden soll.
 * @param scale Struktur, die die Skalierungsfaktoren für x- und y-Achse enthält
 * @return int 0 bei Erfolg, -1 bei ungültigem Bild, -2 bei ungültiger Zielgröße, -3 bei Speicherproblemen
 */
int scale_image(picture_t *source, scale_t scale);

/**
 * @brief Überprüft, ob der angegebene Pfad mit "new-" beginnt und der Prefix "new-" im Pfad enthalten ist
 * 
 * @param path Der Pfad, der auf das Vorhandensein des Präfixes "new-" überprüft wird
 * @return int 0, wenn der Pfad das Präfix "new-" enthält, andernfalls 1.
 */
int validate_output_path(const char *path);

/**
 * @brief Vergleichsfunktion für die `qsort`-Funktion, die zwei `uint8_t` Werte vergleicht
 * 
 * Für das Median-Filter
 * qsort nutzt das Ergebnis der Vergleichsfunktion, um die Elemente zu sortieren. 
 * Wenn die Funktion negativen Wert zurückgibt, wird das erste Element als kleiner angesehen und bleibt vor dem zweiten Element.
 *
 * @param a Zeiger auf das erste zu vergleichende Element
 * @param b Zeiger auf das zweite zu vergleichende Element
 * @return int Ein negativer Wert, null oder ein positiver Wert, der angibt, wie die beiden Elemente verglichen werden:
 *         - Ein Wert < 0, wenn *a < *b
 *         - 0, wenn *a == *b
 *         - Ein Wert > 0, wenn *a > *b
 */
int compare(const void* a, const void* b);

/**
 * @brief Vergleicht zwei Gleitkommazahlen auf Gleichheit unter Berücksichtigung eines Toleranzwerts
 * 
 *  Nutzt den konstanten Toleranzwert EPSILON, um zwei double-Werte auf nahezu gleiche Werte zu prüfen
 *
 * @param a Erster Wert
 * @param b Zweiter Wert
 * @return true Wenn die Werte innerhalb des Toleranzbereichs gleich sind
 * @return false Wenn die Werte ungleich sind
 */
bool are_same(double a, double b);

#endif      /* UTILS_H */