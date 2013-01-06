#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glxew.h>
#include <GL/glext.h>
#include <ctype.h>
#include <stdio.h>

#include <cstdlib>
#include "loaders.h"
#include "drawers.h"


/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/

GLfloat mat_diffuse[] = { 0.7, 0.8, 0.1, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 100.0 };

GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
GLfloat pts1[4][4][3], pts2[4][4][3];
GLfloat pts3[4][4][3], pts4[4][4][3];
GLUnurbsObj *nurb;
int u, v;

/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/


unsigned mainPlanetId= -1;
unsigned mainPlanetId1= -1;
unsigned secPlanetId= -1;
unsigned secPlanetIdDiff= -1;
unsigned torusId[]={-1, -1,-1,-1};
unsigned particleId=-1;

float angle = 0.0f;
float speed=0.5f;
float effect=0.0f;

float mat_amb[]={0.1,0.2,0.1};
float mat_dif[]={0.3,0.5,0.8};
float mat_spec[]={1.0,1.0,1.0};
float shininess=0.7*128;

float light_params1[] ={1.0,1.0,1.0};
float mat_amb1[]={1.0,1.0,1.0};
float mat_dif1[]={1.0,1.0,1.0};
float mat_spec1[]={0.6,0.6,1.0};
float shininess1=128;

/* Max number of particles */
#define MAX_PARTICLES 800


float slowdown = 2.0f; /* Slow Down Particles                                */
float xspeed;          /* Base X Speed  */
float yspeed;          /* Base Y Speed */
float zoom = -0.2f;   /* Used To Zoom Out                                   */

GLuint loop;           /* Misc Loop Variable                                 */
GLuint col = 0;        /* Current Color Selection                            */

GLhandleARB furShader        = 0;         // program handlers
GLhandleARB furVertShader   = 0;
GLhandleARB furFragShader = 0;

GLhandleARB phong        = 0;         // program handlers
GLhandleARB phongVertShader  = 0;
GLhandleARB phongFragShader = 0;

GLhandleARB norm        = 0;         // program handlers
GLhandleARB normVertShader  = 0;
GLhandleARB normFragShader = 0;

GLhandleARB minnaert        = 0;         // program handlers
GLhandleARB minnaertVertShader  = 0;
GLhandleARB minnaertFragShader = 0;

GLhandleARB fog       = 0;         // program handlers
GLhandleARB fogVertShader  = 0;
GLhandleARB fogFragShader = 0;


typedef struct
{
    int   active; /* Active (Yes/No) */
    float life;   /* Particle Life   */
    float fade;   /* Fade Speed      */

    float r;      /* Red Value       */
    float g;      /* Green Value     */
    float b;      /* Blue Value      */

    float x;      /* X Position      */
    float y;      /* Y Position      */
    float z;      /* Z Position      */

    float xi;     /* X Direction     */
    float yi;     /* Y Direction     */
    float zi;     /* Z Direction     */

    float xg;     /* X Gravity       */
    float yg;     /* Y Gravity       */
    float zg;     /* Z Gravity       */
} particle;

/* Rainbow of colors */
static GLfloat colors[12][3] =
{
    { 1.0f,  0.5f,  0.5f},
  { 1.0f,  0.75f, 0.5f},
	{ 1.0f,  1.0f,  0.5f},
	{ 0.75f, 1.0f,  0.5f},
    { 0.5f,  1.0f,  0.5f},
	{ 0.5f,  1.0f,  0.75f},
	{ 0.5f,  1.0f,  1.0f},
	{ 0.5f,  0.75f, 1.0f},
    { 0.5f,  0.5f,  1.0f},
	{ 0.75f, 0.5f,  1.0f},
	{ 1.0f,  0.5f,  1.0f},
	{ 1.0f,  0.5f,  0.75f}
};

/* Our beloved array of particles */
particle particles[MAX_PARTICLES];

