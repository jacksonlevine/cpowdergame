#include <stdio.h>
#include <stdlib.h>


#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "pipeline.h"
#include "gamefacts.h"
#include "particlefuncs.h"
#include "bitfunctions.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

GLFWwindow *WINDOW;
GLuint FORESHADER;
GLuint BACKSHADER;

unsigned char BACKPIXELS[GAMEWIDTH*GAMEHEIGHT];
unsigned char FOREPIXELS[GAMEWIDTH*GAMEHEIGHT];

GLuint BACKTEXTURE;
GLuint FORETEXTURE;

unsigned char *BACKGROUNDIMAGE;
int BIWIDTH, BIHEIGHT, BICHANS;

bool UP = false;
bool DOWN = false;
bool LEFT = false;
bool RIGHT = false;

int CAMERAPOSX = 0;
int CAMERAPOSY = 0;

//for determining deltaTime
float lastFrame = 0;
float deltaTime = 0;

bool MOUSE_CLICKED = false;
bool MOUSE_RIGHTCLICKED = false;
double MOUSEX = 0;
double MOUSEY = 0;

unsigned char SELECTED_TYPE = 1;

int CURRENTWINDOWWIDTH = WINDOWWIDTH;
int CURRENTWINDOWHEIGHT = WINDOWHEIGHT;

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

void stampBackPixelsToFront();

void updateDeltaTime() {
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void bufferBackgroundImage(int offsetX, int offsetY) {
    for(int j = 0; j < GAMEHEIGHT; j++) {
        for(int i = 0; i < GAMEWIDTH; i++) {
            int biIndex = abs((offsetX) + (offsetY*BIWIDTH) + (j * BIWIDTH) + (i)) % (BIWIDTH * BIHEIGHT);
            int pixelsIndex = j * GAMEWIDTH + i;

            BACKPIXELS[pixelsIndex] = BACKGROUNDIMAGE[biIndex];
        }
    }
}

void saveToImage() {
    stbi_write_png("image.png", GAMEWIDTH, GAMEHEIGHT, 1, FOREPIXELS, GAMEWIDTH);
}

void loadFromImage(const char* path) {
    int w, h, ch;
    unsigned char *data = stbi_load(path, &w, &h, &ch, 1);
    memcpy(FOREPIXELS, data, GAMEWIDTH*GAMEHEIGHT);
    STBI_FREE(data);
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    int i;
    for (i = 0;  i < count;  i++)
        loadFromImage(paths[i]);
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
    if(key == GLFW_KEY_G) {
        stampBackPixelsToFront();
    }
    if(key == GLFW_KEY_1) {
        SELECTED_TYPE = 1;
    }
    if(key == GLFW_KEY_2) {
        SELECTED_TYPE = 2;
    }
    if(key == GLFW_KEY_3) {
        SELECTED_TYPE = 3;
    }
    if(key == GLFW_KEY_M) {
        saveToImage();
    }
}

bool moveBackground() {
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

void updatePowderSimulation() {
    static bool isOddFrame = false;

    for(int j = GAMEHEIGHT-1; j > 0; j--) {
        for(int i = GAMEWIDTH-1; i > GAMEWIDTH/2; i--) {

            int index = j * GAMEWIDTH + i;

            bool aboveBottom = j > 1;

            unsigned char* theByte = &FOREPIXELS[index];
            if(aboveBottom) {

                if(getColorBits(*theByte) != 0 && (isOddBit(*theByte) == isOddFrame)) {
                    setOddBit(theByte, isOddFrame ? 0 : 1);
                
                    (*PARTICLEFUNCS[getColorBits(*theByte)])(FOREPIXELS, index, isOddFrame);
                }
                
            }

        }
    }

    for(int j = GAMEHEIGHT-1; j > 0; j--) {
        for(int i = 0; i <= GAMEWIDTH/2; i++) {

            int index = j * GAMEWIDTH + i;

            bool aboveBottom = j > 1;

            unsigned char* theByte = &FOREPIXELS[index];
            if(aboveBottom) {

                if(getColorBits(*theByte) != 0 && (isOddBit(*theByte) == isOddFrame)) {
                    setOddBit(theByte, isOddFrame ? 0 : 1);
                
                    (*PARTICLEFUNCS[getColorBits(*theByte)])(FOREPIXELS, index, isOddFrame);
                }
                
            }

        }
    }
    isOddFrame = !isOddFrame;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS) {
            MOUSE_CLICKED = true;
        } else if (action == GLFW_RELEASE) {
            MOUSE_CLICKED = false;
        }
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if(action == GLFW_PRESS) {
            MOUSE_RIGHTCLICKED = true;
        } else if (action == GLFW_RELEASE) {
            MOUSE_RIGHTCLICKED = false;
        }
    }
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    MOUSEX = xpos;
    MOUSEY = ypos;
}

