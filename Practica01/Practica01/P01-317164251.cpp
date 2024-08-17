#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <windows.h> // Para usar Sleep
#include <stdlib.h> // Para usar rand
#include <time.h>   // Para usar time en srand

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO[9], VBO[9], shader; // Variables para almacenar el identificador de los Vertex Array Objects, Vertex Buffer Objects y el shader program

// Vertex Shader
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0f);  \n\
}";

// Fragment Shader
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
    color = vec4(1.0f, 1.0f, 1.0f, 1.0f); 			\n\
}";

// Función para crear las formas (letras B, C y A)
void CrearLetras() {
    GLfloat letraBParte1[] = {
        -0.8,0.4,0.0f,
        -0.4,0.4,0.0f,
        -0.4,0.3,0.0f,
        -0.7,0.3,0.0f,
        -0.7,-0.4,0.0f,
        -0.8,-0.4,0.0f
    };
    GLfloat letraBParte2[] = {
        -0.4,0.15,0.0f,
        -0.4,0.4,0.0f,
        -0.5,0.4,0.0f,
        -0.5,0.15,0.0f,
        -0.4,0.3,0.0f,
        -0.7,0.0,0.0f,
        -0.57,0.0,0.0f
    };
    GLfloat letraBParte3[] = {
        -0.4,-0.15,0.0f,
        -0.4,-0.4,0.0f,
        -0.5,-0.4,0.0f,
        -0.5,-0.15,0.0f,
        -0.4,-0.3,0.0f,
        -0.7,0.0,0.0f,
        -0.57,0.0,0.0f
    };
    GLfloat letraBParte4[] = {
        -0.8,-0.4,0.0f,
        -0.4,-0.4,0.0f,
        -0.4,-0.3,0.0f,
        -0.8,-0.3,0.0f
    };
    GLfloat letraCParte1[] = {
        -0.2,0.4,0.0f,
         0.2,0.4,0.0f,
         0.2,0.3,0.0f,
        -0.1,0.3,0.0f,
        -0.1,-0.4,0.0f,
        -0.2,-0.4,0.0f,
    };
    GLfloat letraCParte2[] = {
        -0.2,-0.4,0.0f,
         0.2,-0.4,0.0f,
         0.2,-0.3,0.0f,
        -0.2,-0.3,0.0f,
    };
    GLfloat letraAParte1[] = {
         0.6,0.4,0.0f,
         0.4,-0.4,0.0f,
         0.6,0.15,0.0f,
         0.8,-0.4,0.0f,
    };
    GLfloat letraAParte2[] = {
         0.57,-0.1,0.0f,
         0.63,-0.1,0.0f,
         0.66,0.0,0.0f,
         0.54,0.0,0.0f,
         0.4,-0.4,0.0f,
         0.5,-0.4,0.0f
    };
    GLfloat letraAParte3[] = {
         0.63,-0.1,0.0f,
         0.66,0.0,0.0f,
         0.8,-0.4,0.0f,
         0.7,-0.4,0.0f,
    };
    glGenVertexArrays(9, VAO);
    glGenBuffers(9, VBO);

    // Configuración del VAO y VBO para la letra B
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraBParte1), letraBParte1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraBParte2), letraBParte2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraBParte3), letraBParte3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraBParte4), letraBParte4, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Configuración del VAO y VBO para la letra C
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraCParte1), letraCParte1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraCParte2), letraCParte2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Configuración del VAO y VBO para la letra A
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraAParte1), letraAParte1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraAParte2), letraAParte2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(letraAParte3), letraAParte3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error al compilar el shader %d: %s\n", shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    if (!shader) {
        printf("Error creando el shader");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error al linkear el shader: %s\n", eLog);
        return;
    }
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error al validar el shader: %s\n", eLog);
        return;
    }
}

void CambiarColorDeFondo(GLFWwindow* window) {
    srand(time(NULL));
    int colorActual = rand() % 3;
    switch (colorActual) {
    case 0: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break; // Rojo
    case 1: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break; // Verde
    case 2: glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break; // Azul
    }
    Sleep(2000); // Pausar la ejecución por 2 segundos
}

int main() {
    if (!glfwInit()) {
        printf("Falló al inicializar GLFW");
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Ventana", NULL, NULL);
    if (!mainWindow) {
        printf("Falló al crear la ventana con GLFW");
        glfwTerminate();
        return 1;
    }

    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

    glfwMakeContextCurrent(mainWindow);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        printf("Falló al inicializar GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glViewport(0, 0, BufferWidth, BufferHeight);

    CrearLetras();
    CompileShaders();

    while (!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();
        CambiarColorDeFondo(mainWindow);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        // Dibuja la letra B
        glBindVertexArray(VAO[0]);// Dibuja la letraBParte1
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glBindVertexArray(0);
        glBindVertexArray(VAO[1]);// Dibuja la letraBParte2
        glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
        glBindVertexArray(0);
        glBindVertexArray(VAO[2]);// Dibuja la letraBParte3
        glDrawArrays(GL_TRIANGLE_FAN, 0, 7);
        glBindVertexArray(0);
        glBindVertexArray(VAO[3]);// Dibuja la letraBParte4
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        // Dibuja la letra C
        glBindVertexArray(VAO[4]);// Dibuja la letraCParte1
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glBindVertexArray(0);
        glBindVertexArray(VAO[5]);// Dibuja la letraCParte2
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);

        // Dibuja la letra A
        glBindVertexArray(VAO[6]);// Dibuja la letraAParte1
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
        glBindVertexArray(VAO[7]);// Dibuja la letraAParte2
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        glBindVertexArray(0);
        glBindVertexArray(VAO[8]);// Dibuja la letraAParte3
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);


        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }
    glfwTerminate();
    return 0;
}