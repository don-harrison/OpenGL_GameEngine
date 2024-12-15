#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* window = nullptr;
GLuint VAO, VBO;

GLfloat verticesWithColor[] = {
    0.0f,  0.5f, 1.0f, 0.0f, 0.0f,// Top vertex
   -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,// Bottom-left vertex
    0.5f, -0.5f, 1.0f, 0.0f, 0.0f// Bottom-right vertex
};

const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;
    out vec3 vertexColor;
    void main() {
        gl_Position = vec4(aPos, 0.0, 1.0);
        vertexColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(vertexColor, 1.0);
    }
)";

GLuint compileShader(GLenum type, const char* source) {
    //Create empty shader in opengl, type tells opengl what it is and what to compile it as
    GLuint shader = glCreateShader(type);
    
    //Provide Source string and reference to allocated shader position
    glShaderSource(shader, 1, &source, nullptr);

    //Tell opengl to compile the provided source.
    glCompileShader(shader);

    // Check for compile errors
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}


GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for linking errors
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shaders (no longer needed after linking)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

//Release VBOs and VAOs and destroy window and glfw
void cleanup() {
    if(VBO) glDeleteBuffers(1, &VBO);
    if(VAO) glDeleteVertexArrays(1, &VAO);

    if(window){
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

//ModernGL pipeline requires using buffers and shaders to render
void setupBuffers(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Pass vertices to buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWithColor), verticesWithColor, GL_STATIC_DRAW);

    //Tell openGL how the vertex array is structured.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

int renderLinesDeprecated(){
    // Now draw your lines
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);  // Use the best line smoothing quality
    glLineWidth(3.0f);  // Set line thickness to 3 pixels
    glBegin(GL_LINES);
    
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f( 0.5f,  0.5f);
    glVertex2f( 0.5f, -0.5f);
    glVertex2f(-0.5f,  0.5f);
    glEnd();

    return 0;
}

void renderTriangle(){
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangles
    glBindVertexArray(0); //unbind VAO
}

int renderTriangleDeprecated(){
    glBegin(GL_TRIANGLES); //Render in tri
    
    for(int i = 0; i < 6; i+=2){
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2f(verticesWithColor[i], verticesWithColor[i+1]);
    }
    glEnd();

    return 0;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    window = glfwCreateWindow(800, 600, "Spinning Cube", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewInit();
    //Setup Buffers:
    setupBuffers();
    GLuint shaderProgram = createShaderProgram();

    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        //Setup Shaders
        glUseProgram(shaderProgram);

        //Render Item Here
        renderTriangle();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Clean up and exit
    cleanup();


    return 0;
}