/* 
 * File:   util.h
 * Author: wagner
 *
 * Created on 21 de Maio de 2012, 21:22
 */

#ifndef UTIL_H
#define	UTIL_H

#include <GL/glut.h>
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
    float radius;
    vec3d color;
};

// create a display list for the xyz axis
GLuint xyzAxisDL(vec3d center, float size);


#endif	/* UTIL_H */
