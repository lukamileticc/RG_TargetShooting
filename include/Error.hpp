#ifndef RG_TARGETSHOOTING_ERROR_HPP
#define RG_TARGETSHOOTING_ERROR_HPP

#include "glad/glad.h"
#include <iostream>


#define ASSERT(x,msg)              \
do{                                \
if(!(x)){                          \
    std::cerr << msg << std::endl; \
    BREAK_IF_FALSE(false);         \
}                                  \
}while(0);

#define BREAK_IF_FALSE(x)          \
if(!(x)) __builtin_trap();

//ovaj makro pozivamo da proverimo da li imamo gresku u nekoj funkciji
#define GLCALL(x) \
{ gl_clear_error(); x; BREAK_IF_FALSE(gl_log_call(__FILE__, __LINE__, #x)); }



//funkcija koja skida greske sa gl_steka
void gl_clear_error(){
    while(glGetError() != GL_NO_ERROR) {}
}

const char* gl_error_to_string(GLenum error) {
    switch (error) {
        case GL_NO_ERROR : return "GL_NO_ERROR";
        case GL_INVALID_ENUM: return  "GL_INVALID_ENUM";
        case GL_INVALID_VALUE: return  "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return  "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION : return  "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    }

    ASSERT(false, "Passed something that is not an error code");

    return "THIS_SHOULD_NEVER_HAPPEN";
}

//funkcija za detekciju greske(file,linija,funkcija)
bool gl_log_call(const char *file, int line, const char *call) {

    bool success = true;
    while(GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] "  << error << std::endl << "Msg: " << gl_error_to_string(error) << std::endl;
        std::cerr << "File: " << file << std::endl;
        std::cerr << "Line: " << line << std::endl;
        std::cerr << "Call: " << call << std::endl;

        success = false;
    }
    //ukolilko nema greske izlazimo napolje i vracamo
    return success;
}

#endif //RG_TARGETSHOOTING_ERROR_HPP
