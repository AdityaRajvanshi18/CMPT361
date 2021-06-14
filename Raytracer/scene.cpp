#include "sphere.h"
#include <stdio.h>

extern Point light1;
extern float light1_ambient[3];
extern float light1_diffuse[3];
extern float light1_specular[3];

extern float global_ambient[3];
extern Spheres *scene;

extern RGB_float background_clr;
extern float decay_a;
extern float decay_b;
extern float decay_c;

void scene1() {
	  //set background color
	  background_clr.r = 0.5;
	  background_clr.g = 0.05;
	  background_clr.b = 0.8;

	  //global ambience
	  global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

	  //light 1
	  light1.x = -2.0;
	  light1.y = 5.0;
	  light1.z = 1.0;
	  light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.1;
	  light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
	  light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;

	  decay_a = 0.5;
	  decay_b = 0.3;
	  decay_c = 0.0;

	   // sphere 1
	  Point sphere1_ctr = {1.5, -0.5, -3.0};
	  float sphere1_rad = 1.3;
	  float sphere1_ambient[] = {0.7, 0.7, 0.7};
	  float sphere1_diffuse[] = {1.0, 1.0, 0.2};
	  float sphere1_specular[] = {1.0, 1.0, 1.0};
	  float sphere1_shineness = 10;
	  float sphere1_reflectance = 0.1;
	  float sphere1_refraction_index = 1.5;
	  float sphere1_transparency = 1;
	  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
		     sphere1_diffuse, sphere1_specular, sphere1_shineness,
			     sphere1_reflectance, sphere1_refraction_index, sphere1_transparency, 1);

	  // sphere 2
	  Point sphere2_ctr = {-1.5, -0.5, -3.0};
	  float sphere2_rad = 1.3;
	  float sphere2_ambient[] = {0.6, 0.6, 0.6};
	  float sphere2_diffuse[] = {0.2, 0.2, 1.0};
	  float sphere2_specular[] = {1.0, 1.0, 1.0};
	  float sphere2_shineness = 100;
	  float sphere2_reflectance = 0.8;
	  float sphere2_refraction_index = 1.0;
	  float sphere2_transparency = 1;
	  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
		     sphere2_diffuse, sphere2_specular, sphere2_shineness,
			     sphere2_reflectance, sphere2_refraction_index, sphere2_transparency, 2);

	  // sphere 3
	  Point sphere3_ctr = {0.0, 2, -4.25};
	  float sphere3_rad = 1.0;
	  float sphere3_ambient[] = {0.2, 0.2, 0.2};
	  float sphere3_diffuse[] = {0.0, 1.0, 0.25};
	  float sphere3_specular[] = {0.0, 1.0, 0.0};
	  float sphere3_shineness = 10;
	  float sphere3_reflectance = 0.3;
	  float sphere3_refraction_index = 1.8;
	  float sphere3_transparency = 0.3;
	  scene = add_sphere(scene, sphere3_ctr, sphere3_rad, sphere3_ambient,
		     sphere3_diffuse, sphere3_specular, sphere3_shineness,
			     sphere3_reflectance, sphere3_refraction_index, sphere3_transparency, 3);
}

void scene2() {
	  background_clr.r = 0.03;
	  background_clr.g = 0.25;
	  background_clr.b = 0.12;

	  //global ambience
	  global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

	  //light 1
	  light1.x = 0.0;
	  light1.y = 5.0;
	  light1.z = 1.0;
	  light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.1;
	  light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
	  light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;
	  decay_a = 0.5;
	  decay_b = 0.3;
	  decay_c = 0.0;

	  // sphere 1
	  Point sphere1_ctr = {-0.5, 1.3, -2};
	  float sphere1_rad = 1;
	  float sphere1_ambient[] = {0.7, 0.7, 0.7};
	  float sphere1_diffuse[] = {0.5, 0.5, 0};
	  float sphere1_specular[] = {1.0, 1.0, 1.0};
	  float sphere1_shineness = 10;
	  float sphere1_reflectance = 1;
	  float sphere1_refraction_index = 1.5;
	  float sphere1_transparency = 1;
	  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
		     sphere1_diffuse, sphere1_specular, sphere1_shineness,
			     sphere1_reflectance, sphere1_refraction_index, sphere1_transparency, 1);

	  // sphere 2
	  Point sphere2_ctr = {0.5, -1.5, -3.5};
	  float sphere2_rad = 1.2;
	  float sphere2_ambient[] = {0.6, 0.6, 0.6};
	  float sphere2_diffuse[] = {0.2, 0.2, 1.0};
	  float sphere2_specular[] = {1.0, 1.0, 1.0};
	  float sphere2_shineness = 50;
	  float sphere2_reflectance = 0.8;
	  float sphere2_refraction_index = 1.0;
	  float sphere2_transparency = 1;
	  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
		     sphere2_diffuse, sphere2_specular, sphere2_shineness,
			     sphere2_reflectance, sphere2_refraction_index, sphere2_transparency, 2);
}

