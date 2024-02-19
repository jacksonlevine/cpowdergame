#ifndef PIPELINE_H
#define PIPELINE_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "utils.h"

void createShader(const char *vertPath, const char *fragPath, GLuint *out, const char *readableName);

#endif