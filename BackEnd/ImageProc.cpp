
#include "ImageProc.h"
#include <iostream>
#include <stdio.h>       // for memset
using namespace std;
int SCREENWIDTH = 0;
int SCREENHEIGHT = 0;
bool operator < (const COORD A, const COORD B)
{
	if (A.Y != B.Y)
	{
		return A.Y < B.Y;
	}
	return A.X < B.X;
}
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
//BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
//{
//	HDC hDC;
//
//	int iBits;
//
//	WORD wBitCount;
//
//	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
//
//	BITMAP Bitmap;
//
//	BITMAPFILEHEADER bmfHdr;
//
//	BITMAPINFOHEADER bi;
//
//	LPBITMAPINFOHEADER lpbi;
//
//	HANDLE fh, hDib, hPal, hOldPal = NULL;
//
//	hDC = CreateDC(L"images\\DISPLAY", NULL, NULL, NULL);
//	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
//	DeleteDC(hDC);
//	if (iBits <= 1)
//		wBitCount = 1;
//	else if (iBits <= 4)
//		wBitCount = 4;
//	else if (iBits <= 8)
//		wBitCount = 8;
//	else
//		wBitCount = 24;
//	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
//	bi.biSize = sizeof(BITMAPINFOHEADER);
//	bi.biWidth = Bitmap.bmWidth;
//	bi.biHeight = Bitmap.bmHeight;
//	bi.biPlanes = 1;
//	bi.biBitCount = wBitCount;
//	bi.biCompression = BI_RGB;
//	bi.biSizeImage = 0;
//	bi.biXPelsPerMeter = 0;
//	bi.biYPelsPerMeter = 0;
//	bi.biClrImportant = 0;
//	bi.biClrUsed = 0;
//	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
//
//	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
//	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
//	*lpbi = bi;
//
//	hPal = GetStockObject(DEFAULT_PALETTE);
//	if (hPal)
//	{
//		hDC = GetDC(NULL);
//		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
//		RealizePalette(hDC);
//	}
//
//
//	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
//		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);
//
//	if (hOldPal)
//	{
//		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
//		RealizePalette(hDC);
//		ReleaseDC(NULL, hDC);
//	}
//
//	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
//		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
//
//	if (fh == INVALID_HANDLE_VALUE)
//		return FALSE;
//
//	bmfHdr.bfType = 0x4D42; // "BM"
//	dwDIBSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
//	bmfHdr.bfSize = dwDIBSize;
//	bmfHdr.bfReserved1 = 0;
//	bmfHdr.bfReserved2 = 0;
//	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;
//
//	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
//
//	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
//	GlobalUnlock(hDib);
//	GlobalFree(hDib);
//	CloseHandle(fh);
//	return TRUE;
//}

BOOL SaveToFile(HBITMAP hBitmap, LPCTSTR lpszFileName)
{
	HDC hDC;

	int iBits;

	WORD wBitCount;

	DWORD dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;

	BITMAP Bitmap;

	BITMAPFILEHEADER bmfHdr;

	BITMAPINFOHEADER bi;

	LPBITMAPINFOHEADER lpbi;

	HANDLE fh, hDib, hPal, hOldPal = NULL;

	hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else
		wBitCount = 24;
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrImportant = 0;
	bi.biClrUsed = 0;
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = GetDC(NULL);
		hOldPal = SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}


	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize, (BITMAPINFO *)lpbi, DIB_RGB_COLORS);

	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		ReleaseDC(NULL, hDC);
	}

	fh = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	bmfHdr.bfType = 0x4D42; // "BM"
	dwDIBSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
	return TRUE;
}

