#include <3ds.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <citro2d.h>
#include <citro3d.h>
#include <math.h>

#include "common.h"
#include "textures.h"
#include "graphics.h"

#define TOP_SCREEN_WIDTH  400
#define BOTTOM_SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240


// Top Screen Size: 1
// minX = -3
// minY = -16
// maxX = 372
// maxY = 208


// Top Screen Size: 2
// minX = -6
// minY = -32
// maxX = 344
// maxY = 176

// Top Screen Size: 3
// minX = -9
// minY = -48
// maxX = 316
// maxY = 144

bool grounded = false, dead = false, pausegame = false, debug = true;
float posY, posX, Vmomajust = 0, Vmomentum = 0, ajustpY = 0, ajustoY = 0, Hmomentum = 0, ajustpX = 0, ajustoX = 0, offsetX = 0, offsetY = 0;
float acc[4] = {1, 0.6f, 0.4f, 0.3f};
float dec[4] = {2, 2.8f, 2.4f, 3.75f};

int temphorz, tempvert, aapress = 0;

bool LRpress = false;

int a = 0;
int i = 0;
bool init = false;

void Ajustoffset(char* vartoajust, float offsetofvar){ // Scrolling
	if (strcmp(vartoajust, "X") == 0) offsetX = offsetX + offsetofvar;
	if (strcmp(vartoajust, "Y") == 0) offsetY = offsetY + offsetofvar;
	if (strcmp(vartoajust, "pY") == 0) posY = posY + offsetofvar;
	if (strcmp(vartoajust, "pX") == 0) posX = posX + offsetofvar;
}

int keysmove(int select){
	circlePosition pos;
	hidCircleRead(&pos);
	int directX = 0, directY = 0, result = 0;
    //hidScanInput();
	u32 kDown = hidKeysDown();
	u32 kHeld = hidKeysHeld();
	if (select == 1){

        if (pos.dx > 80 || kHeld & KEY_DRIGHT) directX = 1;
        else if (pos.dx < -80 || kHeld & KEY_DLEFT) directX = -1;

        if (pos.dy > 80 || kHeld & KEY_DUP) directY = 1;
        else if (pos.dy < -80 || kHeld & KEY_DDOWN) directY = -1;

        if (directX == -1 && directY == 0) result = 7; //left
        else if (directX == 0 && directY == -1) result = 5; //down
        else if (directX == 1 && directY == 0) result = 3; //right
        else if (directX == 0 && directY == 1) result = 1; //up
        else if (directX == -1 && directY == -1) result = 6; //downleft
        else if (directX == -1 && directY == 1) result = 8; //upleft
        else if (directX == 1 && directY == -1) result = 4; //downright
        else if (directX == 1 && directY == 1) result = 2; //upright
    }

	if (select == 2){
		if (kHeld && KEY_A) result = 1;
	}
	return result;
}

float calcHmomentum(float posX, float offsetX) { // Horizontal speed
    LRpress = false;
    if (keysmove(1) == 2 || keysmove(1) == 3 || keysmove(1) == 4 || keysmove(1) == 6 || keysmove(1) == 7 || keysmove(1) == 8) LRpress = true;

    if (LRpress == true) {
        if (grounded == true) {
            if (keysmove(1) == 7) Hmomentum = Hmomentum - acc[1];
            else if (keysmove(1) == 3) Hmomentum = Hmomentum + acc[1];
        }
        else if (grounded == false) {
            if (keysmove(1) == 7) Hmomentum = Hmomentum - acc[2];
            else if (keysmove(1) == 3) Hmomentum = Hmomentum + acc[2];
        }
    }

    else if (LRpress == false && Hmomentum != 0) {
        if (Hmomentum > 0.7f) Hmomentum = Hmomentum - 0.2f;
    	if (Hmomentum < -0.7f) Hmomentum = Hmomentum + 0.2f;
    	if (Hmomentum > -0.7f && Hmomentum < 0.7f) Hmomentum = 0;
    }

    if (Hmomentum > 5) Hmomentum = 5;
    else if (Hmomentum < -5) Hmomentum = -5;

    Ajustoffset("X", ajustoX);
    Ajustoffset("pX", ajustpX);
    ajustpX = 0, ajustoX = 0;

    float temphmom = 0;
    temphmom = Hmomentum;
    return temphmom;
    
}

