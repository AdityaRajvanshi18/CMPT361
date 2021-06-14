#include "vector.h"

typedef struct sphere {
  int index;               

  Point center;
  float radius;

  float mat_ambient[3];    
  float mat_diffuse[3];
  float mat_specular[3];
  float mat_shineness;

  float refraction_index;
  float transparency;
  float reflectance;       
  struct sphere *next;
} Spheres;   

float intersect_sphere(Point, Vector, Spheres *, Point *);

bool isLightBlocked(Point, Vector, Spheres *);

Spheres *intersect_scene(Point, Vector, Spheres *, Point *);

Vector sphere_normal(Point, Spheres *);

Spheres *add_sphere(Spheres *, Point, float, float [], float [], float [], float, float, float, float, int);

