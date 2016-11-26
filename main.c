// Headers
#include "libps.h"
#include "video.h"
#include "tim.h"
//#include "pad.h"

// Assets

int frame = 0;

int main() {
    // Set video mode    
    InitVideo();

    // Initialize ordering tables
    InitOT();

    // Initialize 3D pipeline
    GsInit3D(); 

    // render loop
    while (1) {
        // Read active buffer and switch to the corresponding OT
        int activeBuff = GsGetActiveBuff();
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
		GsClearOt(0, 0, &WorldOT[activeBuff]);

        // Do rendering...

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