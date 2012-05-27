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
#include "mesh.h"
#include "matrix.hpp"

// ------------------ GLOBALS
Window mainWin;                 // Hold window properties
Mouse mouse;
Camera camera;
Mesh mesh(0.008f);

GLuint axisDL = 0;              // axis display list ID
char filename[255] = "models/fullsphere.txt";

// opengl light settings
GLfloat lightDir[] = {1.0, 0.5, 1.0, 0};
GLfloat lightDiffuse[] = {0.6, 0.6, 0.6, 1.0f};
GLfloat lightAmbient[] = {0.2, 0.2, 0.2, 1.0f};


int main(int argc, char** argv) {

    /* MATRIX TEST*/
    double p[9] = {1,5,2, 3,4,2, 8,1,1};
    Matrix<double> e(3,p);
    Matrix<double> f(3,p);    

    
    //Matrix<double> m(3,e);
    
    std::cout << (e+f);
   //Matrix<double>::identity(3);
    
    
    /* END TEST*/
    return 0;
    
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
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
    
    bar = TwNewBar("TweakBar");  
    twGUI(bar);
    
    initialize();
    atexit(terminate);
    
    glutMainLoop();
    return 0;
}

