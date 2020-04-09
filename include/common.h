#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>



C2D_TextBuf g_staticBuf, g_dynamicBuf;
C2D_Text g_staticText[3];


#define WHITE                 C2D_Color32(255, 255, 255, 255)
#define BLACK                 C2D_Color32(0, 0, 0, 255)
#define RED                   C2D_Color32(255, 0, 0, 255)
#define BLUE                  C2D_Color32(0, 0, 255, 255)
#define GREEN                 C2D_Color32(0, 255, 0, 255)
#define GRAY                  C2D_Color32(128, 128, 128, 255)
#define YELLOW                C2D_Color32(255, 255, 0, 255)
