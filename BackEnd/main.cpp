#include <iostream>
#include "ImageProc.h"
#include "GameBoyScreen.h"
#include "MouseMovement.h"
#include "httpstuff.h"
#include <vector>
#include <algorithm>
#include <thread>

using namespace std;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}
utility::string_t base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
	utility::string_t ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i <4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}
std::string base64_decode(std::string const& encoded_string) {
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}
bool operator <(inputDB A, inputDB B)
{
	return (A.timestamp < B.timestamp);
}
bool operator <(screenVal A, screenVal B)
{
	return false;
}
bool operator ==(screenVal A, screenVal B)
{
	return A.bmpData64 == B.bmpData64;
}
void applyMove(inputDB k)
{
	MouseMovement cur;
	cur.KeyEvent(k.move, k.movek);
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
				auto id = L"no";// findID(scaledX, scaledY, toSearch);
				if (id == L"")
				{
					cout << scaledY << "YandX" << scaledX << endl;
					// TODO Post the new ones to the database
				}
				else
				{
				//	putTest(fresh.pixels[y][x] - 'a', id);// .wait(); // .wait() ???
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
			auto id = L"no";// findID(scaledX, scaledY, toSearch);
			if (id == L"")
			{
				cout << scaledY << "YandX" << scaledX << endl;
				// TODO Post the new ones to the database
			}
			else
			{
			//	putTest(fresh.pixels[y][x] - 'a', id);// .wait(); // .wait() ???
			}
		}
	}

}
std::vector<BYTE> readFileBytes(std::string filename)
{
	// open the file:
	std::streampos fileSize;
	std::ifstream file(filename, std::ios::binary);

	// get its size:
	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	// read the data:
	std::vector<BYTE> fileData(fileSize);
	file.read((char*)&fileData[0], fileSize);
	return fileData;
}
// Command loop
void commandLoop()
{
	GameBoyScreen g;
	Point loc(g.anchor.X, g.anchor.Y+150);
	long long lastTS = 0;
	while (true)
	{
		// TODO Get list of new moves and act on most recent ones
		getTest();
		vector<inputDB> toMove = parseInputDB();
		sort(toMove.begin(), toMove.end());
		// for each move, if its timestamp isn't too old, run the move
		for (auto & k : toMove)
		{
			if (k.timestamp > lastTS)
			{
				quickClick(loc);
				applyMove(k);
				lastTS = k.timestamp;
			}
		}
	}
}
// Screen Grab Loop
void screenGrabLoop()
{
	auto asdf = parseScreenVals();
	int loopcounter = 0;
	while (1)
	{
		cout << loopcounter << endl;
		GameBoyScreen teemo;
		auto x = sizeof(teemo.image);
		SaveToFile(teemo.image, L"images\\gameboy.bmp");
		// Now read it
		auto contents = readFileBytes("images\\gameboy.bmp");
		auto str = base64_encode(&contents[0], contents.size());
		//std::basic_string<wchar_t> toSend(L"data:image/bmp;base64,");
		str = L"data:image/bmp;base64," + str;
		//"data:image/bmp;base64,"
		auto ID = asdf[0].id.substr(1, asdf[0].id.size() - 2); // cut off escape chars
		putTest(0, ID, str);
		Sleep(100);
		loopcounter++;
	}
}
// this function never exits
void client()
{
	thread inputCreator(commandLoop);
	thread screenUpdater(screenGrabLoop);
	while (1){}
	// Grab a copy of the screen vals so we can have ids to update
	auto screenvals = parseScreenVals();
	sort(screenvals.begin(), screenvals.end());
	long long lastTS = 0;
	auto screen = GameBoyScreen();
	updateAll(screen, screenvals);
	int k = 0;
	while (true)
	{
		cout << "loop!" << k++ << endl;
		auto newscreen = GameBoyScreen(); // reset gameboyscreen
		if (newscreen != screen)
		{
			updateDifference(newscreen, screen, screenvals);
			screen = GameBoyScreen();
			screenvals = parseScreenVals();
		}
	}
}

void testbmp()
{
	auto asdf = parseScreenVals();
	/*ScreenShot mundo;
	SaveToFile(mundo.getSubBMP(50,50,450,450), L"grapes.bmp");*/
 	GameBoyScreen teemo;
	auto x = sizeof(teemo.image);
	SaveToFile(teemo.image, L"images\\gameboy.bmp");
	// Now read it
	auto contents = readFileBytes("images\\gameboy.bmp");
	auto str = base64_encode(&contents[0], contents.size());
	auto ID = asdf[0].id.substr(1, asdf[0].id.size() - 2); // cut off escape chars
	putTest(0, ID, str);
	cout << "sup" << endl;
}
int main()
{
	client();
	while (1){}
	testbmp();
	//return 0;
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