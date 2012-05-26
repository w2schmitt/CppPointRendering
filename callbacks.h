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
#include "camera/float3.h"
#include "camera/Camera.h"

extern Window mainWin; 
extern Camera camera;
extern Mouse mouse;
extern Mesh mesh;

extern GLfloat lightDir[];
extern GLfloat lightDiffuse[];
extern GLfloat lightAmbient[];

extern GLuint axisDL;
extern char filename[255];

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