void scene3() {

	  // set background color
	  background_clr.r = 0.05;
	  background_clr.g = 0.05;
	  background_clr.b = 0.12;

	  //global ambience
	  global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.1;

	  //light 1
	  light1.x = -1.0;
	  light1.y = 2.0;
	  light1.z = 1.0;
	  light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.9;
	  light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
	  light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;
	  decay_a = 0.8;
	  decay_b = 0.3;
	  decay_c = 0.0;

	  //sphere 1 //back
	  Point sphere1_ctr = {-0.7, 1.5, -4};
	  float sphere1_rad = 1;
	  float sphere1_ambient[] = {0.7, 0.7, 0.7};
	  float sphere1_diffuse[] = {1.0, 0.4, 0.1};
	  float sphere1_specular[] = {1.0, 1.0, 1.0};
	  float sphere1_shineness = 30;
	  float sphere1_reflectance = 0.5;
	  float sphere1_refraction_index = 1.0;
	  float sphere1_transparency = 1;
	  scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
		     sphere1_diffuse, sphere1_specular, sphere1_shineness,
			     sphere1_reflectance, sphere1_refraction_index, sphere1_transparency, 1);

	  //sphere 2 //front
	  Point sphere2_ctr = {-1.1, 0.5, -2.5};
	  float sphere2_rad = 1.0;
	  float sphere2_ambient[] = {0.6, 0.6, 0.6};
	  float sphere2_diffuse[] = {1.0, 1.0, 1.0};
	  float sphere2_specular[] = {1.0, 1.0, 1.0};
	  float sphere2_shineness = 50;
	  float sphere2_reflectance = 0.5;
	  float sphere2_refraction_index = 1.5;
	  float sphere2_transparency = 1;
	  scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
		     sphere2_diffuse, sphere2_specular, sphere2_shineness,
			     sphere2_reflectance, sphere2_refraction_index, sphere2_transparency, 2);

}

void scene4() {
	//set background color
	background_clr.r = 0.03;
  	background_clr.g = 0.25;
  	background_clr.b = 0.12;

  	// set up global ambient term
  	global_ambient[0] = global_ambient[1] = global_ambient[2] = 0.2;

  	// setup light 1
  	light1.x = -1.0;
  	light1.y = 2.0;
  	light1.z = 1.0;
  	light1_ambient[0] = light1_ambient[1] = light1_ambient[2] = 0.1;
  	light1_diffuse[0] = light1_diffuse[1] = light1_diffuse[2] = 1.0;
  	light1_specular[0] = light1_specular[1] = light1_specular[2] = 1.0;

	// set up decay parameters
	decay_a = 0.5;
	decay_b = 0.3;
	decay_c = 0.0;

  	// sphere 1 -- blue sphere (middle sphere)
	Point sphere1_ctr = {2, -0.2, -3.2};
	float sphere1_rad = 1.23;
	float sphere1_ambient[] = {0.7, 0.7, 0.7};
	float sphere1_diffuse[] = {0.1, 0.5, 0.8};
	float sphere1_specular[] = {1.0, 1.0, 1.0};
	float sphere1_shineness = 100;
	float sphere1_reflectance = 0.75;
	float sphere1_refraction_index = 1.8;
	float sphere1_transparency = 1;
	scene = add_sphere(scene, sphere1_ctr, sphere1_rad, sphere1_ambient,
		sphere1_diffuse, sphere1_specular, sphere1_shineness,
		sphere1_reflectance, sphere1_refraction_index, sphere1_transparency, 1);

	// sphere 2 -- red sphere (closest sphere)
	Point sphere2_ctr = {-2.5, 1.0, -2.5};
	float sphere2_rad = 0.95;
	float sphere2_ambient[] = {0.6, 0.6, 0.6};
	float sphere2_diffuse[] = {1.0, 0.0, 0.25};
	float sphere2_specular[] = {1.0, 1.0, 1.0};
	float sphere2_shineness = 6;
	float sphere2_reflectance = 0.5;
	float sphere2_refraction_index = 1;
	float sphere2_transparency = 0;
	scene = add_sphere(scene, sphere2_ctr, sphere2_rad, sphere2_ambient,
		sphere2_diffuse, sphere2_specular, sphere2_shineness,
		sphere2_reflectance, sphere2_refraction_index, sphere2_transparency, 2);


	// sphere 3 -- yellow sphere (furthest sphere)
  	Point sphere3_ctr = {3.75, -0.3, -3.5};
	float sphere3_rad = 0.75;
	float sphere3_ambient[] = {0.6, 0.6, 0.6};
	float sphere3_diffuse[] = {1.0, 1.0, 0.25};
	float sphere3_specular[] = {1.0, 1.0, 1.0};
	float sphere3_shineness = 5;
	float sphere3_reflectance = 0.4;
	float sphere3_refraction_index = 1.8;
	float sphere3_transparency = 0.1;
	scene = add_sphere(scene, sphere3_ctr, sphere3_rad, sphere3_ambient,
		sphere3_diffuse, sphere3_specular, sphere3_shineness,
		sphere3_reflectance, sphere3_refraction_index, sphere3_transparency, 3);

 	// sphere 4
	Point sphere4_ctr = {0.0, 2, -4.25};
	float sphere4_rad = 1.0;
	float sphere4_ambient[] = {0.2, 0.2, 0.2};
	float sphere4_diffuse[] = {0.0, 1.0, 0.25};
	float sphere4_specular[] = {0.0, 1.0, 0.0};
	float sphere4_shineness = 10;
	float sphere4_reflectance = 0.3;
	float sphere4_refraction_index = 1.8;
	float sphere4_transparency = 0.3;
	scene = add_sphere(scene, sphere4_ctr, sphere4_rad, sphere4_ambient,
		     sphere4_diffuse, sphere4_specular, sphere4_shineness,
			     sphere4_reflectance, sphere4_refraction_index, sphere4_transparency, 3);
}
