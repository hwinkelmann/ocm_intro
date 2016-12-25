// Headers
#include "libps.h"
#include "video.h"
#include "tim.h"

#include "back.h"
//#include "pad.h"

#include "assets/bigbox.h"

// Assets

int frame = 0;

int printStream;

GsDOBJ2 boxInstance;
GsCOORDINATE2 csCamera, csBox;


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

    // Initialize font renderer
    FntLoad(960, 256);
	printStream = FntOpen(-160, -128, 320, 240, 0, 512);


    // Map TMD to memory location
    GsMapModelingData((u_long*)tmd_bigbox + 1);

    GsInitCoordinate2(WORLD, &csBox);
    GsInitCoordinate2(WORLD, &csCamera);
    csBox.coord.t[0] = 0;
    csBox.coord.t[1] = 0;
    csBox.coord.t[2] = 1000;
    csCamera.coord.t[0] = 0;
    csCamera.coord.t[1] = 0;
    csCamera.coord.t[2] = 0;
    csCamera.flg = 0;

    // Set up a box instance and link it to a coordinate system
    GsLinkObject4((u_long)tmd_bigbox + 12, &boxInstance, 0);
    boxInstance.coord2 = &csBox;
    boxInstance.attribute = 0;

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
        //RenderBackground();
        Background.Render();

        csBox.coord.t[2] = (csBox.coord.t[2] + 1) % 1200;
        csBox.flg = 0;
        GsSetRefView2(&camera);
        GsSetProjection(900);

        GsSetAmbient(ONE, ONE, ONE);
        GsGetLs(&csBox, &localScreenMatrix);
        GsSetLsMatrix(&localScreenMatrix);
        GsSortObject4(&boxInstance, ot, OT_LENGTH, getScratchAddr(0));

        FntPrint("box distance: %d\n", (int) csBox.coord.t[2]);

//        DumpTim(printStream, (u_long*)img_backgrade);
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