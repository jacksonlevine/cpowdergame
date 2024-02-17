#include <stdio.h>
#include <stdlib.h>


#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLFWwindow *WINDOW;
GLuint SHADERPROG;

#define GAMEWIDTH 500
#define GAMEHEIGHT 500

unsigned char PIXELS[GAMEWIDTH*GAMEHEIGHT];
GLuint GAMETEXTURE;

unsigned char *BACKGROUNDIMAGE;
int BIWIDTH, BIHEIGHT, BICHANS;

bool UP = false;
bool DOWN = false;
bool LEFT = false;
bool RIGHT = false;

int CAMERAPOSX = 0;
int CAMERAPOSY = 0;

float COLORPALETTE[16 * 3]  = {
    0.0f, 0.0f, 0.0f,//black 0
    0.6f, 0.5f, 0.3f,//tan 1
    0.0f, 0.0f, 1.0f,//blue 2
    0.5f, 0.5f, 0.5f,//grey 3
    1.0f, 0.5f, 0.0f,//orange 4
    0.0f, 1.0f, 1.0f,//cyan 5
    0.0f, 0.5f, 1.0f,//teal 6
    1.0f, 0.0f, 1.0f,//magenta 7
    0.5f, 0.0f, 0.6f,//purple 8
    1.0f, 1.0f, 0.0f,//yellow 9
    1.0f, 0.0f, 0.0f,//red 10
    0.0f, 1.0f, 0.0f,//green 11
    1.0f, 1.0f, 1.0f,//white 12
    1.0f, 1.0f, 1.0f,//white 13
    1.0f, 1.0f, 1.0f,//white 14
    1.0f, 1.0f, 1.0f//white 15
};

char* loadFile(const char* path) {
    FILE* file = fopen(path, "r");
    if(file == NULL) {
        fprintf(stderr, "Error opening file %s \n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(length + sizeof(char));
    memset(buffer, 0, length + sizeof(char));

    fread(buffer, sizeof(char), length, file);
    buffer[length] = '\0';
 
    fclose(file);
    return buffer;
}

void bufferBackgroundImage(int offsetX, int offsetY) {
    for(int j = 0; j < GAMEHEIGHT; j++) {
        for(int i = 0; i < GAMEWIDTH; i++) {
            int biIndex = abs((offsetX) + (offsetY*BIWIDTH) + (j * BIWIDTH) + (i)) % (BIWIDTH * BIHEIGHT);
            int pixelsIndex = j * GAMEWIDTH + i;

            PIXELS[pixelsIndex] = BACKGROUNDIMAGE[biIndex];
        }
    }
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_W) {
        UP = action;
    }
    if(key == GLFW_KEY_A) {
        LEFT = action;
    }
    if(key == GLFW_KEY_S) {
        DOWN = action;
    }
    if(key == GLFW_KEY_D) {
        RIGHT = action;
    }
}

bool reactToInput() {
    bool moved = false;
    if(UP) {
        CAMERAPOSY += 1;
        moved = true;
    }
    if(DOWN) {
        CAMERAPOSY -= 1;
        moved = true;
    }
    if(LEFT) {
        CAMERAPOSX -= 1;
        moved = true;
    }
    if(RIGHT) {
        CAMERAPOSX += 1;
        moved = true;
    }
    return moved;
}
unsigned char colorBits = 0b00001111;
unsigned char oddBit = 0b00010000;

unsigned char getColorBits(unsigned char byte) {
    return (byte & colorBits);
}

bool isOddBit(unsigned char byte) {
    return (byte & oddBit);
}

void setOddBit(unsigned char *byte, unsigned char bit) {
    *byte &= ~oddBit;
    *byte |= (bit ? oddBit : 0);
}

void updatePowderSimulation() {
    static bool isOddFrame = false;

    for(int j = 0; j < GAMEHEIGHT; j++) {
        for(int i = 0; i < GAMEWIDTH; i++) {

            int index = j * GAMEWIDTH + i;

            bool aboveBottom = j > 1;

            unsigned char* theByte = &PIXELS[index];

            if(getColorBits(*theByte) != 0 && (isOddBit(*theByte) == isOddFrame)) {
                setOddBit(theByte, isOddFrame ? 1 : 0);
                if(aboveBottom) {
                    if(getColorBits(PIXELS[index - GAMEWIDTH]) == 0) {
                        PIXELS[index - GAMEWIDTH] = *theByte;
                        *theByte = 0;
                    } else {
                        if(isOddFrame) {
                            if(getColorBits(PIXELS[index - GAMEWIDTH - 1]) == 0) {
                                PIXELS[index - GAMEWIDTH - 1] = *theByte;
                                *theByte = 0;
                            }
                        } else {
                            if(getColorBits(PIXELS[index - GAMEWIDTH + 1]) == 0) {
                                PIXELS[index - GAMEWIDTH + 1] = *theByte;
                                *theByte = 0;
                            }
                        }
                    }
                }
            }

        }
    }

    isOddFrame = !isOddFrame;
}


int main() {

    glfwInit();

    WINDOW = glfwCreateWindow(800, 600, "CPowder Game", NULL, NULL);

    glfwMakeContextCurrent(WINDOW);
    glewInit();

    glfwSetKeyCallback(WINDOW, keyCallback);

    const char* vertexText = loadFile("assets/shaders/vert.glsl");
    const char* fragmentText = loadFile("assets/shaders/frag.glsl");

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
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    SHADERPROG = glCreateProgram();

    glAttachShader(SHADERPROG, vertexShader);
    glAttachShader(SHADERPROG, fragmentShader);

    glLinkProgram(SHADERPROG);
    glGetProgramiv(SHADERPROG, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(SHADERPROG, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free((void*)vertexText);
    free((void*)fragmentText);

    glGenTextures(1, &GAMETEXTURE);
    glBindTexture(GL_TEXTURE_2D, GAMETEXTURE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, GAMEWIDTH, GAMEHEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    float quadVertices[] = {
        //positions   //texture coords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,   1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,   1.0f, 0.0f,
        1.0f,  1.0f,   1.0f, 1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    //position attribute
    GLint pos_attrib = glGetAttribLocation(SHADERPROG, "pos");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //texture coord attribute
    GLint tex_attrib = glGetAttribLocation(SHADERPROG, "texcoord");
    glEnableVertexAttribArray(tex_attrib);
    glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glUseProgram(SHADERPROG);
    glUniform3fv(glGetUniformLocation(SHADERPROG, "colorPalette"), 16, (const float*)&COLORPALETTE[0]);

    //load the background image
    BACKGROUNDIMAGE = stbi_load("assets/space.png", &BIWIDTH, &BIHEIGHT, &BICHANS, 1);

    bufferBackgroundImage(CAMERAPOSX,CAMERAPOSY);

    int movementPad = 25;
    int movementTimer = 0;
 
    

    while (!glfwWindowShouldClose(WINDOW)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        if(movementTimer > movementPad) {
            if(reactToInput()) {
                bufferBackgroundImage(CAMERAPOSX,CAMERAPOSY);
            } else {
                updatePowderSimulation();
            }
            movementTimer = 0;
        } else {
            movementTimer += 1;
        }
        
        

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GAMEWIDTH, GAMEHEIGHT, GL_RED, GL_UNSIGNED_BYTE, PIXELS);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(WINDOW);
        glfwPollEvents();
    }

    return 0;
}