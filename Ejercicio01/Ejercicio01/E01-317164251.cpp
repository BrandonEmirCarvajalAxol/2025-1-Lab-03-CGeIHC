#include <stdio.h>
#include <string.h>
#include <glew.h>
#include <glfw3.h>
#include <windows.h> // Para usar Sleep

// Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
GLuint VAO[2], VBO[2], shader; // Variables para almacenar el identificador de los Vertex Array Objects, Vertex Buffer Objects y el shader program

//Vertex Shader
//recibir color, salida Vcolor
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(1.0f,1.0f,1.0f,1.0f);	 			\n\
}";

// Función para crear las formas (cuadrado y rombo)
void CrearCyR() {
    // Datos de los vértices para el cuadrado
    GLfloat cuadrado[] = {
        -0.65f,   0.25f,  0.0f,  // vertice superior izquierdo
        -0.25f,   0.25f,  0.0f,  // vertice superior derecho
        -0.25f,  -0.25f,  0.0f,  // vertice inferior derecho
        -0.65f,  -0.25f,  0.0f   // vertice inferior izquierdo
    };

    // Datos de los vértices para el rombo
    GLfloat rombo[] = {
        0.25f,  0.0f,  0.0f,  // vertice izquierdo
        0.5f,   0.25f, 0.0f,  // vertice superior
        0.75f,  0.0f,  0.0f,  // vertice derecho
        0.5f,  -0.25f, 0.0f   // vertice inferior
    };

    glGenVertexArrays(2, VAO); // Genera dos Vertex Array Objects (VAOs) y los almacena en la variable VAO
    glGenBuffers(2, VBO); // Genera dos Vertex Buffer Objects (VBOs) y los almacena en la variable VBO

    // Configuración del VAO y VBO para el cuadrado
    glBindVertexArray(VAO[0]); // Asigna el primer VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]); // Asigna el primer VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(cuadrado), cuadrado, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); 
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Configuración del VAO y VBO para el rombo
    glBindVertexArray(VAO[1]); // Asigna el segundo VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]); // Asigna el segundo VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(rombo), rombo, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader
{
    GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
    const GLchar* theCode[1];
    theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);//longitud del texto
    glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
    glCompileShader(theShader);//Se comila el shader
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    //verificaciones y prevención de errores
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

void CompileShaders() {
    shader= glCreateProgram(); //se crea un programa
    if (!shader)
    {
        printf("Error creando el shader");
        return;
    }
    AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
    //Para terminar de linkear el programa y ver que no tengamos errores
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
    //verificaciones y prevención de errores
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al linkear es: %s \n", eLog);
        return;
    }
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("EL error al validar es: %s \n", eLog);
        return;
    }
}
// Función para alternar los colores de fondo
void CambiarColorDeFondo(GLFWwindow* window) {
    static int colorActual = 0; // Índice para alternar los colores

    // Cambia el color de fondo basado en el índice
    switch (colorActual) {
    case 0: glClearColor(1.0f, 0.0f, 0.0f, 1.0f); break; // Rojo
    case 1: glClearColor(0.0f, 1.0f, 0.0f, 1.0f); break; // Verde
    case 2: glClearColor(0.0f, 0.0f, 1.0f, 1.0f); break; // Azul
    }
    colorActual = (colorActual + 1) % 3; // Ciclo entre 0, 1, 2 para alternar los colores
    Sleep(1000); // Pausar la ejecución por 1 segundo

}
// Función principal
int main()
{
    //Inicialización de GLFW
    if (!glfwInit())
    {
        printf("Falló inicializar GLFW");
        glfwTerminate();
        return 1;
    }
    //Asignando variables de GLFW y propiedades de ventana
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //para solo usar el core profile de OpenGL y no tener retrocompatibilidad
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //CREAR VENTANA
    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

    if (!mainWindow)
    {
        printf("Fallo en crearse la ventana con GLFW");
        glfwTerminate();
        return 1;
    }
    //Obtener tamaño de Buffer
    int BufferWidth, BufferHeight;
    glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

    //asignar el contexto
    glfwMakeContextCurrent(mainWindow);

    //permitir nuevas extensiones
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("Falló inicialización de GLEW");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // Asignar valores de la ventana y coordenadas
    //Asignar Viewport
    glViewport(0, 0, BufferWidth, BufferHeight);

    CrearCyR();
    CompileShaders();

    //Loop mientras no se cierra la ventana
    while (!glfwWindowShouldClose(mainWindow)) {
        //Recibir eventos del usuario
        glfwPollEvents();

        CambiarColorDeFondo(mainWindow); // Cambia el color de fondo
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        // Dibuja el cuadrado
        glBindVertexArray(VAO[0]); // Asigna el VAO del cuadrado
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Dibuja el cuadrado usando los 4 vértices
        glBindVertexArray(0);

        // Dibuja el rombo
        glBindVertexArray(VAO[1]); // Asigna el VAO del rombo
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // Dibuja el rombo usando los 4 vértices
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }
    return 0;
}