float calcVmomentum(float locY, float offsetY, int apress, float boost){
    //printf("%f\n", offsetY);
	if (Vmomajust != 0) Vmomentum = Vmomentum + Vmomajust;

 	if (locY < 50 && Vmomentum < 0) { 
        ajustpY = ajustpY - Vmomentum;
        ajustoY = ajustoY + Vmomentum;
    }
    else if (locY < 50 && Vmomentum == 0) {
        ajustpY = ajustpY + 1;
        ajustoY = ajustoY - 1;
    }
    if (locY > 160 && offsetY < 0 && Vmomentum > 0) {
        ajustpY = ajustpY - Vmomentum;
        ajustoY = ajustoY + Vmomentum;
    }
	else if (locY > 160 && offsetY < 0 && Vmomentum == 0) {
        ajustpY = ajustpY - 1;
        ajustoY = ajustoY + 1; }

	int season = 4;
	
    if (grounded == true && apress == 1 && season == 4) Vmomentum = Vmomentum - 3.0f;
    else if (grounded == true && apress == 1 && season == 1) Vmomentum = Vmomentum - 5.0f;
    else if (grounded == true && apress == 1) Vmomentum = Vmomentum - 4.0f;
    else if ((keysmove(2) == 1) && season == 3 && Vmomentum > 0)  Vmomentum = Vmomentum - 0.12f;

    if (offsetY > 0) ajustoY = ajustoY - offsetY;

 	if (grounded == true && Vmomentum > 0) Vmomentum = 0;
 	if (grounded == false) Vmomentum = Vmomentum + 0.15f;
 	grounded = false;

    Ajustoffset("Y", ajustoY);
    Ajustoffset("pY", ajustpY);
    ajustpY = 0, ajustoY = 0, Vmomajust = 0;

    float tempvmom = 0;
    tempvmom = Vmomentum - boost;
    return tempvmom;
}

typedef struct player { // Blob characteristics
    int width;
    int height;
}player;

player blob;




int platcoll(int locX, int locY, float posX, float posY, int pWidth, int pHeight) { // loc = platform, pos = player, pWidth/pHeight = dimensions
    printf("locX:%i  locY:%i\nposX:%f   posY:%f\nHmomentum:%f   Vmomentum:%f\n\nLRpress: %s\n", locX, locY, posX, posY, Hmomentum, Vmomentum, LRpress ? "true" : "false");
    //printf("%i\n", locX + pWidth);
    C2D_DrawRectSolid(locX, locY, 0.5f, pWidth, 1, GREEN); // Top line
    C2D_DrawRectSolid(locX, locY + pHeight, 0.5f, pWidth, 1, GREEN); // Bottom line
    C2D_DrawRectSolid(locX, locY, 0.5f, 1, pHeight, GREEN); // Left line
    C2D_DrawRectSolid(locX + pWidth, locY, 0.5f, 1, pHeight, GREEN); // Right line

    //if (posX < locX + pWidth) printf()

    if ((posX < locX + pWidth && posX > locX) || (posX + blob.width > locX && posX < locX + pWidth)) { // Is the player between the left and right hitboxes of a platform // // if blob.left is past   // posX < locX + topright && posX + blob.width > locX
        if (posY + blob.height >= locY + 2 && posY <= locY + pHeight) {
        //    posY = locY;
            
            // Check if player touched left side or right side
            if (posX + blob.width > locX && posX + blob.width < locX + pWidth / 2) { // Left
                Hmomentum = -Hmomentum;
                printf("Player is touching the left side\n");
            }
            else if (posX < locX + pWidth && posX > locX + pWidth / 2) { // Right
                Hmomentum = -Hmomentum;
                printf("Player is touching the right side\n");
            }
            //posX = locX;
        }

        if (posY + blob.height >= locY) { // Did the player go through the platform

            if (posY + blob.height <= locY + pHeight / 2) { // Is the player on top? aka "higher" than the top
                posY = locY - 5;
                grounded = true;
                printf("Player is touching the top\n");
                Vmomentum = 0;
            }

            else if (posY <= locY + pHeight && posY >= locY + pHeight -2) { // Is the player under? aka approx height of platform - blob height which equal ~10-15
                posY = locY + 32;
                printf("Player is touching the bottom\n");
                Vmomentum = 0.15f;
            }
        }
    }
    
    // Use posY to check the top of blob
    // Use posY + blob.height to check the bottom of blob
    // Use posX to check the left of blob
    // Use posX + blob.width to check the right of blob


    return 0;
}
////////////////////////////////////

