// Headers
#include "libps.h"
#include "video.h"
#include "tim.h"
#include "backgrnd.h"
//#include "pad.h"

// Assets

int frame = 0;

int printStream;

int main() {
    int activeBuff;

    // Set video mode    
    InitVideo();

    InitBackground();

    // Initialize ordering tables
    InitOT();

    // Initialize 3D pipeline
    GsInit3D(); 

    FntLoad(960, 256);
	printStream = FntOpen(-160, -128, 320, 240, 0, 512);

    // render loop
    while (1) {
        // Read active buffer and switch to the corresponding OT
        activeBuff = GsGetActiveBuff();
        ot = &WorldOT[activeBuff];
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
		GsClearOt(0, 0, ot);

        // Do rendering...
        RenderBackground();

        DumpTim(printStream, (u_long*)img_backgrade);
        FntFlush(printStream);

        // Wait for rendering to complete
        DrawSync(0);
        
        // Wait for vertical retrace
        VSync(0);

        // Swap visible buffer of our rendering chain
        GsSwapDispBuff();

        // Add instruction to clear background
        GsSortClear(frame % 255, frame % 255, frame % 255, &WorldOT[activeBuff]);
        frame++;

        // Start rendering of next frame
        GsDrawOt(&WorldOT[activeBuff]);
    }

    // Reset graphics and bail out
    ResetGraph(0);
    return(0);
}