#include <iostream>
//obavezno ide prvo glad pa onda glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    glfwInit();
    // OpenGL 3.3 core

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1200,800,"Target Shooting", nullptr, nullptr);
    if(window == nullptr){
        std::cout << "Failed to create initial window" << std::endl;
        glfwTerminate(); //deinicijalizacija glfw-a
        exit(EXIT_FAILURE);
    }
    //sada kazemo opengl da zelimo da crta u ovom prozoru
    glfwMakeContextCurrent(window);
    //kazemo opengl dimenzije za renderovanje
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    //funkcija koja automatski registruje pritiske na tastaturu(callback) i automatski se poziva
    glfwSetKeyCallback(window,key_callback);

    //sada zovemo glad biblioteku da ucita sve nase opengl funkcije!
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init OpenGL functions by GLAD" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    //incijalna boja pozadine
    glClearColor(0.0f, 0.0f, 0.0f ,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //render function
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
//        update();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
}