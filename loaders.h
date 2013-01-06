#include    <GL/glew.h>
#include    <GL/gl.h>
#include    <GL/glu.h>
#include    <GL/glut.h>
#include    <GL/glxew.h>

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <ctype.h>
#include    <math.h>


bool    checkOpenGLError ();

//
// Print out the information log for a shader object or a program object
//

void printInfoLog ( GLhandleARB object );

bool    loadShader ( GLhandleARB shader, const char * fileName );
GLhandleARB prepareShader (GLhandleARB vert, GLhandleARB frag, const char * path_vert, const char * path_frag);


struct texture_info{

int width;
int height;
unsigned char * img_data;

};

texture_info* TextureObs(const char* path);
GLuint  loadTexture (const  char * fileName );
