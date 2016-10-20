#include "stdafx.h"
#include "geometry.h"

void line2d(vec2i v0, vec2i v1, TGAImage &img, const TGAColor &col) 
{
	bool steep = std::abs(v1.x - v0.x) < std::abs(v0.y - v1.y);
	if (steep) {	// transpose steep lines
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
	}
	if (v0.x > v1.x) {	// always draw from left to right
		std::swap(v0.x, v1.x);
		std::swap(v0.y, v1.y);
	}
	int dx = v1.x-v0.x;
	int dy = v1.y-v0.y;
	int step = (v1.y>v0.y ? 1:-1);
	int derr = std::abs(dy)*2; //std::abs(dy/float(dx));
	int err = 0;
	int y = v0.y;
	for (int x = v0.x; x <= v1.x; x++) {	// draw the line
		if (steep)
			img.set(y, x, col);		// if transposed, de−transpose 
		else
			img.set(x, y, col);
		err += derr;
		if (err > dx) {
			y += step;
			err -= dx*2;
		}
	}
}

void triangle2d(vec2i v0, vec2i v1, vec2i v2, TGAImage &img, const TGAColor &col)
{
	line2d(v0, v1, img, col);
	line2d(v1, v2, img, col);
	line2d(v2, v0, img, col);
}