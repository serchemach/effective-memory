#pragma once

#include <stdbool.h>

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_opengl.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>

#include "ObjLoader.h"

#include "cvector.h"

void startGLRender(SDL_Surface* hud);
void destroyGLRender();

void loadPreviewModel(char* mdl);
void loadPreviewTexture(char* tex);

void renderGL(int w, int h, SDL_Surface* hud, int hudX, int hudY);

void drawMdl(GLuint tex);
void drawGLRect(GLuint tex, float x, float y, float w, float h, float r, float g, float b, float a);

GLuint createTexture(int w, int h, void* data, GLenum format);
void updateTexture(GLuint tex, int w, int h, void* data, GLenum format);
void destroyTexture(GLuint tex);

void bindGLTex(GLenum tex, bool linear, bool mipmap);
void unbindGLTex(GLenum tex);