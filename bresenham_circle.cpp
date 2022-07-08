#include <GL/glut.h>
#include <math.h>
#include <iostream>
#define WCOL 640
#define WROW 480
#define R 0
#define G 0
#define B 1

using namespace std;

int xcenter, ycenter, radius;

void pixelate(int x, int y){
    glBegin(GL_POINTS);
        if(x+xcenter>=0 && y+ycenter>=0)
            glVertex2i(x+xcenter, y+ycenter);
        if(y+xcenter>=0 && x+ycenter>=0)
            glVertex2i(y+xcenter, x+ycenter);
        if(y+xcenter>=0 && -x+ycenter>=0)
            glVertex2i(y+xcenter, -x+ycenter);
        if(x+xcenter>=0 && -y+ycenter>=0)
            glVertex2i(x+xcenter, -y+ycenter);
        if(-x+xcenter>=0 && -y+ycenter>=0)
            glVertex2i(-x+xcenter, -y+ycenter);
        if(-y+xcenter>=0 && -x+ycenter>=0)
            glVertex2i(-y+xcenter, -x+ycenter);
        if(-y+xcenter>=0 && x+ycenter>=0)
            glVertex2i(-y+xcenter, x+ycenter);
        if(-x+xcenter>=0 && y+ycenter>=0)
            glVertex2i(-x+xcenter, y+ycenter);
    glEnd();
    glFlush();
}

void bres_circle(){
    int x=0, y=radius;
    int d=(1-radius);
    int de=3;
    int dse=5-2*radius;
    while(y>x){
        pixelate(x, y);
        if(d>=0){
            d+=dse;
            de+=2;
            dse+=4;
            y--;
        }
        else{
            d+=de;
            de+=2;
            dse+=2;
        }
        x++;
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(R, G, B);
    glPointSize(1.0);
    gluOrtho2D(0.0, WCOL, 0.0, WROW);
    glFlush();
}

void display(){}

int main(int argc, char **argv) {
    if(argc!=4){
        cout << "The number of arguments MUST be 3" << "\n";
        return 1;
    }
    glutInit(&argc, argv);
    xcenter=atoi(argv[1]);
    ycenter=atoi(argv[2]);
    radius=atoi(argv[3]);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WCOL, WROW);
    glutCreateWindow("Bresenham's Circle Drawing Algorithm");
    init();
    glutDisplayFunc(bres_circle);
    glutMainLoop();
    return 0;
}
