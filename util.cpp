
#include "util.h"

GLuint xyzAxisDL(vec3d center, float size){
    
    glDisable(GL_LIGHTING);
    GLuint DLAxisId = glGenLists (1);
    glNewList(DLAxisId, GL_COMPILE);  
   
    // x axis
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(size, center.y, center.z);
    glEnd();
    
    // y axis
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x, size, center.z);
    glEnd();
    
    // z axis
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(center.x, center.y, center.z);
    glVertex3f(center.x, center.y, size);
    glEnd();
    
    glEndList();
    glEnable(GL_LIGHTING);
    
    return DLAxisId;
}

vec3d bestFitPlaneNormal(const std::vector<const Surfel*> verts){
    unsigned int nverts = verts.size();
    
    vec3d normal = vec3d::null();
    vec3d u,v;
    
    for(unsigned int i=0; i<nverts; ++i) {
        u = verts[i]->v;
        v = verts[(i + 1) % nverts]->v;
        normal.x += (u.y - v.y)*(u.z + v.z);
        normal.y += (u.z - v.z)*(u.x + v.x);
        normal.z += (u.x - v.x)*(u.y + v.y);
        //refpt += u;
    }
    
    normal.normalize();
    //std::cout << normal << std::endl;

    return normal;
}
int coutn=0;
vec3d leastVarianceDirection(const std::vector<const Surfel*> verts){
     unsigned int nverts = verts.size();
     
     vec3d centroid(0,0,0);
     for(unsigned int i=0; i<nverts; ++i) {
         centroid = centroid + verts[i]->v;
     }
     centroid.x = centroid.x/nverts;
     centroid.y = centroid.y/nverts;
     centroid.z = centroid.z/nverts;
     
     double p[9] = {0,0,0,0,0,0,0,0,0};
     Matrix<double> r(3,p);
     //Matrix<double> t(3,p);
     for (int i=0; i<nverts; ++i){
         vec3d v = verts[i]->v - centroid;
         double p1[9] = {v.x*v.x, v.x*v.y, v.x*v.z,  v.y*v.x, v.y*v.y, v.y*v.z, v.z*v.x, v.z*v.y, v.z*v.z};
         r = r + Matrix<double>(3,p1);
         //r = r + t;
     }
     
     double eig[3] = {0,0,0};
     double eigvec[3] = {0,0,0};
     
     r.eigenvalues(eig,eigvec);
     vec3d normal(eigvec[0], eigvec[1], eigvec[2]);
     if (normal.length()< 1.E-14){
         //std::cout << normal << std::endl;
         normal = vec3d(0,1,0);
     }
     normal.normalize();
     
     
     //if (normal.length()< 1.E-5){
         //std::cout << normal << std::endl;
     //}
     //std::cout << normal << std::endl;
     return normal;
}

