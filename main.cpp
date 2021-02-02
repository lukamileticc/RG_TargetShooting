#include <iostream>
//obavezno ide prvo glad pa onda glfw
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <stb_image.h>
#include "include/Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "include/Error.hpp"




void framebuffer_size_callback(GLFWwindow* window,int width,int height);
void updateProcesInput(GLFWwindow *window);

//screen settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

//camera
glm::vec3 cameraPos = glm::vec3(0.0f ,0.0f, 2.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

//time
float delta_time = 0.0f;
float last_frame = 0.0f;

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



    //sada zovemo glad biblioteku da ucita sve nase opengl funkcije!
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to init GLAD\n" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    //kazemo da hocemo da enablujemo dubinu
    glEnable(GL_DEPTH_TEST);

    Shader shader("../resources/shaders/vertexShader.vs",
                  "../resources/shaders/fragmentShader.fs");


    //pravimo kvadar u 3D-u
    float vertices[] = {
            //prednji pravougaonik
            //first triangle
            1.0, 0.5, 1.0, 0.0, 1.0, 1.0,   1.0, 0.0,//top right
            -1.0, 0.5 , 1.0, 0.0, 1.0, 1.0,    0.0, 0.0,//top left
            1.0, -0.5, 1.0 ,  0.0, 1.0, 1.0,   0.0, 0.0,// bottom right
            //second triangle
            1.0, -0.5, 1.0,  0.0, 1.0, 1.0,   0.0, 0.0,//bottom right
            -1.0, 0.5, 1.0,   0.0, 1.0, 1.0,   0.0, 0.0,//top left
            -1.0, -0.5, 1.0,  0.0, 1.0, 1.0,   0.0, 0.0, // bottom left

            //desni pravougaonik
            1.0, 0.5, 1.0,   1.0, 0.0, 0.0,   0.0, 0.0,
            1.0, -0.5, 1.0,  1.0, 0.0, 0.0,   0.0, 0.0,
            1.0, 0.5, -6.0,  1.0, 0.0, 0.0,   0.0, 0.0,
            1.0, 0.5, -6.0,  1.0, 0.0, 0.0,   0.0, 0.0,
            1.0, -0.5, 1.0,  1.0, 0.0, 0.0,   0.0, 0.0,
            1.0, -0.5, -6.0,  1.0, 0.0, 0.0,   0.0, 0.0,

            //levi pravouganik
            -1.0, 0.5 , 1.0, 0.0, 1.0, 0.0,   0.0, 0.0,
            -1.0, -0.5, 1.0, 0.0, 1.0, 0.0,   0.0, 0.0,
            -1.0, 0.5, -6.0, 0.0, 1.0, 0.0,   0.0, 0.0,
            -1.0, -0.5, 1.0, 0.0, 1.0, 0.0,   0.0, 0.0,
            -1.0, 0.5, -6.0, 0.0, 1.0, 0.0,   0.0, 0.0,
            -1.0, -0.5, -6.0, 0.0, 1.0, 0.0,   0.0, 0.0,

            //zadnja stranica
            1.0, 0.5, -6.0,   0.0, 0.0, 1.0,    0.0, 0.0,
            -1.0, 0.5 , -6.0,   0.0, 0.0, 1.0,  0.0, 0.0,
            1.0, -0.5, -6.0 ,   0.0, 0.0, 1.0,  0.0, 0.0,
            1.0, -0.5, -6.0,    0.0, 0.0, 1.0,  0.0, 0.0,
            -1.0, 0.5, -6.0,    0.0, 0.0, 1.0,  0.0, 0.0,
            -1.0, -0.5, -6.0,   0.0, 0.0, 1.0,  0.0, 0.0,

            //donja strana
            1.0, -0.5, 1.0,   0.0 ,0.6, 0.7,   1.0 ,0.0,
            -1.0, -0.5, 1.0,   0.0 ,0.6, 0.7,  0.0, 0.0,
            -1.0, -0.5, -6.0,  0.0 ,0.6, 0.7,  0.0, 1.0,
            1.0, -0.5, 1.0,    0.0 ,0.6, 0.7,  1.0 ,0.0,
            1.0, -0.5, -6.0,  0.0 ,0.6, 0.7,   1.0, 1.0,
            -1.0, -0.5, -6.0,  0.0 ,0.6, 0.7,  0.0, 1,0,

            //gornja strana
            -1.0, 0.5 , 1.0,  1.0, 1.0, 0.2,   0.0, 0.0,
            1.0, 0.5, 1.0,   1.0, 1.0, 0.2,     0.0, 0.0,
            -1.0, 0.5, -6.0,   1.0, 1.0, 0.2,   0.0, 0.0,
            -1.0, 0.5, -6.0, 1.0, 1.0, 0.2,    0.0, 0.0,
            1.0, 0.5, 1.0,    1.0, 1.0, 0.2,   0.0, 0.0,
            1.0, 0.5, -6.0,   1.0, 1.0, 0.2,  0.0, 0.0,



    };



    unsigned int VAO,VBO;
    //kreirmao objekat koji ce da kaze sta znace podaci iz VBO
    glGenVertexArrays(1,&VAO);
    //kreiramo baffer preko kog saljemo podatke ka gpu
    glGenBuffers(1,&VBO);

    //aktiviramo ovaj objekat
    glBindVertexArray(VAO);


    //aktiviramo ovaj bafer
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    //smestamo podatke u ovaj bafer
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);


    //kazemo sta znace podaci iz vbo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8* sizeof(float), (void*)(6*sizeof (float )));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    //pravimo teksturu
    unsigned  int texture;
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    //parametri za teksturu
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //ucitavamo sliku
    int width,height,nChannel;
    unsigned char* pixels = stbi_load("../resources/textures/floor.jpg",&width, &height,&nChannel, 0);
    if(pixels){
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height, 0 , GL_RGB, GL_UNSIGNED_BYTE,pixels);
        //generisi mip_mape
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(pixels);

    //namestamo uniform promeljivu
    shader.use();
    shader.setUniform1i("texture0",0);




// inicijalno postavljanje boje
    glClearColor(0.2, 0.3, 0.3,1.0);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //ovde moze i GL_LINE

//aktiviramo shader
    shader.use();

    //pravimo projekciju
    glm::mat4 projection = glm::mat4(1.0f);
    //postavljam perspektivnu projekciju sa 45 stepeni
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    shader.setMat4("projection", projection);

    //RENDER loop
    while(!glfwWindowShouldClose(window))
    {
        //postavljamo boju
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = glfwGetTime();
        delta_time = currentFrame - last_frame;
        last_frame = currentFrame;

        updateProcesInput(window);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        shader.use();

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraFront + cameraPos, cameraUp);
        shader.setMat4("view", view);
        //Pravimo transformaciju
        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 0.0f));
        unsigned int modelLoc = glGetUniformLocation(shader.getShaderId(), "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.getShaderId(), "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,&model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);




        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,36);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //brisemo sve objekte koji nam vise nisu potrebni
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

    shader.deleteProgram();


    glfwTerminate();
    return 0;
}
void updateProcesInput(GLFWwindow* window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
    float cameraSpeed = 2.5f * delta_time;
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        cameraPos += cameraFront * cameraSpeed;
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        cameraPos -= cameraFront * cameraSpeed;
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        cameraPos += glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        cameraPos -= glm::normalize(glm::cross(cameraFront,cameraUp)) * cameraSpeed;
    }
}

void framebuffer_size_callback(GLFWwindow* window,int width,int height){
    glViewport(0,0,width,height);
}