void drawForegroundPixels(double xpos, double ypos, unsigned char type) {
    static int drawPixels[] = {
        0, 0,
        1, 0,
        -1, 0,
        0, 1,
        0, -1,
        -1, 1,
        -1, -1,
        1, 1,
        1, -1,
        -2, 0,
        -2, 1,
        -2, -1,
        2, 0,
        2, 1,
        2, -1,
        0, 2,
        1, 2,
        -1, 2,
        0, -2,
        1, -2,
        -1, -2
    };

    int yp = CURRENTWINDOWHEIGHT - (int)ypos;

    int dx, dy;
    dx = (int)(xpos * ((float)GAMEWIDTH / CURRENTWINDOWWIDTH));
    dy = (int)(yp * ((float)GAMEHEIGHT / CURRENTWINDOWHEIGHT));
    
    static bool liqTravLeft = false; //only for liquids. Make every other pixel travel left, the rest travel right.

    for(int i = 0; i < sizeof(drawPixels)/sizeof(int); i+= 2) {
        int index = (dy+drawPixels[i+1]) * GAMEWIDTH + dx + drawPixels[i];

        unsigned char byte = type;
        
        if(liqTravLeft) {
            byte |= liquidTravLeftBit;
        }

        if(index > 0 && index <= GAMEWIDTH*GAMEHEIGHT - 1) {
            FOREPIXELS[index] = byte;
        }
        
    }
    liqTravLeft = !liqTravLeft;
}

void stampBackPixelsToFront() {
    for(int j = 0; j < GAMEHEIGHT; j++) {
        for(int i = 0; i < GAMEWIDTH; i++) {
            int index = j * GAMEWIDTH + i;
            if(getColorBits(FOREPIXELS[index]) == 0) {
                FOREPIXELS[index] = BACKPIXELS[index];
            }
        }
    }
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    CURRENTWINDOWWIDTH = width;
    CURRENTWINDOWHEIGHT = height;
}

