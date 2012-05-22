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
#include "camera/float3.h"
#include "camera/Camera.h"


// ------------------ GLOBALS
Window mainWin;                 // Hold window properties
Camera camera;

GLuint axisDL = 0;              // axis display list ID


int main(int argc, char** argv) {

    // Pointer to the tweak bar
    TwBar *bar; 
    
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
    
    glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
    glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    
    
    bar = TwNewBar("TweakBar");  
    twGUI(bar);
    
    initialize();
    atexit(terminate);
    
    glutMainLoop();
    return 0;
}

