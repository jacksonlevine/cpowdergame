#include "pipeline.h"

void createShader(const char *vertPath, const char *fragPath, GLuint *out, const char *readableName) {
    const char* vertexText = loadFile(vertPath);
    const char* fragmentText = loadFile(fragPath);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexText, NULL);
    glShaderSource(fragmentShader, 1, &fragmentText, NULL);

    GLint success;
    GLchar infoLog[512];

    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        fprintf(stderr, "%s ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", readableName, infoLog);
    }
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "%s ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", readableName, infoLog);
    }

    *out = glCreateProgram();

    glAttachShader(*out, vertexShader);
    glAttachShader(*out, fragmentShader);

    glLinkProgram(*out);
    glGetProgramiv(*out, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*out, 512, NULL, infoLog);
        fprintf(stderr, "%s ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", readableName, infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free((void*)vertexText);
    free((void*)fragmentText);
}