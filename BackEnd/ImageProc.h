#pragma once
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <set>
#define BYTESPERPIXEL 4
extern int SCREENWIDTH;
extern int SCREENHEIGHT;

BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName);
extern bool operator < (const COORD A, const COORD B);


void GetDesktopResolution(int& horizontal, int& vertical);
struct RGBA
{
	char r;
	char g;
	char b;
	char a;
};
class ScreenShot
{
public:
	//Methods
	ScreenShot()
	{
		// initialize if not set
		if (!SCREENHEIGHT || !SCREENWIDTH)
		{
			GetDesktopResolution(SCREENWIDTH, SCREENHEIGHT);
		}
		//get client DC
		desktophandle = GetDC(NULL);
		//create memory DC
		memHDC = CreateCompatibleDC(desktophandle);
		//create new HBITMAP compatible with client dc
		Teemo = CreateCompatibleBitmap(desktophandle, SCREENWIDTH, SCREENHEIGHT);
		//select bitmap into memory DC
		SelectObject(memHDC, Teemo);
		//copy from client DC to memory DC
		BitBlt(memHDC, 0, 0, SCREENWIDTH, SCREENHEIGHT, desktophandle, 0, 0, SRCCOPY);
		// GetBitmapBits
		long numBits = SCREENWIDTH*SCREENHEIGHT * 4;
		//	lpvBITBUF = new LPVOID;
		bmpBuffer = (BYTE*)GlobalAlloc(GPTR, SCREENWIDTH*SCREENHEIGHT * 4);
		GetBitmapBits(Teemo, numBits, bmpBuffer);
		SaveToFile(Teemo, L"Teemo.bmp");
		ReleaseDC(NULL, desktophandle);
		DeleteDC(memHDC);
	}
	~ScreenShot()
	{
		DeleteObject(Teemo);
		//	delete lpvBITBUF;
		GlobalFree(bmpBuffer);
	}
	std::set<COORD> FindBMP(LPCTSTR testBMP);
	//allocates and returns a byte array of a subBMP

	RGBA getPixel(int x, int y)
	{
		RGBA ret;
		ret.r = bmpBuffer[((y)*SCREENWIDTH + (x)) * 4];
		ret.g = bmpBuffer[((y)*SCREENWIDTH + (x)) * 4 + 1];
		ret.b = bmpBuffer[((y)*SCREENWIDTH + (x)) * 4 + 2];
		return ret;
	}
	HBITMAP getSubBMP(int ulx, int uly, int brx, int bry)
	{
		return bitmapFromBytes(getBytes(ulx, uly, brx, bry), abs(brx - ulx), abs(bry - uly));
	}
private:
	//Methods
	BYTE * getBytes(int ulx, int uly, int brx, int bry)
	{
		//w = x + (x % 4);
		int xx = abs(brx - ulx);
		int width = xx + (xx % 4); // include padding
		int height = abs(bry - uly);
		int area = width * height;
		BYTE * ret = new BYTE[area * BYTESPERPIXEL];
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (x < xx)
				{
					// copy source into dest
					auto c = getPixel(ulx + x, uly + y);
					ret[(y*width + x) * BYTESPERPIXEL + 0] = c.r;
					ret[(y*width + x) * BYTESPERPIXEL + 1] = c.g;
					ret[(y*width + x) * BYTESPERPIXEL + 2] = c.b;
					ret[(y*width + x) * BYTESPERPIXEL + 3] = c.a;
				}
				else
				{
					ret[(y*width + x) * BYTESPERPIXEL + 0] = 0;
					ret[(y*width + x) * BYTESPERPIXEL + 1] = 0;
					ret[(y*width + x) * BYTESPERPIXEL + 2] = 0;
					ret[(y*width + x) * BYTESPERPIXEL + 3] = 0;
				}
			}
		}
		return ret;
	}
	HBITMAP bitmapFromBytes(BYTE arr[], int width, int height);
	//Guts
	HDC desktophandle;
	HDC memHDC;
	HBITMAP Teemo;
	//LPVOID lpvBITBUF;
	BYTE* bmpBuffer;
};