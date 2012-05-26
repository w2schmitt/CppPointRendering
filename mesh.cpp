/* 
 * File:   Mesh.cpp
 * Author: wagner
 * 
 * Created on 24 de Maio de 2012, 20:41
 */

#include <iostream>
#include <GL/glew.h>

#include "mesh.h"

Mesh::Mesh(float r) {
    radius = r;
    normalSize = 0.05;
}

Mesh::Mesh(const Mesh& orig) {
}

Mesh::~Mesh() {
}

void Mesh::clear(){
    surfels.clear();
}

void Mesh::loadTxt(char* filename)
{
    clear();
    
    char type;
    std::ifstream in(filename);
    if (in.is_open()){
        while (in >> type){            
            if (type=='v'){
                Surfel s;
                in >> s.v.x >> s.v.y >> s.v.z;
                s.n = vec3d::up();
                surfels.push_back(s);
            }                 
            else if (type=='#'){
                in.ignore(4096);        // skip line
                continue;
            }
        }
    }
    else {
        std::cerr << "[ERROR] cannot open file " << filename << std::endl;
    }
    
    std::clog << "loaded " << surfels.size() << " points" << std::endl;
    in.close();
    
    computeNormals();
}

void Mesh::drawMesh(GLenum primitive){
    if (surfels.size()==0) 
        return;
    
    glBegin(primitive);
    for (unsigned int i=0; i<surfels.size(); ++i)
        drawSurfel(surfels[i], primitive);
        //glVertex3dv(&surfels[i].v.x);
    glEnd();
}

void Mesh::computeNormals(){
    
    const int n = 5;
    double min=10000, value=0;
    const Surfel* ptmin = NULL;
    
    // find the n nearest neighbours of surfels[j]
    for (unsigned int j=0; j<surfels.size(); ++j){
        for (int i=0; i<n; ++i){
            for (unsigned int k=0; k<surfels.size(); ++k){
                 if (j==k) continue;
                 value =  squaredDistance(surfels[j].v, surfels[k].v);
                 if ( (value <= min) && 
                      (std::find(surfels[j].neighbour.begin(), surfels[j].neighbour.end(), &surfels[k]) == surfels[j].neighbour.end())){
                     min = value;
                     ptmin= &surfels[k];
                 }
            }
            surfels[j].neighbour.push_back(ptmin);
            min=10000;
            ptmin=NULL;
        }
        // compute normal
        //for (int i=0; i<surfels[j].neighbour.size(); ++i)
        //    std::cout << squaredDistance(surfels[j].v,surfels[j].neighbour[i]->v) << std::endl;
        //std::cout << std::endl;
        surfels[j].n = bestFitPlaneNormal(surfels[j].neighbour);
    }
}

void Mesh::drawSurfel(Surfel s, GLenum primitive){
    
    //s.n.normalize();

    double rotAngle = acos(dot(vec3d::up(), s.n))*RAD_TO_DEG;
    vec3d rotAxis = cross(vec3d::up(), s.n);
    
    if (rotAngle < 0.05 || rotAngle > 179.95 ){
        rotAxis = vec3d::forward();
    }    
    
    //if (isNull(rotAxis)) return;
    //std::cout << rotAngle << std::endl;
    glPushMatrix();
    glTranslated(s.v.x, s.v.y, s.v.z);
    glRotated(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z );
    
    glColor3f(0.7,0.2,0.1);
    glBegin(primitive);
    glNormal3f(s.n.x, s.n.y, s.n.z);
    glVertex3f(-radius, 0, -radius);
    //glNormal3f(s.n.x, s.n.y, s.n.z);
    glVertex3f(-radius, 0,  radius);
    //glNormal3f(s.n.x, s.n.y, s.n.z);
    glVertex3f( radius, 0, radius);
    //glNormal3f(s.n.x, s.n.y, s.n.z);
    glVertex3f( radius, 0, -radius);
    glEnd();
    
   
    //glTranslated(-s.v.x, -s.v.y, -s.v.z );
    glPopMatrix();
    
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslated(s.v.x, s.v.y, s.v.z);
    glColor3f(0.5f,0.7f,0.1f);
    glBegin(GL_LINES);
    glVertex3f(0,0,0);
    glVertex3f(s.n.x*normalSize, s.n.y*normalSize, s.n.z*normalSize);
    glEnd();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

