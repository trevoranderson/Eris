#include <iostream>
#include "ImageProc.h"
#include "GameBoyScreen.h"
#include "MouseMovement.h"
#include "httpstuff.h"
#include <vector>
#include <algorithm>


using namespace std;
bool operator <(inputDB A, inputDB B)
{
	return (A.timestamp < B.timestamp);
}
bool operator <(screenVal A, screenVal B)
{
	if (A.row < B.row)
	{
		return true;
	}
	if (B.row < A.row)
	{
		return false;
	}
	if (A.col < B.col)
	{
		return true;
	}
	return false;
}
bool operator ==(screenVal A, screenVal B)
{
	return A.row == B.row && A.col == B.col;
}
void applyMove(inputDB k)
{
	MouseMovement cur;
	cur.KeyEvent(k.move, k.movek);
	Sleep(500);
}
// Looks up the id in the vector
std::basic_string<wchar_t>findID(int row, int col, std::vector<screenVal> haystack)
{
	for (auto k : haystack)
	{
		if (k.row == row && k.col == col)
		{
			return k.id.substr(1, k.id.size() - 2); // cut off escape chars
		}
	}
	return L"";
}

void updateDifference(GameBoyScreen & fresh, GameBoyScreen & stale, std::vector<screenVal> toSearch)
{
	// x and y correspond to the original screen
	for (int y = 0; y < GBHEIGHT; y += 4)
	{
		for (int x = 0; x < GBWIDTH; x += 4)
		{
			if (fresh.pixels[y][x] != stale.pixels[y][x])
			{
				// Calculate the DB's version of the coords
				int scaledX = x / scalefactor;
				int scaledY = y / scalefactor;
				//update according to fresh's color
				auto id = findID(scaledX, scaledY, toSearch);
				if (id == L"")
				{
					cout << scaledY << "YandX" << scaledX << endl;
					// TODO Post the new ones to the database
				}
				else
				{
					putTest(fresh.pixels[y][x] - 'a', id);// .wait(); // .wait() ???
				}
			}
		}
	}
}
void updateAll(GameBoyScreen & fresh, std::vector<screenVal> toSearch)
{
	// x and y correspond to the original screen
	for (int y = 0; y < GBHEIGHT; y += 4)
	{
		for (int x = 0; x < GBWIDTH; x += 4)
		{
			// Calculate the DB's version of the coords
			int scaledX = x / scalefactor;
			int scaledY = y / scalefactor;
			//update according to fresh's color
			auto id = findID(scaledX , scaledY, toSearch);
			if (id == L"")
			{
				cout << scaledY << "YandX" << scaledX << endl;
				// TODO Post the new ones to the database
			}
			else
			{
				putTest(fresh.pixels[y][x] - 'a', id);// .wait(); // .wait() ???
			}
		}
	}

}
// this function never exits
void client()
{
	// Grab a copy of the screen vals so we can have ids to update
	auto screenvals = parseScreenVals();
	sort(screenvals.begin(), screenvals.end());
	long long lastTS = 0;
	auto screen = GameBoyScreen();
	updateAll(screen, screenvals);
	int k = 0;
	while (true)
	{
		cout << "loop!"<< k++ << endl;
		// TODO Get list of new moves and act on most recent ones
		getTest();
		vector<inputDB> toMove = parseInputDB();
		sort(toMove.begin(), toMove.end());
		// for each move, if its timestamp isn't too old, run the move
		for (auto & k : toMove)
		{
			if (k.timestamp > lastTS)
			{
				//applyMove(k);
				lastTS = k.timestamp;
			}
		}

		auto newscreen = GameBoyScreen(); // reset gameboyscreen
		if (newscreen != screen)
		{
			updateDifference(newscreen, screen, screenvals);
			screen = GameBoyScreen();
			//screenvals = parseScreenVals();
		}
	}
}

int main()
{
	//auto screenvals = parseScreenVals();
	//auto id = findID(4, 4, screenvals);
	//putTest(2, id);
	Sleep(5000);
	client();
	auto s = parseScreenVals();
	cout << endl << "Posting" << endl;
	auto p = postTest(1, 0, 0);
	cout << "Done posting" << endl << endl;

	getTest();
	//auto teemo = new GameBoyScreen();
	parseInputDB();
	cout << "hello world" << endl;
	//test();
	return 0;
}