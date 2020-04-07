#include <stddef.h>
#include <citro2d.h>

C2D_Image neutral, stoneplatform, iceplatform, woodplatform, metalplatform, placeholder, smwback, yeet;

typedef struct Platforms {
    int width;
    int height;
}Platforms;

Platforms stone;
Platforms ice;
Platforms wood;
Platforms metal;

int dimensions[2];

void texload();
void Textures_Free();

void initplatforms();

//void getplatformsize(int boxtype, int size, int* width, int* height);

void getplatformsize(int boxtype, int size, int dimensions[]);

C2D_Image getplatform(int boxtype);