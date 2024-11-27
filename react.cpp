#include <windows.h>
#include <vector>

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "./glm/glm/glm.hpp"

#include "physics.h"
#include "display.h"
#include "react.h"


extern std::vector <Particle> particle;
extern Player player;
extern Camera camera;


struct IndexPoint {
	int i;
	glm::vec3 p;
	IndexPoint(): i(0),p(0.0f) {
	}
	IndexPoint(int _i,glm::vec3 _p): i(_i),p(_p) {
	}
};

inline IndexPoint getIntersection(glm::vec3 pos,glm::vec3 dir,float delta_r,bool is_embed) { // dir should be normalized; return -1 of no intersection
	int i_ans=-1;
	glm::vec3 p_ans(0.0f);
	float t_min=1e30;
	for(std::vector<Particle>::iterator it=particle.begin();it<particle.end();it++)
		if(it->type!=PLAYER) {
			float t_this=0.0f;
			float r=(is_embed&&it->type==BLOCK?0.5f*(R1[it->type]+R2[it->type]):R[it->type])+delta_r;
			glm::vec3 origin=it->p;
			
			float b_div_2=glm::dot(pos-origin,dir);
			float delta_div_4=b_div_2*b_div_2-glm::dot(pos-origin,pos-origin)+r*r;
			if(delta_div_4<0.0f)
				continue;
			float sqrt_delta_div_4=glm::sqrt(delta_div_4);
			float t_p=-b_div_2+sqrt_delta_div_4;
			float t_m=-b_div_2-sqrt_delta_div_4;
			if(t_p<0.0f)
				continue;
			if(t_m<0.0f)
				t_this=t_p;
			else
				t_this=t_m;
			
			if(t_this<t_min) {
				i_ans=it-particle.begin();
				p_ans=pos+t_this*dir;
				t_min=t_this;
			}
		}
	return IndexPoint(i_ans,p_ans);
}


void keybd(unsigned char key,int x,int y) {
	if(key=='q') {
		player.is_rotating^=1;
		if(player.is_rotating)
			glutWarpPointer(camera.window_mid_x,camera.window_mid_y);
	}
	if(key=='e')
		player.is_flying^=1;
	if(key=='f')
		player.is_fine_tuning=true;
	if(key=='r')
		player.is_fine_tuning=false;
	if(key=='1')
		player.hand_mode=SHOOT;
	if(key=='2')
		player.hand_mode=SET;
	if(key=='3')
		player.hand_mode=EMBED;
	if(key=='z')
		player.is_pausing=true;
	if(key=='x')
		player.is_pausing=false;
	if(key=='c')
		player.dt+=0.001f;
	if(key=='v'&&player.dt>0.001f)
		player.dt-=0.001f;
}
void mouse(int button,int state,int x,int y) {
	glm::vec3 dir=camera.getDir(player);
	
	if(state==GLUT_DOWN)
		return;
	if(button==GLUT_LEFT_BUTTON) {
		IndexPoint ip=getIntersection(player.pos,dir,0.0f,false);
		if(ip.i==-1)
			return;
		if(particle[ip.i].type==BLOCK)
			particle.erase(particle.begin()+ip.i);
		return;
	}
	if(player.hand_mode==SHOOT) {
		particle.push_back(Particle(player.pos+(R[PLAYER]+R[BLOCK])*dir,V_SHOOT*dir,BLOCK));
		return;
	}
	if(player.hand_mode==SET||player.hand_mode==EMBED) {
		float delta_r;
		IndexPoint ip;
		delta_r=(player.hand_mode==EMBED?0.5f*(R1[BLOCK]+R2[BLOCK]):R[BLOCK]);
		ip=getIntersection(player.pos,dir,delta_r,player.hand_mode==EMBED);
		if(ip.i==-1||particle[ip.i].type==PLANET) {
			delta_r=R[BLOCK];
			ip=getIntersection(player.pos,dir,delta_r,false);
		}
		if(ip.i==-1)
			return;
		glm::vec3 p_center=particle[ip.i].p;
		particle.push_back(Particle(ip.p,glm::vec3(0.0f),BLOCK));
		return;
	}
}
void mousePass(int x,int y) {
	if(player.is_rotating) {
		float delta_phi=float(x-camera.window_mid_x)*STEP_PHI;
		float delta_theta=-float(y-camera.window_mid_y)*STEP_THETA;
		glutWarpPointer(camera.window_mid_x,camera.window_mid_y);
		player.front+=delta_phi*glm::cross(player.front,player.up);
		if(delta_theta>=0.0f&&camera.theta<MAX_THETA||delta_theta<=0.0f&&camera.theta>MIN_THETA)
			camera.theta+=delta_theta;
	}
}                   
