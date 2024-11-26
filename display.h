#ifndef _DISPLAY_H
#define _DISPLAY_H

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"

#include "physics.h"


const int PARTICLE_SLICE_NUM=20;
const int PARTICLE_STACK_NUM=16;

const float FOVY=75.0f;
const float Z_NEAR=0.05f;
const float Z_FAR=10.0f;


struct Camera {
	float theta;
	int window_mid_x;
	int window_mid_y;
	Camera(): theta(0.0f),window_mid_x(0),window_mid_y(0) {
	}
	
	glm::vec3 getDir(const Player &player) const {
		return glm::cos(theta)*player.front+glm::sin(theta)*player.up;
	}
};

void display();
void reshape(int width,int height);


#endif

