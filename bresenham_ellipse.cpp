#include <GL/glut.h>
#include <math.h>
#include <iostream>
#define WCOL 640
#define WROW 480
#define R 0
#define G 0
#define B 1

using namespace std;

int xcenter, ycenter, a, b;

void pixelate(int x, int y){
    glBegin(GL_POINTS);
        if(x+xcenter>=0 && y+ycenter>=0)
            glVertex2i(x+xcenter, y+ycenter);
        if(x+xcenter>=0 && -y+ycenter>=0)
            glVertex2i(x+xcenter, -y+ycenter);
        if(-x+xcenter>=0 && -y+ycenter>=0)
            glVertex2i(-x+xcenter, -y+ycenter);
        if(-x+xcenter>=0 && y+ycenter>=0)
             glVertex2i(-x+xcenter, y+ycenter);
    glEnd();
    glFlush();
}

void bres_ellipse(){
    int x=0, y=b;
    
    //region 1:
    int d=4*b*b-a*a*(4*b-1);
    int de=12*b*b;
    int dse=12*b*b-8*a*a*(b-1);
    while(a*a*(2*y-1)>2*b*b*(x+1)){
        pixelate(x, y);
        if(d>=0){
            d+=dse;
            de=de+(8*b*b);
            dse=dse+(8*(a*a+b*b));
            y--;
        }
        else{
            d+=de;
            de=de+(8*b*b);
            dse=dse+(8*b*b);
        }
        x++;
    }
    
    //region 2:
    d=(4*b*b*x*x+4*b*b*x+b*b) + (4*a*a*(y-1)*(y-1)) - 4*a*a*b*b;
    int ds=4*a*a*(3-2*y);
    dse=4*a*a*(3-2*y)+8*b*b*(x+1);
    while(y>0){
        pixelate(x, y);
        if(d>=0){
            d+=ds;
            ds=ds+8*a*a;
            dse=dse+8*a*a;
        }
        else{
            d+=dse;
            ds=ds+8*a*a;
            dse=dse+8*(a*a+b*b);
            x++;
        }
        y--;
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
    if(argc!=5){
        cout << "The number of arguments MUST be 4" << "\n";
        return 1;
    }
    glutInit(&argc, argv);
    xcenter=atoi(argv[1]);
    ycenter=atoi(argv[2]);
    a=atoi(argv[3]);
    b=atoi(argv[4]);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WCOL, WROW);
    glutCreateWindow("Bresenham's Ellipse Drawing Algorithm");
    init();
    glutDisplayFunc(bres_ellipse);
    glutMainLoop();
    return 0;
}
