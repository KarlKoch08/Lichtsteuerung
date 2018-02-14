#include <iostream>
#include "SonnenWecker.h"

using namespace std;

SonnenWecker wecker;

int main(void)
{
	cout << "starting...\r\n";
	wecker.initialize();
	return 0;
}
