/* 
 * File:   GLCam.h
 * Author: wagner
 *
 * Created on 17 de Junho de 2012, 11:54
 */

#ifndef GLCAM_H
#define	GLCAM_H

//#define DEG_TO_RAD M_PI/180	

#include <iostream>
#include <GL/glew.h>
#include <cmath>
#include <assert.h>

#include "vector.hpp"
#include "quaternion.h"


class GLCam {
public:    
    static const double DEG_TO_RAD2 = 0.01745329238;
    static const double RAD_TO_DEG2 = 57.29578; 
    
public:
    GLCam(vec3d _pos, vec3d _lookAt, vec3d _up);
    GLCam(const GLCam& orig);
    virtual ~GLCam();
    
    void update();
    
    //CAMERA
    void rotateAroundObj(double _dx, double _dy);
    void rotateXYZ(double _alphaX, double _alphaY, double _alphaZ);
    void translate(double _dx, double _dy, double _dz);
    
    //GETS
    vec3d getPos()      { return pos;}
    vec3d getLookAt()   { return lookAt;}
    vec3d getUp()       { return upVec;}
    
    
    //SETS
    void setPerspective(double _fov, double _aspect, double _zNear, double _zFar);
    void setSphericCoords(const vec3d &_sphericCoord);
    
private:
    // perspective properties
    double zNear;
    double zFar;
    double fov;
    double aspect;
    
    //Axis
    vec3d u,v,n;                // = x,y,z
    
    // Camera 
    vec3d pos;
    vec3d lookAt;
    vec3d upVec;
    
    vec3d sphericPos;           // radius, theta, phi,    
    
    //FUNCTIONS ---------------------------
    void _updateCamAxis();
    vec3d _spherical2Cartesian(const vec3d &coord);
    vec3d _cartesian2spherical(const vec3d &coord);
    
};

#endif	/* GLCAM_H */

