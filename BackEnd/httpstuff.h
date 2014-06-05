#pragma once
#include "cpprest/containerstream.h"
#include "cpprest/filestream.h"
#include "cpprest/http_client.h"
#include "cpprest/json.h"
#include "cpprest/producerconsumerstream.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>         // std::locale, std::ctype, std::use_facet
#include <string>         // std::wstring
const std::basic_string<wchar_t> url = L"http://erisgaming.cloudapp.net:3000/collectionapi/";
#define STARTB 0x43
#define SELECTB 0x51
#define UPB 0x57
#define DOWNB 0x53
#define LEFTB 0x41
#define RIGHTB 0x44
#define AB 0x59
#define BB 0x5A
// Scan Codes
#define STARTK 0x2E
#define SELECTK 0x10
#define UPK 0x11
#define DOWNK 0x1F
#define LEFTK 0x1E
#define RIGHTK 0x20
#define AK 0x2D
#define BK 0x2C
pplx::task<int> postTest(int color, int row, int col);
int testPost();
int bingtest();
int getTest();
pplx::task<int> putTest(int color, std::basic_string<wchar_t> id, utility::string_t data);
extern std::string jsonval;
struct inputDB
{
	inputDB(int m = -1,int mk = -1, long long t = -1)
	{
		move = m;
		movek = mk;
		timestamp = t;
	}
	int move;  // the virtual key #
	int movek; // The hardware key thing
	long long timestamp; // when it was received
};
struct screenVal
{
	screenVal(){}
	screenVal(std::basic_string<wchar_t> sid, utility::string_t data64)
	{
		id = sid;
		bmpData64 = data64;
	}
	std::basic_string<wchar_t> id;
	utility::string_t bmpData64;
};
 std::vector<inputDB> parseInputDB();
 std::vector<screenVal> parseScreenVals();
