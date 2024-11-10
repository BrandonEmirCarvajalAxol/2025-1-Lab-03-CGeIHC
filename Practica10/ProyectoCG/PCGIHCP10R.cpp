#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS //Arreglar un error en la apertura de archivos


#include <iostream>
#include <random>  // Biblioteca para números aleatorios
#include <ctime>   // Biblioteca para obtener la semilla basada en el tiempo actual
#include <windows.h> // Para Sleep en Windows

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

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include "Model.h"
#include "Skybox.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;


//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

FILE* fk = fopen("keyframesProyecto.txt", "w");

float movOffset;

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


float movDadoX;
float movDadoy;
float movDado8y;
float movDado4y;
float rotDado8;
float rotDado4;

float rotDado8x;
float rotDado8y;
float rotDado8z;

float rotDado4x;
float rotDado4y;
float rotDado4z;

bool banderaDado = false;
bool dadof=false;

float rotCasaArbolKND;

int indiceCasillas;
int avanzaDado;
int texturaTablero;

float movPerX;
float movPerZ;
float rotPery;

float movObjCasillaY;
float rotObjCasillaY;
bool personajeCasilla = false;
bool primerTiro = true;
bool restaDado = true;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;


Texture Tablero0;
Texture Tablero1;
Texture Tablero2;
Texture Tablero3;
Texture Tablero4;
Texture Tablero5;
Texture Tablero6;
Texture Tablero7;
Texture Tablero8;
Texture Tablero9;

Texture Tablero10;
Texture Tablero11;
Texture Tablero12;
Texture Tablero13;
Texture Tablero14;
Texture Tablero15;
Texture Tablero16;
Texture Tablero17;
Texture Tablero18;
Texture Tablero19;

Texture Tablero20;
Texture Tablero21;
Texture Tablero22;
Texture Tablero23;
Texture Tablero24;
Texture Tablero25;
Texture Tablero26;
Texture Tablero27;
Texture Tablero28;
Texture Tablero29;

Texture Tablero30;
Texture Tablero31;
Texture Tablero32;
Texture Tablero33;
Texture Tablero34;
Texture Tablero35;
Texture Tablero36;
Texture Tablero37;
Texture Tablero38;
Texture Tablero39;
Texture Tablero40;

Texture D4;
Texture D8;

Model Dado8c;
Model Dado4c;
Model Tablero;

//Modelos Los chicos del barrio
Model CasaArbolKND;
Model CommonCold;
Model CoolBus;
Model Dragstuh;
Model Elimonater;
Model Musket;
Model Mosquito;
Model Mushi;
Model N1;
Model N2;
Model N4;
Model N9;
Model Roadstar;
Model SCAMPP;
Model SPLANKE;
Model GumGun;
Model Machineggun;
Model SimioArcoiris;
Model Visor;

//Modelos Bob Esponja
Model Larry;
Model CasaBob;
Model CasaPat;
Model Crustaceo;
Model CasaCala;
Model OsoMar;
Model GymLarry;
Model BaldeCar;
Model CasaDonC;
Model Medusa;
Model IslaBob;
Model ChozaBob;
Model Gary;
Model CangreBur;
Model Patricio;
Model PieDerPat;
Model BrazoDerPat;
Model PieIzqPat;
Model BrazoIzqPat;

//Modelos Alicia en el pais de las maravillas
Model Oruga;
Model Alice;
Model FlamGame;
Model Deck;
Model rabbitsHouse;
Model queenGarden;
Model mushrooms;
Model whiteRabbit;
Model teaTime;
Model insideRabbit;
Model potionCake;
Model puerta;
Model queenCastle;

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
	

	unsigned int tableroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat tableroVertices[] = {
	   -60.0f,  0.0f,  60.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		60.0f,  0.0f,  60.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		60.0f,  0.0f, -60.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	   -60.0f,  0.0f, -60.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

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
	obj5->CreateMesh(tableroVertices, tableroIndices, 32, 6);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


///////////////////////////////KEYFRAMES/////////////////////


bool anima = false;



//NEW// Keyframes
float	movVisor_x = 0.0f, movVisor_y = 0.0f;
float giroVisor = 0;

#define MAX_FRAMES 100
int i_max_steps = 90;
int i_curr_steps = 7;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movVisor_x;		//Variable para PosicionX
	float movVisor_y;		//Variable para PosicionY
	float movVisor_xInc;		//Variable para IncrementoX
	float movVisor_yInc;		//Variable para IncrementoY
	float giroVisor;
	float giroVisorInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(const char* archivo) //tecla L
{

	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movVisor_x = movVisor_x;
	KeyFrame[FrameIndex].movVisor_y = movVisor_y;
	KeyFrame[FrameIndex].giroVisor = giroVisor;
	// para hacerlo no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FILE* f;
	f = fopen(archivo, "a");

	if (f == nullptr) {
		perror("Error al abrir el archivo");
		return;
	}

	fprintf(f, "KeyFrame[%d].movVisor_x = %.2f\n", FrameIndex, KeyFrame[FrameIndex].movVisor_x);
	fprintf(f, "KeyFrame[%d].movVisor_y = %.2f\n", FrameIndex, KeyFrame[FrameIndex].movVisor_y);
	fprintf(f, "KeyFrame[%d].giroVisor = %.2f\n", FrameIndex, KeyFrame[FrameIndex].giroVisor);
	fprintf(f, "\n\n");
	fclose(f);
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{

	movVisor_x = KeyFrame[0].movVisor_x;
	movVisor_y = KeyFrame[0].movVisor_y;
	giroVisor = KeyFrame[0].giroVisor;
}

void interpolation(void)
{
	KeyFrame[playIndex].movVisor_xInc = (KeyFrame[playIndex + 1].movVisor_x - KeyFrame[playIndex].movVisor_x) / i_max_steps;
	KeyFrame[playIndex].movVisor_yInc = (KeyFrame[playIndex + 1].movVisor_y - KeyFrame[playIndex].movVisor_y) / i_max_steps;
	KeyFrame[playIndex].giroVisorInc = (KeyFrame[playIndex + 1].giroVisor - KeyFrame[playIndex].giroVisor) / i_max_steps;

}

void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animación entre frames?
		{
			playIndex++;
			printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animación con último frame?
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolación del próximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animación
			movVisor_x += KeyFrame[playIndex].movVisor_xInc;
			movVisor_y += KeyFrame[playIndex].movVisor_yInc;
			giroVisor += KeyFrame[playIndex].giroVisorInc;
			i_curr_steps++;
		}

	}
}

