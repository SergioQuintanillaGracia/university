#include <iostream>
#include <gl\freeglut.h>
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}
void reshape(GLint w, GLint h)
{
}
void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(200, 200);
	glutCreateWindow("Hola Mundo");
	std::cout << "Hola Mundo running" << std::endl;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
}