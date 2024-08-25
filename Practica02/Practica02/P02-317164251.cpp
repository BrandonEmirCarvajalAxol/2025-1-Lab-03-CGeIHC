//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f/180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* vShaderRojo = "shaders/shaderRojo.vert";
static const char* vShaderVerde = "shaders/shaderVerde.vert";
static const char* vShaderAzul = "shaders/shaderAzul.vert";
static const char* vShaderCafe = "shaders/shaderCafe.vert";
static const char* vShaderVerdeOscuro = "shaders/shaderVerdeOscuro.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular cuadrangular
void CreaPiramide()
{
	unsigned int indices[] = {
		//base
		0,1,2,
		2,3,0,
		//frontal
		0,1,4,
		//derecha
		1,2,4,
		//trasera
		2,3,4,
		//izquierda
		3,0,4
	};
	GLfloat vertices[] = {
	   -0.55f,  0.0f,  0.55f,	//0
		0.55f,  0.0f,  0.55f,	//1
		0.55f,  0.0f, -0.55f,	//2
	   -0.55f,  0.0f, -0.55f,	//3
	    0.0f,   0.70f, 0.0f,    //4
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 15, 18);
	meshList.push_back(obj1);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

GLfloat cubo_vertices[] = {
	// front
	-0.45f, -0.5f,  0.5f,
	0.45f, -0.5f,  0.5f,
	0.45f,  0.5f,  0.5f,
	-0.45f,  0.5f,  0.5f,
	// back
	-0.45f, -0.5f, -0.5f,
	0.45f, -0.5f, -0.5f,
	0.45f,  0.5f, -0.5f,
	-0.45f,  0.5f, -0.5f
};
Mesh *cubo = new Mesh();
cubo->CreateMesh(cubo_vertices, cubo_indices,24, 36);
meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_letras[] = {
	    //X			Y			Z			R			G			B	
		//Letra B
		-0.8,		0.4,		0.0,		0.0f,		0.737f,		0.639f,
		-0.4,		0.4,		0.0,		0.0f,		0.737f,		0.639f,
		-0.4,		0.3,		0.0,		0.0f,		0.737f,		0.639f,

		-0.4,		0.3,		0.0,		0.0f,		0.737f,		0.639f,
		-0.7,		0.3,		0.0,		0.0f,		0.737f,		0.639f,
		-0.8,		0.4,		0.0,		0.0f,		0.737f,		0.639f,

		-0.8,		0.4,		0.0,	    0.0f,		0.737f,		0.639f,
		-0.7,		0.3,		0.0,		0.0f,		0.737f,		0.639f,
		-0.7,		-0.4,		0.0,		0.0f,		0.737f,		0.639f,

		-0.7,		-0.4,		0.0,		0.0f,		0.737f,		0.639f,
		-0.8,		-0.4,		0.0,		0.0f,		0.737f,		0.639f,
		-0.8,		0.4,		0.0,		0.0f,		0.737f,		0.639f,


		-0.4,		0.15,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		0.4,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.5,		0.4,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.5,		0.4,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.5,		0.15,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		0.15,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.4,		0.15,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		0.3,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.7,		0.0,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.7,		0.0,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.57,		0.0,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		0.15,		0.0f,		0.0f,		0.737f,		0.639f,


		-0.4,		-0.15,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		-0.4,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.5,		-0.4,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.5,		-0.4,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.5,		-0.15,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		-0.15,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.4,		-0.15,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		-0.3,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.7,		-0.0,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.7,		-0.0,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.57,		-0.0,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		-0.15,		0.0f,		0.0f,		0.737f,		0.639f,


		-0.8,		-0.4,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		-0.4,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.4,		-0.3,		0.0f,		0.0f,		0.737f,		0.639f,

		-0.4,		-0.3,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.8,		-0.3,		0.0f,		0.0f,		0.737f,		0.639f,
		-0.8,		-0.4,		0.0f,		0.0f,		0.737f,		0.639f,

		//Letra C
		-0.2,		0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		 0.2,		0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		 0.2,		0.3,		0.0f,		0.886f,		0.960f,		0.721f,

		 0.2,		0.3,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.1,		0.3,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.2,		0.4,		0.0f,		0.886f,		0.960f,		0.721f,

		-0.2,		0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.1,		0.3,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.1,		-0.4,		0.0f,		0.886f,		0.960f,		0.721f,

		-0.1,		-0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.2,		-0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.2,		0.4,		0.0f,		0.886f,		0.960f,		0.721f,


		-0.2,		-0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		 0.2,		-0.4,		0.0f,		0.886f,		0.960f,		0.721f,
		 0.2,		-0.3,		0.0f,		0.886f,		0.960f,		0.721f,

		 0.2,		-0.3,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.2,		-0.3,		0.0f,		0.886f,		0.960f,		0.721f,
		-0.2,		-0.4,		0.0f,		0.886f,		0.960f,		0.721f,

		//Letra A
		 0.6,		0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.4,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.6,		0.15,		0.0f,		0.521f,		0.878f,		0.839f,

		 0.6,		0.15,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.8,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.6,		0.4,		0.0f,		0.521f,		0.878f,		0.839f,


		 0.57,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.63,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.66,		0.0,		0.0f,		0.521f,		0.878f,		0.839f,

		 0.66,		0.0,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.54,		0.0,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.57,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,

		 0.57,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.4,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.54,		0.0,		0.0f,		0.521f,		0.878f,		0.839f,

		 0.4,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.5,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.57,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,


		 0.63,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.66,		0.0,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.8,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,

		 0.8,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.7,		-0.4,		0.0f,		0.521f,		0.878f,		0.839f,
		 0.63,		-0.1,		0.0f,		0.521f,		0.878f,		0.839f,
	};
	MeshColor * letras = new MeshColor();
	letras->CreateMeshColor(vertices_letras,504);
	meshColorList.push_back(letras);

	GLfloat vertices_triangulorojo[] = {
		//X			Y			Z			R		G		B
		-1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		1.0f,	-1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		0.0f,	1.0f,		0.5f,			1.0f,	0.0f,	0.0f,
		
	};

	MeshColor* triangulorojo = new MeshColor();
	triangulorojo->CreateMeshColor(vertices_triangulorojo, 18);
	meshColorList.push_back(triangulorojo);

	GLfloat vertices_cuadradoverde[] = {
		//X			Y			Z			R		G		B
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,		0.5f,			0.0f,	1.0f,	0.0f,

	};

	MeshColor* cuadradoverde = new MeshColor();
	cuadradoverde->CreateMeshColor(vertices_cuadradoverde, 36);
	meshColorList.push_back(cuadradoverde);


}


