﻿#include "stdafx.h"
#include "geometry.h"
#include "model.h"

static enum RenderMode { LINES, FACES };

void pixel2d(vec2i v0, TGAImage &img, const TGAColor &col)
{
	img.set(v0.x, v0.y, col);
}

void circle2d(vec2i v0, int radius, TGAImage &img, const TGAColor &col)
{
	// bresenhams circle algorithm?
	img.set(v0.x, v0.y, col);
}

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

vec3f barycentric(vec2i *pts, vec2i p) 
{
	vec3f u = vec3f::cross(vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - p.x), 
					vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - p.y));
	if (std::abs(u.z)<1) return vec3f(-1, 1, 1); // triangle is degenerate, in this case return smth with negative coordinates 
	return vec3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void triangle2d(vec2i *pts, TGAImage &img, TGAColor col) 
{
	// calculate bb (bounding box)
	vec2i bboxmin(IMG_WIDTH-1, IMG_HEIGHT-1);
	vec2i bboxmax(0, 0);
	vec2i clamp(IMG_WIDTH-1, IMG_HEIGHT-1);

	for (int i = 0; i<3; i++) {
		for (int j = 0; j<2; j++) {
			bboxmin.raw[j] = MAX(0, MIN(bboxmin.raw[j], pts[i].raw[j]));
			bboxmax.raw[j] = MIN(clamp.raw[j], MAX(bboxmax.raw[j], pts[i].raw[j]));
		}
	}
	
	// search for polygon pixels within bb
	vec2i p;
	for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++) {
		for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++) {
			vec3f bc_screen = barycentric(pts, p);
			if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
			img.set(p.x, p.y, col);
		}
	}
}

// line sweeping
void triangle2d_lnswp(vec2i v0, vec2i v1, vec2i v2, TGAImage &img, const TGAColor &col)
{
	// v0 -> v2 = va
	// v0 -> v1 = vb
	// v1 -> v2 = vc
	if (v0.y == v1.y && v0.y == v2.y) 
		return; // skip degenerate triangles 

	// order from bottom to top by y
	if (v0.y > v1.y) std::swap(v0.y, v1.y);
	if (v0.y > v2.y) std::swap(v0.y, v2.y);
	if (v1.y > v2.y) std::swap(v1.y, v2.y);

	// rasterization
	int triheight = v2.y-v0.y;
	for (int i = 0; i <= triheight; i++) {
		bool tophalf = (i>v1.y-v0.y) || (v1.y==v0.y);
		int segheight = (tophalf) ? v2.y - v1.y : v1.y - v0.y;
		float a = (float)i / triheight;
		float b = (float)(i - ((tophalf) ? v1.y - v0.y : 0)) / segheight;
		vec2i va = v0 + (v2-v0)*a;
		vec2i vb = (tophalf) ? v1 + (v2 - v1)*b : v0 + (v1 - v0)*b;
		line2d(va, vb, img, col);
	}
}

void render(Model *m, TGAImage &img, RenderMode rm)
{
	switch (rm)
	{
	case RenderMode::LINES:
		for (int i = 0; i < m->nfaces(); i++) {
			std::vector<int> face = m->face(i);
			assert(face.size() == 3);
			for (int j = 0; j < 3; j++) {
				vec3f vf0 = m->vert(face[j]) * (1.0f / m->size);;
				vec3f vf1 = m->vert(face[(j + 1) % 3]) * (1.0f / m->size);;
				vec2i vi0 = vec2i((vf0.x + 1.0f)*IMG_WIDTH / 2.0f, (vf0.y + 1.0f)*IMG_HEIGHT / 2.0f);
				vec2i vi1 = vec2i((vf1.x + 1.0f)*IMG_WIDTH / 2.0f, (vf1.y + 1.0f)*IMG_HEIGHT / 2.0f);
				line2d(vi0, vi1, img, TGA_WHITE);
			}
		}
		break;
	case RenderMode::FACES:
		for (int i = 0; i<m->nfaces(); i++) {
			std::vector<int> face = m->face(i);
			vec2i screen_coords[3];
			for (int j = 0; j<3; j++) {
				vec3f world_coords = m->vert(face[j]) * (1.0f / m->size);
				screen_coords[j] = vec2i((world_coords.x + 1.0f)*IMG_WIDTH / 2.0f, (world_coords.y + 1.0f)*IMG_HEIGHT / 2.0f);
			}
			triangle2d_lnswp(screen_coords[0], screen_coords[1], screen_coords[2], img, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
		}
		break;
	}
}