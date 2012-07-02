/* 
 * File:   Mesh.h
 * Author: wagner
 *
 * Created on 24 de Maio de 2012, 20:41
 */

#ifndef MESH_H
#define	MESH_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <math.h>
#include <assert.h>

#include "util.h"


class Mesh {
public:
    float radius;
    float normalSize;
    
    Mesh(float r);
    Mesh(const Mesh& orig);
    virtual ~Mesh();
    
    std::vector<Surfel> surfels;
    
    void loadTxt(char* filename, bool useNormals, bool usePCA);
    void loadObj(char* filename);
    
    void drawMesh(GLenum primitive);
            
private:
    void clear();
    void drawSurfel(Surfel s, GLenum primitive);
    void computeNormals(bool usePCA, char *filename);
    
};

#endif	/* MESH_H */

