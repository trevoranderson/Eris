#pragma once
#include <iostream>
#include "ImageProc.h"
#define GBHEIGHT 144
#define GBWIDTH 160
#define XOFFSET -6
#define YOFFSET 53
const int scalefactor = 4;
class GameBoyScreen
{
public:
	GameBoyScreen()
	{
		//ensure reliable construction
		auto screen = new ScreenShot();
		auto gbicon = screen->FindBMP(L"images\\visualboy.bmp");
		auto gbicon2 = screen->FindBMP(L"images\\visualboy2.bmp");
		gbicon.insert(gbicon2.begin(), gbicon2.end());
		while (gbicon.size() == 0)
		{
			ScreenShot * tmp = screen;
			screen = new ScreenShot();
			delete tmp;
			gbicon = screen->FindBMP(L"images\\visualboy.bmp");
			gbicon2 = screen->FindBMP(L"images\\visualboy2.bmp");
			gbicon.insert(gbicon2.begin(), gbicon2.end());
		}
		anchor.X = gbicon.begin()->X;
		anchor.Y = gbicon.begin()->Y;
		//Actual pixels are offset from the reference img by -6 and 53 pixels
		for (int y = 0; y < GBHEIGHT; y++)
		{
			for (int x = 0; x < GBWIDTH; x++)
			{
				auto pix = screen->getPixel(x + anchor.X + XOFFSET, y + anchor.Y + YOFFSET);
				if (pix.r == 48 && pix.g == -32 && pix.b == 112)
				{
					pixels[y][x] = 'a';
				}
				else if (pix.r == 32 && pix.g == 80 && pix.b == 0)
				{
					pixels[y][x] = 'b';
				}
				else if (pix.r == 0 && pix.g == -80 & pix.b == 0)
				{
					pixels[y][x] = 'c';
				}
				else if (pix.r == 24 && pix.g == 104 && pix.b == 0)
				{

					pixels[y][x] = 'd';
				}
				else
				{
					pixels[y][x] = 'a';
				}
			}
		}
		delete screen;
	}
	~GameBoyScreen()
	{
	//	delete screen;
	}
	// Data
	COORD anchor; // BOTTOM RIGHT OF REFERENCE IMG
	//ScreenShot * screen;
	//Resolution is 144y by 160x [ROW][COLUMN]
	char pixels[GBHEIGHT][GBWIDTH];
};

bool operator==(GameBoyScreen A, GameBoyScreen B);
bool operator!=(GameBoyScreen A, GameBoyScreen B);