void ResetParticle( int num, int color, float xDir, float yDir, float zDir )
{
    // Make the particels active
    particles[num].active = true;
    // Give the particles life
    particles[num].life = 1.0f;
    /* Random Fade Speed */
    particles[num].fade = ( float )( rand( ) %100 ) / 1000.0f + 0.003f;
    // Select Red Rainbow Color
    particles[num].r = colors[color][0];
    // Select Green Rainbow Color
    particles[num].g = colors[color][1];
    // Select Blue Rainbow Color
    particles[num].b = colors[color][2];
    // Set the position on the X axis
    particles[num].x = 0.0f;
    // Set the position on the Y axis
    particles[num].y = 0.0f;
    // Set the position on the Z axis
    particles[num].z = 0.0f;
    // Random Speed On X Axis
    particles[num].xi = xDir;
    // Random Speed On Y Axi
    particles[num].yi = yDir;
    // Random Speed On Z Axis
    particles[num].zi = zDir;
    // Set Horizontal Pull To Zero
    particles[num].xg = 0.0f;
    // Set Vertical Pull Downward
    particles[num].yg = -0.4f;
    // Set Pull On Z Axis To Zero
    particles[num].zg = 0.0f;

    return;
}

void init ()
{

	glClearColor ( 0.0, 0.0, 0.0, 1.0 );
//loadTextures

	mainPlanetId=loadTexture("Textures/water2.bmp");
	mainPlanetId1=loadTexture("Textures/texture2.bmp");
	secPlanetId=loadTexture("Textures/normalmap.bmp");
	secPlanetIdDiff=loadTexture("Textures/stone1.bmp");
	torusId[0]=loadTexture("Textures/fur-bmp/fur-map2-00.bmp");
	particleId=loadTexture("Textures/particle.bmp");


/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/
//enable lighting :)
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	GLfloat light_ambient[] = { 0.1, 0.2, 0.1, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 0.1, 1.0, 1.0 };
	GLfloat light_specular[] = { 0, 0, 0, 1.0 };
	GLfloat light_position[] = { -0.4, 0.8, -0.2, 1.0 };


	glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv (GL_LIGHT0, GL_POSITION, light_position);

	glEnable (GL_LIGHT0);

	GLfloat light_ambient1[] = { 1.0, 0.3, 0.5, 1.0 };
	GLfloat light_diffuse1[] = { 1.0, 0.1, 1.0, 1.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position1[] = { 1.0, 0.0, 0.0, 1.0 };

	glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
	glEnable (GL_LIGHT1);
	glEnable (GL_LIGHTING);


/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/
//prepare shaders
    glewInit();

    furShader=prepareShader(furVertShader, furFragShader, "Shaders/furShader.vsh", "Shaders/furShader.fsh");
    phong=prepareShader(phongVertShader, phongFragShader, "Shaders/phong.vsh", "Shaders/phong.fsh");
    norm=prepareShader(normVertShader, normFragShader, "Shaders/norm.vsh", "Shaders/norm.fsh");
    minnaert = prepareShader (minnaertVertShader,minnaertFragShader,"Shaders/minnaert.vsh", "Shaders/minnaert.fsh");
    fog= prepareShader (fogVertShader,fogFragShader,"Shaders/fog.vsh", "Shaders/fog.fsh");
    printf("%s\n",glGetString(GL_VERSION));

/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/
//prepare particles

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );

    for ( loop = 0; loop < MAX_PARTICLES; loop++ )
    {
	    int color = ( loop + 1 ) / ( MAX_PARTICLES / 12 );
	    float xi, yi, zi;
	    xi =  ( float )( ( rand( ) % 50 ) - 26.0f ) * 10.0f;
	    yi = ( float )( ( rand( ) % 50 ) - 25.0f ) * 10.0f;
	    zi = ( float )( ( rand( ) % 50 ) - 25.0f ) * 10.0f;

	    ResetParticle( loop, color, xi, yi, zi );
     }

/*==========================================================================================*/
/*==========================================================================================*/
/*==========================================================================================*/
//initialize nurbs
  nurb = gluNewNurbsRenderer();
  gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 25.0);
  gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);

  // Build control points for NURBS hills.
  for(u=0; u<4; u++)
    {
        for(v=0; v<4; v++)
        {//1-st
          pts1[u][v][0] = 2.0*((GLfloat)u);
          pts1[u][v][1] = 2.0*((GLfloat)v);

          if((u==1 || u == 2) && (v == 1 || v == 2))
            pts1[u][v][2] = 10.0;
          else
            pts1[u][v][2] = 0.0;

          pts2[u][v][0] = 2.0*((GLfloat)u - 3.0);
          pts2[u][v][1] = 2.0*((GLfloat)v - 3.0);
          if((u==1 || u == 2) && (v == 1 || v == 2))
            if(u == 1 && v == 1)
                pts2[u][v][2] = 15.0;
            else
                pts2[u][v][2] = -2.0;
          else
            pts2[u][v][2] = 0.0;

          //2-nd
          pts3[u][v][0] = 2.0*((GLfloat)u - 3.0);
          pts3[u][v][1] = 2.0*((GLfloat)v);
          if((u==1 || u == 2) && (v == 1 || v == 2))
            if(u == 1 && v == 2)
              pts3[u][v][2] = 10.0;
            else
              pts3[u][v][2] = 2.0;
          else
            pts3[u][v][2] = 0.0;

          //3-rd
          pts4[u][v][0] = 2.0*((GLfloat)u);
          pts4[u][v][1] = 2.0*((GLfloat)v - 3.0);
          if((u==1 || u == 2 || u == 3) && (v == 1 || v == 2))
            if(v == 1)
              pts4[u][v][2] = -2.0;
            else
              pts4[u][v][2] = 9.0;
          else
            pts4[u][v][2] = 0.0;
    }
  }


}

