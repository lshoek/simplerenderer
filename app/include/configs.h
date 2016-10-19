#include "tgaimage.h"

#define APPNAME "simplerenderer"
#define CREATOR "lshoek (Lesley van Hoek)"

#define OUTPUT_PATH "../data/output/output.tga"

#define TEAPOT_OBJ_PATH "../data/obj/teapot.obj"
#define HEAD_OBJ_PATH "../data/obj/head.obj"

#define IMG_WIDTH 640
#define IMG_HEIGHT 640

const TGAColor TGA_WHITE = TGAColor(255, 255, 255, 255);
const TGAColor TGA_RED = TGAColor(255, 0, 0, 255);
const TGAColor TGA_GREEN = TGAColor(0, 255, 0, 255);
const TGAColor TGA_BLUE = TGAColor(0, 0, 255, 255);