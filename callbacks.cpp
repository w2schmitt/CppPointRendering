
#include "callbacks.h"


void initialize(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   
    // create display list for axis
    axisDL = xyzAxisDL(vec3d(0,0,0), 100.0);
}

int terminate(void){
    TwTerminate();
    
    glDeleteLists(axisDL, 1);
}

void specialKeys(int key, int x, int y){
    
    if (key == GLUT_KEY_LEFT) camera.lookLefRigObj(-0.080 ); //camera.MoveFrente(0.025);
    if (key == GLUT_KEY_RIGHT) camera.lookLefRigObj(0.080 ); //camera.MoveFrente(-0.025);
    if (key == GLUT_KEY_UP) camera.MoveFrente(0.080);
    if (key == GLUT_KEY_DOWN) camera.MoveFrente(-0.080);
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
    camera.setPerspec(45, (double)w/h, 0.5, 1000);

    // Get Back to the Modelview
    glMatrixMode(GL_MODELVIEW); 
}

void render(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    camera.update();	
    camera.lookAt();

    glCallList(axisDL);
    
    TwDraw();
    
    glutSwapBuffers();
    glutPostRedisplay();
}


void twGUI(TwBar *bar){
    
}
