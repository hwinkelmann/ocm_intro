// Headers
#include "libps.h"
#include "video.h"
#include "tim.h"

#include "back.h"
#include "scenes/boxes.h"
//#include "pad.h"

// Assets

int frame = 0;

int printStream;

GsCOORDINATE2 csCamera;


int main() {
    int activeBuff;
    GsRVIEW2 camera;
    MATRIX localScreenMatrix;

    // Set video mode    
    InitVideo();

    Background.Initialize();

    // Initialize ordering tables
    InitOT();

    // Initialize 3D pipeline
    GsInit3D(); 

    Boxes.Initialize();

    // Initialize font renderer
    FntLoad(960, 256);
	printStream = FntOpen(-160, -128, 320, 240, 0, 512);

    GsInitCoordinate2(WORLD, &csCamera);
    csCamera.coord.t[0] = 0;
    csCamera.coord.t[1] = 0;
    csCamera.coord.t[2] = 0;
    csCamera.flg = 0;

    camera.vpx = 0;
    camera.vpy = 0;
    camera.vpz = 0;
    camera.vrx = 0;
    camera.vry = 0;
    camera.vrz = 1;
    camera.rz = 0;
    camera.super = &csCamera;

    // render loop
    while (1) {
        // Read active buffer and switch to the corresponding OT
        activeBuff = GsGetActiveBuff();
        ot = &WorldOT[activeBuff];
        GsSetWorkBase((PACKET*)GpuPacketArea[activeBuff]);
		GsClearOt(0, 0, ot);

        // Do rendering...
        Background.Render();


        camera.vpz = -5 * (frame%1000);
        GsSetRefView2(&camera);
        GsSetProjection(900);

        GsSetAmbient(ONE, ONE, ONE);

        Boxes.Render();

//        DumpTim(printStream, (u_long*)img_backgrade);

        FntPrint("FRAME %d", frame);

        FntFlush(printStream);

        // Wait for rendering to complete
        DrawSync(0);
        
        // Wait for vertical retrace
        VSync(0);

        // Swap visible buffer of our rendering chain
        GsSwapDispBuff();

        // Add instruction to clear background
        GsSortClear(frame % 255, frame % 255, frame % 255, ot);
        frame++;

        // Start rendering of next frame
        GsDrawOt(ot);
    }

    // Reset graphics and bail out
    ResetGraph(0);
    return(0);
}