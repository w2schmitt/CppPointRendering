/* 
 * File:   Mesh.h
 * Author: wagner
 *
 * Created on 24 de Maio de 2012, 20:41
 */

#ifndef MESH_H
#define	MESH_H

#define DEG_TO_RAD 0.01745329238
#define RAD_TO_DEG 57.29578

#include <vector>
#include <algorithm>
#include <fstream>
#include <GL/glut.h>
#include <math.h>

#include "util.h"


class Mesh {
public:
    float radius;
    float normalSize;
    
    Mesh(float r);
    Mesh(const Mesh& orig);
    virtual ~Mesh();
    
    std::vector<Surfel> surfels;
    
    void loadTxt(char* filename);
    void loadObj(char* filename);
    
    void drawMesh(GLenum primitive);
            
private:
    void clear();
    void drawSurfel(Surfel s, GLenum primitive);
    void computeNormals();
    
};

#endif	/* MESH_H */

