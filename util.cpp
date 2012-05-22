
#include "util.h"

GLuint xyzAxisDL(vec3d center, float size){
    
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
    
    return DLAxisId;
}
