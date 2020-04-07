#include "textures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>
#include <unistd.h>

#include "blob.h"

#include "platforms.h"

#include "background.h"

static C2D_SpriteSheet blob;
static C2D_SpriteSheet platforms;

static C2D_SpriteSheet background;

C2D_Image temp;

void texload(){
	blob = C2D_SpriteSheetLoad("romfs:/gfx/blob.t3x");
	platforms = C2D_SpriteSheetLoad("romfs:/gfx/platforms.t3x");
	background = C2D_SpriteSheetLoad("romfs:/gfx/background.t3x");

	neutral = C2D_SpriteSheetGetImage(blob, blob_neutral_idx);
	iceplatform = C2D_SpriteSheetGetImage(platforms, platforms_GroundWinter_idx);
	placeholder = C2D_SpriteSheetGetImage(platforms, platforms_PlaceHolder_idx);
	smwback = C2D_SpriteSheetGetImage(background, background_smwBack_idx);
	yeet = C2D_SpriteSheetGetImage(background, background_yeet_idx);
}

void Textures_Free() {
	C2D_SpriteSheetFree(blob);
	C2D_SpriteSheetFree(platforms);
	C2D_SpriteSheetFree(background);
}

C2D_Image getplatform(int boxtype) {
	C2D_Image temp = placeholder;

	if (boxtype == 0) temp = stoneplatform;
	else if (boxtype == 1) temp = iceplatform;
	else if (boxtype == 2) temp = woodplatform;
	else if (boxtype == 3) temp = metalplatform;
	else temp = placeholder;


	return temp;
}

void initplatforms() {
	stone.width = 50;
	stone.height = 25;
	ice.width = 50;
	ice.height = 25;
}

void getplatformsize(int boxtype, int size, int dimensions[]) {

	initplatforms();

	if (boxtype == 0) {
		dimensions[0] = stone.width;
		dimensions[1] = stone.height;
	}
	else if (boxtype == 1) {
	/*	*width = ice.width;
    	*height = ice.height;*/
		dimensions[0] = ice.width;
		dimensions[1] = ice.height;
	}
	else if (boxtype == 2) temp = woodplatform;
	else if (boxtype == 3) temp = metalplatform;
	else temp = placeholder;

}