/*******************************************************************
BYTE* ConvertRGBToBMPBuffer ( BYTE* Buffer, int width,
int height, long* newsize )


This function takes as input an array of RGB values, it's width
and height.
The buffer gets then transformed to an array that can be used
to write to a windows bitmap file. The size of the array
is returned in newsize, the array itself is the
return value of the function.
Both input and output buffers must be deleted by the
calling function.

The input buffer is expected to consist of width * height
RGB triplets. Thus the total size of the buffer is taken as
width * height * 3.

The function then transforms this buffer so that it can be written
to a windows bitmap file:
First the RGB triplets are converted to BGR.
Then the buffer is swapped around since .bmps store
images uside-down.
Finally the buffer gets DWORD ( 32bit ) aligned,
meaning that each scanline ( 3 * width bytes ) gets
padded with 0x00 bytes up to the next DWORD boundary


*******************************************************************/
BYTE* ConvertRGBToBMPBuffer(BYTE* Buffer, int width, int height, long* newsize)
{

	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// now we have to find with how many bytes
	// we have to pad for the next DWORD boundary	

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// we can already store the size of the new padded buffer
	*newsize = height * psw;

	// and create new buffer
	BYTE* newbuf = new BYTE[*newsize];

	// fill the buffer with zero bytes then we dont have to add
	// extra padding zero bytes later on
	memset(newbuf, 0, *newsize);

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int y = 0; y < height; y++)
	for (int x = 0; x < 3 * width; x += 3)
	{
		bufpos = y * 3 * width + x;     // position in original buffer
		newpos = (height - y - 1) * psw + x;           // position in padded buffer

		newbuf[newpos] = Buffer[bufpos + 2];       // swap r and b
		newbuf[newpos + 1] = Buffer[bufpos + 1]; // g stays
		newbuf[newpos + 2] = Buffer[bufpos];     // swap b and r
	}

	return newbuf;
}

/***************************************************************
BYTE* ConvertBMPToRGBBuffer ( BYTE* Buffer,
int width, int height )

This function takes as input the data array
from a bitmap and its width and height.
It then converts the bmp data into an RGB array.
The calling function must delete both the input
and output arrays.
The size of the returned array will be
width * height * 3
On error the returb value is NULL, else the
RGB array.


The Buffer is expected to be the exact data read out
from a .bmp file.
The function will swap it around, since images
are stored upside-down in bmps.
The BGR triplets from the image data will
be converted to RGB.
And finally the function removes padding bytes.
The returned arraay consits then of
width * height RGB triplets.

*****************************************************************/
BYTE* ConvertBMPToRGBBuffer(BYTE* Buffer, int width, int height)
{
	// first make sure the parameters are valid
	if ((NULL == Buffer) || (width == 0) || (height == 0))
		return NULL;

	// find the number of padding bytes

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;

	// create new buffer
	BYTE* newbuf = new BYTE[width*height * 3];

	// now we loop trough all bytes of the original buffer, 
	// swap the R and B bytes and the scanlines
	long bufpos = 0;
	long newpos = 0;
	for (int y = 0; y < height; y++)
	for (int x = 0; x < 3 * width; x += 3)
	{
		newpos = y * 3 * width + x;
		bufpos = (height - y - 1) * psw + x;

		newbuf[newpos] = Buffer[bufpos + 2];
		newbuf[newpos + 1] = Buffer[bufpos + 1];
		newbuf[newpos + 2] = Buffer[bufpos];
	}

	return newbuf;
}

/***********************************************
bool LoadBMPIntoDC ( HDC hDC, LPCTSTR bmpfile )

Takes in a device context and the name of a
bitmap to load. If an error occurs the function
returns false, else the contents of the bmp
are blitted to the HDC

************************************************/
bool LoadBMPIntoDC(HDC hDC, LPCTSTR bmpfile)
{
	// check if params are valid 
	if ((NULL == hDC) || (NULL == bmpfile))
		return false;

	// load bitmap into a bitmap handle
	HANDLE hBmp = LoadImage(NULL, bmpfile, IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);

	if (NULL == hBmp)
		return false;        // failed to load image

	// bitmaps can only be selected into memory dcs:
	HDC dcmem = CreateCompatibleDC(NULL);

	// now select bitmap into the memory dc
	if (NULL == SelectObject(dcmem, hBmp))
	{	// failed to load bitmap into device context
		DeleteDC(dcmem);
		return false;
	}


	// now get the bmp size
	BITMAP bm;
	GetObject(hBmp, sizeof(bm), &bm);
	// and blit it to the visible dc
	if (BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, dcmem,
		0, 0, SRCCOPY) == 0)
	{	// failed the blit
		DeleteDC(dcmem);
		return false;
	}

	DeleteDC(dcmem);  // clear up the memory dc

	return true;
}

