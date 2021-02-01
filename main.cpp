#include <iostream>
//obavezno ide prvo glad pa onda glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include "include/Shader.hpp"
#include "include/Error.hpp"


void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

//screen settings
const unsigned int SCR_WIDTH = 1400;
const unsigned int SCR_HEIGHT = 900;

int main() {
    glfwInit();
    // OpenGL 3.3 core

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH,SCR_HEIGHT,"Target Shooting", nullptr, nullptr);
    if(window == nullptr){
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate(); //deinicijalizacija glfw-a
        return EXIT_FAILURE;
    }

    //sada kazemo opengl da zelimo da crta u ovom prozoru
    glfwMakeContextCurrent(window);
    //kazemo opengl dimenzije za renderovanje
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    //funkcija koja registruje pritiske na tastaturu(callback) i automatski se poziva
    glfwSetKeyCallback(window,key_callback);


    //sada zovemo glad biblioteku da ucita sve nase opengl funkcije!
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD\n" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }


    Shader shader("../resources/shaders/vertexShader.vs",
                  "../resources/shaders/fragmentShader.fs");



    float vertices[] = {
            0.5f,0.5f,0.0f,  //top right
            0.5f,-0.5f,0.0f, // bottom right
            -0.5f,-0.5f,0.0f, // bottom left
            -0.5f,0.5f,0.0f  // top left
    };

    unsigned indices[] = {
            //first triangle
            0,1,3,
            //second triangle
            1,2,3
    };

    unsigned int VAO,VBO,EBO;
    //kreirmao objekat koji ce da kaze sta znace podaci iz VBO
    glGenVertexArrays(1,&VAO);
    //kreiramo baffer preko kog saljemo podatke ka gpu
    glGenBuffers(1,&VBO);
    //kreiramo element buffer koji nam predstavlja redne brojeve trougla
    glGenBuffers(1,&EBO);
    //aktiviramo ovaj objekat
    glBindVertexArray(VAO);


    //aktiviramo ovaj bafer
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //smestamo podatke u ovaj bafer
    GLCALL(glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW));


    //aktiviramo ga
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    //smestamo podatke u njega
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


    //kazemo sta znace podaci iz vbo
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);



    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    //RENDER loop
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //postavljamo boju
        glClearColor(0.2, 0.3, 0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);



        shader.use();
        shader.setUniform4f("gColor",sin(glfwGetTime() / 2.0 + 0.5), 0.0, 0.0, 1.0);


        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
    }

    //brisemo sve objekte koji nam vise nisu potrebni
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);

    shader.deleteProgram();


    glfwTerminate();
    return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
    //R
    if(key == GLFW_KEY_R && action == GLFW_PRESS){
        glClearColor(1.0,0.0,0.0,1.0);
    }
    //B
    if(key == GLFW_KEY_B && action == GLFW_PRESS){
        glClearColor(0.0,0.0,1.0,1.0);
    }
    //G
    if(key == GLFW_KEY_G && action == GLFW_PRESS){
        glClearColor(0.0,1.0,0.0,1.0);
    }
}
void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}
