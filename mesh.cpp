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

void Mesh::loadTxt(char* filename, bool useNormals, bool usePCA)
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

    char *fname = new char[255];
    strcpy(fname, filename);
    
    if (usePCA)
        strcat(fname,"_normalsPCA.txt");            
    else
        strcat(fname,"_normalsBFP.txt"); 
    
    if (useNormals){
        std::ifstream n_in;        
        n_in.open(fname);
        
        assert(n_in.is_open());
        
        std::cout << "loaded Normals: " << surfels.size() << " " << fname << std::endl;
        for (unsigned int j=0; j<surfels.size(); ++j){
            n_in >> surfels[j].n.x >> surfels[j].n.y >> surfels[j].n.z;
            //std::cout << j << "  " << surfels[j].n << std::endl;
        }
        n_in.close();
    }
    else{
        computeNormals(usePCA, fname);
    }
    delete [] fname;
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

void Mesh::computeNormals(bool usePCA, char *filename){
    
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

        if (usePCA){
                surfels[j].n = leastVarianceDirection(surfels[j].neighbour);
                if (surfels[j].n == vec3d::up()){
                     surfels[j].n = bestFitPlaneNormal(surfels[j].neighbour);
                }
        }
        else
                surfels[j].n = bestFitPlaneNormal(surfels[j].neighbour); 
    }
    
    // save surfels
    std::ofstream n_out;
    //if (usePCA)
    //    n_out.open("models/normalsPCA.txt");
    //else
    //    n_out.open("models/normalsBFP.txt");
    n_out.open(filename);
    std::cout << "saved normal file: "<<  filename << std::endl;
    for (unsigned int j=0; j<surfels.size(); ++j){
        vec3d *n = &surfels[j].n;
        n_out << n->x << " " << n->y << " " << n->z << std::endl;
    }
    n_out.close();
}


void Mesh::drawSurfel(Surfel s, GLenum primitive){
    
    const float _2pi = 2*M_PI;

    double rotAngle = acos(dot(vec3d::up(), s.n))*RAD_TO_DEG;
    vec3d rotAxis = cross(vec3d::up(), s.n);
    
    if (rotAngle < 0.05 || rotAngle > 179.95 ){
        rotAxis = vec3d::forward();
    }    

    glPushMatrix();
    glTranslated(s.v.x, s.v.y, s.v.z);
    glRotated(rotAngle, rotAxis.x, rotAxis.y, rotAxis.z );
    
    glColor3f(0.83,0.67,0.23);
    
    glBegin(primitive);
    if (primitive==GL_POLYGON){    
        glNormal3f(s.n.x, s.n.y, s.n.z);
        for (float i=0; i<_2pi; i+=_2pi/30.0){
            glVertex3f(radius*cos(i),0,radius*sin(i));
        }
    }
    else {
        glNormal3f(s.n.x, s.n.y, s.n.z);
        glVertex3f(-radius, 0, -radius);
        glVertex3f(-radius, 0,  radius);
        glVertex3f( radius, 0, radius);
        glVertex3f( radius, 0, -radius);        
    }
    glEnd();
      
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

