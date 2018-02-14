#include <iostream>
#include <fstream>
#include <string>
extern "C"{
#include <cstdlib>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pigpio.h>
}
#include "SonnenWecker.h"

using namespace std;

SonnenWecker::~SonnenWecker()
{
	gpioTerminate();
}

SonnenWecker::SonnenWecker()
{
	redValue = 0;
	blueValue = 0;
	greenValue = 0;
	GPIOred = 17;
	GPIOblue = 24;
	GPIOgreen =22;
	targetColor = color1;
	timeConstant = 60;
}

void SonnenWecker::initialize()
{
	int test = gpioInitialise();
	if(test !=  -1)
	{
		gpioSetMode(GPIOred, PI_OUTPUT);
		gpioSetMode(GPIOgreen, PI_OUTPUT);
		gpioSetMode(GPIOblue, PI_OUTPUT);
		getTimeOfDay();
	}
	else
	{
		cout << "initialize failed\r\n";
	}
}

void SonnenWecker::shutdown()
{
	gpioWrite(GPIOred, 0);
	gpioWrite(GPIOblue, 0);
	gpioWrite(GPIOgreen, 0);
}

void SonnenWecker::run()
{
	targetColor = color1;
	redValue = 0;
	blueValue = 0;
	greenValue = 0;
	while(redValue != endColor[0] || greenValue != endColor[1] || blueValue != endColor[2])
	{
//		cout << "red: " << redValue << "green: " << greenValue << "blue: " << blueValue << "\r\n";
//		cout << "redEnd: " << endColor[0] << "endGreen: " << endColor[1] << "endBlue: " << endColor[2] << "\r\n";
		while(blueValue != targetColor[2] || redValue != targetColor[0] || greenValue != targetColor[1])
		{
//			cout << "curretColor: " << redValue << " " << greenValue << " " << blueValue << "\r\n";
			if(redValue != targetColor[0] && (abs(targetColor[0] - redValue)) != 0)
			{
//				cout << "target: " << targetColor[0] << " " << redValue << "\r\n";
//				cout << "steps: " <<  (targetColor[0] - redValue)/abs(targetColor[0] - redValue) << "\r\n";
//				cout << "red:" << redValue << "\r\n";
//				cout << "redDone\r\n";
				updateColor(red, (targetColor[0] - redValue)/abs(targetColor[0] - redValue));
			}
			if(greenValue != targetColor[1] && (abs(targetColor[1] - greenValue)) != 0)
			{
//				cout << "reason: " << targetColor[1] << " " << greenValue << "\r\n";
//				cout << "here?\r\n" << (abs(targetColor[1] - greenValue)) << "\r\n";
//				cout << "stepsgreen: " << (targetColor[1] - greenValue)/abs(targetColor[1] - greenValue) << "\r\n";
//				cout << "green:" << greenValue << "\r\n";
//				cout << "greenDone\r\n";
				updateColor(green, (targetColor[1] - greenValue)/abs(targetColor[1] - greenValue));
			}
			if(blueValue != targetColor[2] && (abs(targetColor[2] - blueValue)) != 0)
			{
//				cout << "stepsblue: " << (targetColor[2] - blueValue)/abs(targetColor[2] - blueValue) << "\r\n";
//				cout << "blue:" << blueValue << "\r\n";
//				cout << "targetColor: " << targetColor[0] << " " << targetColor[1] << " " << targetColor[2] << "\r\n";
//				cout << "diff: " << targetColor[2] - blueValue << "\r\n";
//				cout << "blueDone\r\n";
				updateColor(blue, (targetColor[2] - blueValue)/abs(targetColor[2] - blueValue));
			}
//			cout << ((targetColor[1] - greenValue)/abs(targetColor[1] - greenValue)) << "\r\n";
//			cout << ((targetColor[2] - blueValue)/abs(targetColor[2] - blueValue)) << "\r\n";
//			cout << "-----\r\n";
			updateStrip();
			gpioSleep(0,10,30000);
		}
//		cout << "targetColor: " << targetColor[0] << " " << targetColor[1] << " " << targetColor[2] << "\r\n";
		if(targetColor == color1 && redValue == targetColor[0] && greenValue == targetColor[1] && blueValue == targetColor[2])
		{
//			cout << " == color1" << (targetColor == color1) << "\r\n";
			targetColor = color2;
		}
		else if(targetColor == color2 && redValue == targetColor[0] && greenValue == targetColor[1] && blueValue == targetColor[2])
		{
//			cout << " == color2" << (targetColor == color2) << "\r\n";
			targetColor = color3;
//			cout << "targetColor: " << targetColor[0] << " " << targetColor[1] << " " << targetColor[2] << "\r\n";
		}
		else if(targetColor == color3 && redValue == targetColor[0] && greenValue == targetColor[1] && blueValue == targetColor[2])
		{
//			cout << " == color3" << (targetColor == color3) << "\r\n";
			targetColor = endColor;
		}
	}
	gpioSleep(0,900,0);
	shutdown();
	getTimeOfDay();
}

void SonnenWecker::updateColor(Color color, int step)
{
	switch(color)
	{
		case red: redValue = redValue + step; break;
		case green: greenValue = greenValue + step; break;
		case blue: blueValue = blueValue + step; break;
	}
}

void SonnenWecker::updateStrip()
{
	gpioPWM(GPIOred, redValue);
	gpioPWM(GPIOgreen, greenValue);
	gpioPWM(GPIOblue, blueValue);
}

void SonnenWecker::getTimeOfDay()
{
	time_t rawtime;
	time(&rawtime);
	struct tm *timeinfo;
	timeinfo = localtime(&rawtime);
	cout << asctime(timeinfo) << "\r\n";
	if(timeinfo->tm_hour == 6)
	{
		if(timeinfo->tm_min > 30)
		{
			run();
		}
		else
		{
			sleep(30);
			getTimeOfDay();
		}
	}
	else
	{
		sleep(360);
		getTimeOfDay();
	}
}
