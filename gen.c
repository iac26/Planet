
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <math.h>
#include <time.h>

#define N_R 10

void affichage(void);
void keyboard(char key, int state, int x, int y);
void reshape(int w, int h);
void quit(void);
float noise(float x);
void noise_init(void);
void color_init(void);

float atmR, atmG, atmB;
float oceR, oceG, oceB;
float lndR, lndG, lndB;

void planet(void);
void island(float scaleX, float scaleY, float x, float y, float r, float g, float b);
void atmosphere(void);

static float random_amps[N_R];
static float random_shifts[N_R];


int main(int argc, char **argv) {
	srand(time(NULL));
	
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL | GLUT_ALPHA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(500,500);
	glutCreateWindow("Planet");
	
	
	
	glutKeyboardFunc(keyboard); 
	glutDisplayFunc(affichage);
	glutReshapeFunc(reshape);
	
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glutMainLoop();
}

void affichage(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 10, 0, 10, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	color_init();
	
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  


	glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT);
	
	planet();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);
	glStencilMask(0x00);
	
	
	
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	planet();
    int m = rand() % 10 + 2;
    for(int i = 0; i < m; i++) {
		island(0.5, 0.5, 3 + (float)rand()/RAND_MAX*4, 3 + (float)rand()/RAND_MAX*4, lndR, lndG, lndB);
	}
	
	island(0.4, 0.4, 3 + (float)rand()/RAND_MAX*4, 3 + (float)rand()/RAND_MAX*4, lndR, lndG, lndB);
	island(0.3, 0.3, 3 + (float)rand()/RAND_MAX*4, 3 + (float)rand()/RAND_MAX*4, lndR, lndG, lndB);
	island(0.2, 0.2, 3 + (float)rand()/RAND_MAX*4, 3 + (float)rand()/RAND_MAX*4, lndR, lndG, lndB);
	if(rand()%3 < 2) {
		island(1.2, 0.5, 5, 8.3, 1.0, 1.0, 1.0);
		island(1.2, 0.5, 5, 1.7, 1.0, 1.0, 1.0);
	}
	
	glStencilFunc(GL_EQUAL, 0, 0xFF);
	
	int ne = rand() % 1000 + 600;
	glBegin(GL_POINTS);
	glColor3f(1.0 - (float)rand()/(8*RAND_MAX), 1.0 - (float)rand()/(8*RAND_MAX), 1.0 - (float)rand()/(8*RAND_MAX));
	for(int i = 0; i < ne; i++) {
		glVertex2f(10*(float)rand()/RAND_MAX, 10*(float)rand()/RAND_MAX);
	}
	glEnd();
	
	glDisable(GL_STENCIL_TEST);
	atmosphere();
	//glutPostRedisplay();
	glutSwapBuffers();
}

void planet(void) {
	glPushMatrix();
	float rad = 3.0f;
	
	glTranslatef(5.0, 5.0, 0.0);

	glBegin(GL_POLYGON);
	glColor3f(oceR, oceG, oceB);
	for(float x = 0; x < 2*M_PI; x += M_PI/24.) {
		glVertex3f(rad*sin(x), rad*cos(x), 0.0);
	}
	glEnd();
	glPopMatrix();
}

void atmosphere(void) {
	glPushMatrix();
	float rad = 3.2f;
	
	glTranslatef(5.0, 5.0, 0.0);

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(atmR, atmG, atmB, 0.3);
	glVertex3f(0.0, 0.0, 0.0);
	glColor4f(1.1*atmR, 1.1*atmG, 1.1*atmB, 0.6);
	for(float x = 0; x <= 2*M_PI+0.01; x += M_PI/24.) {
		glVertex3f(rad*sin(x), rad*cos(x), 0.0);
	}
	glEnd();
	glPopMatrix();
}

void island(float scaleX, float scaleY, float x, float y, float r, float g, float b) {
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glScalef(scaleX, scaleY, 0.0);
	
	noise_init();
	

	glBegin(GL_POLYGON);
	glColor4f(r, g, b, 1.0);
	
	for(float x = 0; x < 10; x += 0.10) {
		float a = (2*(sin(x/1.5) + noise(x + 20)));
		float b = (2*(cos(x/1.5) + noise(x)));
		float dist = 1.0;//((5-a)*(5-a) + (5-b)*(5-b))/30;
		glVertex3f(a/dist, b/dist, -0.2);
	}
	glEnd();
	//noise_init();
	//glColor3f(0.0, 0.5, 0.0);
	//glBegin(GL_POLYGON);
	//for(float x = 0; x < 10; x += 0.1) {
		//glVertex3f((sin(x/1.5) + noise(x + 20)), (cos(x/1.5) + noise(x)), -0.5);
	//}
	//glEnd();
	glPopMatrix();
}

float noise(float x) {
	float out = 0;
	x = x*5;
	for(int i = 0; i < N_R; i++) {
		out += random_amps[i] * (sin(x + random_shifts[i])/(x + random_shifts[i]));
	}
	return out;
}

void noise_init(void) {
	for(int i = 0; i < N_R; i++) {
		random_amps[i] = 0.5 * (float)rand()/RAND_MAX;
		random_shifts[i] = (-100 * (float)rand()/RAND_MAX);
	}
}

void color_init(void) {
	
	int tp_a = rand() % 5;
	int tp_l = rand() % 5;
	int tp_o = rand() % 5;
	
	switch(tp_a) {
		case 0:
		atmR = 0.2;
		atmG = 0.2;
		atmB = 0.8;
		break;
		case 1:
		atmR = 0.8;
		atmG = 0.2;
		atmB = 0.8;
		break;
		case 2:
		atmR = 0.3;
		atmG = 0.8;
		atmB = 0.4;
		break;
		case 3:
		atmR = 0.7;
		atmG = 0.8;
		atmB = 1.0;
		break;
		case 4:
		atmR = 0.7;
		atmG = 0.4;
		atmB = 1.0;
		break;
	}
	
	switch(tp_o) {
		case 0:
		oceR = 0.0;
		oceG = 0.0;
		oceB = 0.5;
		break;
		case 1:
		oceR = 0.3;
		oceG = 0.0;
		oceB = 0.5;
		break;
		case 2:
		oceR = 0.1;
		oceG = 0.7;
		oceB = 0.2;
		break;
		case 3:
		oceR = 0.5;
		oceG = 0.7;
		oceB = 0.2;
		break;
		case 4:
		oceR = 0.1;
		oceG = 0.7;
		oceB = 1.0;
		break;
	}
	
	switch(tp_l) {
		case 0:
		lndR = 1.0;
		lndG = 0.8;
		lndB = 0.5;
		break;
		case 1:
		lndR = 1.0;
		lndG = 0.5;
		lndB = 0.2;
		break;
		case 2:
		lndR = 0.3;
		lndG = 0.6;
		lndB = 0.1;
		break;
		case 3:
		lndR = 0.7;
		lndG = 0.7;
		lndB = 0.0;
		break;
		case 4:
		lndR = 0.8;
		lndG = 0.4;
		lndB = 0.2;
		break;
	}
}

void reshape(int w, int h) {
	glutReshapeWindow(500, 500);
}

void keyboard(char key, int state, int x, int y) {
	//printf("%d\n", key);
	if(key == 27)
		quit();
	if(key == ' ')
		glutPostRedisplay();
}


void quit(void) {
	glutDestroyWindow(1);
	exit(0);
}





