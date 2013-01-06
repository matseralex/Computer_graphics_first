#include "drawers.h"
#include "Models/n_sphere.h"
#include "Models/n_torus.h"

#define GL_FOG_COORDINATE_SOURCE_EXT 0x8450
#define GL_FOG_COORDINATE_EXT 0x8451 //

//PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL; // Our glFogCoordfEXT Function
//typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord); // Declare Function Prototype
//glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");

int Extension_Init()
{
    char extensions[ 16384 ];
    char Extension_Name[] = "GL_EXT_fog_coord";
    char *buf;

    sprintf( extensions, "%s", (char *)glGetString( GL_EXTENSIONS ) );
    buf = strtok( extensions, " " );
    while( buf != NULL )
    {
  if( !strcmp( Extension_Name, buf ) )
	{
	    printf( "%s found, great.\n", Extension_Name );
	    return true;
	}
	buf = strtok( NULL, " " );
    }

    printf( "%s\n", (char *)glGetString( GL_EXTENSIONS ) );
    printf( "%s not found, dammit.\n", Extension_Name );

    return false;
}

void DrawSphere() {

    //Extension_Init();
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.0, 1.0,1.0);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_struct), vertexs);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof (struct vertex_struct), vertexs+ 6 * sizeof(float) );

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof (struct vertex_struct), vertexs+3*sizeof(float));
	glDrawElements(GL_TRIANGLES, sizeof(indexes)/sizeof(indexes[0]), GL_UNSIGNED_SHORT, indexes);


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void DrawTorus(){

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex_struct), vertexs1);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof (struct vertex_struct), vertexs1+ 6 * sizeof(float) );

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof (struct vertex_struct), vertexs1+3*sizeof(float));
	glDrawElements(GL_TRIANGLES, sizeof(indexes1)/sizeof(indexes1[0]), GL_UNSIGNED_SHORT, indexes1);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void DrawBox( GLfloat x1, GLfloat x2, GLfloat y1, GLfloat y2, GLfloat z1, GLfloat z2 )
{
    //Extension_Init();
//    printf("start box\n");
	glBegin ( GL_POLYGON );         // front face
          glNormal3f   ( 0.0, 0.0, 1.0 );
		   glTexCoord2f ( 0, 0 ); glVertex3f   ( x1, y1, z2 );
		  glTexCoord2f ( 1, 0 ); glVertex3f   ( x2, y1, z2 );
		  glTexCoord2f ( 1, 1 ); glVertex3f   ( x2, y2, z2 );
		  glTexCoord2f ( 0, 1 ); glVertex3f   ( x1, y2, z2 );
        glEnd ();

        glBegin ( GL_POLYGON );         // back face
          glNormal3f ( 0.0, 0.0, -1.0 );
		  glTexCoord2f ( 0, 0 ); glVertex3f   ( x2, y1, z1 );
		  glTexCoord2f ( 1, 0 ); glVertex3f   ( x1, y1, z1 );
		  glTexCoord2f ( 1, 1 ); glVertex3f   ( x1, y2, z1 );
		  glTexCoord2f ( 0, 1 ); glVertex3f   ( x2, y2, z1 );
        glEnd ();

        glBegin ( GL_POLYGON );         // left face
          glNormal3f ( -1.0, 0.0, 0.0 );
		  glTexCoord2f (0, 0); glVertex3f   (x1, y1, z1);
		  glTexCoord2f (1, 0); glVertex3f   (x1, y1, z2);
		  glTexCoord2f (1, 1); glVertex3f   (x1, y2, z2);
		  glTexCoord2f (0, 1); glVertex3f   (x1, y2, z1);
        glEnd ();

        glBegin ( GL_POLYGON );         // right face
          glNormal3f ( 1.0, 0.0, 0.0 );
		  glTexCoord2f ( 0, 0 ); glVertex3f(x2, y1, z2 );
		  glTexCoord2f ( 1, 0 ); glVertex3f(x2, y1, z1 );
		  glTexCoord2f ( 1, 1 ); glVertex3f   ( x2, y2, z1 );
		  glTexCoord2f ( 0, 1 ); glVertex3f   ( x2, y2, z2 );
        glEnd ();

        glBegin ( GL_POLYGON );         // top face
          glNormal3f(0.0, 1.0, 0.0);
		  glTexCoord2f(0, 1); glVertex3f(x1, y2, z2);
		  glTexCoord2f(1, 1); glVertex3f(x2, y2, z2);
		  glTexCoord2f(1, 0); glVertex3f(x2, y2, z1);
		  glTexCoord2f(0, 0); glVertex3f(x1, y2, z1);
        glEnd ();

        glBegin (GL_POLYGON);         // bottom face
          glNormal3f(0.0, -1.0, 0.0);
		  glTexCoord2f(0, 1); glVertex3f(x2, y1, z2);
		  glTexCoord2f(1, 1); glVertex3f(x1, y1, z2);
		  glTexCoord2f(1, 0); glVertex3f(x1, y1, z1);
		  glTexCoord2f(0, 0); glVertex3f(x2, y1, z1);
        glEnd ();
}
