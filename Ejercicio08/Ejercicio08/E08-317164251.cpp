/*
Ejercicio 8: Iluminacion2
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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;

Texture LlantaDerBackimport;
Texture Carroceriaimport;
Texture LampImport;

Texture dadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;

Model Carroceria;
Model LlantaDerFront;
Model LlantaDerBack;
Model LlantaIzqFront;
Model LlantaIzqBack;
Model Cofre;

Model Lamp;

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

PointLight pointLightsDos[MAX_POINT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
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

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


void CrearDado()
{
	unsigned int cubo_indices[] = {
		// 1
		0, 1, 2,
		2, 3, 0,
		// 2
		4, 5, 6,
		6, 7, 4,
		//3
		8,9,10,
		10,11,8,
		//4
		12,13,14,
		14,15,12,
		//5
		16,17,18,
		18,19,16,
		//6
		20,21,22,
		22,23,20,
		//7
		24,25,26,
		26,27,24,
		//8
		28,29,30,
		30,31,28,
		//9
		32,33,34,
		34,35,32,
		//10
		36,37,38,
		38,39,36
	};

	GLfloat cubo_vertices[] = {
		// 1
		//x		y		z		Sx		Ty			NX		NY		NZ
		0.0f, -0.1f, -0.79f,	0.026f, 0.184f,		-0.002f,	-0.6591f,	0.7509f,	//0
		0.47f, 0.1f, -0.64f,	0.060f, 0.292f,		-0.002f,	-0.6591f,	0.7509f,	//1
		0.0f,  0.8f,  0.0f,		0.421f, 0.254f,		-0.002f,	-0.6591f,	0.7509f,	//2
	   -0.46f, 0.1f, -0.64f,	0.144f, 0.110f,		-0.002f,	-0.6591f,	0.7509f,	//3
	   // 2
	   //x		y		z		S		T
	   0.75f, -0.1f,  -0.25f,  0.2823126609747,0.0213648541817,		-0.704f,-0.659f,0.264f,	//4
	   0.75f,  0.1f,  0.25f,	0.183928221351,0.0919449956508,		-0.704f,-0.659f,0.264f,	//5
	   0.0f,  0.8f,  0.0f,		0.4213341517473,0.2331052785891,	-0.704f,-0.659f,0.264f,	//6
	   0.47f, 0.1f, -0.64f,	0.4491384499018,0.0363363993418,		-0.704f,-0.659f,0.264f,	//7
	   // 3
	   //x		y		z		S		T
	   0.47f, -0.1f,  0.64f,   0.6533931017292,0.0448915680047,		-0.4374f,	-0.661f,	-0.6083f,	//8
	   0.0f,  0.1f,  0.79f,	0.4865673128021,0.0406139836733,		-0.4374f,	-0.661f,	-0.6083f,	//9
	   0.0f,  0.8f,  0.0f,		0.4502078459846,0.2373828629206,		-0.4374f,	-0.661f,	-0.6083f,	//10
	   0.75f,  0.1f,  0.25f,	0.7325284118612,0.1283044624683,		-0.4374f,	-0.661f,	-0.6083f,	//11
	   // 4
	   //x		y		z		S		T
	   -0.46f, -0.1f,  0.64f,  0.4320257815137,0.5081003336641,		0.4253f,	0.6658f,	1.0f,	//12
	   -0.75f,  0.1f,  0.24f,	0.5699802072656,0.4598172481567,		0.4253f,	0.6658f,	1.0f,	//13
	   0.0f,  0.8f,  0.0f,		0.4352341251665,0.2803079343194,		0.4253f,	0.6658f,	1.0f,	//14
	   0.0f,  0.1f,  0.79f,	0.2855208492233,0.4576784559909,		0.4253f,	0.6658f,	1.0f,	//15
	   // 5
	   //x		y		z		S		T 
	   -0.75f, -0.1f, -0.24f,   0.1,    0.4,		                0.727,0.658,0.198,	//16
	   -0.46f,  0.1f, -0.64f,	0.253200727169,0.4493074362276,		0.727,0.658,0.198,	//17
	   0.0f,  0.8f,  0.0f,		0.404426005241,0.274083747633,		0.727,0.658,0.198,	//18
	   -0.75f,  0.1f,  0.24f,	0.0732126482501,0.309904904515,		0.727,0.658,0.198,	//19
	   // 6
	   //x		y		z		S		T 
	   0.47f, 0.1f, -0.64f,	0.5954440797602,0.4826857610856,		0.4184f,	0.6682f,	1.0,    //20
	   0.75f, -0.1f,  -0.25f,  0.741980716338,0.5294258951664,		0.4184f,	0.6682f,	1.0,	//21
	   0.0f,  -0.8f,  0.0f,	0.5992338203614,0.7125966908886,		0.4184f,	0.6682f,	1.0,	//22
	   0.0f, -0.1f, -0.79f,	0.4525105198524,0.5272812847868,		0.4184f,	0.6682f,	1.0,	//23
	   // 7
	   //x		y		z		S		T
	   0.75f,  0.1f,  0.25f,	0.2737126400363,0.585846688167,		-0.729f, 0.659f ,-0.185f,  //24
	   0.47f, -0.1f,  0.64f,   0.2426129223489,0.6802565454325,		-0.729f, 0.659f ,-0.185f,  //25
	   0.0f,  -0.8f,  0.0f,	0.5791562958953,0.720241896745,		    -0.729f, 0.659f ,-0.185f,  //26
	   0.75f, -0.1f,  -0.25f,  0.4258791158643,0.540307815839,		-0.729f, 0.659f ,-0.185f,  //27
	   // 8
	   //x		y		z		S		T  
	   0.0f,  0.1f,  0.79f,	    0.2,   0.8f,	                 	-0.0417f,0.659f,-0.751f,  //28
	   -0.46f, -0.1f,  0.64f,  0.2332122618582,0.6984870535938,		-0.0417f,0.659f,-0.751f,  //29
	   0.0f,  -0.8f,  0.0f,	0.5762020435444,0.7365970293367,		-0.0417f,0.659f,-0.751f,  //30
	   0.47f, -0.1f,  0.64f,   0.3246762036412,0.8748817984609,		-0.0417f,0.659f,-0.751f,	//31
	   // 9
	   //x		y		z		S		T
	   -0.75f,  0.1f,  0.24f,	0.4515593680707,0.9701293420356,	0.7140f,	0.6579f,	-0.2358f,	//32
	   -0.75f, -0.1f, -0.24f,  0.3414977175722,0.8924387652132,		0.7140f,	0.6579f,	-0.2358f,	//33
	   0.0f,  -0.8f,  0.0f,	0.5880239320777,0.7540446913227,		0.7140f,	0.6579f,	-0.2358f,  //34
	   -0.46f, -0.1f,  0.64f,  0.6231260585535,0.9547530820395,		0.7140f,	0.6579f,	-0.2358f,	//35
	   // 10
	   //x		y		z		S		T
	   -0.46f,  0.1f, -0.64f,	0.8157709424386,0.9454741449923,		-0.4568f,	0.6656f,	1.0f,  //36
	   0.0f, -0.1f, -0.79f,	0.65,0.95,		-0.4568f,	0.6656f,	1.0f,	//37
	   0.0f,  -0.8f,  0.0f,	0.6116258492602,0.7474841802514,		-0.4568f,	0.6656f,	1.0f,  //38
	   -0.75f, -0.1f, -0.24f,  0.9032772423706,0.8579735154832,		-0.4568f,	0.6656f,	1.0f,	//39
	};
	Mesh* dado = new Mesh();
	dado->CreateMesh(cubo_vertices, cubo_indices, 320, 60);
	meshList.push_back(dado);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	CreateObjects();
	CreateShaders();
	CrearDado();
	bool bandera = true;
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

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

	dadoTexture = Texture("Textures/numDeca.tga");
	dadoTexture.LoadTextureA();

	LlantaDerBackimport = Texture("Textures/llanta.tga");
	LlantaDerBackimport.LoadTexture();
	Carroceriaimport = Texture("Textures/CarsN1KND-317164251.tga");
	Carroceriaimport.LoadTexture();
	LampImport = Texture("Textures/Lamp.tga");
	LampImport.LoadTexture();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	

	Carroceria = Model();
	Carroceria.LoadModel("Models/Carroceria.obj");

	LlantaDerFront = Model();
	LlantaDerFront.LoadModel("Models/LlantaDerFront.obj");

	LlantaDerBack = Model();
	LlantaDerBack.LoadModel("Models/LlantaDerBack.obj");

	LlantaIzqFront = Model();
	LlantaIzqFront.LoadModel("Models/LlantaIzqFront.obj");

	LlantaIzqBack = Model();
	LlantaIzqBack.LoadModel("Models/LlantaIzqBack.obj");

	Cofre = Model();
	Cofre.LoadModel("Models/Cofre.obj");

	Lamp = Model();
	Lamp.LoadModel("Models/LampPos.obj");

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
		0.3f, 0.3f, // intensidad ambiental e intensidad difusa
		0.0f, 0.0f, -1.0f); //direccion iluminado del sol

	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f, //rojo
		0.5f, 0.5f,
		-6.0f, 1.5f, 1.5f,//posicion
		0.3f, 0.0f, 0.1f); //atenuacion
	pointLightCount++;

	//luz puntual lampara
	pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,  //blanco
		0.3f, 1.0f,
		-30.0f, 10.0f, 10.0,//posicion
		0.0f, 0.05f, 0.005f); //atenuacion
	pointLightCount++;

	/*pointLights[2] = PointLight(0.0f, 1.0f, 0.0f,  //verde
		0.5f, 0.5f,
		-30.0f, 1.5f, 1.5f,//posicion
		0.3f, 0.0f, 0.1f); //atenuacion
	pointLightCount++;*/

	pointLightsDos[0] = pointLights[2];
	pointLightsDos[1] = pointLights[1];
	//pointLightsDos[2] = pointLights[0];


	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f); // tamaño circulo
	spotLightCount++;

	//Faro carro 1
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		18.0f, 10.0f, -25.0f, //posicion
		-2.f, -1.0f, 0.0f, //direccion
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	
	//Faro carro 2
	spotLights[2] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		18.0f, 10.0f, -25.0f, //posicion
		-2.f, -1.0f, 0.0f, //direccion
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	//Luz helicptero
	spotLights[3] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		18.0f, 10.0f, -25.0f, //posicion
		0.0f, -5.0f, 0.0f, //direccion
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

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
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
			glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		

		if (mainWindow.getBandera()) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else {
			shaderList[0].SetPointLights(pointLights, pointLightCount-1);
		}

		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();


		//Dado de Opengl
		//Ejercicio 1: Texturizar su cubo con la imagen dado_animales ya optimizada por ustedes
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Mustang
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-25.0f, -0.5f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f,0.0f + mainWindow.getmuevex()));

		glm::vec3 mustangPos = glm::vec3(model[3]); // Obtener la posición del mustang a partir del modelo transformado
		glm::vec3 mustangFaro = glm::vec3(-15.0f, 5.0f, 5.0f); // Dezplazamiento adicional para que la luz este en el faro
		glm::vec3 lightPos = mustangPos + mustangFaro;  // Dezplazamiento adicional + dezplazamiento del carro (posicion actual del carro + movimiento del carro)
		spotLights[1].SetPos(lightPos); // Heredar la posición desplazada a spotLights[1] (FARO1)

		mustangFaro = glm::vec3(-15.0f, 5.0f, -5.0f); // Dezplazamiento adicional para que la luz este en el faro
		lightPos = mustangPos + mustangFaro;  // Dezplazamiento adicional + dezplazamiento del carro (posicion actual del carro + movimiento del carro)
		spotLights[2].SetPos(lightPos); // Heredar la posición desplazada a spotLights[2] (FARO 2)

		modelaux2 = model;

		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carroceria.RenderModel();

		model = modelaux2;

		// LlantaDerFront
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(6.5f, 2.3f, -10.5f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDerFront.RenderModel();
		model = modelaux2;

		// LlantaDerBack
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(6.5f, 2.3f, 9.5f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaDerBack.RenderModel();
		model = modelaux2;

		// LlantaIzqFront
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-6.5f, 2.3f, -10.5f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaIzqFront.RenderModel();
		model = modelaux2;

		// LlantaIzqBack
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-6.5f, 2.3f, 9.5f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion8()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion9()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LlantaIzqBack.RenderModel();
		model = modelaux2;

		// Cofre
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.15f, -7.0f));

		model = glm::rotate(model, glm::radians(mainWindow.getarticulacion10()), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cofre.RenderModel();


		//Instancia del coche 
		model = glm::mat4(1.0);
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, 00.0f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, 0.0f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, 0.0f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, 0.0f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	
		//helicptero
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f + mainWindow.getarticulacion1(), 0.0f));

		// Obtener la posición del helicoptero a partir del modelo transformado
		glm::vec3 heliPos = glm::vec3(model[3]);
		// Heredar la posición a spotLights[3]
		spotLights[3].SetPos(heliPos);

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();


		//lampara poste
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.0f, 10.0));

		glm::vec3 lampPos = glm::vec3(model[3]);
		glm::vec3 lampPosA = glm::vec3(0.0f, 40.0f, 0.0f); // Altura donde se encuentra el foco de la lampara
		glm::vec3 lampLuz = lampPos + lampPosA;  // Altura donde se encuentra el foco de la lampara + posicion de la lampara
		pointLights[1].SetPos2(lampLuz); // Heredar la posición desplazada a pointLights[1]

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lamp.RenderModel();


		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
