#include <vector>
#include <windows.h>

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"

#include "physics.h"


std::vector <Particle> particle;
Player player;


inline bool KEY(int key_code) {
	return (GetKeyState(key_code)&0x8000)?1:0;
}


void initParticles() {
	particle.push_back(Particle(glm::vec3(0.0f),glm::vec3(0.0f),PLANET));
	particle.push_back(Particle(glm::vec3((R[PLANET]+R[PLAYER])),glm::vec3(0.0f),PLAYER));
}


void initPlayer() {
	player.pos=particle[1].p;
	player.up=glm::normalize(particle[1].p-particle[0].p);
	player.front=glm::normalize(glm::vec3(0.0f,player.up.z,-player.up.y));
	player.hand_mode=SHOOT;
	player.is_fine_tuning=false;
	player.is_pausing=false;
	player.dt=0.001f;
}

void updatePlayer() {
	player.pos=particle[1].p;
	player.up=glm::normalize(particle[1].p-particle[0].p);
	player.front=glm::normalize(player.front-glm::dot(player.up,player.front)*player.up);
}


inline float fN(float d,float r1,float r2,float r,float k) {
	float rm=0.5f*(r2+r);
	if(d<r1)
		return k*(r1-d);
	if(d>r2&&d<rm)
		return -k*(d-r2);
	if(d>=rm&&d<r)
		return 5.0f*k*(r-d);
	return 0.0f;
}

inline float fNFineTuning(float d,float r1,float r2,float r,float k) {
	float ro=2.0f*r-r2;
	if(d<r1)
		return k*(r1-d);
	if(d>r2&&d<r)
		return -k*(d-r2);
	if(d>=r&&d<ro)
		return -k*(r-r2);
	return 0.0f;
}

inline glm::vec3 fF(glm::vec3 vr,glm::vec3 n,float gamma_n,float gamma_tau) {
	glm::vec3 vr_n=glm::dot(vr,n)*n;
	glm::vec3 vr_tau=vr-vr_n;
	return gamma_n*vr_n+gamma_tau*vr_tau;
}

void calculateF() {
	for(std::vector<Particle>::iterator it=particle.begin();it<particle.end();it++)
		it->F=glm::vec3(0.0f);
	player.is_inside=false;
	for(std::vector<Particle>::iterator it=particle.begin();it<particle.end();it++)
		for(std::vector<Particle>::iterator jt=it+1;jt<particle.end();jt++) {
			glm::vec3 pr=(jt->p)-(it->p);
			float d=glm::length(pr);
			glm::vec3 F=G[it->type][jt->type]*pr/(d*d*d);
			
			float r1=R1[it->type]+R1[jt->type];
			float r2=R2[it->type]+R2[jt->type];
			float r=R[it->type]+R[jt->type];
			if(player.is_fine_tuning&&it->type==BLOCK&&jt->type==BLOCK) {
				if(d<r1||d>r2&&d<2.0f*r-r2) {
					glm::vec3 n=glm::normalize(pr);
					F+=(-fNFineTuning(d,r1,r2,r,K[it->type][jt->type])*n
						+fF((jt->v)-(it->v),n,GAMMA_N[it->type][jt->type],GAMMA_TAU[it->type][jt->type]));
				}
			} else {
				if(d<r1||d>r2&&d<r) {
					glm::vec3 n=glm::normalize(pr);
					F+=(-fN(d,r1,r2,r,K[it->type][jt->type])*n
						+fF((jt->v)-(it->v),n,GAMMA_N[it->type][jt->type],GAMMA_TAU[it->type][jt->type]));
				}
			}
			if((it->type==PLAYER||jt->type==PLAYER)&&d<r)
				player.is_inside=true;
			
			it->F+=F;
			jt->F-=F;
		}
} 

void calculatePlayer() {
	if(player.is_flying) {
		particle[1].v=player.front*(V_FRONT*KEY('W')-V_BACK*KEY('S'))
			+glm::cross(player.up,player.front)*(V_LEFT*KEY('A')-V_RIGHT*KEY('D'))
			+player.up*(V_UP*KEY(32)-V_DOWN*KEY(16));
	}
	if(player.is_inside&&!player.is_flying) {
		if(KEY('W'))
			particle[1].F+=F_FRONT*player.front;
		if(KEY('S'))
			particle[1].F-=F_BACK*player.front;
		if(KEY('A'))
			particle[1].F+=F_LEFT*glm::cross(player.up,player.front);
		if(KEY('D'))
			particle[1].F-=F_RIGHT*glm::cross(player.up,player.front);
		if(KEY(32)&&glm::dot(particle[1].v,player.up)<0.5f*V_JUMP)
			particle[1].v+=V_JUMP*player.up;
		if(KEY(16)&&glm::dot(particle[1].v,-player.up)<0.5f*V_DIVE)
			particle[1].v+=-V_DIVE*player.up;
	}
}

void updateState(float dt) {
	for(std::vector<Particle>::iterator it=particle.begin();it<particle.end();it++) {
		if(player.is_pausing&&it->type!=PLAYER)
			continue;
		it->p+=(it->v)*dt;
		it->v+=(it->F)*dt/M[it->type];
	}
}

void evolution(float dt) {
	updatePlayer();
	calculateF();
	calculatePlayer();
	updateState(dt);
}                 

