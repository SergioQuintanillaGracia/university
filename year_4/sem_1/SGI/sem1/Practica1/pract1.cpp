#include <iostream>
#include <gl\freeglut.h>
#include <codebase.h>

using namespace cb;

void display()
{
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	gluLookAt(0, 0, 0, -1, -1, -1, 0, 1, 0);

	ejes();

	glColor3f(1, 0, 0);
	glutSolidTeapot(0.49);

	glColor3fv(BLANCO);
	glutWireTeapot(0.5);

	glFlush();
}
void reshape(GLint w, GLint h)
{
}
void main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 400);
	glutInitWindowPosition(50, 600 - 400);

	// Crear ventana
	glutCreateWindow("Primer programa en OpenGL");
	std::cout << "Practica 1 running" << std::endl;

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}