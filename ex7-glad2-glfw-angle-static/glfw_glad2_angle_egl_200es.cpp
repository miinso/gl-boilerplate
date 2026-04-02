#include <glad/egl.h>
#include <glad/gles2.h>
#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_EGL 1
#include <GLFW/glfw3native.h>
#include <iostream>
#include <cmath>

// shader sources
const char* vertex_shader_source = R"(
#version 100
attribute vec3 a_pos;
attribute vec3 a_color;
varying vec3 v_color;
uniform mat4 u_transform;

void main() {
    gl_Position = u_transform * vec4(a_pos, 1.0);
    v_color = a_color;
}
)";

const char* fragment_shader_source = R"(
#version 100
precision mediump float;
varying vec3 v_color;

void main() {
    gl_FragColor = vec4(v_color, 1.0);
}
)";

// print gl and egl information
void gl_print_info() {
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "GL Vendor: " << vendor << std::endl;
    std::cout << "GL Renderer: " << renderer << std::endl;
    std::cout << "GL Version: " << version << std::endl;
    std::cout << "GLSL Version: " << glsl_version << std::endl;

    // print max viewport dimensions (important for gles)
    GLint viewport[2];
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewport);
    std::cout << "Max Viewport Dimensions: " << viewport[0] << "x" << viewport[1] << std::endl;
}

void egl_print_info(EGLDisplay display) {
    const char* vendor = eglQueryString(display, EGL_VENDOR);
    const char* version = eglQueryString(display, EGL_VERSION);
    const char* apis = eglQueryString(display, EGL_CLIENT_APIS);

    std::cout << "EGL Vendor: " << vendor << std::endl;
    std::cout << "EGL Version: " << version << std::endl;
    std::cout << "EGL Client APIs: " << apis << std::endl;
}

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

bool check_shader_errors(GLuint shader) {
    GLint success;
    GLchar info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, info_log);
        std::cout << "Shader compilation error:\n" << info_log << std::endl;
        return false;
    }
    return true;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // request OpenGL ES 2.0 with EGL
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL ES 2.0 EGL (GLAD2)", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // initialize EGL
    EGLDisplay display = glfwGetEGLDisplay();
    int egl_version = gladLoaderLoadEGL(display);
    if (egl_version == 0) {
        std::cerr << "Failed to initialize GLAD EGL" << std::endl;
        return -1;
    }

    std::cout << "EGL Version: " << GLAD_VERSION_MAJOR(egl_version) << "." 
              << GLAD_VERSION_MINOR(egl_version) << std::endl;
    egl_print_info(display);

    // initialize GLES2
    int gles_version;
    
    // using GLFW loader (recommended)
    gles_version = gladLoadGLES2(glfwGetProcAddress);
    
    /* alternative: using GLAD's own loader
    gles_version = gladLoaderLoadGLES2();
    */

    if (gles_version == 0) {
        std::cerr << "Failed to initialize GLAD GLES2" << std::endl;
        return -1;
    }

    gl_print_info();
    std::cout << "GLAD GLES version: " << GLAD_VERSION_MAJOR(gles_version) << "." 
              << GLAD_VERSION_MINOR(gles_version) << std::endl;

    // create and compile shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    if (!check_shader_errors(vertex_shader)) return -1;

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    if (!check_shader_errors(fragment_shader)) return -1;

    // create and link shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // check program linking
    GLint success;
    GLchar info_log[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        std::cerr << "Shader program linking failed:\n" << info_log << std::endl;
        return -1;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // vertex data
    float vertices[] = {
        // positions        // colors
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // red
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // green
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f   // blue
    };

    // create and bind buffer
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // get attribute locations
    GLint pos_attrib = glGetAttribLocation(shader_program, "a_pos");
    GLint color_attrib = glGetAttribLocation(shader_program, "a_color");
    GLint transform_loc = glGetUniformLocation(shader_program, "u_transform");

    // enable vertex attributes
    glEnableVertexAttribArray(pos_attrib);
    glEnableVertexAttribArray(color_attrib);

    // set vertex attributes
    glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glVertexAttribPointer(color_attrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
                         (void*)(3 * sizeof(float)));

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        // calculate rotation matrix
        float time = (float)glfwGetTime();
        float angle = time * 90.0f;  // 90 degrees per second
        float radians = angle * 3.14159f / 180.0f;
        
        float transform[] = {
            cosf(radians), -sinf(radians), 0.0f, 0.0f,
            sinf(radians),  cosf(radians), 0.0f, 0.0f,
            0.0f,          0.0f,          1.0f, 0.0f,
            0.0f,          0.0f,          0.0f, 1.0f
        };

        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, transform);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // cleanup
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(shader_program);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}