#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glxew.h>
#include <GL/glext.h>
//#include <SDL/SDL.h>
#include    <ctype.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>



int Extension_Init();

void DrawSphere();
//void _DrawSphere();
void DrawTorus();
void DrawBox( GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2 );
