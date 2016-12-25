#ifndef _TIM_H_
#define _TIM_H_

#include "libps.h"

static void DumpCell(GsCELL *cell) {
	FntPrint("CELL u: %d, v: %d, tpage: %d\n", cell->u, cell->v, cell->tpage);
}

static void DumpTim(u_long *ptr) {
	GsIMAGE tim;

	// Wait for all non-blocking drawing to terminate
	//while (DrawSync(1)) ;
	
	// Note: TIM header (10 00 00 00) needs to be skipped!
	GsGetTimInfo(ptr + 1, &tim);

	FntPrint("IMAGE:     X: %d, Y: %d, W: %d, H: %d\n", tim.px, tim.py, tim.pw, tim.ph);

	switch	(tim.pmode & 3) {
		case 0: FntPrint("4bit CLUT: "); break;
		case 1: FntPrint("8bit CLUT: "); break;
		case 2: FntPrint("16bit DIRECT"); break;
		case 3: FntPrint("24bit DIRECT"); break;
		default: FntPrint("pmode: %d\n", tim.pmode); break;
	}
	
	if ((tim.pmode & 3) < 2)
		FntPrint("X: %d, Y: %d, W: %d, H: %d\n", tim.cx, tim.cy, tim.cw, tim.ch);
	
}

// Copies a TIM and it's CLUT to frame buffer
// Initializes the privided GsCELL instance with image data contained
// in the .TIM time where *ptr points to. I'm assuming a header here.
static void LoadCellFromTim(u_long *ptr, GsCELL *cell, u_char invert_h, u_char invert_v) {
	GsIMAGE tim;
	int tx, ty;
	RECT rect;
	unsigned short flags = 0;

	// Wait for all non-blocking drawing to terminate. Should never happen
	// during initialization, but you never know who is calling this when.
	while (DrawSync(1)) ;
	
	// Note: TIM header (10 00 00 00) needs to be skipped!
	GsGetTimInfo(ptr + 1, &tim);

	// Copy over image data
	rect.x = tim.px;
	rect.y = tim.py;
	rect.w = tim.pw;
	rect.h = tim.ph;

	LoadImage(&rect, tim.pixel);

	// Set up mirroring flags
	cell->flag = 0;

	if (invert_v)
		cell->flag += 1;

	if (invert_h)
		cell->flag += 2;	

	tx = (tim.px / 64) * 64;
	ty = (tim.py / 256) * 256;

	cell->tpage = GetTPage(
		// Texture mode: 0 = 4bit CLUT, 1 = 8bit CLUT, 2 = 16bit direct color
		tim.pmode & 3,
		
		// abr: 0 = 0.5 back + 0.5 front, 1 = back + front, 2 = back - front, 3 = back + 0.25 front
		0,

		// Texture page address
		tx, ty
	);

	// u,v = Offset within texture page
	cell->u = tim.px - tx;
	cell->v = tim.py - ty;

	// cba
	//  bit 0-5: CLUT x / 16
	//  bit 6-16: CLUT y
	cell->cba = (tim.cx >> 4) + (tim.cy << 6);

	DrawSync(0);
}


// Copies a TIM and it's CLUT to frame buffer
// Initializes the privided GsSPRITE instance with image data contained
// in the .TIM time where *ptr points to. I'm assuming a header here.
static void LoadTim(u_long *ptr, GsSPRITE* sprite)
{
	GsIMAGE tim;
	RECT rect;

	// Wait for all non-blocking drawing to terminate
	//while (DrawSync(1)) ;
	
	// Note: TIM header (10 00 00 00) needs to be skipped!
	GsGetTimInfo(ptr + 1, &tim);

	// Copy over image data
	rect.x = tim.px;
	rect.y = tim.py;
	rect.w = tim.pw;
	rect.h = tim.ph;

	LoadImage(&rect, tim.pixel);

	// Copy CLUT if needed
	
	// TIM pmode: 0: 4bit w/ CLUT, 1: 8bit w/ CLUT, 2: 16bit direct, 3: 24bit direct
	if ((tim.pmode & 3) < 2)
	{
		rect.x = tim.cx;
		rect.y = tim.cy;
		rect.w = tim.cw;
		rect.h = tim.ch;

		LoadImage(&rect, tim.clut);
	}
	
	// set sprite properties if desired
	if (sprite != NULL) {
		// Texture page
		sprite->tpage = GetTPage(tim.pmode & 3, 0, tim.px, tim.py);
		
		// Set reasonable defaults here
		// Bit 6: If set, brightness adjustment is disabled
		// Bit 27: If set, rotation/scaling is disabled
		sprite->attribute = (1<<6) + (1<<27);
		
		// Bits 24/25 - 0: 4bit CLUT, 1: 8bit CLUT, 2: 15-bit direct
		sprite->attribute = sprite->attribute + ((tim.pmode & 3) << 24);
		
		// 128 => original color
		sprite->r = 128;
		sprite->g = 128;
		sprite->b = 128;
		
		// Default scale
		sprite->scalex = ONE;
		sprite->scaley = ONE;
		
		// Dimensions
		switch (tim.pmode & 3) {
			case 0: sprite->w = tim.pw * 4; break;
			case 1: sprite->w = tim.pw * 2; break;
			case 2: sprite->w = tim.pw; break;
		}
		
		sprite->h = tim.ph;
		
		// Pivot
		sprite->mx = sprite->w / 2;
		sprite->my = sprite->h / 2;
		
		// No rotation (1deg => 4096)
		sprite->rotate = 0;
		
		// In-page offset
		sprite->u = tim.px % 64;
		sprite->v = tim.py % 256;
		
		// Clut address
		sprite->cx = tim.cx;
		sprite->cy = tim.cy;
	}
	
	DrawSync(0);
}
#endif