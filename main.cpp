/* 
 * File:   main.cpp
 * Author: wagner
 *
 * Created on 21 de Maio de 2012, 20:50
 */

#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <AntTweakBar.h>

#include "util.h"
#include "callbacks.h"
//#include "camera/float3.h"
//#include "camera/Camera.h"
#include "GLCam.h"
#include "mesh.h"


// ------------------ GLOBALS
Window mainWin;                 // Hold window properties
Mouse mouse;
GLCam camera(vec3d(10,10,10), vec3d(0,0,0), vec3d(0,1,0));
Mesh mesh(0.008f);

GLuint axisDL = 0;              // axis display list ID
char filename[255] = "models/fullsphere.txt";

// opengl light settings
GLfloat lightDir[] = {1.0, 0.5, 1.0, 0};
GLfloat lightDiffuse[] = {0.6, 0.6, 0.6, 1.0f};
GLfloat lightAmbient[] = {0.2, 0.2, 0.2, 1.0f};


int main(int argc, char** argv) {

    TwBar *bar;;
    
    // Initialize glut/glew/Anttweakbar/etc...
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(mainWin.posx, mainWin.posy);
    glutInitWindowSize(mainWin.x, mainWin.y);
    mainWin.id = glutCreateWindow("Point Rendering");
    
    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK){
      std::cerr << "[ERROR] "<< glewGetErrorString(glewStatus)<<std::endl;
    }
    
    TwInit(TW_OPENGL, NULL);
    
    // callbacks
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);    
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    
    bar = TwNewBar("TweakBar");  
    twGUI(bar);
    
    initialize();
    atexit(terminate);
    
    glutMainLoop();
    return 0;
}

