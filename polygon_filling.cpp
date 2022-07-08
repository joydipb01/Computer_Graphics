#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <algorithm>
#include <math.h>
#define WCOL 640
#define WROW 480

using namespace std;

class poly_edge {
public:
    GLint ymin;
    GLint ymax;
    GLint xmin;
    GLint xmax;
    GLfloat xval;
    GLfloat slope;
    poly_edge(tuple<GLint, GLint> v1, tuple<GLint, GLint> v2);
};

poly_edge::poly_edge(tuple<GLint, GLint> v1, tuple<GLint, GLint> v2){
    this->ymin = min(get<1>(v1), get<1>(v2));
    this->ymax = max(get<1>(v1), get<1>(v2));
    this->xmin = min(get<0>(v1), get<0>(v2));
    this->xmax = max(get<0>(v1), get<0>(v2));
    if (min(get<1>(v1), get<1>(v2)) == get<1>(v1)) {
        this->xval = get<0>(v1);
    } else {
        this->xval = get<0>(v2);
    }
    this->slope = static_cast<GLfloat>(static_cast<GLfloat>(get<1>(v1) - get<1>(v2))) / static_cast<GLfloat>((get<0>(v1) - get<0>(v2)));
}

struct less_than_key{
    inline bool operator() (const poly_edge& struct1, const poly_edge& struct2){
        if (struct1.ymin != struct2.ymin)
            return (struct1.ymin < struct2.ymin);
        return (struct1.xmin < struct2.xmin);
    }
};

struct by_x_val_key{
    inline bool operator() (const poly_edge& struct1, const poly_edge& struct2){
        return (struct1.xval < struct2.xval);
    }
};

struct RGBType {
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

GLint scanline;
bool pix_fl;
vector<tuple<GLint, GLint>> points;
vector<poly_edge> allEdges;
vector<poly_edge> activeEdges;
RGBType *pixels; 

void pixelate(GLfloat x1, GLfloat x2) {
    int i1 = roundf(x1);
    int i2 = roundf(x2);
    int count = 0;
    
    for (int i = ((WCOL * (WROW - scanline)) + i1); i < ((WCOL * (WROW - scanline)) + i2); i++, count++) {
        pixels[i].r = 1;
        pixels[i].b = 1;
        pixels[i].g = 1;
        glutPostRedisplay();
    }
}


void removeActiveEdgesByScanline() {
    for (vector<poly_edge>::iterator it = activeEdges.begin(); it < activeEdges.end(); ) {
        if (it->ymax == scanline) {
            activeEdges.erase(it);
        } else {
            it++;
        }
    }
}

void updateXValues() {
    for (vector<poly_edge>::iterator it = activeEdges.begin(); it < activeEdges.end(); it++) {
        it->xval += (1/it->slope);
    }
}

void updateActiveEdges() {
    for (vector<poly_edge>::iterator it = allEdges.begin(); it < allEdges.end(); it++) {
        if (it->ymin == scanline) {
            activeEdges.push_back(*it);
        }
        if (it->ymin > scanline) {
            return;
        }
    }
}

void initScanline() {
    if (allEdges.size() != 0) {
        scanline = allEdges.at(0).ymin;
        glutPostRedisplay();
    }
}

void sortAndFilterEdges() {
    sort(allEdges.begin(), allEdges.end(), less_than_key());
    for (vector<poly_edge>::iterator it = allEdges.begin(); it < allEdges.end(); it++) {
        if (it->slope == 0) {
            allEdges.erase(it);
        }
    }
}

void sortActiveEdgesByXValues() {
    sort(activeEdges.begin(), activeEdges.end(), by_x_val_key());
}

void fillPolygon() {
    while (activeEdges.size() != 0) {
        for (vector<poly_edge>::iterator it = activeEdges.begin(); it < activeEdges.end(); it++) {
            pixelate(it->xval, (it+1)->xval);
            it++;
        }
        scanline++;
        removeActiveEdgesByScanline();
        updateXValues();
        updateActiveEdges();
        sortActiveEdgesByXValues();
        glutPostRedisplay();
    }
}

void init(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, WCOL, WROW, 0.0, 0.0, 1.0);
    pixels = new RGBType[WCOL*WROW];
    for (int i = 0; i < WCOL*WROW; i++) {
        pixels[i].r = 0;
        pixels[i].g = 0;
        pixels[i].b = 0;
    }
    pix_fl = false;
}

void menu(int id) {
    switch (id) {
        case 1:
            poly_edge newEdge(points.at(0), points.at(points.size()-1));
            allEdges.push_back(newEdge);
            sortAndFilterEdges();
            initScanline();
            updateActiveEdges();
            pix_fl = true;
            glutPostRedisplay();
            fillPolygon();
            break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN && !pix_fl) {
                points.push_back(tuple<GLint, GLint>(x, y));
                if (points.size() > 1) {
                    poly_edge newEdge(points.at(points.size()-2), points.at(points.size()-1));
                    allEdges.push_back(newEdge);
                }
                glutPostRedisplay();
            }
    }
    glutPostRedisplay();
}

void display(void) {
    GLenum err_code;
    do {
        err_code = glGetError();
        if (err_code != GL_NO_ERROR)
            printf("Error: %s\n", gluErrorString(err_code));
    } while (err_code != GL_NO_ERROR);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WCOL, WROW, GL_RGB, GL_FLOAT, pixels);
    glPointSize(5);
    glColor3f(1.0, 1.0, 0.0);
    if (!pix_fl) {
        for (int i = 0; i < points.size(); i++) {
            glBegin(GL_POINTS);
            glVertex2f(get<0>(points.at(i)), get<1>(points.at(i)));
            glEnd();
        }
    }
    if (pix_fl) {
        glDrawPixels(WCOL, WROW, GL_RGB, GL_FLOAT, pixels);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_POLYGON);
        for (int i = 0; i < points.size(); i++) {
            glVertex2f(get<0>(points.at(i)), get<1>(points.at(i)));
        }
        glEnd();
    }
    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(WCOL, WROW);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Polygon-Filling Scanline Algorithm");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutCreateMenu(menu);
    glutAddMenuEntry("Draw Polygon", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}
