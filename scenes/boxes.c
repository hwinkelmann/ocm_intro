#ifndef _SCENES_BOXES_H_
#define _SCENES_BOXES_H_

#include "../libps.h"
#include "boxes.h"
#include "../assets/bigbox.h"
#include "../video.h"

#define LOGO_WIDTH 24
#define LOGO_HEIGHT 7
#define BOX_UNITS 40
#define NUM_BOXES 100 // 83 actually... Reserve space for this many instances

char placement[LOGO_HEIGHT][LOGO_WIDTH] = {
    ".OOOO....OOOO...OO...OO",
    "OO..OO..OO..OO..OOO.OOO",
    "OO..OO..OO......OOOOOOO",
    "OO..OO..OO......OO.O.OO",
    "OO..OO..OO......OO...OO",
    "OO..OO..OO..OO..OO...OO",
    ".OOOO....OOOO...OO...OO"
};

GsDOBJ2 box_objects[NUM_BOXES];
GsCOORDINATE2 box_systems[NUM_BOXES];
int numBoxes;

// Creates box instances which form our logo at the end of the day.
// Returns the number of instances used to do so.
void InitializeBoxes() {
    int i, j;
    GsCOORDINATE2 *cs;
    GsDOBJ2 *obj;
    int xc, yc;

    // Map TMD to memory location
    GsMapModelingData((u_long*)tmd_bigbox + 1);

    for (i=0; i<LOGO_HEIGHT; i++)
    for (j=0; j<LOGO_WIDTH; j++) {
        if (placement[i][j] == 'O') {
            // Get a pointer to the instances we're working with
            cs = &box_systems[numBoxes];
            obj = &box_objects[numBoxes];

            // Calculate box location
            xc = -(BOX_UNITS * LOGO_WIDTH) / 2 + j * BOX_UNITS;
            yc = -(BOX_UNITS * LOGO_HEIGHT) / 2 + i * BOX_UNITS;
                        
            // Initialize coordinate system
            GsInitCoordinate2(WORLD, cs);
            cs->coord.t[0] = xc;
            cs->coord.t[1] = yc;
            cs->coord.t[2] = 1400;
            cs->flg = 0;

            // Link box geometry with model instance
            GsLinkObject4((u_long)tmd_bigbox + 12, obj, 0);
            obj->coord2 = cs;
            obj->attribute = 0;

            // Increment box count and continue
            numBoxes++;
        }
    }
}

void RenderBoxes() {
    int i;
    MATRIX localScreenMatrix;

    FntPrint("%d BOXES\n", numBoxes);

    for (i=0; i<numBoxes; i++) {
        GsGetLs(&box_systems[i], &localScreenMatrix);
        GsSetLsMatrix(&localScreenMatrix);  
        GsSortObject4(&box_objects[i], ot, OT_LENGTH, getScratchAddr(0));
    }
}

const struct boxes Boxes = {
    .Initialize = InitializeBoxes,
    .Render = RenderBoxes
};

#endif