#ifdef EXT_PROMPT

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colours.h"

// BONUS! Adds a splash of colour to the prompt

// List of colours
char *colours[] = {
    "red",
    "green",
    "yellow",
    "blue",
    "magenta",
    "cyan",
    "reset",
    NULL
};

// List of CORRESPONDING colour codes
char *colourCodes[] = {
    "\x1b[31m", // Red
    "\x1b[32m", // Green
    "\x1b[33m", // Yellow
    "\x1b[34m", // Blue
    "\x1b[35m", // Magenta
    "\x1b[36m", // Cyan
    COLOUR_RESET, // Reset
    NULL
};

// Constructs a mapping from colour names to colour codes
ColourMap getColourMap() {
    printf(DEFAULT_COLOUR); // Set default colour
    int count = 0;
    ColourMap this;
    while (colourCodes[count] != NULL) {
        count++;
    }
    this.map = malloc(count * sizeof(ColourPair));
    for (int i = 0; i < count; i++) {
        this.map[i].colour = colours[i];
        this.map[i].code = colourCodes[i];
    }
    return this;
}

// Gets the colour code for a given colour
char *getColourCode(ColourMap this, char *colour) {
    ColourPair *map = this.map;
    for (int i = 0; map[i].colour != NULL; i++) {
        if (strcmp(map[i].colour, colour) == 0) {
            return map[i].code;
        }
    }
    return NULL;
}

// Frees the memory of a ColourMap
void freeColourMap(ColourMap this) {
    printf("\x1b[0m"); // Reset colour
    free(this.map);
}

#endif