///////////////* FIN KEYFRAMES*////////////////////////////


int main()
{
	std::mt19937 generator(static_cast<unsigned int>(std::time(0)));

	int RD8 = 0;
	int RD4 = 0;

	mainWindow = Window(1366, 768);
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
	pisoTexture = Texture("Textures/pisoaj.tga");
	pisoTexture.LoadTextureA();

	Tablero0 = Texture("Textures/Tablero0.tga");
	Tablero0.LoadTextureA();
	Tablero1 = Texture("Textures/tablero1.tga");
	Tablero1.LoadTextureA();
	Tablero2 = Texture("Textures/tablero2.tga");
	Tablero2.LoadTextureA();
	Tablero3 = Texture("Textures/tablero3.tga");
	Tablero3.LoadTextureA();
	Tablero4 = Texture("Textures/tablero4.tga");
	Tablero4.LoadTextureA();
	Tablero5 = Texture("Textures/tablero5.tga");
	Tablero5.LoadTextureA();
	Tablero6 = Texture("Textures/tablero6.tga");
	Tablero6.LoadTextureA();
	Tablero7 = Texture("Textures/tablero7.tga");
	Tablero7.LoadTextureA();
	Tablero8 = Texture("Textures/tablero8.tga");
	Tablero8.LoadTextureA();
	Tablero9 = Texture("Textures/tablero9.tga");
	Tablero9.LoadTextureA();
	Tablero10 = Texture("Textures/tablero10.tga");
	Tablero10.LoadTextureA();
	Tablero11 = Texture("Textures/tablero11.tga");
	Tablero11.LoadTextureA();
	Tablero12 = Texture("Textures/tablero12.tga");
	Tablero12.LoadTextureA();
	Tablero13 = Texture("Textures/tablero13.tga");
	Tablero13.LoadTextureA();
	Tablero14 = Texture("Textures/tablero14.tga");
	Tablero14.LoadTextureA();
	Tablero15 = Texture("Textures/tablero15.tga");
	Tablero15.LoadTextureA();
	Tablero16 = Texture("Textures/tablero16.tga");
	Tablero16.LoadTextureA();
	Tablero17 = Texture("Textures/tablero17.tga");
	Tablero17.LoadTextureA();
	Tablero18 = Texture("Textures/tablero18.tga");
	Tablero18.LoadTextureA();
	Tablero19 = Texture("Textures/tablero19.tga");
	Tablero19.LoadTextureA();
	Tablero20 = Texture("Textures/tablero20.tga");
	Tablero20.LoadTextureA();
	Tablero21 = Texture("Textures/tablero21.tga");
	Tablero21.LoadTextureA();
	Tablero22 = Texture("Textures/tablero22.tga");
	Tablero22.LoadTextureA();
	Tablero23 = Texture("Textures/tablero23.tga");
	Tablero23.LoadTextureA();
	Tablero24 = Texture("Textures/tablero24.tga");
	Tablero24.LoadTextureA();
	Tablero25 = Texture("Textures/tablero25.tga");
	Tablero25.LoadTextureA();
	Tablero26 = Texture("Textures/tablero26.tga");
	Tablero26.LoadTextureA();
	Tablero27 = Texture("Textures/tablero27.tga");
	Tablero27.LoadTextureA();
	Tablero28 = Texture("Textures/tablero28.tga");
	Tablero28.LoadTextureA();
	Tablero29 = Texture("Textures/tablero29.tga");
	Tablero29.LoadTextureA();
	Tablero30 = Texture("Textures/tablero30.tga");
	Tablero30.LoadTextureA();
	Tablero31 = Texture("Textures/tablero31.tga");
	Tablero31.LoadTextureA();
	Tablero32 = Texture("Textures/tablero32.tga");
	Tablero32.LoadTextureA();
	Tablero33 = Texture("Textures/tablero33.tga");
	Tablero33.LoadTextureA();
	Tablero34 = Texture("Textures/tablero34.tga");
	Tablero34.LoadTextureA();
	Tablero35 = Texture("Textures/tablero35.tga");
	Tablero35.LoadTextureA();
	Tablero36 = Texture("Textures/tablero36.tga");
	Tablero36.LoadTextureA();
	Tablero37 = Texture("Textures/tablero37.tga");
	Tablero37.LoadTextureA();
	Tablero38 = Texture("Textures/tablero38.tga");
	Tablero38.LoadTextureA();
	Tablero39 = Texture("Textures/tablero39.tga");
	Tablero39.LoadTextureA();
	Tablero40 = Texture("Textures/tablero40.tga");
	Tablero40.LoadTextureA();

	D8 = Texture("Textures/D8C.jpg");
	D8.LoadTextureA();
	D4 = Texture("Textures/D4C.jpg");
	D4.LoadTextureA();

	Dado8c = Model();
	Dado8c.LoadModel("Models/Dado8C.obj");
	Dado4c = Model();
	Dado4c.LoadModel("Models/Dado4C.obj");


	CasaArbolKND = Model();
	CasaArbolKND.LoadModel("Models/CasaArbolKND.obj");
	CommonCold = Model();
	CommonCold.LoadModel("Models/CommonCold.obj");
	CoolBus = Model();
	CoolBus.LoadModel("Models/CoolBus.obj");
	Dragstuh = Model();
	Dragstuh.LoadModel("Models/Dragstuh.obj");
	Elimonater = Model();
	Elimonater.LoadModel("Models/Elimonater.obj");
	Musket = Model();
	Musket.LoadModel("Models/Musket.obj");
	Mosquito = Model();
	Mosquito.LoadModel("Models/Mosquito.obj");
	Mushi = Model();
	Mushi.LoadModel("Models/Mushi.obj");
	N1 = Model();
	N1.LoadModel("Models/N1.obj");
	N2 = Model();
	N2.LoadModel("Models/N2.obj");
	N4 = Model();
	N4.LoadModel("Models/N4.obj");
	N9 = Model();
	N9.LoadModel("Models/N9.obj");
	Roadstar = Model();
	Roadstar.LoadModel("Models/Roadstar.obj");
	SCAMPP = Model();
	SCAMPP.LoadModel("Models/SCAMPP.obj");
	SPLANKE = Model();
	SPLANKE.LoadModel("Models/SPLANKE.obj");
	GumGun = Model();
	GumGun.LoadModel("Models/GumGun.obj");
	Machineggun = Model();
	Machineggun.LoadModel("Models/Machineggun.obj");
	SimioArcoiris = Model();
	SimioArcoiris.LoadModel("Models/SimioArcoiris.obj");
	Visor = Model();
	Visor.LoadModel("Models/Visor.obj");


	Larry = Model();
	Larry.LoadModel("Models/Larry.obj");
	CasaBob = Model();
	CasaBob.LoadModel("Models/PiñaBob.obj");
	CasaPat = Model();
	CasaPat.LoadModel("Models/CasaPat.obj");
	CasaCala = Model();
	CasaCala.LoadModel("Models/CasaCalamardo.obj");
	Crustaceo = Model();
	Crustaceo.LoadModel("Models/Crustaceo.obj");
	OsoMar = Model();
	OsoMar.LoadModel("Models/OsoMarino.obj");
	GymLarry = Model();
	GymLarry.LoadModel("Models/GymLarry.obj");
	BaldeCar = Model();
	BaldeCar.LoadModel("Models/BaldeCarnada.obj");
	CasaDonC = Model();
	CasaDonC.LoadModel("Models/CasaDonCangrejo.obj");
	Medusa = Model();
	Medusa.LoadModel("Models/Medusa.obj");
	IslaBob = Model();
	IslaBob.LoadModel("Models/IslaBob.obj");
	ChozaBob = Model();
	ChozaBob.LoadModel("Models/Choza.obj");
	Gary = Model();
	Gary.LoadModel("Models/Gary.obj");
	CangreBur = Model();
	CangreBur.LoadModel("Models/CangreBurg.obj");

	Patricio = Model();
	Patricio.LoadModel("Models/patricio.obj");
	PieDerPat = Model();
	PieDerPat.LoadModel("Models/piederpat.obj");
	PieIzqPat = Model();
	PieIzqPat.LoadModel("Models/pieizqpat.obj");
	BrazoDerPat = Model();
	BrazoDerPat.LoadModel("Models/brazoderpat.obj");
	BrazoIzqPat = Model();
	BrazoIzqPat.LoadModel("Models/brazoizqpat.obj");

	/*Oruga = Model();
	Oruga.LoadModel("Models/Oruga.obj");
	Alice = Model();
	Alice.LoadModel("Models/Alice.obj");
	FlamGame = Model();
	FlamGame.LoadModel("Models/flam-game.obj");
	Deck = Model();
	Deck.LoadModel("Models/Deck.obj");*/

	/*teaTime = Model();
	teaTime.LoadModel("Models/Tea time.obj");
	puerta = Model();
	puerta.LoadModel("Models/puerta.obj");
	rabbitsHouse = Model();
	rabbitsHouse.LoadModel("Models/rabbitsHouse.obj");

	queenCastle = Model();
	queenCastle.LoadModel("Models/queenCastle.obj");
	insideRabbit = Model();
	insideRabbit.LoadModel("Models/insideRabbit.obj");

	mushrooms = Model();
	mushrooms.LoadModel("Models/mushrooms.obj");*/

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");

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
	/*pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	/*
	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	movOffset = 0.3f;
	movDadoX = 0.0f;
	movDadoy = 0.0f;
	movDado8y = 0.0f;
	movDado4y = 0.0f;
	rotDado8 = 0.0f;
	rotDado4 = 0.0f;

	rotDado8x = 0.0f;
	rotDado8y = 0.0f;
	rotDado8z = 0.0f;

	rotDado4x = 0.0f;
	rotDado4y = 0.0f;
	rotDado4z = 0.0f;

	rotCasaArbolKND = 0.0f;

	indiceCasillas = 0;
	avanzaDado = 0;
	texturaTablero = 0;


	movPerX = 70;
	movPerZ = 53;

	movObjCasillaY = -10.0f;
	rotObjCasillaY = 0.0f;

	float rotOffDado = 15.0f;
	float rotOffCasaArbolKND = 1.5f;


	// Variables para controlar las fases de la animación
	int animationPhase = 1; // Inicia con la primera animación
	float phaseStartTime = glfwGetTime(); // Guarda el tiempo de inicio de la fase
	const float ANIMATION_PAUSE_DURATION = 3.0f; // Duración de la pausa en segundos

	//---------PARA TENER KEYFRAMES GUARDADOS NO VOLATILES QUE SIEMPRE SE UTILIZARAN SE DECLARAN AQUÍ

	printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
	printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para mover en X negativa\n6.-Presiona 2 para habilitar mover variables\n");
	printf("7.-Presiona 3 para mover en X positiva\n8.-Presiona 4 para mover en Y negativa\n9.-Presiona 5 para mover en Y positiva\n10.-Presiona 6 para girar 90 grados\n");
	printf("11.-Presiona 7 para girar menos 90 grados\n");


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		double temp = glfwGetTime();
		float rotExtPat = 5 * sin(6.0f * glfwGetTime() * 1.0f);

		rotCasaArbolKND += rotOffCasaArbolKND * deltaTime;


		glm::vec3 avatarPos;

		//Va al principio del while y antes del if de getbandera
		if (mainWindow.getcamara() == 0)
		{
			projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
			glm::vec3 cameraPosition = avatarPos + glm::vec3(0.0f, 5.0f, -10.0f); // Detrás y arriba del avatar
			glm::mat4 view = glm::lookAt(cameraPosition, avatarPos, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		//else if (mainWindow.getcamara() == 1)
		//{
		//	// Proyección ortográfica mirando el tablero desde arriba
		//	float orthoSize = 80.0f;
		//	projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 25.0f, 200.0f);

		//	// Definir la vista desde arriba
		//	glm::mat4 view = glm::lookAt(
		//		glm::vec3(0.0f, 90.0f, 0.0f), // Posición de la cámara arriba del tablero
		//		glm::vec3(0.0f, 0.0f, 0.0f),  // Mirando al centro del tablero
		//		glm::vec3(0.0f, 0.0f, -1.0f)
		//	);
		//	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		//	camera.keyControl(mainWindow.getsKeys(), deltaTime);
		//}
		//else if (mainWindow.getcamara() == 2)
		//{
		//	// Cámara estática
		//	projection = glm::perspective(90.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

		//	// Calcular la posición de Patricio (asumiendo que tienes esta información)
		//	glm::vec3 patricioPosition = glm::vec3(5.0f, 0.0f, 1.0f); // Ajusta esto a la posición real de Patricio

		//	// Definir la vista siguiendo a Patricio
		//	glm::mat4 view = glm::lookAt(
		//		patricioPosition + glm::vec3(0.0f, 5.0f, -10.0f), // Posición de la cámara detrás y arriba de Patricio
		//		patricioPosition, // Mirando a Patricio
		//		glm::vec3(0.0f, 0.0f, 1.0f)  // Vector "arriba"
		//	);
		//	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		//	camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		//}

		//Recibir eventos del usuario
		glfwPollEvents();
		//-------Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

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




		if (mainWindow.getBandera()) //Presionar Z para mover el dado y X para reiniciar la tirada
		{
			if (movDadoy > 0.0 && movDadoX > -5.0)
			{
				movDadoX += movOffset * deltaTime;
				movDadoy -= movOffset * deltaTime;
				movDado8y -= movOffset * deltaTime;
				movDado4y -= movOffset * deltaTime;
				rotDado8 += rotOffDado * deltaTime;
				rotDado4 += rotOffDado * deltaTime;
				
				if (banderaDado == false) { // if para objener una sola vez por tirada de dados el valor de cada uno
					// Distribución para el dado de 8 caras en el rango [1, 8]
					std::uniform_int_distribution<int> distribution1(1, 8);
					RD8 = distribution1(generator);

					// Distribución para el dado de 4 caras en el rango [1, 4]
					std::uniform_int_distribution<int> distribution2(1, 4);
					RD4 = distribution2(generator);
					avanzaDado = 22; //variable para saber cuantas casillas avanzar 
					std::cout << "avanzaDado: " << avanzaDado << std::endl;
					banderaDado = true;
				}

			}
			else { 
				dadof = true; 
				banderaDado = false;
			}
			if (dadof) {

				if (RD8 == 1) { // D8=1
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = -30;
					rotDado8y = 0;
					rotDado8z = -70;
					RD8 = 0;
				}
				if (RD8 == 2) {//D8=2
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = 30;
					rotDado8y = 0;
					rotDado8z = 110;
					RD8 = 0;
				}
				if (RD8 == 3) { // D8=3
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = -30;
					rotDado8y = 0;
					rotDado8z = 70;
					RD8 = 0;
				}
				if (RD8 == 4) { // D8=4
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = 150;
					rotDado8y = 0;
					rotDado8z = 70;
					RD8 = 0;
				}
				if (RD8 == 5) {//D8=5
					movDado8y = -1.6;
					rotDado8 = 0;
					rotDado8x = 71;
					rotDado8y = 0;
					rotDado8z = 0;
					RD8 = 0;
				}
				if (RD8 == 6) {//D8=6
					movDado8y =0.3;
					rotDado8 = 0;
					rotDado8x = -110;
					rotDado8y = 0;
					rotDado8z = 0;
					RD8 = 0;
				}
				if (RD8 == 7) {//D8=7
					movDado8y = 0.3;
					rotDado8 = 0;
					rotDado8x = 0;
					rotDado8y = 0;
					rotDado8z = -180;
					RD8 = 0;
				}
				if (RD8 == 8) {//D8=8
					movDado8y = -1.6;
					rotDado8 = 0;
					rotDado8x = 0;
					rotDado8y = 0;
					rotDado8z = 0;
					RD8 = 0;
				}

				if (RD4 == 1) {//D4=1
					movDado4y = -1.6;
					rotDado4 = 0;
					rotDado4x = 0;
					rotDado4y = 0;
					rotDado4z = 0;
					RD4 = 0;
				}
				if (RD4 == 2) {//D4=2
					movDado4y = -0.7;
					rotDado4 = 0;
					rotDado4x = 110;
					rotDado4y = -120;
					rotDado4z = 0;
					RD4 = 0;
				}
				if (RD4 == 3) {//D4=3
					movDado4y = -0.7;
					rotDado4 = 0;
					rotDado4x = 110;
					rotDado4y = 0;
					rotDado4z = 0;
					RD4 = 0;
				}
				if (RD4 == 4) {//D4=4
					movDado4y = -0.7;
					rotDado4 = 0;
					rotDado4x = 110;
					rotDado4y = 120;
					rotDado4z = 0;
					RD4 = 0;
				}
			}

		} else {
			movDadoX = 0.0;
			movDadoy = 5.0;
			movDado8y = 5.0;
			movDado4y = 5.0;
			rotDado8 = 0.0;
			rotDado4 = 0.0;
			dadof = false;

		}
		
		if (avanzaDado != 0 && primerTiro == true) {
			indiceCasillas = 1;
			primerTiro = false;
		}

		if (indiceCasillas == 1) { //lateral inferior tablero y rotate de personaje

			texturaTablero = 0;

			if (movPerZ < 53.0) {
				movPerZ += 0.1f;
			}else if (movPerX > 51.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < 60.0) {
				texturaTablero = 1;
			}

			if (movPerX < 51.0) {

				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}
			

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 1;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(51.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Larry.RenderModel();
			} else if (personajeCasilla==true){
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 2;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 2) {

			texturaTablero = 0;

			if (movPerX > 39.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < 42.0) {
				texturaTablero = 2;
			}

			if (movPerX < 39.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 2;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(39.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, -140 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CasaBob.RenderModel();
			} else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 3;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 3) {

			texturaTablero = 0;

			if (movPerX > 29.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < 33.0) {
				texturaTablero = 3;
			}

			if (movPerX < 29.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 3;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(29.0f, movObjCasillaY, 45.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				N4.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 4;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 4) {

			texturaTablero = 0;

			if (movPerX > 19.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < 23.0) {
				texturaTablero = 4;
			}

			if (movPerX < 19.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 4;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(19.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Oruga.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 5;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 5) {

			texturaTablero = 0;

			if (movPerX > 9.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < 13.0) {
				texturaTablero = 5;
			}

			if (movPerX < 9.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 4.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 5;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(9.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CasaPat.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 6;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 6) {

			texturaTablero = 0;

			if (movPerX > 0.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < 0.0) {
				texturaTablero = 6;
			}

			if (movPerX < 9.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 6;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(0.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				SCAMPP.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 7;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 7) {

			texturaTablero = 0;

			if (movPerX > -10.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < -5.0) {
				texturaTablero = 7;
			}

			if (movPerX < -10.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 7;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-10.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Alice.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 8;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 8) {

			texturaTablero = 0;

			if (movPerX > -20.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < -15.0) {
				texturaTablero = 8;
			}

			if (movPerX < -20.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 8;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-20.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CasaCala.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 9;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 9) {

			texturaTablero = 0;

			if (movPerX > -30.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < -25.0) {
				texturaTablero = 9;
			}

			if (movPerX < -30.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 9;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-30.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				SPLANKE.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 10;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}



		if (indiceCasillas == 10) {

			texturaTablero = 0;

			if (movPerX > -40.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < -35.0) {
				texturaTablero = 10;
			}

			if (movPerX < -30.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 10;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-40.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				FlamGame.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 11;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 11) {  //rotate para el modelo personaje

			texturaTablero = 0;

			if (movPerX > -54.0) {
				movPerX -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX < -45.0) {
				texturaTablero = 11;
			}

			if (movPerX < -54.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 11;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-50.0f, movObjCasillaY, 45.0f));
				model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Crustaceo.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 12;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 12) {  //Lateral Izquierda Tablero

			texturaTablero = 0;

			if (movPerZ > 38.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < 43.0) {
				texturaTablero = 12;
			}

			if (movPerZ < 38.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 12;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, 38.0f));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mosquito.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 13;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 13) {

			texturaTablero = 0;

			if (movPerZ > 28.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < 33.0) {
				texturaTablero = 13;
			}

			if (movPerZ < 28.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 13;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, 28.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Deck.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 14;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 14) {

			texturaTablero = 0;

			if (movPerZ > 18.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < 23.0) {
				texturaTablero = 14;
			}

			if (movPerZ < 18.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 14;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, 18.0f));
				model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				OsoMar.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 15;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 15) {

			texturaTablero = 0;

			if (movPerZ > 8.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < 13.0) {
				texturaTablero = 15;
			}

			if (movPerZ < 8.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 15;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, 8.0f));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CoolBus.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 16;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 16) {

			texturaTablero = 0;

			if (movPerZ > 0.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < -5.0) {
				texturaTablero = 16;
			}

			if (movPerZ < 0.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 16;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				rabbitsHouse.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 17;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 17) {

			texturaTablero = 0;

			if (movPerZ > -10.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < -5.0) {
				texturaTablero = 17;
			}

			if (movPerZ < -10.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 17;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, -10.0f));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				GymLarry.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 18;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 18) {

			texturaTablero = 0;

			if (movPerZ > -20.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < -15.0) {
				texturaTablero = 18;
			}

			if (movPerZ < -20.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 18;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, -20.0f));
				model = glm::scale(model, glm::vec3(0.15f, 0.15f, 0.15f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Dragstuh.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 19;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 19) {

			texturaTablero = 0;

			if (movPerZ > -30.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < -25.0) {
				texturaTablero = 19;
			}

			if (movPerZ < -30.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 19;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, -30.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				queenGarden.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 20;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}



		if (indiceCasillas == 20) {

			texturaTablero = 0;

			if (movPerZ > -38.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < -33.0) {
				texturaTablero = 20;
			}

			if (movPerZ < -38.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 20;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, -38.0f));
				model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				BaldeCar.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 21;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 21) { //rotate personaje

			texturaTablero = 0;

			if (movPerZ > -54.0) {
				movPerZ -= 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ < -43.0) {
				texturaTablero = 21;
			}

			if (movPerZ < -54.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 21;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-45.0f, movObjCasillaY, -50.0f));
				model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Elimonater.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 22;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}
		
		
		if (indiceCasillas == 22) {

			texturaTablero = 0;

			if (movPerX < -38.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > -43.0) {
				texturaTablero = 22;
			}

			if (movPerX > -38.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 22;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-38.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				mushrooms.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 23;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}



		if (indiceCasillas == 23) {

			texturaTablero = 0;

			if (movPerX < -28.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > -33.0) {
				texturaTablero = 23;
			}

			if (movPerX > -28.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 23;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-28.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CasaDonC.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 24;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 24) {

			texturaTablero = 0;

			if (movPerX < -18.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > -23.0) {
				texturaTablero = 24;
			}

			if (movPerX > -18.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 24;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-18.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Roadstar.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 25;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 25) {

			texturaTablero = 0;

			if (movPerX < -10.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > -15.0) {
				texturaTablero = 25;
			}

			if (movPerX > -10.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 25;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(-10.0f, movObjCasillaY, -45.0f));
				//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				whiteRabbit.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 26;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 26) {

			texturaTablero = 0;

			if (movPerX < 0.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > -5.0) {
				texturaTablero = 26;
			}

			if (movPerX > 0.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 26;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(0.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Medusa.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 27;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 27) {

			texturaTablero = 0;

			if (movPerX < 10.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > 5.0) {
				texturaTablero = 27;
			}

			if (movPerX > 10.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 27;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(10.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				N2.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 28;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 28) {

			texturaTablero = 0;

			if (movPerX < 19.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > 14.0) {
				texturaTablero = 28;
			}

			if (movPerX > 19.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 28;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(19.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				teaTime.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 29;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 29) {

			texturaTablero = 0;

			if (movPerX < 29.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > 24.0) {
				texturaTablero = 29;
			}

			if (movPerX > 29.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 29;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(29.0f, movObjCasillaY, -45.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				IslaBob.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 30;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 30) {

			texturaTablero = 0;

			if (movPerX < 39.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > 34.0) {
				texturaTablero = 30;
			}

			if (movPerX > 39.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 30;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(39.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				N9.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 31;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 31) { //rotate personaje

			texturaTablero = 0;

			if (movPerX < 54.0) {
				movPerX += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerX > 43.0) {
				texturaTablero = 31;
			}

			if (movPerX > 54.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 31;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(50.0f, movObjCasillaY, -45.0f));
				model = glm::scale(model, glm::vec3(0.08f, 0.08f, 0.08f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				insideRabbit.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 32;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 32) { //Laterar derecho tablero

			texturaTablero = 0;

			if (movPerZ < -38.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > -43.0) {
				texturaTablero = 32;
			}

			if (movPerZ > -38.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 32;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, -38.0f));
				model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				ChozaBob.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 33;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 33) { //Laterar derecho tablero

			texturaTablero = 0;

			if (movPerZ < -28.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > -33.0) {
				texturaTablero = 33;
			}

			if (movPerZ > -28.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 33;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, -28.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Musket.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 34;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 34) {

			texturaTablero = 0;

			if (movPerZ < -18.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > -23.0) {
				texturaTablero = 34;
			}

			if (movPerZ > -18.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 34;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, -18.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				potionCake.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 35;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 35) {

			texturaTablero = 0;

			if (movPerZ < -9.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > -14.0) {
				texturaTablero = 35;
			}

			if (movPerZ > -9.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 35;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, -9.0f));
				model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
				model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Gary.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 36;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 36) {

			texturaTablero = 0;

			if (movPerZ < 0.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > -5.0) {
				texturaTablero = 36;
			}

			if (movPerZ > 0.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 36;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, 0.0f));
				//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CommonCold.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 37;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 37) {

			texturaTablero = 0;

			if (movPerZ < 10.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > 5.0) {
				texturaTablero = 37;
			}

			if (movPerZ > 10.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 37;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, 10.0f));
				model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				puerta.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 38;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 38) {

			texturaTablero = 0;

			if (movPerZ < 19.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > 14.0) {
				texturaTablero = 38;
			}

			if (movPerZ > 19.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 38;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, 19.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				CangreBur.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 39;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		if (indiceCasillas == 39) {

			texturaTablero = 0;

			if (movPerZ < 28.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > 23.0) {
				texturaTablero = 39;
			}

			if (movPerZ > 28.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 39;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, 28.0f));
				model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				Mushi.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 40;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}


		if (indiceCasillas == 40) {

			texturaTablero = 0;

			if (movPerZ < 38.0) {
				movPerZ += 0.1f;
			}
			else {
				personajeCasilla = true;
			}

			if (movPerZ > 33.0) {
				texturaTablero = 40;
			}

			if (movPerZ > 38.0) {
				// Control de las fases de la animación
				if (animationPhase == 1) {
					// Fase de Animación 1: Incremento de posición y rotación
					if (movObjCasillaY < 5.0f) {
						movObjCasillaY += 0.5f * deltaTime;
						rotObjCasillaY += 0.5f * deltaTime;
					}
					else {
						// Cambia a la fase de pausa y reinicia el temporizador
						animationPhase = 2;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 2) {
					// Fase de pausa: Solo rotación durante 3 segundos
					if (now - phaseStartTime < ANIMATION_PAUSE_DURATION) {
						rotObjCasillaY += 5.0f * deltaTime;
					}
					else {
						// Cambia a la segunda animación y reinicia el temporizador
						animationPhase = 3;
						phaseStartTime = now;
					}
				}
				else if (animationPhase == 3) {
					// Fase de Animación 2: Decremento de posición y rotación
					if (movObjCasillaY > -20.0f) {
						movObjCasillaY -= 0.5f * deltaTime;
						rotObjCasillaY -= 0.5f * deltaTime;
					}
				}
			}

			if (avanzaDado != 0 && restaDado == true) {
				avanzaDado -= 1;
				restaDado = false;
			}

			if (avanzaDado == 0) {
				indiceCasillas = 40;
				model = glm::mat4(1.0);
				model = glm::translate(model, glm::vec3(45.0f, movObjCasillaY, 38.0f));
				model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
				model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				queenCastle.RenderModel();
			}
			else if (personajeCasilla == true) {
				restaDado = true;
				personajeCasilla = false;
				indiceCasillas = 1;
				texturaTablero = 0;
				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;

				animationPhase = 1; // Reinicia la animación a la fase 1
				movObjCasillaY = 0.0f; // Reinicia la posición
				rotObjCasillaY = 0.0f; // Reinicia la rotación
				phaseStartTime = now;  // Reinicia el tiempo de fase
			}
		}

		//Dado8c
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-movDadoX, movDadoy, -2.0f));
		model = glm::translate(model, glm::vec3(-movDadoX, movDado8y, -2.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, rotDado8 * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));

		
		model = glm::rotate(model, rotDado8x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDado8y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotDado8z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado8c.RenderModel();

		//Dado4c
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-movDadoX, movDadoy, -2.0f));
		model = glm::translate(model, glm::vec3(-movDadoX, movDado4y, -2.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, rotDado4 * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, rotDado4z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotDado4x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDado4y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado4c.RenderModel();

		//Objetos MAPA
		//CasaArbolKND
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100, 10, -100.0f));
		model = glm::rotate(model, rotCasaArbolKND * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaArbolKND.RenderModel();

		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, -90.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GumGun.RenderModel();

		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f + movVisor_x, -2.0f + movVisor_y, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, giroVisor * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Visor.RenderModel();

		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SimioArcoiris.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Machineggun.RenderModel();




		//Personaje
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movPerX, -2.0f, movPerZ));
		//model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, rotPery * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		N1.RenderModel();

		

		toffset = glm::vec2(0.0, 0.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (texturaTablero == 0) {
			Tablero0.UseTexture();
		}
		else if (texturaTablero == 1) {
			Tablero1.UseTexture();
		}
		else if (texturaTablero == 2) {
			Tablero2.UseTexture();
		}
		else if (texturaTablero == 3) {
			Tablero3.UseTexture();
		}
		else if (texturaTablero == 4) {
			Tablero4.UseTexture();
		}
		else if (texturaTablero == 5) {
			Tablero5.UseTexture();
		}
		else if (texturaTablero == 6) {
			Tablero6.UseTexture();
		}
		else if (texturaTablero == 7) {
			Tablero7.UseTexture();
		}
		else if (texturaTablero == 8) {
			Tablero8.UseTexture();
		}
		else if (texturaTablero == 9) {
			Tablero9.UseTexture();
		}
		else if (texturaTablero == 10) {
			Tablero10.UseTexture();
		}
		else if (texturaTablero == 11) {
			Tablero11.UseTexture();
		}
		else if (texturaTablero == 12) {
			Tablero12.UseTexture();
		}
		else if (texturaTablero == 13) {
			Tablero13.UseTexture();
		}
		else if (texturaTablero == 14) {
			Tablero14.UseTexture();
		}
		else if (texturaTablero == 15) {
			Tablero15.UseTexture();
		}
		else if (texturaTablero == 16) {
			Tablero16.UseTexture();
		}
		else if (texturaTablero == 17) {
			Tablero17.UseTexture();
		}
		else if (texturaTablero == 18) {
			Tablero18.UseTexture();
		}
		else if (texturaTablero == 19) {
			Tablero19.UseTexture();
		}
		else if (texturaTablero == 20) {
			Tablero20.UseTexture();
		}
		else if (texturaTablero == 21) {
			Tablero21.UseTexture();
		}
		else if (texturaTablero == 22) {
			Tablero22.UseTexture();
		}
		else if (texturaTablero == 23) {
			Tablero23.UseTexture();
		}
		else if (texturaTablero == 24) {
			Tablero24.UseTexture();
		}
		else if (texturaTablero == 25) {
			Tablero25.UseTexture();
		}
		else if (texturaTablero == 26) {
			Tablero26.UseTexture();
		}
		else if (texturaTablero == 27) {
			Tablero27.UseTexture();
		}
		else if (texturaTablero == 28) {
			Tablero28.UseTexture();
		}
		else if (texturaTablero == 29) {
			Tablero29.UseTexture();
		}
		else if (texturaTablero == 30) {
			Tablero30.UseTexture();
		}
		else if (texturaTablero == 31) {
			Tablero31.UseTexture();
		}
		else if (texturaTablero == 32) {
			Tablero32.UseTexture();
		}
		else if (texturaTablero == 33) {
			Tablero33.UseTexture();
		}
		else if (texturaTablero == 34) {
			Tablero34.UseTexture();
		}
		else if (texturaTablero == 35) {
			Tablero35.UseTexture();
		}
		else if (texturaTablero == 36) {
			Tablero36.UseTexture();
		}
		else if (texturaTablero == 37) {
			Tablero37.UseTexture();
		}
		else if (texturaTablero == 38) {
			Tablero38.UseTexture();
		}
		else if (texturaTablero == 39) {
			Tablero39.UseTexture();
		}
		else if (texturaTablero == 40) {
			Tablero40.UseTexture();
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();


		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}


void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\n presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;

			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1 && reproduciranimacion>0)
		{
			printf("Ya puedes reproducir de nuevo la animación con la tecla de barra espaciadora'\n");
			reproduciranimacion = 0;
			habilitaranimacion++; //solo entro una vez al if
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame("keyframesProyecto.txt");
			printf("movVisor_x es: %f\n", movVisor_x);
			printf("movVisor_y es: %f\n", movVisor_y);
			printf("giroVisor es: %f\n", giroVisor);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			reinicioFrame++;
			printf("Ya puedes guardar otro frame presionando la tecla L'\n");
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movVisor_x -= 1.0f;
			printf("\n movVisor_x es: %f\n", movVisor_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			ciclo2++;
			printf("\n Ya puedes modificar tu variable de nuevo \n");
		}
	}

	if (keys[GLFW_KEY_3])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movVisor_x += 1.0f;
			printf("\n movVisor_x es: %f\n", movVisor_x);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
	if (keys[GLFW_KEY_4])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movVisor_y -= 1.0f;
			printf("\n movVisor_y es: %f\n", movVisor_y);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}

	if (keys[GLFW_KEY_5])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movVisor_y += 1.0f;
			printf("\n movVisor_y es: %f\n", movVisor_y);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}

	if (keys[GLFW_KEY_6])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			giroVisor += 90.0f;
			printf("\n giroVisor es: %f\n", giroVisor);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}

	if (keys[GLFW_KEY_7])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			giroVisor -= 90.0f;
			printf("\n giroVisor es: %f\n", giroVisor);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
		}

	}
}