void display ()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glUseProgramObjectARB(phong);
    glDisable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, particleId);

/*===============================================================================================================*/
/*===============================================================================================================*/
/*===============================================================================================================*/

    glLoadIdentity( );
    glScalef(0.03,0.03,0.03);
    glRotatef(angle, 0.3f,0.5f,0.2f);
    glEnable( GL_BLEND );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
//draw and redraw particles :)
    for ( loop = 0; loop < MAX_PARTICLES; loop++ )
	{
	    if ( particles[loop].active )
		{
		    // Particle X Position
		    float x = particles[loop].x;
		    // Particle Y Position
		    float y = particles[loop].y;
		    // Particlee Z Position + Zoom
		    float z = particles[loop].z + zoom;

		    glBegin( GL_QUADS );

		      glTexCoord2d( 1, 1 ); glVertex3f( x + 0.5f, y + 0.5f, z );// Top Right
		      glTexCoord2d( 0, 1 ); glVertex3f( x - 0.5f, y + 0.5f, z );// Top Left
		      glTexCoord2d( 1, 0 ); glVertex3f( x + 0.5f, y - 0.5f, z );// Bottom Right
		      glTexCoord2d( 0, 0 ); glVertex3f( x - 0.5f, y - 0.5f, z );// Bottom Left

		    glEnd( );
		    //printf("%f x %f y%f z\n", x, y,z);

		    // Move On The X Axis By X Speed
		    particles[loop].x += particles[loop].xi /
			( slowdown * 100 );
		    // Move On The Y Axis By Y Speed
		    particles[loop].y += particles[loop].yi /
			( slowdown * 100 );
		    // Move On The Z Axis By Z Speed
		    particles[loop].z += particles[loop].zi /
			( slowdown * 100 );

		    // Take Pull On X Axis Into Account
		    particles[loop].xi += particles[loop].xg;
		    // Take Pull On Y Axis Into Account
		    particles[loop].yi += particles[loop].yg;
		    // Take Pull On Z Axis Into Account
		    particles[loop].zi += particles[loop].zg;
		    // Reduce Particles Life By 'Fade'
		    particles[loop].life -= particles[loop].fade;

		    // If the particle dies, revive it
		    if ( particles[loop].life < 0.0f )
			{
			    float xi, yi, zi;
			    xi = xspeed + ( float )( ( rand( ) % 60 ) - 32.0f );
			    yi = yspeed + ( float)( ( rand( ) % 60 ) - 30.0f );
			    zi = ( float )( ( rand( ) % 60 ) - 30.0f );

			    ResetParticle( loop, col, xi, yi, zi );
            }
		}
	}

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/


// drawing a central planet in system from array of vertexes
    glPushMatrix();
	glLoadIdentity();
	glRotatef(angle, 0.2f, 0.5f, 0.5f);
	glScalef(0.2,0.2,0.2);
//prepare materials
//turn-on Phong shader

    glUseProgramObjectARB(phong);
    glUniform4fARB(glGetUniformLocationARB(phong, "eyePos"), 0.0,-1.0,0.0,1.0);
    glUniform4fARB(glGetUniformLocationARB(phong, "LightPos"), 0.9,0.0,0.2,1.0);