// Ah shit, here we go again!

int main(int argc, char **argv)
{

    gfxInitDefault();
	gfxSetDoubleBuffering(GFX_TOP, false);
	gfxSetDoubleBuffering(GFX_BOTTOM, false);
	romfsInit();



    // Citro stuff
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
    texload();

    int LocX[6], LocY[6], dimensions[2];

    blob.width = 25;
    blob.height = 16;

    a = 0;
    posY = 80, posX = 80;
    int C = 0;

    int charge = 0;
    int boost = 0;

    bool showPlatforms = true;

    while (aptMainLoop())
    {

        hidScanInput();
        consoleInit(GFX_BOTTOM, NULL);
        u32 kDown = hidKeysDown();
        u32 kHeld = hidKeysHeld();
        u32 kUp = hidKeysUp();

        if (init == false) {
            for (i = 0; i < 5; i++) {
                srandom(svcGetSystemTick() * osGetTime()); 
                //LocX[i] = (400 - random() % 400+1);
                //LocY[i] = (240 - random() % 40+1);
                LocX[i] = 80 * i;
                LocY[i] = 200 - 40 * i;
            }
            init = true;
            i = 0;
        }

        LocX[5] = 0;
        LocY[5] = 130;


        if (kDown & KEY_START){
			break;
		}

        if (kDown & KEY_X){
			if (showPlatforms == true) showPlatforms = false;
            else if (showPlatforms == false) showPlatforms = true;
		}

        //getplatformsize(1, 1, &width, &height);
        getplatformsize(1, 1, dimensions);
        //printf("%i\n%i\n", dimensions[0], dimensions[1]);
        
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_SceneBegin(top);
        C2D_TargetClear(top, BLACK);

        if (showPlatforms == true) {
            for (i = 0; i < 6; i++) {
                C2D_DrawImageAt(getplatform(1), LocX[i], LocY[i] - offsetY, 0.5f, NULL, 1, 1);
            }
        }



        //C2D_DrawRectangle(0, 0, 0.5f, 400, 240, RED, YELLOW, GREEN, BLUE);


        /*i = 0;
        while (i < 5) {
            platcoll(LocX[i], LocY[i], posX, posY, dimensions[0], dimensions[1]);
            i++;
        }*/

        platcoll(LocX[C], LocY[C]- offsetY, posX, posY, dimensions[0], dimensions[1]);

//        C2D_DrawRectangle(LocX[C], LocY[C], 0.5f, dimensions[0], dimensions[1], RED, GREEN, BLUE, YELLOW);
        
        if (kHeld & KEY_A)
        {
            aapress = 1;
            //printf("Oi cunt\n");
        }

        if (kDown & KEY_L) C = C - 1;
        if (kDown & KEY_R) C = C + 1;

        if (C > 5) C = 0;
        if (C < 0) C = 5;


        if (kHeld & KEY_B && charge < blob.width) charge++;
        else if (charge != 0) charge--;


        if (kUp & KEY_B && charge > 0 && grounded == true) {
            boost--;
            charge--;
        }

        posX = posX + calcHmomentum(posX, offsetX);

        posY = posY + calcVmomentum(posY, offsetY, aapress, boost);    

        aapress = 0;

        

        tempvert = offsetY;
        temphorz = offsetX;

        //printf("X:%f\nY:%f\n", posX, posY);

        C2D_DrawRectSolid(posX, posY - 5, 0.0f, charge, blob.height / 5, C2D_Color32(255, 0, 0, 255));
        C2D_DrawImageAt(neutral, posX, posY, 0.5f, NULL, 1, 1);

        C2D_DrawRectSolid(posX, posY, 0.5f, blob.width, 1, RED); // Top line
        C2D_DrawRectSolid(posX, posY + blob.height, 0.5f, blob.width, 1, RED); // Bottom line
        C2D_DrawRectSolid(posX, posY, 0.5f, 1, blob.height, RED); // Left line
        C2D_DrawRectSolid(posX + blob.width, posY, 0.5f, 1, blob.height, RED); // Right line


        C3D_FrameEnd(0);

        gfxFlushBuffers();
		gfxSwapBuffers();
		gspWaitForVBlank();
	}
    Textures_Free();
	C2D_Fini();
	C3D_Fini();
	romfsExit();
	gfxExit();
	return 0;
}