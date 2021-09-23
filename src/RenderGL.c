#include <stdlib.h>
#include <stdio.h>

#include "RenderGL.h"

GLuint modelTex;
GLuint modelCoordVBO, modelUVVBO;

GLuint hudTex;
GLuint rectCoordVBO, rectUVVBO;

void startGLRender(SDL_Surface* hud) {
	modelTex = 0;
	modelCoordVBO = 0;
	modelUVVBO = 0;
	
	rectCoordVBO = 0;
	glGenBuffers(1, &rectCoordVBO); //Creates a VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, rectCoordVBO); //Loads the current VBO to store the data
	short rectPos[12] = {
			0, 0, 0, 1, 0, 0,
			1, 1, 0, 0, 1, 0
		};
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectPos), rectPos, GL_STATIC_DRAW);
	
	rectUVVBO = 0;
	glGenBuffers(1, &rectUVVBO); //Creates a VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, rectUVVBO); //Loads the current VBO to store the data
	short rectUV[8] = {
			0, 0, 1, 0,
			1, 1, 0, 1
		};
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectUV), rectUV, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	hudTex = createTexture(hud->w, hud->h, 0, GL_RGBA);
}

void destroyGLRender() {
	glDeleteBuffers(1, &rectCoordVBO);
	glDeleteBuffers(1, &rectUVVBO);
	destroyTexture(hudTex);
}

void renderGL(int w, int h, SDL_Surface* hud, int hudX, int hudY) {
	//2d mode
	glViewport(0, 0, w, h);
	
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, h, 0, 0, 40000);
	
	//just testing
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	updateTexture(hudTex, hud->w, hud->h, hud->pixels, GL_RGBA);
	
	drawGLRect(hudTex, hudX, hudY, hud->w, hud->h, 1, 1, 1, 1);
}

void drawGLRect(GLenum tex, float x, float y, float w, float h, float r, float g, float b, float a) {
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	glScalef(w, h, 0);
	
	glColor4f(r, g, b, a);
	
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if(tex) bindGLTex(tex, false, false);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, rectCoordVBO);
	glVertexPointer(3, GL_SHORT, 0, 0);
        
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, rectUVVBO);
	glTexCoordPointer(2, GL_SHORT, 0, 0);
        
	glDrawArrays(GL_QUADS, 0, 4);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if(tex) unbindGLTex(tex);
	
	glDisable(GL_BLEND);
	
	glColor4f(1, 1, 1, 1);
    
	glPopMatrix();
}

GLuint createTexture(int w, int h, void* data, GLenum format) {
	GLuint tex = 0;
	glGenTextures(1, &tex);
	
	glBindTexture(GL_TEXTURE_2D, tex);
	if(data) glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return tex;
}

void updateTexture(GLuint tex, int w, int h, void* data, GLenum format) {
	glGetError();
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, format?format:GL_RGBA, GL_UNSIGNED_BYTE, data);
	GLenum err = glGetError();
	if(err != 0) printf("err: %d\n", err);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void destroyTexture(GLuint tex) {
	glDeleteTextures(1, &tex);
}

void bindGLTex(GLenum tex, bool linear, bool mipmap) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	
	GLint mag = linear ? GL_LINEAR : GL_NEAREST;
	GLint interp = (mipmap) ?
                (linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR)
                : mag;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interp);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void unbindGLTex(GLenum tex) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}