void CreateShaders()
{
	Shader* shader1 = new Shader(); //shader color rojo
	shader1->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader(); //shader color verde
	shader2->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shader2);

	Shader* shader3 = new Shader(); //shader color azul
	shader3->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shader3);

	Shader* shader4 = new Shader(); //shader color cafe
	shader4->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shader4);

	Shader* shader5 = new Shader(); //shader color verde oscuro
	shader5->CreateFromFiles(vShaderVerdeOscuro, fShader);
	shaderList.push_back(*shader5);

	Shader *shader6 = new Shader(); //shader para usar color como parte del VAO: letras 
	shader6->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader6);

	Shader* shader7 = new Shader(); //shader para usar índices: objetos: cubo y  pirámide (color clamp)
	shader7->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader7);
}


int main()
{
	mainWindow = Window(700, 600);
	mainWindow.Initialise();
	CreaPiramide(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f)	,mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	
	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad
	
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad

		/*
		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA y se envían al shader como variables de tipo uniform
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();
		*/

		//Para el cubo con shader rojo
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para la piramide con shader azul
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Para los cubos con shader verde
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();
		model = glm::mat4(1.0);	
		model = glm::translate(model, glm::vec3(0.0f, -0.71f, -1.63f));	
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.2f, -0.2f, -1.63f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -0.2f, -1.63f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Para las piramides con shader verde oscuro
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.75f, -1.63f));
		model = glm::scale(model, glm::vec3(0.35f, 0.65f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.75f, -1.63f));
		model = glm::scale(model, glm::vec3(0.35f, 0.65f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Para los cubos con chader color cafe
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.85f, -1.63f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.85f, -1.63f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.0f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();


		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/