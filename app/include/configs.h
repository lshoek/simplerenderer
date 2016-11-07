#include "tgaimage.h"

#define APPNAME "simplerenderer"
#define CREATOR "lshoek (Lesley van Hoek)"

#define OUTPUT_PATH "../data/output/output.tga"

#define TEAPOT0_OBJ_PATH "../data/obj/teapot0.obj"
#define TEAPOT1_OBJ_PATH "../data/obj/teapot1.obj"
#define HEAD_OBJ_PATH "../data/obj/head.obj"

#define PALETTES_PATH "../data/col/palettes.txt"

#define IMG_WIDTH 640
#define IMG_HEIGHT 640

static const TGAColor TGA_WHITE = TGAColor(255, 255, 255, 255);
static const TGAColor TGA_RED = TGAColor(255, 0, 0, 255);
static const TGAColor TGA_GREEN = TGAColor(0, 255, 0, 255);
static const TGAColor TGA_BLUE = TGAColor(0, 0, 255, 255);