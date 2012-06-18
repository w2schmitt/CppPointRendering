/* 
 * File:   GLCam.cpp
 * Author: wagner
 * 
 * Created on 17 de Junho de 2012, 11:54
 */

#include "GLCam.h"

GLCam::GLCam(vec3d _pos, vec3d _lookAt, vec3d _up) {
    // perspective properties
    zNear = 0.1;
    zFar = 1000;
    fov = 45;
    aspect = 1;
    
    // camera properties    
    pos = _pos;
    lookAt = _lookAt;
    upVec = _up;
    
    sphericPos = _cartesian2spherical(pos);
    
    _updateCamAxis();
    
    //upVec = cross(pos,lookAt); 
    //upVec.normalize();
}

GLCam::GLCam(const GLCam& orig) {
}

GLCam::~GLCam() {
}

void GLCam::setPerspective(double _fov, double _aspect, double _zNear, double _zFar){
    fov = _fov;
    aspect = _aspect;
    zNear = _zNear;
    zFar = _zFar;
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    
    gluPerspective( fov, aspect, zNear, zFar); 
    update();
}

void GLCam::rotateAroundObj(double _dx, double _dy){
       
    //vec3d spher = _cartesian2spherical(pos);
    
    //spher.y -= _dy;
    //spher.z += _dx;

    //vec3d vecPos = -pos;
    //vec3d vecNewPos = -_spherical2Cartesian(spher);
    
    //vecPos.normalize();
    //vecNewPos.normalize();
    //u.normalize();
    Quaternion rotU(_dy,u);
    Quaternion rotV(_dx,v);
    
    Quaternion vecPos(0,-pos);
    
    Quaternion p1 = rotU*vecPos*rotU.conj();
    pos = -p1.q;
    
    std::cout << sin(_dy) << std::endl;
    
    
    //double angle = acos(dot(vecPos, vecNewPos));
    //vec3d axis = cross(vecPos, vecNewPos);    
    
    //Quaternion q1;
    //q1 = q1.toAngleAxis(angle,axis);
    
    //Quaternion result = q1*Quaternion(0,-pos)*q1.conj();
    //pos = -result.q;
    //std::cout << pos << std::endl;
    _updateCamAxis();

}

void GLCam::rotateXYZ(double _alphaX, double _alphaY, double _alphaZ){
    vec3d newPos = vec3d::null();
    // ROTATION ON THE X-AXIS
    newPos.y = pos.y*cos(_alphaX) - pos.z*sin(_alphaX);
    newPos.z = pos.z*cos(_alphaX) + pos.y*sin(_alphaX);
    pos.y = newPos.y;
    pos.z = newPos.z;
    
    // ROTATION ON THE Y-AXIS
    newPos.z = pos.z*cos(_alphaY) - pos.x*sin(_alphaY);
    newPos.x = pos.x*cos(_alphaY) + pos.z*sin(_alphaY);
    pos.x = newPos.x;
    
    // ROTATION ON THE Z-AXIS
    newPos.x = pos.x*cos(_alphaZ) - pos.y*sin(_alphaZ);
    newPos.y = pos.y*cos(_alphaZ) + pos.x*sin(_alphaZ);
    
    pos = newPos;    
}



void GLCam::setSphericCoords(const vec3d &_sphericCoord){
    
    pos = _spherical2Cartesian(_sphericCoord);    
    _updateCamAxis();
}


void GLCam::update() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(pos.x, pos.y, pos.z,  lookAt.x, lookAt.y, lookAt.z, upVec.x, upVec.y, upVec.z);
}



// ----------------------------------------- PRIVATE METHODS       
void GLCam::_updateCamAxis() {    
    // ------ N axis
    n = lookAt - pos;
    n.normalize();
    
    // ------ U axis
    u = cross(upVec, n);
    u.normalize();
    
    // ------ V axis
    v = cross(n,u);
    v.normalize();    
}

vec3d GLCam::_spherical2Cartesian(const vec3d &coord){
    vec3d result;
    
    double rad = coord.x;
    double theta = coord.y;
    double phi = coord.z;
       
    result.x = rad*cos(theta)*sin(phi);
    result.y = rad*sin(theta)*sin(phi);
    result.z = rad*cos(phi);
    
    return result;
}

vec3d GLCam::_cartesian2spherical(const vec3d& coord){
    vec3d result;
    double s = sqrt(coord.x*coord.x + coord.y*coord.y);
    //assert(coord.x != 0);
    
    result.x = coord.length();  
    assert(result.x!=0);    
    result.z = acos(coord.z/result.x);  
    
    if (coord.x>=0){
        result.y = asin(coord.y/s);
    } else {
        result.y = M_PI - asin(coord.y/s);
    }
    
    return result;
}