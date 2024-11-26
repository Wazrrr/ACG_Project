#include <windows.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"

#include "physics.h"
#include "display.h"

extern std::vector <Particle> particle;
extern Player player;
Camera camera;


void updateView() {
	glm::vec3 center=player.pos+camera.getDir(player);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(player.pos.x,player.pos.y,player.pos.z,center.x,center.y,center.z,player.up.x,player.up.y,player.up.z);
	glRotatef(camera.theta,1.0f,0.0f,0.0f);
}

void drawParticles() {
	for(std::vector<Particle>::iterator it=particle.begin();it<particle.end();it++) {
		if(it->type!=PLAYER) {
			glPushMatrix();
			glTranslatef(it->p.x,it->p.y,it->p.z);
			if(it->type==PLANAR)
				glutWireSphere(R[it->type],PARTICLE_SLICE_NUM,PARTICLE_STACK_NUM);
			else
				glutSolidSphere(R[it->type],PARTICLE_SLICE_NUM,PARTICLE_STACK_NUM);
			glPopMatrix();
		}
	}
}

void display() {
	updateView();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawParticles();
	glutSwapBuffers();
}

void reshape(int width,int height) {
	camera.window_mid_x=width/2;
	camera.window_mid_y=height/2;
	
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY,float(width)/float(height),Z_NEAR,Z_FAR);
	glutPostRedisplay();
}                    

