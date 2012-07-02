/* 
 * File:   callbacks.h
 * Author: wagner
 *
 * Created on 21 de Maio de 2012, 21:39
 */

#ifndef CALLBACKS_H
#define	CALLBACKS_H

#include <iostream>
#include <GL/glut.h>
#include <AntTweakBar.h>

#include "util.h"
#include "mesh.h"
#include "vector.hpp"
#include "GLCam.h"

//#include "camera/float3.h"
//#include "camera/Camera.h"

extern Window mainWin; 
extern GLCam camera;
extern Mouse mouse;
extern Mesh mesh;

extern GLfloat lightDir[];
extern GLfloat lightDiffuse[];
extern GLfloat lightAmbient[];

extern GLuint axisDL;
extern char filename[255];
extern bool primitive;
extern bool useNormalsFile;
extern bool usePCA;

void initialize();
int terminate(void);

void reshape(int w, int h);
void render();
void specialKeys(int key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotion(int x, int y);

void twGUI(TwBar *bar);
void TW_CALL loadModel(void*);


#endif	/* CALLBACKS_H */

