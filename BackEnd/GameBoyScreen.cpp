#include "GameBoyScreen.h"

bool operator==(GameBoyScreen A, GameBoyScreen B)
{
	for (int y = 0; y < GBHEIGHT; y+= 1)
	{
		for (int x = 0; x < GBWIDTH; x+= 1)
		{
			if (A.pixels[y][x] != B.pixels[y][x])
			{
				return false;
			}
		}
	}
	return true;
}
bool operator!=(GameBoyScreen A, GameBoyScreen B)
{
	return !(A == B);
}