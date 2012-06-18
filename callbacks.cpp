
#include <GL/glew.h>

#include "callbacks.h"
#include "mesh.h"


void initialize(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial ( GL_FRONT, GL_AMBIENT_AND_DIFFUSE ) ;
    
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    
    glShadeModel(GL_FLAT);    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
    
   
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    
    
    // create display list for axis
    axisDL = xyzAxisDL(vec3d(0,0,0), 100.0);
    
    // mouse
    mouse.sensitivity = 0.0025;
    mouse.x = mouse.y = 0;
}

int terminate(void){
    TwTerminate();
    
    glDeleteLists(axisDL, 1);
}

void specialKeys(int key, int x, int y){
    
    //if (key == GLUT_KEY_LEFT) camera.lookLefRigObj(-0.080 ); //camera.MoveFrente(0.025);
    //if (key == GLUT_KEY_RIGHT) camera.lookLefRigObj(0.080 ); //camera.MoveFrente(-0.025);
    //if (key == GLUT_KEY_UP) camera.MoveFrente(0.080);
    //if (key == GLUT_KEY_DOWN) camera.MoveFrente(-0.080);
}

void mouseFunc(int button, int state, int x, int y){
    if (TwEventMouseButtonGLUT(button, state, x, y)){
        mouse.buttons[button]=false;
        return;
    }
    
    // update mouse buttons and coordinates
    mouse.buttons[button] = !mouse.buttons[button];
    mouse.x = x;
    mouse.y = y;

}

double phi = 0.25;

void mouseMotion(int x, int y){
    if (TwEventMouseMotionGLUT(x,y)){        
        return;
    }
    
    mouse.Dx = mouse.x - x;
    mouse.Dy = mouse.y - y;
    
    // rotate camera 
    if (mouse.buttons[0]){    
        //camera.setSphericCoords(10, phi, 1.25);
        //phi+=0.005;
        //camera.lookLefRigObj(mouse.Dx*mouse.sensitivity);
        //camera.lookUpDownObj(mouse.Dy*mouse.sensitivity);
        camera.rotateAroundObj(mouse.Dx*mouse.sensitivity, mouse.Dy*mouse.sensitivity);
    }
    // zoom camera
    else if (mouse.buttons[2]){
        //camera.MoveFrente(mouse.Dy*mouse.sensitivity);
    }
    
    // update mouse coords
    mouse.x = x;
    mouse.y = y;
}


void reshape(int w, int h){
    // Prevent a divide by zero, when window is too short
    if(h == 0) h = 1; 
    mainWin.x = w;
    mainWin.y = h;
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w, h);
    
    // Send the new window size to AntTweakBar
    TwWindowSize(w, h);

    //gluPerspective(45, (double)w/h, 0.5, 1000);
    camera.setPerspective(45, (double)w/h, 0.1, 250);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW); 
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //camera.update();	
    //camera.lookAt();
    camera.update();
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

    //glutWireCube(5);
    glCallList(axisDL);
    mesh.drawMesh(GL_QUADS);
    //glutSolidSphere(2,16,16);
    
    TwDraw();
    
    glutSwapBuffers();
    glutPostRedisplay();
}


void twGUI(TwBar *bar){
    
    TwAddVarRW(bar, "Model path", TW_TYPE_CSSTRING(255), &filename, " group='Model' ");
    TwAddButton(bar, "Load", loadModel, NULL, " group='Model' ");
    
    TwAddVarRW(bar, "Radius", TW_TYPE_FLOAT, &mesh.radius, " group='Surfels' min=0 max=1 step = 0.002 ");
    TwAddVarRW(bar, "Normal Size", TW_TYPE_FLOAT, &mesh.normalSize, " group='Surfels' min=0 max=1 step = 0.02 ");

}

void TW_CALL loadModel(void*){
    
    mesh.loadTxt(filename);
}