//prepare textures

    glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture ( GL_TEXTURE_2D, mainPlanetId );

	DrawSphere();

	glScalef(1.01,1.01,1.01);

	glColor3f(0.5,1.0,0.0);

    glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glUseProgramObjectARB(0);
    glPopMatrix();
//central planet is ready now

/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/


//draw second planet in system from the same sphere-array but in another position
// with normal-mapping
	glPushMatrix();
	glLoadIdentity();
	glRotatef(angle, 0.1f, 0.2f, 0.3f);
	glTranslatef (0.5,0.0, 0.2);
	glScalef(0.15,0.15,0.15);
	glColor3f(0.2, 1.0, 1.0);
//prepare materials
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec1);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess1);

    glUseProgramObjectARB(norm);

    glUniform4fARB(glGetUniformLocationARB(norm, "eyePos"), 0.0,1.0,0.0,1.0);
    glUniform4fARB(glGetUniformLocationARB(norm, "LightPos"), 0.7,1.0,0.2,1.0);
    glUniform1fARB(glGetUniformLocationARB(norm, "effect"), effect);
    glUniform1iARB(glGetUniformLocationARB(norm, "bumpMap"), 1);
   	glUniform1iARB(glGetUniformLocationARB(norm, "diffuseMap"), 2);



    glActiveTextureARB (GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, secPlanetId);

    glActiveTextureARB(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, secPlanetIdDiff);

	DrawSphere();
    glDisable(GL_TEXTURE_2D);

//second planet is ready now

	glPopMatrix();


/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/
// third planet in system. with per-vertex for-shader
    glPushMatrix();
    glLoadIdentity();
    glRotatef(2*angle,0.5,1.0,0.3);
    glTranslatef(-0.2, 0.7,0.2);


    glScalef(0.2,0.2,0.2);
    glEnable(GL_TEXTURE_2D);
    glUseProgramObjectARB(fog);

    glUniform4fARB(glGetUniformLocationARB(fog, "viewVec"), 0.0,1.0,0.0,1.0);
    glUniform4fARB(glGetUniformLocationARB(fog, "lightVec"), 0.7,1.0,0.2,1.0);
    glUniform1iARB(glGetUniformLocationARB(fog, "colorMap"), 2);
    glUniform1fARB(glGetUniformLocationARB(fog, "effect"), effect);

    glActiveTextureARB (GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, secPlanetIdDiff);

    glUniform1iARB(glGetUniformLocationARB(fog, "colorMap2"), 1);
    glUniform1iARB(glGetUniformLocationARB(fog, "normalMap"), 0);
    glEnable(GL_TEXTURE_2D);


    glActiveTextureARB (GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, secPlanetId);

    glActiveTextureARB(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, secPlanetId);

    DrawSphere();
    //DrawBox(-0.5,0.5,-0.5,0.5,-0.5,0.5);
    glPopMatrix();
    glActiveTexture(GL_TEXTURE0);
    glUseProgramObjectARB(0);
/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/



