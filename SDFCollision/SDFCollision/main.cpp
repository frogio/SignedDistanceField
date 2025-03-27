#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "glut.h"
#include "SDFTree.h"


#include "Lope.h"

#define PI		3.141592

void glReshape(int w, int h);
void glDisplay();
void Update();
void mouseMotion(int x, int y);
void Mouse(int button, int state, int x, int y);
void keyboard(unsigned char key, int x, int y);

Vector2D gravity = Vector2D(0, -0.003f);
Lope lope(0.9f, 100, 0.005, true, gravity);
SDFTree* tree;
Vector2D gradient;

void Init();

void main() {

	Init();

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(700, 700);
	glutCreateWindow("Lope 2D");
	glutReshapeFunc(glReshape);
	glutDisplayFunc(glDisplay);
	glutIdleFunc(Update);
	glutMouseFunc(Mouse);
	glutMotionFunc(mouseMotion);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
}

void Init() {

	tree = new SDFTree(15);
	tree->buildTree(Vector2D(0.f, 0.f), Vector2D(0.6f, 0.6f));
	tree->SetLope(&lope);

}

void glReshape(int w, int h) {
	glLoadIdentity();						// 단위행렬
	glViewport(0, 0, w, h);					// 뷰포트 지정 (윈도우에 그릴 영역)
	gluOrtho2D(0.f, 1.f, 0.f, 1.f);			// 좌표계 재지정, X : 0 ~ 100, Y : 0 ~ 100
}

void glDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);

	lope.Draw();
	//tree->RenderTree();

	glColor3f(1.f, 1.f, 1.f);

	float cx = 0.3f, cy = 0.3f;
	float radius = 0.2f;

	glBegin(GL_LINES);

	for (int i = 0; i < 360; i++) {
		float radian = 2 * PI * i / 360.f;
		float x = radius * cos(radian), 
				y = radius * sin(radian);
		
		glVertex2f(x + cx, y + cy);
	}
	
	glEnd();

	/* For Gradient Debug
	glBegin(GL_LINES);
	
	glVertex2f(0.4f, 0.4f);
	glVertex2f(gradient.x * 2, gradient.y * 2);

	glEnd();
	*/
	
	glFlush();

}

void keyboard(unsigned char key, int x, int y) {


}

int lastx, lasty;
Vector2D cursorVector;
Particle * selectedParticle = NULL;

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		
		float cvt_x, cvt_y;

		cvt_x = (x - 700 / 2) / (float)700 * 2 * 100,
		cvt_y = -((y - 700 / 2) / (float)700 * 2) * 100;

		selectedParticle = lope.GetParticle(cvt_x, cvt_y);

	}
}


void mouseMotion(int x, int y) {

		float cvt_x, cvt_y;
		cvt_x = (x - 700 / 2) / (float)700 * 2 * 100,
		cvt_y = -((y - 700 / 2) / (float)700 * 2) * 100;
		//cursorVector = Vector2D(cvt_x - cvt_lastx, cvt_y - cvt_lasty);
		
		if(selectedParticle != NULL){
			selectedParticle->SetPosition(Vector2D(cvt_x, cvt_y));
		}

}

int offset = 1;
float headX = 90.f, headY = 90.f;

void Update() {

	tree->CollisionDetect();
	lope.Update();

	glutPostRedisplay();
}