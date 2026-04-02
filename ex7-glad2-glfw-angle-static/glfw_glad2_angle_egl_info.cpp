#include <glad/egl.h>
#include <glad/gles2.h>
#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_EGL 1
#include <GLFW/glfw3native.h>
#include <iostream>

// print glfw version and available apis
void glfw_print_info() {
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    std::cout << "GLFW Version: " << major << "." << minor << "." << rev << std::endl;
    
    std::cout << "\nAvailable client APIs:" << std::endl;
    if (glfwVulkanSupported()) {
        std::cout << "- Vulkan is supported" << std::endl;
    }

    // test opengl support with a dummy window
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    GLFWwindow* gl_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (gl_window) {
        std::cout << "- OpenGL is supported" << std::endl;
        glfwDestroyWindow(gl_window);
    }
    
    // test opengl es support
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    GLFWwindow* gles_window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    if (gles_window) {
        std::cout << "- OpenGL ES is supported" << std::endl;
        glfwDestroyWindow(gles_window);
    }
}

// print gl information
void gl_print_info() {
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

    std::cout << "\nOpenGL Information:" << std::endl;
    std::cout << "GL Vendor: " << vendor << std::endl;
    std::cout << "GL Renderer: " << renderer << std::endl;
    std::cout << "GL Version: " << version << std::endl;
    std::cout << "GLSL Version: " << glsl_version << std::endl;
}

// print egl information
void egl_print_info(EGLDisplay display) {
    std::cout << "\nEGL Information:" << std::endl;
    const char* vendor = eglQueryString(display, EGL_VENDOR);
    const char* version = eglQueryString(display, EGL_VERSION);
    const char* apis = eglQueryString(display, EGL_CLIENT_APIS);
    const char* extensions = eglQueryString(display, EGL_EXTENSIONS);

    std::cout << "EGL Vendor: " << vendor << std::endl;
    std::cout << "EGL Version: " << version << std::endl;
    std::cout << "EGL Client APIs: " << apis << std::endl;
    
    // print extensions in a formatted way
    std::cout << "\nEGL Extensions:" << std::endl;
    std::string ext_str(extensions);
    size_t pos = 0;
    while ((pos = ext_str.find(' ')) != std::string::npos) {
        std::string ext = ext_str.substr(0, pos);
        if (!ext.empty()) {
            std::cout << "- " << ext << std::endl;
        }
        ext_str.erase(0, pos + 1);
    }
    if (!ext_str.empty()) {
        std::cout << "- " << ext_str << std::endl;
    }
}

// glfw error callback
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    // set error callback first
    glfwSetErrorCallback(glfw_error_callback);
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // print glfw and api information
    glfw_print_info();

    // try creating window with egl
    std::cout << "\nAttempting to create EGL window..." << std::endl;
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    
    std::cout << "Window hints set:" << std::endl;
    std::cout << "- Client API: OpenGL ES" << std::endl;
    std::cout << "- Context Version: 2.0" << std::endl;
    std::cout << "- Context Creation API: EGL" << std::endl;

    GLFWwindow* window = glfwCreateWindow(800, 600, "EGL Information Demo (GLAD2)", nullptr, nullptr);
    
    if (!window) {
        std::cerr << "\nFailed to create window with EGL context API" << std::endl;
        std::cout << "Trying native context API..." << std::endl;
        
        // try native context api as fallback
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
        window = glfwCreateWindow(800, 600, "EGL Information Demo (GLAD2, Native)", nullptr, nullptr);
        
        if (!window) {
            std::cerr << "Failed to create window with native context API" << std::endl;
            glfwTerminate();
            return -1;
        }
        std::cout << "Successfully created window with native context API" << std::endl;
    } else {
        std::cout << "Successfully created window with EGL context API" << std::endl;
    }

    glfwMakeContextCurrent(window);

    // try loading egl
    if (window) {
        std::cout << "\nAttempting to get EGL display..." << std::endl;
        EGLDisplay display = glfwGetEGLDisplay();
        
        if (display == EGL_NO_DISPLAY) {
            std::cerr << "Failed to get EGL display" << std::endl;
        } else {
            std::cout << "Successfully got EGL display" << std::endl;
            
            // initialize glad egl
            int egl_version = gladLoaderLoadEGL(display);
            if (egl_version) {
                std::cout << "Successfully loaded EGL " << GLAD_VERSION_MAJOR(egl_version) 
                         << "." << GLAD_VERSION_MINOR(egl_version) << std::endl;
                egl_print_info(display);
            } else {
                std::cerr << "Failed to load EGL with GLAD" << std::endl;
            }
            
            // initialize glad gles
            int gles_version = gladLoadGLES2(glfwGetProcAddress);
            if (gles_version) {
                std::cout << "\nSuccessfully loaded GLES " << GLAD_VERSION_MAJOR(gles_version) 
                         << "." << GLAD_VERSION_MINOR(gles_version) << std::endl;
                gl_print_info();
            } else {
                std::cerr << "Failed to load GLES with GLAD" << std::endl;
            }
        }
    }

    // cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}