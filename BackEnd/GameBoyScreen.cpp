#include "GameBoyScreen.h"

bool operator==(GameBoyScreen A, GameBoyScreen B)
{
	for (int y = 0; y < GBHEIGHT; y++)
	{
		for (int x = 0; x < GBWIDTH; x++)
		{
			if (A.pixels[y][x] != B.pixels[y][x])
			{
				return false;
			}
		}
	}
	return true;
}