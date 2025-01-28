#ifdef EXT_PROMPT

#ifndef COLOURS_H
#define COLOURS_H

// We decided to have cyan as the default colour
#define DEFAULT_COLOUR "\x1b[36m" // Cyan
#define COLOUR_RESET "\x1b[0m" // Reset

// A mapping from a colour name to a colour code
typedef struct {
    char *colour;
    char *code;
} ColourPair;

// A list of mappings from colour names to colour codes
typedef struct {
    ColourPair *map;
} ColourMap;

ColourMap getColourMap();
char *getColourCode(ColourMap this, char *colour);
void freeColourMap(ColourMap this);

#endif

#endif