/***************************************************************
bool SaveBMP ( BYTE* Buffer, int width, int height,
long paddedsize, LPCTSTR bmpfile )

Function takes a buffer of size <paddedsize>
and saves it as a <width> * <height> sized bitmap
under the supplied filename.
On error the return value is false.

***************************************************************/
bool SaveBMP(BYTE* Buffer, int width, int height, long paddedsize, LPCTSTR bmpfile)
{
	// declare bmp structures 
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER info;

	// andinitialize them to zero
	memset(&bmfh, 0, sizeof (BITMAPFILEHEADER));
	memset(&info, 0, sizeof (BITMAPINFOHEADER));

	// fill the fileheader with data
	bmfh.bfType = 0x4d42;       // 0x4d42 = 'BM'
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+paddedsize;
	bmfh.bfOffBits = 0x36;		// number of bytes to start of bitmap bits

	// fill the infoheader

	info.biSize = sizeof(BITMAPINFOHEADER);
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;			// we only have one bitplane
	info.biBitCount = 24;		// RGB mode is 24 bits
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;		// can be 0 for 24 bit images
	info.biXPelsPerMeter = 0x0ec4;     // paint and PSP use this values
	info.biYPelsPerMeter = 0x0ec4;
	info.biClrUsed = 0;			// we are in RGB mode and have no palette
	info.biClrImportant = 0;    // all colors are important

	// now we open the file to write to
	HANDLE file = CreateFile(bmpfile, GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == NULL)
	{
		CloseHandle(file);
		return false;
	}

	// write file header
	unsigned long bwritten;
	if (WriteFile(file, &bmfh, sizeof (BITMAPFILEHEADER), &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}
	// write infoheader
	if (WriteFile(file, &info, sizeof (BITMAPINFOHEADER), &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}
	// write image data
	if (WriteFile(file, Buffer, paddedsize, &bwritten, NULL) == false)
	{
		CloseHandle(file);
		return false;
	}

	// and close file
	CloseHandle(file);

	return true;
}

/*******************************************************************
BYTE* LoadBMP ( int* width, int* height, long* size
LPCTSTR bmpfile )

The function loads a 24 bit bitmap from bmpfile,
stores it's width and height in the supplied variables
and the whole size of the data (padded) in <size>
and returns a buffer of the image data

On error the return value is NULL.

NOTE: make sure you [] delete the returned array at end of
program!!!
*******************************************************************/
BYTE* LoadBMP(int* width, int* height, long* size, LPCTSTR bmpfile)
{
	// declare bitmap structures
	BITMAPFILEHEADER bmpheader;
	BITMAPINFOHEADER bmpinfo;
	// value to be used in ReadFile funcs
	DWORD bytesread;
	// open file to read from
	HANDLE file = CreateFile(bmpfile, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (NULL == file)
		return NULL; // coudn't open file


	// read file header
	if (ReadFile(file, &bmpheader, sizeof (BITMAPFILEHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	//read bitmap info

	if (ReadFile(file, &bmpinfo, sizeof (BITMAPINFOHEADER), &bytesread, NULL) == false)
	{
		CloseHandle(file);
		return NULL;
	}

	// check if file is actually a bmp
	if (bmpheader.bfType != 'MB')
	{
		CloseHandle(file);
		return NULL;
	}

	// get image measurements
	*width = bmpinfo.biWidth;
	*height = abs(bmpinfo.biHeight);

	// check if bmp is uncompressed
	if (bmpinfo.biCompression != BI_RGB)
	{
		CloseHandle(file);
		return NULL;
	}

	// check if we have 24 bit bmp
	if (bmpinfo.biBitCount != 24)
	{
		CloseHandle(file);
		return NULL;
	}


	// create buffer to hold the data
	*size = bmpheader.bfSize - bmpheader.bfOffBits;
	BYTE* Buffer = new BYTE[*size];
	// move file pointer to start of bitmap data
	SetFilePointer(file, bmpheader.bfOffBits, NULL, FILE_BEGIN);
	// read bmp data
	if (ReadFile(file, Buffer, *size, &bytesread, NULL) == false)
	{
		delete[] Buffer;
		CloseHandle(file);
		return NULL;
	}

	// everything successful here: close file and return buffer

	CloseHandle(file);

	return Buffer;
}
void TestBMPCopy(LPCTSTR input, LPCTSTR output)
{
	int x, y;
	long s;
	BYTE* b = LoadBMP(&x, &y, &s, input);
	SaveBMP(b, x, y, s, output);
	delete[] b;
}
void TestBMPCopy2(LPCTSTR input, LPCTSTR output)
{
	int x, y;
	long s, s2;
	BYTE* a = LoadBMP(&x, &y, &s, input);
	BYTE* b = ConvertBMPToRGBBuffer(a, x, y);
	BYTE* c = ConvertRGBToBMPBuffer(b, x, y, &s2);
	SaveBMP(c, x, y, s2, output);
	delete[] a;
	delete[] b;
	delete[] c;
}

/*
	This returns a set of ordered COORDS (by Y value) of where the given BMP was found on the ScreenShot

	Caveat: the width of the LOADBMP array is padded to be a multiple of 4 (WTF)
*/
set<COORD> ScreenShot::FindBMP(LPCTSTR testBMP)
{
	set<COORD> ret;
	int x, y, w;
	long s, s2;
	BYTE * test = LoadBMP(&x, &y, &s, testBMP);
	if (test == nullptr)
	{
		cout << "couldn't open " << testBMP << endl;
		return ret;
	}
	w = x + (x % 4);
	//Each allowable X in screenshot
	//Each allowable Y in screenShot
	for (int ssY = 0; ssY < SCREENHEIGHT - y; ssY++)
	{
		for (int ssX = 0; ssX < SCREENWIDTH - x; ssX++)
		{
			bool matchFound = true;
			int skipped = 0;
			//each Y in BMP
			for (int bmpY = 0; bmpY < y; bmpY++)
			{
				//Each X in BMP
				for (int bmpX = 0; bmpX < x; bmpX++)
				{
						// test[ (curHeight*width + curWidth) * elementsperpixel + height*paddingperheight
					if (   test[(bmpY*x + bmpX) * 3 + bmpY*(w - x)] == this->bmpBuffer[((ssY - bmpY)*SCREENWIDTH + (ssX + bmpX)) * 4]
						&& test[(bmpY*x + bmpX) * 3 + bmpY*(w - x) + 1] == this->bmpBuffer[((ssY - bmpY)*SCREENWIDTH + (ssX + bmpX)) * 4 + 1]
						&& test[(bmpY*x + bmpX) * 3 + bmpY*(w - x) + 2] == this->bmpBuffer[((ssY - bmpY)*SCREENWIDTH + (ssX + bmpX)) * 4 + 2]
						)
					{
						//All good in the hood: they match
						matchFound = true;
					}
					else
					{
						matchFound = false;
						break;
					}
				}
				//skipped++;
				if (!matchFound)
				{
					break;
				}
			}
			if (matchFound)
			{
				//cout << "ssX: "<< ssX<<" ssY: "<< ssY << endl;
				COORD toInsert;
				toInsert.X = ssX;
				toInsert.Y = ssY;
				ret.insert(toInsert);
			}
		}
	}
	delete test;
	return ret;
}

HBITMAP ScreenShot::bitmapFromBytes(BYTE arr[],int width, int height)
{
	auto ret = CreateBitmap(width, height, 1, 32, arr);
	return ret;
}

BYTE * hbitmapToBytes(HBITMAP input)
{
	char* pImage = NULL;
	HANDLE hImage = input;
	pImage = (char *)GlobalLock(hImage);
	return (BYTE *)pImage;
}