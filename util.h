/* 
 * File:   util.h
 * Author: wagner
 *
 * Created on 21 de Maio de 2012, 21:22
 */

#ifndef UTIL_H
#define	UTIL_H

#include <GL/glut.h>
#include <vector>
#include <iostream>
#include "vector.hpp"

struct Window {
    GLuint id;
    int posx, posy;
    int x,y;
    
    Window() : posx(0), posy(0), x(800), y(480), id(0){}
};

struct Surfel {
    vec3d v;            // vertex position
    vec3d n;            // vertex normal
    std::vector<const Surfel*> neighbour;
};

struct Mouse {
    int x,y;
    int Dx, Dy;
    bool buttons[3];
    float sensitivity;
};

// create a display list for the xyz axis
GLuint xyzAxisDL(vec3d center, float size);
vec3d bestFitPlaneNormal(const std::vector<const Surfel*> verts);


#endif	/* UTIL_H */