//now we wil draw torus
//fur rendering here :)
	glPushMatrix();
	glLoadIdentity();
	glScalef(0.3,0.3,0.3);
	glRotatef(angle, 0.5f, 0.5f, 0.0);


	glEnable(GL_TEXTURE_2D);
    glUseProgramObjectARB(furShader);

    glUniform4fARB(glGetUniformLocationARB(furShader, "eyePos"), 0.0,1.0,0.0,1.0);
   	glUniform4fARB(glGetUniformLocationARB(furShader, "lightPos"), 0.7,1.0,0.2,1.0);
   	glUniform1fARB(glGetUniformLocationARB(furShader, "tl"), 0.05);
    glUniform1iARB(glGetUniformLocationARB(furShader, "furTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture ( GL_TEXTURE_2D, torusId[0] );

	glColor4f(1.0,1.0,1.0,1.0f);
    glEnable  ( GL_TEXTURE_GEN_S );
    glEnable  ( GL_TEXTURE_GEN_T );
	glTexGeni     ( GL_S, GL_TEXTURE_GEN_MODE, GL_LINEAR    );
	glTexGeni     ( GL_T, GL_TEXTURE_GEN_MODE, GL_LINEAR    );

	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	DrawTorus();

//	glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	for (int i=1; i<=15; i++)
	{
		glScalef(1.01,1.01,1.01);
		glColor4f(1,1,1,0.1);

        glUniform1iARB(glGetUniformLocationARB(furShader, "furTexture"), 0);

		glTexGeni     ( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP   );
		glTexGeni     ( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP    );
		glEnable  ( GL_TEXTURE_GEN_S );
		glEnable  ( GL_TEXTURE_GEN_T );

		DrawTorus();

		glDisable ( GL_TEXTURE_GEN_S );
		glDisable ( GL_TEXTURE_GEN_T );

	}

	glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPopMatrix();
/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/

//and one more planet in the system
// with non-standard Minnaert lighting

    glPushMatrix();
    glLoadIdentity();
    glRotatef(angle, 0.5f, 0.7f, 0.4f);
    glTranslatef(-0.5, 0.0, -0.2);
    glScalef(0.1,0.1,0.1);
    glColor3f(0.0,1.0,1.0);

    glUseProgramObjectARB(minnaert);

    glEnable(GL_TEXTURE_2D);

    glUniform4fARB(glGetUniformLocationARB(minnaert, "eyePos"), 0.0,1.0,0.0,1.0);
    glUniform4fARB(glGetUniformLocationARB(minnaert, "LightPos"), 0.7,1.0,0.2,1.0);
    glUniform1iARB(glGetUniformLocationARB(minnaert, "colorTexture"), 0);



    glActiveTextureARB (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainPlanetId1);

    DrawSphere();
    glDisable(GL_TEXTURE_2D);

    glUseProgramObjectARB(0);

    glPopMatrix();


/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/
// And now nurbs ; 4 more hills
//    glUseProgramObjectARB(0); //turn-off all shaders, use standard GL lighting without any shaman dances

    glPushMatrix();
    glLoadIdentity();
    glTranslatef(-0.0,-0.9,0.0);
    glScalef(0.06,0.06,0.06);
    glRotatef(200, 1.0,1.0,1.0);
    glRotatef(30, 0.3,1.0,0.0);
    glRotatef(-10, 0.0, 0.2,0.0);
    glRotatef(angle, 0.0,0.0,1.0);

    glEnable(GL_LIGHTING);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
//start drawing
//hill-1
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots, 4 * 3, 3, &pts1[0][0][0],  4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

//hill-2

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots, 4 * 3, 3, &pts2[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

// hill-3
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_dif);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);


    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots, 4 * 3, 3, &pts3[0][0][0],4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

//hill-4

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    gluBeginSurface(nurb);
      gluNurbsSurface(nurb, 8, knots, 8, knots, 4 * 3, 3, &pts4[0][0][0], 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(nurb);

    glDisable(GL_AUTO_NORMAL);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);

    glPopMatrix();

//Clear all buffers
    glutSwapBuffers();

}

void reshape ( int w, int h )
{


    glViewport     ( 0, 0, (GLsizei)w, (GLsizei)h );

   glLoadIdentity ();
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    glLoadIdentity();
    glOrtho(0,w,0,h,-1.0,1.0);
    gluPerspective (90.0f,(GLfloat)w/(GLfloat)h,1.0f,1.0f);
    gluLookAt      ( 1.0,0.0,1.0, 0, 0, 1,0, 1, 0);

/*============================================================================================*/
/*============================================================================================*/
/*============================================================================================*/


}

void	animate ()
{
	angle +=  speed;
    glutPostRedisplay ();
}


void key ( unsigned char key, int x, int y )
{
	if ( key == 27 )		//quit requested
		exit ( 0 );
	if (key=='Q'|| key =='q')
		speed=speed-0.1f;
	if (key=='E' || key == 'e')
		speed=speed+0.1f;
    if (key==32) {
        speed=0.0;
    }
    if (key=='O'|| key == 'o') {
    effect=1.0f;
    }
    if (key=='P'|| key =='p') {
    effect=0.0f;
    }

}


int main ( int argc, char** argv )
{
//initialise glut
    glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize  ( 800, 540 );
	int glWin=glutCreateWindow ( "Planets" );
					// create window

	glutDisplayFunc  ( display );
	glutReshapeFunc  ( reshape );
	glutIdleFunc     ( animate );
 	glutKeyboardFunc ( key );


    init();

	glutMainLoop ();
	return 0;
}
