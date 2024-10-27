/*
Animación:
Sesión 1:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada
Sesión 2
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

float angulovariaALA1 = 0.001f;
float angulovariaALA2 = 0.0f;
float rotDragon = 0.0f;
float movDragonZ = 0.0f;
float movDragonX = 0.0f;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;

float movArco;
float movOffset2;

bool baja;


Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;

Texture LKND;

Texture Marble1;
Texture Marble2;
Texture Marble4;
Texture Marble6;
Texture Marble7;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Dragon_Ala1;
Model Dragon_Ala2;
Model Dragon_C1;
Model Dragon_C2;
Model Dragon_C3;
Model Dragon_C4;
Model Dragon_C5;
Model Tiamat_M;

Model PilaresArco;
Model ArcoSup;
Model Letrero;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void animacion();

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.8f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.8f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.8f, 0.0f, -0.8f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();

	LKND = Texture("Textures/LKND.tga");
	LKND.LoadTextureA();

	Marble1 = Texture("Textures/Map__1_Marble.tga");
	Marble1.LoadTextureA();
	Marble2 = Texture("Textures/Map__2_Marble.tga");
	Marble2.LoadTextureA();
	Marble4 = Texture("Textures/Map__4_Marble.tga");
	Marble4.LoadTextureA();
	Marble6 = Texture("Textures/Map__6_Smoke.tga");
	Marble6.LoadTextureA();
	Marble7 = Texture("Textures/Map__7_Marble.tga");
	Marble7.LoadTextureA();


	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dragon_M = Model();
	Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");
	Dragon_Ala1 = Model();
	Dragon_Ala1.LoadModel("Models/Ala1.obj");
	Dragon_Ala2 = Model();
	Dragon_Ala2.LoadModel("Models/Ala2.obj");
	Dragon_C1 = Model();
	Dragon_C1.LoadModel("Models/Cabeza1.obj");
	Dragon_C2 = Model();
	Dragon_C2.LoadModel("Models/Cabeza2.obj");
	Dragon_C3 = Model();
	Dragon_C3.LoadModel("Models/Cabeza3.obj");
	Dragon_C4 = Model();
	Dragon_C4.LoadModel("Models/Cabeza4.obj");
	Dragon_C5 = Model();
	Dragon_C5.LoadModel("Models/Cabeza5.obj");

	PilaresArco = Model();
	PilaresArco.LoadModel("Models/PilaresArco.obj");
	ArcoSup = Model();
	ArcoSup.LoadModel("Models/ArcoSup.obj");
	Letrero = Model();
	Letrero.LoadModel("Models/Letrero.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;



	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movCoche = 0.0f;
	movOffset = 0.01f;
	rotllanta = 0.0f;
	rotllantaOffset = 10.0f;

	
	movArco = 0.0f;
	movOffset2 = 0.5f;
	baja = true;

	double aux = 0.0f;

	float tiempoAnterior = 0.0f;
	float intervaloCambio = 1.0f; // Cambiar de textura cada 1 segundo (ajusta según prefieras)
	static int numeroActual = 0; // Empezamos con el número 1
	static bool usarNumero1Texture = true;
	


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		double temp = glfwGetTime();
		animacion();
		if (movCoche < 30.0f)
		{
			movCoche -= movOffset * deltaTime;
			//printf("avanza%f \n ",movCoche);
			rotllanta += rotllantaOffset * deltaTime;
		}
	
		if (temp - aux >= 2.0)
		{
			if (baja)
			{
				if (movArco > -50.0f)
				{
					movArco -= movOffset2 * deltaTime;
				}
				else
				{
						baja = !baja;
						aux = temp;
				}
			}
			else
			{
				if (movArco < 0.0f)
				{
					movArco += movOffset2 * deltaTime;
				}
				else
				{
						baja = !baja;
						aux = temp;
				}
			}
		}
		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche-50.0f, 0.5f, -2.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -rotllanta * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();


		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f-movDragonX/6.0, 5.0f+(3*sin(glm::radians(angulovaria*10))), movDragonZ));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDragon * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		modelaux = model;
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		/*color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_M.RenderModel();
		
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.385f, 1.121f, 2.952f));
		model = glm::rotate(model, (1 * sin(glm::radians(angulovariaALA1))), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 1.0f, 1.0f); //Color cyan
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_Ala1.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.617f, -0.544f, 2.916));
		model = glm::rotate(model, (1 * sin(glm::radians(angulovariaALA2))), glm::vec3(1.0f, 0.0f, 0.0f));
		color = glm::vec3(0.0f, 1.0f, 0.0f); //Color verde
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_Ala2.RenderModel();


		float a = 5.0f;  // Controla la velocidad de rotación en X
		float b = 5.0f;  // Controla la velocidad de rotación en Y
		float angulovaria = glfwGetTime() * 10.0f;  // Incrementar el ángulo con el tiempo
		float t = glm::radians(angulovaria);  // Parámetro t basado en el tiempo

		// Calcular los ángulos de rotación en X e Y basados en la elipse
		float angleX = a * cos(t);
		float angleY = b * sin(t);

		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.951f, 1.312f, 2.216f));
		model = glm::rotate(model, angleX, glm::vec3(1.0f, 0.0f, 1.0f));  // Rotación en X
		model = glm::rotate(model, angleY, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotación en Y
		color = glm::vec3(1.0f, 0.0f, 0.0f); //Color ROJO
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_C1.RenderModel();


		float A = 1.0f;  // Amplitud de la rotación en el eje X
		float B = 0.5f;  // Amplitud de la rotación en el eje Y
		float a2 = 5.0f;  // Frecuencia en el eje X
		float b2 = 2.0f;  // Frecuencia en el eje Y
		float delta = glm::radians(90.0f);  // Fase entre las rotaciones

		float angulovaria2 = glfwGetTime();  // Parámetro basado en el tiempo
		float t2 = angulovaria2 * 10.0f;

		// Calcular los ángulos de rotación en X e Y basados en la curva Lissajous
		float angleX2 = A * sin(a2 * t2 + delta);
		float angleY2 = B * sin(b2 * t2);

		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.311f, 0.927f, 3.978f));
		model = glm::rotate(model, angleX, glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación en X
		model = glm::rotate(model, angleY, glm::vec3(0.0f, 1.0f, 0.0f));  // Rotación en Y
		color = glm::vec3(0.0f, 0.0f, 1.0f); //Color AZUL
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_C2.RenderModel();


		float a3 = 1.0f;  // Parámetro inicial de la espiral
		float b3 = 0.1f;  // Controla la expansión de la espiral
		float angulovaria3 = glfwGetTime() * 10.0f;  // Tiempo en segundos
		float theta = angulovaria3;  // Ángulo para la espiral

		// Calcular el ángulo de rotación en X basado en la espiral
		float angleX3 = a3 + b3 * theta;

		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.836f, 0.103f, 3.368f));
		model = glm::rotate(model, glm::radians(angleX3), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación en X
		model = glm::rotate(model, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));   // Rotación en Y progresiva
		color = glm::vec3(0.0f, 1.0f, 0.0f); //Color VERDE
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_C3.RenderModel();


		float A4 = 45.0f;  // Amplitud de la rotación
		float omega = 6.0f;  // Frecuencia angular
		float angulovaria4 = glfwGetTime() * 10.0f;  // Tiempo en segundos
		float angleY4 = A4 * sin(glm::radians(omega * angulovaria4));  // Ángulo de rotación en Y basado en el seno

		model = modelaux;
		model = glm::translate(model, glm::vec3(-2.5f, -0.631f, 3.826f));
		model = glm::rotate(model, glm::radians(angleY4), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotación en Y
		color = glm::vec3(1.0f, 1.0f, 1.0f); //Color BLANCO
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_C4.RenderModel();


		float a5 = 45.0f;  // Amplitud de la rotación
		float angulovaria5 = glfwGetTime() * 10.0f;  // Tiempo en segundos
		float t5 = angulovaria;  // Parámetro para la lemniscata

		// Calcular los ángulos de rotación basados en la lemniscata
		float angleX5 = (a5* cos(t5)) / (1 + sin(t5) * sin(t5));  // Rotación en X
		float angleY5 = (a5 * sin(t5) * cos(t5)) / (1 + sin(t5) * sin(t5));  // Rotación en Y

		model = modelaux;
		model = glm::translate(model, glm::vec3(-3.387f, -1.108f, 2.132f));
		model = glm::rotate(model, glm::radians(angleX5), glm::vec3(1.0f, 0.0f, 0.0f));  // Rotación en X
		model = glm::rotate(model, glm::radians(angleY5), glm::vec3(0.0f, 1.0f, 0.0f));  // Rotación en Y
		color = glm::vec3(0.6313f, 0.5098f, 0.3843f); //Color CAFE
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dragon_C5.RenderModel();

		/*color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));*/
		//Agave ¿qué sucede si lo renderizan antes del coche y de la pista?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		
 
		//ArcoMeta
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0, -2.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PilaresArco.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0, movArco, 0.0f));
		modelaux = model;
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArcoSup.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 0.7f, 1.0f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Letrero.RenderModel();


		toffsetnumerocambiau += 0.0005;
		if (toffsetnumerocambiau > 1.0)
			toffsetnumerocambiau = 0.0;
		toffsetnumerov = 0.0;
		toffset = glm::vec2(toffsetnumerocambiau, toffsetnumerov);
		modelaux = model;
		model = glm::translate(model, glm::vec3(1.5f, 55.0f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LKND.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();


		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

void animacion()
{

	//Movimiento del coche

		if (recorrido1)
		{
			rotDragon = 0;
			angulovaria += 0.5f * deltaTime;
			angulovariaALA1 += 5.0f * deltaTime;
			angulovariaALA2 -= 5.0f * deltaTime;
			movDragonX += 0.5f * deltaTime;
			if (movDragonX > 50)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			rotDragon = 90;
			angulovaria += 0.5f * deltaTime;
			angulovariaALA1 += 5.0f * deltaTime;
			angulovariaALA2 -= 5.0f * deltaTime;
			movDragonZ += 0.01f;
			if (movDragonZ > 50)
			{
				recorrido2 = false;
				recorrido3 = true;

			}
		}

		if (recorrido3)
		{
			rotDragon = 180;
			angulovaria += 0.5f * deltaTime;
			angulovariaALA1 += 5.0f * deltaTime;
			angulovariaALA2 -= 5.0f * deltaTime;
			movDragonX -= 0.01f;
			if (movDragonX < 0)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			rotDragon = 270;
			angulovaria += 0.5f * deltaTime;
			angulovariaALA1 += 5.0f * deltaTime;
			angulovariaALA2 -= 5.0f * deltaTime;
			movDragonZ -= 0.01f;
			if (movDragonZ < 0)
			{
				recorrido4 = false;
				recorrido1 = true;
			}
		}
}