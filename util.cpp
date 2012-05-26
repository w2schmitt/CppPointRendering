
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
