#include <iostream>
#include "ImageProc.h"
#include "GameBoyScreen.h"
#include "MouseMovement.h"
#include "httpstuff.h"
using namespace std;
int main()
{

	auto f = getTest();
	cout << endl << "Posting" << endl;
	auto p = testPost();
	cout << "Done posting" << endl << endl;
	f = getTest();

	//auto teemo = new GameBoyScreen();
	cout << "hello world" << endl;
	//test();
	return 0;
}