#include <vector>

class SonnenWecker
{
public:
	~SonnenWecker();
	SonnenWecker();
	void initialize();
	void run();
	void shutdown();

enum Color{red, green, blue};

private:
	int redValue;
	int blueValue;
	int greenValue;
	int GPIOred;
	int GPIOgreen;
	int GPIOblue;
	int step;
	std::vector<int> color1 = {20,10,60};
	std::vector<int> color2 = {120, 20, 45};
	std::vector<int> color3 = {255, 175, 5};
	std::vector<int> startColor = {0,0,0};
	std::vector<int> targetColor;
	std::vector<int> endColor = {255, 255, 255};
	int timeConstant;

	void updateColor(Color color, int step);
	void updateStrip();
	void getTimeOfDay();
};
