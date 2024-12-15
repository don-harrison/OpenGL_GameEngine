#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Rotation angles
float angleX = 0.0f;
float angleY = 0.0f;

// Initialize OpenGL settings
void initOpenGL() {
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
}

// Function to set up a perspective projection matrix manually
void setProjectionMatrix(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set up a perspective projection using glFrustum
    float aspectRatio = (float)width / (float)height;
    float near = 0.1f;
    float far = 100.0f;
    float fov = 45.0f;

    float top = near * tan(fov * 0.5f * 3.14159f / 180.0f);
    float bottom = -top;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;

    glFrustum(left, right, bottom, top, near, far);
    glMatrixMode(GL_MODELVIEW);
}

// Function to draw a cube
void drawCube() {
    glBegin(GL_QUADS);

    // Front face (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back face (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Top face (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Bottom face (Yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face (Magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left face (Cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();
}

// Main loop to display and update the cube's rotation
void mainLoop(GLFWwindow* window) {
    int width, height;
    while (!glfwWindowShouldClose(window)) {
        // Get window size
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Set up the projection matrix
        setProjectionMatrix(width, height);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

        glLoadIdentity(); // Reset transformations

        // Move the cube back so we can see it
        glTranslatef(0.0f, 0.0f, -5.0f);

        // Rotate the cube on the X and Y axes
        glRotatef(angleX, 1.0f, 0.0f, 0.0f); // X-axis rotation
        glRotatef(angleY, 0.0f, 1.0f, 0.0f); // Y-axis rotation

        drawCube(); // Draw the cube

        angleX += 0.5f; // Increase X rotation angle
        angleY += 0.5f; // Increase Y rotation angle

        glfwSwapBuffers(window); // Swap the buffer to update the display
        glfwPollEvents(); // Poll for events
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }

    // Create a windowed mode window and OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Spinning Cube", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        return -1;
    }

    initOpenGL(); // Initialize OpenGL settings
    mainLoop(window); // Run the main loop

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}