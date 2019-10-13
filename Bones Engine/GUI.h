#pragma once

#include <GL/freeglut.h>

class GUI
{
public:
	const int DEFULT_X = 0;
	const int DEFULT_Y = 0;

	const int DEFULT_WIDTH = 800;
	const int DEFULT_HEIGHT = 400;

	GUI(int argc, char** argv);
	~GUI();

	void createWindow() const;

private:
	static void displayFunction();
	void init() const;
};


