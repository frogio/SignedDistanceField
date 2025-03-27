#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include "glut.h"
#include "ADFTree.h"

using namespace std;

void display();
void reshape(int w, int h);
void Init();
void keyboard(unsigned char key, int x, int y);

ADFTree* tree;

int main()
{
	Init();

	glutInitWindowSize(700, 700);			// â ũ��
	glutInitWindowPosition(100, 100);		// â ù ��ġ
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("ADF Tree");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

void keyboard(unsigned char key, int x, int y) {

}

void display() {
	glClearColor(0.f, 0.f, 0.f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	tree->RenderTree();

	glFlush();
}

void reshape(int w, int h) {
	glLoadIdentity();						// �������
	glViewport(0, 0, w, h);	// �׷��� ����ġ���� ������
	gluOrtho2D(0, 1, 0, 1);
}

void Init() {

	tree = new ADFTree(17);
	tree->buildTree(Vector2D(0, 0), Vector2D(1, 1));
}