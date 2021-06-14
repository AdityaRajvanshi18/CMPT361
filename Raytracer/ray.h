/*	ray.h	*/
#ifndef RAY_H
#define RAY_H

#include "include/Angel.h"

class Ray{

private:
	vec3 p0;
	vec3 v;


public:
	Ray(vec3 ori, vec3 dir) : p0(ori), v(dir) {}

	vec3 getOrigin() const{
		return p0;
	}
	
	vec3 getDir() const{
		return v;
	}

	vec3 getPoint(double t) const{
		return p0 + v * t;
	}
	
	vec3 reflectBy(const vec3 normal) const{ //may need ampersand
		vec3 dotProd;
		dotProd = dot(normal,v);
		return v - normal * dotProd *2;
	}
	//void setVx(vec3 v, int vx);
	//void setVy(vec3 v, int vy);
	//void setp0(vec3 val);
};

	
#endif
	

