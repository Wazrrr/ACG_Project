#include <windows.h>
#include <time.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"

#include "physics.h"
#include "display.h"
#include "react.h"


const int WINDOW_WIDTH=800;
const int WINDOW_HEIGHT=800;
const char* WINDOW_TITLE="ACG_project";

//float material_specular[]={1.0f,1.0f,1.0f,1.0f};
//float material_shininess[]={50.0f}; 
float light_position[]={1.0f,1.0f,1.0f,0.0f};
float light_color[]={1.0f,1.0f,1.0f,1.0f};
float ambient_color[]={0.0f,1.0f,0.0f,1.0f};

extern Player player;

void start() {
	srand((unsigned)time(NULL));
	initParticles();
	initPlayer();
}
void loop() {
	evolution(player.dt);
	glutPostRedisplay();
}


int main(int argc,char* argv[]) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(WINDOW_WIDTH,WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);
	
//    glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
//    glMaterialfv(GL_FRONT,GL_SHININESS,material_shininess);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_color);
//    glLightfv(GL_LIGHT0,GL_SPECULAR,light_color);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient_color);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glutDisplayFunc(&display);
	glutIdleFunc(&loop);
	glutKeyboardFunc(&keybd);
	glutMouseFunc(&mouse);
	glutPassiveMotionFunc(&mousePass);
	glutReshapeFunc(&reshape);
	start();
	glutMainLoop();
	return 0;
}                                      

