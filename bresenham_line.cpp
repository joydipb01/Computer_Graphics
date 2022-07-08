#include <GL/glut.h>
#include <math.h>
#include <iostream>

using namespace std;

void bresenham(int x0, int y0, 
               int xend, int yend, 
               int switch_role, int ymo){
    int x = x0;
    int y = y0;
    int dy = yend - y0;
    int dx = xend - x0;
    if (switch_role && ymo) {
        int t = dx;
        dx = dy;
        dy = t;
    }
    if (ymo)
        dy*=-1;
    int p = 2*dy-dx;
    int tdy=2*dy;
    int tdydx=2*(dy-dx);
    glBegin(GL_POINTS);
        if (switch_role)
            glVertex2i(y, x);
        else
            glVertex2i(x, y);
    glEnd();
    glFlush();
    while (x < xend) {
        x++;
        if (p <= 0)
            p+=tdy;
        else {
            p+=tdydx;
            if (ymo)
                y--;
            else
                y++;
        }
        glBegin(GL_POINTS);
            if (switch_role)
               glVertex2i(y, x);
            else
                glVertex2i(x, y);
        glEnd();
        glFlush();
    }
}

void mouse(int click, int pos, int x, int y){
    static int flag = 0, x0, y0;
    int dx, dy;
    if(click == GLUT_LEFT_BUTTON && pos == GLUT_DOWN){
        if(flag == 0){
            x0 = x;
            y0 = 480-y;
            flag = 1;
        }
        else{
            y=480-y;
            glColor3f(0.0, 0.0, 1.0);
            dx=x-x0;
            dy=y-y0;
            if(abs(dy) <= abs(dx)){
                if(dx>0 && dy>=0)
                    bresenham(x0, y0, x, y, 0, 0);
                else if(dx<0 && dy<=0)
                    bresenham(x, y, x0, y0, 0, 0);
                else if(dx>0 && dy<=0)
                    bresenham(x0, y0, x, y, 0, 1);
                else if(dx<0 && dy>=0)
                    bresenham(x, y, x0, y0, 0, 1);
            }
            else{
                if (dx>=0 && dy>0)
                    bresenham(y0, x0, y, x, 1, 0);
                else if(dx<=0 && dy<0)
                    bresenham(y, x, y0, x0, 1, 0);
                else if(dx<=0 && dy>0)
                    bresenham(y0, x0, y, x, 1, 1);
                else if(dx>=0 && dy<0)
                    bresenham(y, x, y0, x0, 1, 1);
            }
            flag=0;
        }
    }
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void display(void){}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Bresenham's Line Drawing Algorithm");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
    return 0;
}
