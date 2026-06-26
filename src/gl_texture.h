// gl_texture.h  Cross-vendor D3D11-to-OpenGL texture upload
// Simple synchronous glTexSubImage2D — no PBO, no async.
// Works on NVIDIA / AMD / Intel.
#pragma once
#include <GL/gl.h>

struct GLTextureUpload {
    GLuint tex      = 0;
    int    width    = 0;
    int    height   = 0;
    bool   active   = false;
};

bool GLTex_Init(GLTextureUpload& gt, int width, int height);

void GLTex_Upload(GLTextureUpload& gt, const void* data, int stride);

bool GLTex_Resize(GLTextureUpload& gt, int width, int height);

GLuint GLTex_GetTexture(const GLTextureUpload& gt);

void GLTex_Shutdown(GLTextureUpload& gt);