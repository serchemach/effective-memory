#include <stdlib.h>
#include <stdio.h>

#include "RenderGL.h"
#include "RenderGUI.h"

GLuint modelTex;
GLuint modelCoordVBO, modelUVVBO, modelVerts;

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

void loadPreviewModel(char* mdl) {
	cvector_vector_type(float) verts = {0};
	cvector_vector_type(float) uvs = {0};
	cvector_vector_type(float) norms = {0};
	
	int error = loadObj(mdl, &verts, &uvs, &norms);
	if(error) return;
	
	glDeleteBuffers(1, &modelCoordVBO);
	glDeleteBuffers(1, &modelUVVBO);
	
	modelCoordVBO = 0;
	glGenBuffers(1, &modelCoordVBO); //Creates a VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, modelCoordVBO); //Loads the current VBO to store the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cvector_size(verts), verts, GL_STATIC_DRAW);
	
	modelUVVBO = 0;
	glGenBuffers(1, &modelUVVBO); //Creates a VBO ID
	glBindBuffer(GL_ARRAY_BUFFER, modelUVVBO); //Loads the current VBO to store the data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * cvector_size(uvs), uvs, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	modelVerts = cvector_size(verts) / 3;
	
	cvector_free(verts);
	cvector_free(uvs);
	cvector_free(norms);
}

void loadPreviewTexture(char* tex) {
	
	SDL_Surface *image = IMG_Load(tex);
	
	if(image) {
		SDL_LockSurface(image);
		if(image->format->format == SDL_PIXELFORMAT_RGB888 || image->format->format == SDL_PIXELFORMAT_RGBA8888 || image->format->format == SDL_PIXELFORMAT_RGBA32) {
			destroyTexture(modelTex);
			modelTex = createTexture(image->w, image->h, image->pixels, image->format->format == SDL_PIXELFORMAT_RGB888?GL_RGB:GL_RGBA);
		}
		SDL_FreeSurface(image);
	}
}

void destroyGLRender() {
	glDeleteBuffers(1, &modelCoordVBO);
	glDeleteBuffers(1, &modelUVVBO);
	
	glDeleteBuffers(1, &rectCoordVBO);
	glDeleteBuffers(1, &rectUVVBO);
	
	destroyTexture(hudTex);
	destroyTexture(modelTex);
}

void renderGL(struct Quaternion quat, int w, int h, SDL_Surface* hud, int hudX, int hudY) {
	glViewport(hudX + hud->w, hudY, w - hud->w, h);
	glEnable(GL_TEXTURE_2D);
	glClearColor(BG_BRIGHTNESS_3D / 255.f, BG_BRIGHTNESS_3D / 255.f, BG_BRIGHTNESS_3D / 255.f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//3d mode
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (float)(w - hud->w) / h, 1, 40000);
	
	drawMdl(quat, modelTex, modelCoordVBO, modelUVVBO, modelVerts);
	
	//2d mode
	glViewport(hudX, hudY, hud->w, hud->h);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, hud->w, hud->h, 0, 0, 40000);
	
	updateTexture(hudTex, hud->w, hud->h, hud->pixels, GL_RGBA);
	
	drawGLRect(hudTex, hudX, hudY, hud->w, hud->h, 1, 1, 1, 1);
}

void drawMdl(struct Quaternion quat, GLuint tex, GLuint posVBO, GLuint uvVBO, GLsizei verts) {
	float rotMat[3][3] = {0};
	Quaternion_toRotationMatrix(quat, rotMat);
	
	float glMat[16] = {0};
	glMat[0] = rotMat[0][0];
	glMat[4] = rotMat[1][0];
	glMat[8] = rotMat[2][0];
	glMat[12] = 0; //x offset
	
	glMat[1] = rotMat[0][1];
	glMat[5] = rotMat[1][1];
	glMat[9] = rotMat[2][1];
	glMat[13] = 0; //y offset
	
	glMat[2] = -rotMat[0][2];
	glMat[6] = -rotMat[1][2];
	glMat[10] = -rotMat[2][2];
	glMat[14] = -700; //z offset
	
	glMat[15] = 1; //w
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixf(glMat);
	
	if(tex) bindGLTex(tex, false, false);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glVertexPointer(3, GL_FLOAT, 0, 0);
        
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);
        
	glDrawArrays(GL_TRIANGLES, 0, verts);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if(tex) unbindGLTex(tex);
    
	glPopMatrix();
}

void drawGLRect(GLuint tex, float x, float y, float w, float h, float r, float g, float b, float a) {
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