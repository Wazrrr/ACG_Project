#ifndef _REACT_H
#define _REACT_H

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"


const float STEP_PHI=0.001f;
const float STEP_THETA=0.001f;
const float MAX_THETA=1.5f;
const float MIN_THETA=-1.5f;

const float V_SHOOT=1.0f;


void keybd(unsigned char key,int x,int y);
void mouse(int button,int state,int x,int y);
void mousePass(int x,int y);


#endif

