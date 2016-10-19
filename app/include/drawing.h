#include <cmath>
#include "tgaimage.h"

void line2d(int x0, int y0, int x1, int y1, TGAImage &img, const TGAColor &col) 
{
	bool steep = std::abs(x1 - x0) < std::abs(y0 - y1);
	if (steep) {	// transpose steep lines
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if (x0 > x1) {	// always draw from left to right
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1-x0;
	int dy = y1-y0;
	int step = (y1>y0 ? 1:-1);
	int derr = std::abs(dy)*2; //std::abs(dy/float(dx));
	int err = 0;
	int y = y0;
	for (int x=x0; x<=x1; x++) {	// draw the line
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