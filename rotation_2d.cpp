#include <GL/glut.h>
#include <math.h>
#include <list>
#include <iostream>
#include <unistd.h>
#define WCOL 640
#define WROW 480
#define R 0
#define G 0
#define B 1

float rot_angle;

class point{
    public:
        float x;
        float y;
        point();
        void translate(float x1, float y1);
        void rotate(float angle);
};

point::point(){}

void point::translate(float x1, float y1){
    float xnew=this->x+x1;
    float ynew=this->y+y1;
    this->x=xnew;
    this->y=ynew;
}

void point::rotate(float angle){
    angle=angle*(M_PI/180);
    float xr=this->x;
    float yr=this->y;
    this->x=(xr*cos(angle)-yr*sin(angle));
    this->y=(xr*sin(angle)+yr*cos(angle));
}

class poly{
    public:
        std::list<point> pt_list;
        poly();
        void drawPoly();
        void translate_poly(float xpivot, float ypivot);
        void rotate_poly(float ang);
};

poly::poly(){}

void poly::drawPoly(){
    glBegin(GL_POLYGON);
        for(auto p=pt_list.begin(); p!=pt_list.end(); ++p)
            glVertex2f(p->x, p->y);
    glEnd();
    glFlush();
}

void poly::translate_poly(float xpivot, float ypivot){
    for(auto p=pt_list.begin(); p!=pt_list.end(); ++p)
        p->translate(xpivot, ypivot);
}

void poly::rotate_poly(float ang){
    for(auto p=pt_list.begin(); p!=pt_list.end(); ++p)
        p->rotate(ang);
}

using namespace std;

void mouse(int button, int state, int x, int y){
    y=WROW-y;
    static poly p;
    static int point_no=0, input=1;
    if(input){
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            point pts=point();
            pts.x=x;
            pts.y=y;
            p.pt_list.push_back(pts);
            point_no++;     
        }
        else if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
            if(point_no<3){
                cout << "Insufficient Points for polygon\n";
                return;
            }
            point_no=0;
            p.drawPoly();
            input=0;
        }
    }
    else{
        if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
            point pvt=point();
            pvt.x=x;
            pvt.y=y;
            while(1){
                glClearColor(1.0, 1.0, 1.0, 0.0);  
                glClear(GL_COLOR_BUFFER_BIT);
                p.translate_poly(-pvt.x, -pvt.y);
                p.rotate_poly(rot_angle);
                p.translate_poly(pvt.x, pvt.y);
                p.drawPoly();
                usleep(10000);
            }
        }
    }
}

void display(){  
    glClearColor(1.0, 1.0, 1.0, 0.0);  
    glClear(GL_COLOR_BUFFER_BIT);  
    glColor3f(0.0, 0.0, 0.0);
    glPointSize(1.0);
    gluOrtho2D(0.0, WCOL, 0.0, WROW);  
}  
    
int main(int argc, char **argv){
    if(argc!=2){
        cout << "The program MUST have 1 argument\n";
        return 1;
    }
    glutInit(&argc, argv);
    rot_angle=atof(argv[1]);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WCOL, WROW);
    glutCreateWindow("Polygon Rotate");
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
