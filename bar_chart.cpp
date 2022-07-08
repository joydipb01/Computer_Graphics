#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define M 256

#include <stdlib.h>
#include <cfloat>
#include <string.h>
#include <iostream>
using namespace std;

int wid = 640, hei = 480;
char file_name[M];

void init(void){
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(0.0, 640.0, 0.0, 500.0);
}

void reshape(GLint w, GLint h){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, GLdouble(w), 0.0, GLdouble(h));
  glClear(GL_COLOR_BUFFER_BIT);
}

void read_data(char *file){
  FILE *f = fopen(file, "r");
  if (f == NULL){
    fprintf(stderr, "No file found!\n");
    exit(1);
  }
  int filectr = 0;
  char *str = (char *)malloc(sizeof(char) * 1024);
  double min = DBL_MAX, max = -DBL_MAX;
  int barno = 0;
  while (fscanf(f, "%[^\n] ", str) != EOF){
    int len = strlen(str);
    if (filectr == 0){
      double mid = 0;
      for (int i = 0; i < len; i++){
        glColor3f(0.0, 1.0, 0.0);
        mid += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, str[i]);
      }
      mid /= 4.0;
      glRasterPos2i((wid / 2) - mid, 20);
      for (int i = 0; i < len; i++){
        glColor3f(0.0, 1.0, 0.0);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
      }
    }
    else if (filectr == 1){
      glPushMatrix();
      glRasterPos2i(12, hei - 30);
      for (int i = 0; i < len; i++){
        glColor3f(0.0, 0.0, 1.0);
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
      }
      glPopMatrix();
    }
    else if (filectr == 2){
      FILE temp = *f;
      size_t len_read = 0, len;
      int i = 0;
      barno = stoi(str);
      double val;
      while ((len_read = getline(&str, &len, f)) != -1){
        char *temp = strtok(str, " ");
        temp = strtok(NULL, "\n");
        val = atof(temp);
        if (val < min){
          min = val;
        }
        if (val > max){
          max = val;
        }
        i++;
      }
      *f = temp;
      char str2[M];
      for (int k = 0; k < 11; k++){
        double position = (k / 10.0) * (hei - 120);
        double val = ((k / 10.0) * (max - min)) + min;
        strfromf64(str2, 256, "%.2f", val);
        double wide = 0;
        int len1 = strlen(str2);
        for (int l = 0; l < len1; l++){
          wide += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, str2[l]);
        }
        glRasterPos2f(60.0-wide, 60.0+position);
        for (int l = 0; l < len1; l++)
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str2[l]);
      }
    }
    else{
      size_t len_read = 0, len;
      int i = 0;
      double val, bar_width = (double)(wid - 90) / (double)(2 * barno);
      do{
        char *iter_str1 = strtok(str, " ");
        char *iter_str2 = strtok(NULL, "\n");
        val = atof(iter_str2);
        val = (val - min) * (hei - 120) / (max - min);
        double bar_dist=bar_width*2*i;
        glRectf(80.0+bar_dist, 60.0, 80.0+bar_width+bar_dist, 60.0+val);
        int len1 = strlen(iter_str1);
        double mid = 0;
        for (int k = 0; k < len1; k++)
          mid += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, iter_str1[k]);
        mid /= 4.0;
        glPushMatrix();
        glRasterPos2f(80.0+bar_width/2.0+bar_dist-mid, 48.0);
        for (int k = 0; k < len1; k++){
          glColor3f(0.0, 0.0, 1.0);
          glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, iter_str1[k]);
        }
        glPopMatrix();
        len1 = strlen(iter_str2);
        mid = 0;
        for (int k = 0; k < len1; k++)
          mid += glutBitmapWidth(GLUT_BITMAP_HELVETICA_12, iter_str2[k]);
        mid /= 4.0;
        glPushMatrix();
        glRasterPos2f(80.0+bar_width/2.0 + bar_width*2*i-mid, 60.0 + val + 3);
        glPopMatrix();
        i++;
      }while ((len_read = getline(&str, &len, f)) != -1);
    }
    filectr++;
  }
}

void bar_chart(void){
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);
  glBegin(GL_LINES);
  glVertex2i(60, 60);
  glVertex2i(60, hei - 60);
  glEnd();
  glBegin(GL_LINES);
  glVertex2i(60, 60);
  glVertex2i(wid - 60, 60);
  glEnd();
  read_data(file_name);
  glFlush();
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  if (argc != 2){
    fprintf(stderr, "Incorrect number of arguments!\n");
    return 1;
  }
  strcpy(file_name, argv[1]);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(wid, hei);
  glutCreateWindow("Ex-1: Bar Chart");
  init();
  glutDisplayFunc(bar_chart);
  glutReshapeFunc(reshape);
  glutMainLoop();
  return 0;
}
