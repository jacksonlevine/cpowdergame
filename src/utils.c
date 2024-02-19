#include "utils.h"

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