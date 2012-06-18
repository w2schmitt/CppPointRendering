#ifndef QUATERNION_H
#define QUATERNION_H

#pragma once

#include <cmath>

#include "vector.hpp"

class Quaternion{
  public:
        
    double w;
    vec3d q; 

    Quaternion():w(1),q(vec3d::null()){}
    Quaternion(const Quaternion &q0):w(q0.w),q(q0.q){}
    Quaternion(double _w, vec3d _q):w(_w),q(_q){}
    Quaternion(vec3d eulerAngles){
        _createQuaternionFromEulerAngles(eulerAngles.x, eulerAngles.y, eulerAngles.z);
    }
    Quaternion(double thetaX, double thetaY, double thetaZ){
        _createQuaternionFromEulerAngles(thetaX, thetaY, thetaZ);
    }
    Quaternion(double angle);
            
    double length() { return sqrt( squared_length() );}
    double squared_length() { return w*w + q.x*q.x + q.y*q.y + q.z*q.z; }
    void normalize(){
        double l = length();
        w   /= l;
        q.x /= l;
        q.y /= l;
        q.z /= l;
    }
    
    Quaternion toAngleAxis(double angle, vec3d axis){
        double s,omega;
        double l = axis.length();
        
        omega = -0.5*angle;
        s = sin(omega)/l;        
        
        return Quaternion( cos(omega), vec3d(s*axis.x, s*axis.y, s*axis.z));;       
    }
    
    Quaternion conj(){
        return Quaternion(w,-q);
    }
    
    // ------------------------------------------------ OPERATIONS
    Quaternion &operator=(const Quaternion &q0)
    { w=q0.w; q=q0.q; return *this;}
    
    const Quaternion operator+(const Quaternion &q0) const
    { return Quaternion(w+q0.w, q+q0.q);}
    
    const Quaternion operator-(const Quaternion &q0) const
    { return Quaternion(w-q0.w, q-q0.q);}
    
    const Quaternion operator*(const Quaternion &q0) const
    { 
        double _w = w*q0.w - dot(q,q0.q);
        vec3d _q = (q0.w*q) + (w * q0.q) + cross(q0.q, q);
        return Quaternion (_w,_q);
    }
   
        
private:
    void _createQuaternionFromEulerAngles(double thetaX, double thetaY, double thetaZ){
        double cosZ = cos(0.5*thetaZ);  double sinZ = sin(0.5*thetaZ);
        double cosY = cos(0.5*thetaY);  double sinY = sin(0.5*thetaY);
        double cosX = cos(0.5*thetaX);  double sinX = sin(0.5*thetaX);
        
        w   = cosZ*cosY*cosX + sinZ*sinY*sinX;
        q.x = cosZ*cosY*sinX - sinZ*sinY*cosX;
        q.y = cosZ*sinY*cosX + sinZ*cosY*sinX;
        q.z = sinZ*cosY*cosX - cosZ*sinY*sinX;    
    }
    
    
};


#endif
