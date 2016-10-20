#include <windows.h>
#include "stdafx.h"
#include "configs.h"
#include "tgaimage.h"
#include "drawing.h"
#include "model.h"
#include "geometry.h"

// main folder: simplerenderer/app/
// g++ -std=c++0x -ggdb -g -pg -O0 -o test -I include src/main.cpp src/tgaimage.cpp src/model.cpp
// test.exe
// gprof test.exe > result.txt

HINSTANCE hInst;
HWND hWnd = NULL;

TCHAR szWindowClass[] = _T("simplerenderer");
TCHAR szTitle[] = _T("simplerenderer");

Model *model = NULL;

int testdrawing(TGAImage &image, int freq)
{
	clock_t start = clock();
	for (int i = 0; i < freq; i++) {
		vec2i t0[3] = { vec2i(10, 70), vec2i(50, 160), vec2i(70, 80) };
		vec2i t1[3] = { vec2i(180, 50), vec2i(150, 1), vec2i(70, 180) };
		vec2i t2[3] = { vec2i(180, 150), vec2i(120, 160), vec2i(130, 180) };
		triangle2d(t0[0], t0[1], t0[2], image, TGA_RED);
		triangle2d(t1[0], t1[1], t1[2], image, TGA_WHITE);
		triangle2d(t2[0], t2[1], t2[2], image, TGA_GREEN);
	}
	float duration = (float)(clock() - start) / CLOCKS_PER_SEC;
	std::ostringstream os;
	os << "Executed testdrawing() in " << duration << " seconds.";
	printf(os.str().c_str());
	return 1;
}

long __stdcall WindowProcedure(HWND window, unsigned int msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_DESTROY:
		std::cout << "\ndestroying window\n";
		PostQuitMessage(0);
		return 0L;
	case WM_KEYDOWN:
		std::cout << "\ndestroying window\n";
		PostQuitMessage(0);
		return 0L;
	case WM_LBUTTONDOWN:
		std::cout << "\nmouse left button down at (" << LOWORD(lp) << ',' << HIWORD(lp) << ")\n";
		// fall through
	default:
		return DefWindowProc(window, msg, wp, lp);
	}
}

int constructHWND()
{
	// draw to window
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_OWNDC;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(0);
	wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), szTitle, NULL);
		return 1;
	}

	hInst = GetModuleHandle(0);

	hWnd = CreateWindow(
		szWindowClass,					// the name of the application
		szTitle,						// the text that appears in the title bar
		WS_OVERLAPPEDWINDOW,			// the type of window to create
		CW_USEDEFAULT, CW_USEDEFAULT,	// initial position (x, y)
		IMG_WIDTH, IMG_HEIGHT,			// initial size (width, length)
		NULL,							// the parent of this window
		NULL,							// this application does not have a menu bar
		hInst,							// the first parameter from WinMain
		NULL							// not used in this application
		);

	if (!hWnd)
		return 0;

	ShowWindow(hWnd, 1);
	UpdateWindow(hWnd);
	return 1;
}

int main(int argc, char** argv)
{
	TGAImage image(IMG_WIDTH, IMG_HEIGHT, TGAImage::RGB);

	// testdrawing function
	testdrawing(image, 1); // freq 500000

	// load model
	if (argc == 2)
		model = new Model(argv[1]);
	else
		model = new Model(TEAPOT_OBJ_PATH);

	// draw model
	for (int i = 0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		assert(face.size() == 3);
		for (int j=0; j<3; j++) {
			vec3f vf0 = model->vert(face[j]) * (1.0f/model->size);;
			vec3f vf1 = model->vert(face[(j+1)%3]) * (1.0f/model->size);;
			vec2i vi0 = vec2i((vf0.x + 1.0f)*IMG_WIDTH/2.0f, (vf0.y + 1.0f)*IMG_HEIGHT/2.0f);
			vec2i vi1 = vec2i((vf1.x + 1.0f)*IMG_WIDTH/2.0f, (vf1.y + 1.0f)*IMG_HEIGHT/2.0f);
			line2d(vi0, vi1, image, TGA_WHITE);
		}
	}
	delete model;

	// set origin to bottom left corner and write to file
	image.flip_vertically();
	image.write_tga_file(OUTPUT_PATH);

	// print result message
	std::ostringstream os;
	os << "Succesfully wrote result to " << OUTPUT_PATH << "!";
	printf(os.str().c_str());

	// construct window
	constructHWND();
	HDC hdc = GetDC(hWnd);
	HPEN hOldPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(128, 128, 128)));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	// draw to window
	SetROP2(hdc, R2_NOTXORPEN);
	for (int y=0; y<IMG_WIDTH; y++) {
		for (int x=0; x<IMG_HEIGHT; x++) {
			TGAColor c = image.get(x, y);
			SetPixel(hdc, x, y, RGB(c.r, c.g, c.b));
		}
	}
	SelectObject(hdc, hOldBrush);
	DeleteObject(SelectObject(hdc, hOldPen));
	ReleaseDC(hWnd, hdc);

	// message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
