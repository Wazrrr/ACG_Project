#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <vector>

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"


enum ParticleType {
	PLANET,PLAYER,BLOCK,MONSTER,NUM_PARTICLE_TYPES
};
const float R1[NUM_PARTICLE_TYPES]={0.8f,0.32f,0.05f,0.08f};
const float R2[NUM_PARTICLE_TYPES]={0.9f,0.36f,0.05f,0.09f};
const float R[NUM_PARTICLE_TYPES]={1.0f,0.4f,0.12f,0.1f};                           // Radius
const float M[NUM_PARTICLE_TYPES]={1000000.0f,1.0f,1.0f,1.0f};                     // Mass
const float H[NUM_PARTICLE_TYPES]={1000000.0f,100.0f,1.0f,1.0f};                   // Health
const float HRR[NUM_PARTICLE_TYPES]={0.0f,1.0f,1.0f,1.0f};                         // Health Regenerating Rate
const float G[NUM_PARTICLE_TYPES][NUM_PARTICLE_TYPES]={                            // Gravity
	{0.0f,1.0f,1.0f,1.0f},
	{1.0f,0.0f,0.0f,0.1f},
	{1.0f,0.0f,0.0f,0.0f},
	{1.0f,0.1f,0.0f,0.0f}
}; 
const float K[NUM_PARTICLE_TYPES][NUM_PARTICLE_TYPES]={                            // Coefficient of Boundary
	{0.0f,500.0f,500.0f,500.0f},
	{500.0f,200.0f,500.0f,200.0f},
	{500.0f,500.0f,500.0f,100.0f},
	{500.0f,200.0f,100.0f,100.0f}
};
const float GAMMA_N[NUM_PARTICLE_TYPES][NUM_PARTICLE_TYPES]={                      // Resistance Coefficient in n direction
	{20.0f,20.0f,20.0f,20.0f},
	{20.0f,0.0f,20.0f,0.0f},
	{20.0f,20.0f,20.0f,0.0f},
	{20.0f,0.0f,0.0f,0.0f}
};
const float GAMMA_TAU[NUM_PARTICLE_TYPES][NUM_PARTICLE_TYPES]={                    // Resistance Coefficient in tau direction
	{2.0f,2.0f,2.0f,2.0f},
	{2.0f,0.0f,2.0f,0.0f},
	{2.0f,2.0f,0.0f,0.0f},
	{2.0f,0.0f,0.0f,0.0f}
};

struct Particle {
	glm::vec3 p;
	glm::vec3 v;
	glm::vec3 F;
	ParticleType type;
	
	Particle(): p(0.0f),v(0.0f),F(0.0f),type(BLOCK) {
	}
	Particle(glm::vec3 _p,glm::vec3 _v,ParticleType _type): p(_p),v(_v),F(0.0f),type(_type) {
	}
};


const float F_FRONT=1.0f;
const float F_BACK=1.0f;
const float F_LEFT=1.0f;
const float F_RIGHT=1.0f;
const float V_JUMP=0.5f;
const float V_DIVE=0.5f;

const float V_FRONT=0.5f;
const float V_BACK=0.5f;
const float V_LEFT=0.5f;
const float V_RIGHT=0.5f;
const float V_UP=0.5f;
const float V_DOWN=0.5f;


enum HandMode {
	SHOOT,SET,EMBED,NUM_HAND_MODE
};

struct Player {
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 up;
//	glm::vec3 left;
	bool is_rotating;
	bool is_flying;
	bool is_inside;
	HandMode hand_mode;
	bool is_fine_tuning;
	bool is_pausing;
	float dt;
}; 


void initParticles();
void initPlayer();
void evolution(float dt);


#endif

