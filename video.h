// Contains rendering stuff and helpers for working with ordering tables
#ifndef _VIDEO_H_
#define _VIDEO_H_

#define OT_LENGTH  13  
#define PACKETMAX 10000
#define OT_FURTHEST ((1<<OT_LENGTH)-1)

int ViewportWidth;
int ViewportHeight;

GsOT *ot;

GsOT WorldOT[2];
GsOT_TAG OTTags[2][1<<OT_LENGTH]; 
PACKET GpuPacketArea[2][PACKETMAX*24];

static void InitVideo() {
	// Detect video mode. SCEE => Europe, and PAL. NTSC otherwise.
	if (*(char *)0xbfc7ff52=='E') {
		// PAL
		SetVideoMode(MODE_PAL);
		ViewportWidth = 320;
		ViewportHeight = 256;
		GsDISPENV.screen.y = 0;
		GsDISPENV.screen.h = 256;
	} else {
		// NTSC
		SetVideoMode(MODE_NTSC);
		ViewportWidth = 320;
		ViewportHeight = 240;
	}
	
	ResetGraph(0) ;
	
	// Non-Interlaced, GPU offsets
	GsInitGraph(ViewportWidth, ViewportHeight, GsNONINTER|GsOFSGPU, 0, 0);
	GsDefDispBuff(0, 0, 0, ViewportHeight);
	SetDispMask(1);
}


static void InitOT() {
	int i;

	for (i=0; i<2; i++) {
		WorldOT[i].length = OT_LENGTH;
		WorldOT[i].org = OTTags[i];
		GsClearOt(0, 0, &WorldOT[i]);
	}
}
#endif