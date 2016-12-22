#include "assets/backgrad.h"

GsSPRITE background;

void InitBackground() {
    LoadTim((u_long*)img_backgrade, &background);    
}

void RenderBackground() {
    int i, j;
    for (i = -ViewportWidth / 2; i < ViewportWidth / 2; i += background.w)
    for (j = -ViewportHeight / 2; j < ViewportHeight / 2; j += background.h) {
        background.x = i;
        background.y = j;
        //GsSortFastSprite(&background, ot, OT_FURTHEST);
        GsSortFastSprite(&background, ot, OT_FURTHEST);
    }
}