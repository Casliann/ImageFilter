# English 

## Compilation

To compile the project, you can use the provided Makefile. Run the following command:

```bash
make
```

### Running the Program

After compiling, you can start the program with:

```bash
./imagefilter
```
Options must be provided to run the program. The available options will be explained below.

### Help

To get detailed usage instructions, run the following command:

```bash
./imagefilter help
```

This will display the correct syntax and available options.

## Options

The program supports the following options for customizing image filtering:

- `if=<filename>` : Input image (e.g., `if=image.ppm`)
- `of=<filename>` : Output image
- `ff=<filename>` : Optional filter image for overlay effects
- `color=<color>` : Optional, color of the filter (e.g., `red`, `green`, `blue`)
- `filter=<option>` : Choose a filter:
  - `overlay`: Overlay the filter image onto the input image
  - `emboss`: Emboss effect
  - `blur-median`: Median blur
  - `blur-light`: Light blur
  - `blur-medium`: Medium blur
  - `blackframe`: Black frame
  - `whiteframe`: White frame
  - `snowflakes`: Snowflakes
  - `hearts`: Hearts
  - `stars`: Stars
- `help`: Displays a help message

## Example Usage

```bash
./imagefilter if=input.ppm of=newfile.ppm filter=hearts
```

This command applies the Hearts filter to the input image `input.ppm` and saves the result as `newfile.ppm`.



```bash
./imagefilter if=input.ppm ff=input2.ppm filter=overlay color=blue
```

This command applies the overlay filter to the image `input.ppm` by overlaying `input2.ppm` with a blue transparency color and saves the result. The result will automatically be saved in the file `new-input.ppm`.

## Features

- Supports PPM files in ASCII and binary formats.
- The output is saved in a new file.
- Filter templates for Stars, Snowflakes, Hearts, and Frames, as well as input images, are provided in the /assets directory.
- Allows overlaying images with custom transparency colors.
- The second image is scaled to match the first images size when overlaying.
- Extended blur filters: blur-light and blur-medium.

---

# Deutsch

## Kompilierung

Zur Kompilierung des Projekts kann das mitgelieferte Makefile verwendet werden. Dazu muss der folgende Befehl ausgeführt werden:

```bash
make
```

### Ausführung des Programms

Nach der Kompilierung kann das Programm mit folgendem Befehl gestartet werden:

```bash
./imagefilter
```
Es müssen Optionen angegeben werden, um das Programm zu verwenden. Die verfügbaren Optionen werden im Folgenden erklärt.

### Hilfe

Um detaillierte Anweisungen zur Verwendung zu erhalten, führe den folgenden Befehl aus:

```bash
./imagefilter help
```

Dieser Befehl zeigt die richtige Syntax und die verfügbaren Optionen an.

## Optionen

Das Programm unterstützt die folgenden Optionen zur Anpassung der Bildfilterung:

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

Dieser Befehl wendet den Hearts-Filter auf das Eingabebild `input.ppm` an und speichert das Ergebnis unter dem Namen `newfile.ppm`.



```bash
./imagefilter if=input.ppm ff=input2.ppm filter=overlay color=blue
```

Dieser Befehl wendet den Overlay-Filter auf das Bild `input.ppm` an, indem es das Overlay-Bild `input2.ppm` mit einer blauen Transparenzfarbe überlagert und das Ergebnis speichert. Das Ergebnis wird automatisch im File `new-input.ppm` gespeichert.

## Features

- Unterstützung für PPM-Dateien im ASCII- und Binärformat.
- Das Ergebnis wird in einer neuen Datei gespeichert.
- In der /assets-Verzeichnis befinden sich Filtervorlagen für Stars, Snowflakes, Hearts und Frames sowie Eingabebilder.
- Unterstützung für die Überlagerung von Bildern mit benutzerdefinierter Transparenzfarbe.
- Das zweite Bild wird dem ersten skaliert.
- Erweiterte Blur-Filter: blur-light und blur-medium.