int main() {

    glfwInit();

    WINDOW = glfwCreateWindow(WINDOWWIDTH, WINDOWHEIGHT, "CPowder Game", NULL, NULL);

    glfwMakeContextCurrent(WINDOW);
    glewInit();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glfwSetMouseButtonCallback(WINDOW, mouse_button_callback);
    glfwSetCursorPosCallback(WINDOW, cursor_position_callback);
    glfwSetKeyCallback(WINDOW, keyCallback);
    glfwSetFramebufferSizeCallback(WINDOW, framebuffer_size_callback);
    glfwSetDropCallback(WINDOW, drop_callback);

    createShader("assets/shaders/vert.glsl", "assets/shaders/backfrag.glsl", &BACKSHADER, "Background shader");
    createShader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl", &FORESHADER, "Foreground shader");

    glGenTextures(1, &BACKTEXTURE);
    glBindTexture(GL_TEXTURE_2D, BACKTEXTURE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, GAMEWIDTH, GAMEHEIGHT, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &FORETEXTURE);
    glBindTexture(GL_TEXTURE_2D, FORETEXTURE);
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

    GLuint foreVAO;
    GLuint foreVBO;
    glGenVertexArrays(1, &foreVAO);
    glGenBuffers(1, &foreVBO);

    glBindVertexArray(foreVAO);
    glBindBuffer(GL_ARRAY_BUFFER, foreVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    //position attribute
    GLint forepos_attrib = glGetAttribLocation(FORESHADER, "pos");
    glEnableVertexAttribArray(forepos_attrib);
    glVertexAttribPointer(forepos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //texture coord attribute
    GLint foretex_attrib = glGetAttribLocation(FORESHADER, "texcoord");
    glEnableVertexAttribArray(foretex_attrib);
    glVertexAttribPointer(foretex_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glUseProgram(FORESHADER);
    glUniform3fv(glGetUniformLocation(FORESHADER, "colorPalette"), 16, (const float*)&COLORPALETTE[0]);


    GLuint backVAO;
    GLuint backVBO;
    glGenVertexArrays(1, &backVAO);
    glGenBuffers(1, &backVBO);

    glBindVertexArray(backVAO); 
    glBindBuffer(GL_ARRAY_BUFFER, backVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    //position attribute
    GLint backpos_attrib = glGetAttribLocation(FORESHADER, "pos");
    glEnableVertexAttribArray(backpos_attrib);
    glVertexAttribPointer(backpos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    //texture coord attribute
    GLint backtex_attrib = glGetAttribLocation(FORESHADER, "texcoord");
    glEnableVertexAttribArray(backtex_attrib);
    glVertexAttribPointer(backtex_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glUseProgram(BACKSHADER);
    glUniform3fv(glGetUniformLocation(BACKSHADER, "colorPalette"), 16, (const float*)&COLORPALETTE[0]);


    //load the background image
    BACKGROUNDIMAGE = stbi_load("assets/space.png", &BIWIDTH, &BIHEIGHT, &BICHANS, 1);

    float movementPad = 0.015;
    float movementTimer = 0;
    
    bufferBackgroundImage(CAMERAPOSX,CAMERAPOSY);
    
    while (!glfwWindowShouldClose(WINDOW)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        if(movementTimer > movementPad) {
            
            if(moveBackground()) {
                bufferBackgroundImage(CAMERAPOSX,CAMERAPOSY);
                glBindTexture(GL_TEXTURE_2D, BACKTEXTURE);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GAMEWIDTH, GAMEHEIGHT, GL_RED, GL_UNSIGNED_BYTE, BACKPIXELS);
            }

            updatePowderSimulation();
            
            movementTimer = 0;
        } else {
            movementTimer += deltaTime;
        }

        glUseProgram(BACKSHADER);
        glBindVertexArray(backVAO);
        glBindBuffer(GL_ARRAY_BUFFER, backVBO);
        glBindTexture(GL_TEXTURE_2D, BACKTEXTURE);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GAMEWIDTH, GAMEHEIGHT, GL_RED, GL_UNSIGNED_BYTE, BACKPIXELS);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if(MOUSE_CLICKED) {
            drawForegroundPixels(MOUSEX, MOUSEY, SELECTED_TYPE);
        }
        if(MOUSE_RIGHTCLICKED) {
            drawForegroundPixels(MOUSEX, MOUSEY, 0);
        }

        glUseProgram(FORESHADER);
        glBindVertexArray(foreVAO);
        glBindBuffer(GL_ARRAY_BUFFER, foreVBO);
        glBindTexture(GL_TEXTURE_2D, FORETEXTURE);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, GAMEWIDTH, GAMEHEIGHT, GL_RED, GL_UNSIGNED_BYTE, FOREPIXELS);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(WINDOW);
        glfwPollEvents();
        updateDeltaTime();
    }

    return 0;
}