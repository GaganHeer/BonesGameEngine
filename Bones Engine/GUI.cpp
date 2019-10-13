#include "GUI.h"

GUI::GUI(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(DEFULT_X, DEFULT_Y);
	glutInitWindowSize(DEFULT_WIDTH, DEFULT_HEIGHT);
}

GUI::~GUI()
{

}

void GUI::init() const
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

void GUI::displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();

	glutSwapBuffers();
}

void GUI::createWindow() const
{
	glutCreateWindow("a test window");

	init();
	// Register a callback function for the window's repainting event
	glutDisplayFunc(displayFunction);
	// enter GLUT event processing cycle
	glutMainLoop();
}