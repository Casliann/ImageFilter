## Kompilierung

Alle benötigten Header-Dateien und Bibliotheken sind im Projekt enthalten.

Zur Kompilierung des Projekts kann das mitgelieferte Makefile verwendet werden. Dazu muss der folgende Befehl ausgeführt werden:
```bash
make
```
Das Programm kann mit diesem Befehl gestartet werden: (Die genaue Ausführung ist unten beschrieben)
```bash
./imagefilter
```

## Ausführung

Um die Anwendung zu starten, tippe `./imagefilter help` ein, um eine Hilfe-Nachricht zu erhalten. Diese enthält den richtigen Syntax und die verfügbaren Optionen.

## Optionen

- `if=<filename>` : Eingabebild (z.B. `if=image.ppm`)
- `of=<filename>` : Ausgabebild
- `ff=<filename>` : Optional, Filterbild für Overlay-Effekte
- `color=<color>` : Optional, Farbe des Filters (z.B. `red`, `green`, `blue`)
- `filter=<option>` : Auswahl des Filters:
  - `overlay`: Überlagert das Filterbild auf das Eingabebild
  - `emboss`: Emboss-Effekt
  - `blur-median`: Median-Blur
  - `blur-light`: Leichter Blur
  - `blur-medium`: Mittlerer Blur
  - `blackframe`: Schwarzer Rahmen
  - `whiteframe`: Weißer Rahmen
  - `snowflakes`: Schneeflocken
  - `hearts`: Herzen
  - `stars`: Sterne
- `help`: Zeigt eine Hilfe-Nachricht an

## Beispiele für die Anwendung

```bash
./imagefilter if=input.ppm of=newfile.ppm filter=hearts
```
Der Befehl wendet den Hearts-Filter auf das Eingabebild `input.ppm` an und speichert das Ergebnis unter dem Namen `newfile.ppm`.

```bash
./imagefilter if=input.ppm ff=input2.ppm filter=overlay color=blue
```
Der Befehl wendet den Overlay-Filter auf das Bild `input.ppm` an, indem er das Overlay-Bild `input2.ppm` mit einer blauen Transparenzfarbe überlagert und das Ergebnis speichert. Das Ergebnis wird automatisch im File `new-input.ppm` gespeichert.

## Abgeschlossene Aufgaben
- Alle Tasks (Task 1 - 5) außer Task 6 sind abgeschlossen.
- Implementierung eines Filtersystems mit verschiedenen Filtern (Blur, Emboss, Rahmen)
- Unterstützung für PPM-Dateien im ASCII- und Binärformat.
- Das Ergebnis wird in einer neuen Datei gespeichert.
- In `/assets` sind Filtervorlagen für Stars, Snowflakes, Hearts und Frames zu finden, sowie Eingabebilder.
- Unterstützung für Bildüberlagerungen mit benutzerdefinierter Transparenzfarbe und Farbauswahl für die Überlagerung
- Bildskalierung: Das zweite Bild wird dem ersten skaliert.
- Erweiterung des Blur-Filters: `blur-light`, `blur-medium`

## Zusätzliche Features
- Wenn kein Ausgabepfad angegeben wird, wird automatisch eine neue Datei mit dem Präfix `new-` erstellt (z.B. new-image.ppm).
