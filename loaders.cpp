#include "loaders.h"

//printf-s fpr shaders debuggin on lines: 17,45,52

// "framework-procedures" for shaders;
bool    checkOpenGLError ()
{
    bool    retCode = true;

    for ( ; ; )
    {
        GLenum  glErr = glGetError ();

        if ( glErr == GL_NO_ERROR )
            return retCode;

        printf ( "glError: %s\n", gluErrorString ( glErr ) );

        retCode = false;
        glErr   = glGetError ();
    }

    return retCode;
}

void printInfoLog ( GLhandleARB object )
{
    GLint       logLength     = 0;
    GLsizei     charsWritten  = 0;
    GLcharARB * infoLog;

    checkOpenGLError ();               	// check for OpenGL errors

    glGetObjectParameterivARB ( object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLength );

    if ( !checkOpenGLError() )              // check for OpenGL errors
        exit ( 1 );

    if ( logLength > 0 )
    {
        infoLog = (GLcharARB*) malloc ( logLength );

        if ( infoLog == NULL )
        {
            printf("ERROR: Could not allocate InfoLog buffer\n");

            exit ( 1 );
        }

        glGetInfoLogARB ( object, logLength, &charsWritten, infoLog );

        printf ( "InfoLog:\n%s\n\n", infoLog );
        free   ( infoLog );
    }

    if ( !checkOpenGLError () )             // check for OpenGL errors
        exit ( 1 );
}

bool    loadShader ( GLhandleARB shader, const char * fileName )
{
    printf ( "Loading %s\n", fileName );

	FILE * file = fopen ( fileName, "rb" );

	if ( file == NULL )
	{
		printf ( "Error opening %s\n", fileName );
		exit   ( 1 );
	}

	fseek ( file, 0, SEEK_END );

	GLint size = ftell ( file );

	if ( size < 1 )
	{
		fclose ( file );
		printf ( "Error loading file %s\n", fileName );
		exit   ( 1 );
	}

	char * buf = (char *) malloc ( size );

	fseek ( file, 0, SEEK_SET );

	if ( fread ( buf, 1, size, file ) != size )
	{
		fclose ( file );
		printf ( "Error loading file %s\n", fileName );
		exit   ( 1 );
	}

	fclose ( file );

    GLint   compileStatus;

    glShaderSourceARB ( shader, 1, (const char **) &buf, &size );

	free ( buf );
                                        // compile the particle vertex shader, and print out
    glCompileShaderARB ( shader );

    if ( !checkOpenGLError() )          // check for OpenGL errors
        return false;

    glGetObjectParameterivARB ( shader, GL_OBJECT_COMPILE_STATUS_ARB, &compileStatus );
    printInfoLog              ( shader );

    return compileStatus != 0;
}

GLhandleARB prepareShader (GLhandleARB vert, GLhandleARB frag, const char * path_vert, const char * path_frag)
{
        GLint       local_linked;
        GLhandleARB local;

                                        // create a vertex shader object and a fragment shader object
    vert   = glCreateShaderObjectARB ( GL_VERTEX_SHADER_ARB   );
    frag = glCreateShaderObjectARB ( GL_FRAGMENT_SHADER_ARB );

                                        // load source code strings into shaders
    if ( !loadShader ( vert, path_vert ) )
        exit ( 1 );

    if ( !loadShader ( frag, path_frag ) )
        exit ( 1 );
                                        // create a program object and attach the
                                        // two compiled shaders
    local = glCreateProgramObjectARB ();

    glAttachObjectARB ( local, vert   );
    glAttachObjectARB ( local, frag );

                                        // link the program object and print out the info log
    glLinkProgramARB ( local );

    if ( !checkOpenGLError() )          // check for OpenGL errors
        exit ( 1 );

    glGetObjectParameterivARB ( local, GL_OBJECT_LINK_STATUS_ARB, &local_linked );

    printInfoLog ( local);
    if ( !local_linked )
        exit(0);

    return local;

}


/*  procedure to load textures from bmp-files */
texture_info* TextureObs(const char* path) {
     FILE* input;
     FILE* test;
     unsigned char tmp;
     texture_info* output;
     int other;
     int idx_1, idx_2;
     input = fopen(path, "rb");
     if (input == NULL) {
         exit(3);
         return NULL;
     }
     test = fopen("test.txt","w");
     /*preparing the structure where the texture
      and its parametres will be stored*/
     output = (texture_info *) malloc (sizeof(texture_info));
     /*analysing the image*/
     /*reading BM*/
     fgetc(input);
     fgetc(input);
     /*reading filesize*/
     tmp = fgetc(input);
     other = tmp;
     tmp = fgetc(input);
     other += (tmp << 8);
     tmp = fgetc(input);
     other += (tmp << 16);
     tmp = fgetc(input);
     other += (tmp << 24);
     fprintf(test,"%d%s",other,"\n");
     /*reading two reserved spaces, an offset and the size of info header*/
     for (idx_1 = 0; idx_1 <= 11; idx_1++) fgetc(input);
     /*reading width of the image*/
     tmp = fgetc(input);
     output->width = tmp;
     tmp = fgetc(input);
     output->width += (tmp << 8);
     tmp = fgetc(input);
     output->width += (tmp << 16);
     tmp = fgetc(input);
     output->width += (tmp << 24);
     fprintf(test,"%d%s",output->width,"\n");
     /*reading height of the image*/
     tmp = fgetc(input);
     output->height = tmp;
     tmp = fgetc(input);
     output->height += (tmp << 8);
     tmp = fgetc(input);
     output->height += (tmp << 16);
     tmp = fgetc(input);
     output->height += (tmp << 24);
     fprintf(test,"%d%s",output->height,"\n");
     /*reserved field*/
     fgetc(input);
     fgetc(input);
     /*read color depth*/
     tmp = fgetc(input);
     other = tmp;
     tmp = fgetc(input);
     other += (tmp << 8);
     fprintf(test,"%d%s",other,"\n");
     //if (other != 24) return NULL;
     /*read compression*/
     tmp = fgetc(input);
     other = tmp;
     tmp = fgetc(input);
     other += (tmp << 8);
     tmp = fgetc(input);
     other += (tmp << 16);
     tmp = fgetc(input);
     other += (tmp << 24);
     fprintf(test,"%d%s",other,"\n");
     if (other != 0) {
         printf("%s","Comression!!!");
         return NULL;
     }
     for (idx_1 = 0; idx_1 <= 19; idx_1++) fgetc(input);
     /*reading the image itself*/
     output->img_data = (unsigned char*) malloc (output->width * output->height * 3 * sizeof(unsigned char));
     for (idx_1 = 0; idx_1 <= output->height-1; idx_1++)
         for (idx_2 = 0; idx_2 <= output->width-1; idx_2++) {
             output->img_data[(idx_1 * output->width * 3) + (idx_2 * 3) + 2] = fgetc(input);
             output->img_data[(idx_1 * output->width * 3) + (idx_2 * 3) + 1] = fgetc(input);
             output->img_data[(idx_1 * output->width * 3) + (idx_2 * 3)] = fgetc(input);
         }
     return output;
     fclose(input);
     fclose(test);
}

GLuint	loadTexture (const  char * fileName )
{
    texture_info* raw_image;
    GLuint texture_stor;
    glGenTextures(1,&texture_stor);
    /*Create cracked concrete texture*/
    raw_image = TextureObs(fileName);
    if (raw_image == NULL) exit(2);

    glBindTexture(GL_TEXTURE_2D, texture_stor);
    //glTexImage2D(GL_TEXTURE_2D, 0, 3, raw_image->width, raw_image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, raw_image->img_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    gluBuild2DMipmaps ( GL_TEXTURE_2D, GL_RGB,
	                raw_image -> width, raw_image -> height,
		            GL_RGB, GL_UNSIGNED_BYTE,
		            raw_image -> img_data );


    free(raw_image);
    return texture_stor;
}
