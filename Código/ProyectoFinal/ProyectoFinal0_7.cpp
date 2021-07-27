/*---------------------------------------------------------*/
/* ---------------- Proyecto Final ------------------------*/
/*-------------- Computación Gráfica ----------------------*/
/*-------------------- 2021-2 -----------------------------*/
/*------------------- Alumnos: ----------------------------*/
/*--------------- Cadena Campos Luis ----------------------*/
/*---------- Campos Rodríguez Leonardo José ---------------*/
/*----------- Martínez Matías Joan Eduardo ----------------*/
/*- -------- Morales Rodríguez Alicia Nataly ------------- */

#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture
#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>
#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>
//agregando libreria para audio
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

// Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		giroR2D2 = 0.0f,
		movPataIzq = 0.0f,
		movPataDer = 0.0f,
		movPataCh = 0.0f,
		movCuerpo = 0.0f,
		movCabeza = 0.0f,
		giro2R2D2 = 0.0f,
		giro3R2D2 = 0.0f,
		movGrafico = 0.0f,
		incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroR2D2Inc = 0.0f,
		giro2R2D2Inc = 0.0f,
		giro3R2D2Inc = 0.0f,
		movPataIzqInc = 0.0f,
		movPataDerInc = 0.0f,
		movPataChInc = 0.0f,
		movCuerpoInc = 0.0f,
		movCabezaInc = 0.0f,
		movYoda_z = 0.0f,
		movYoda_x = 0.5f,
		orientaYoda = 0.0f;

// PodRacer
float	posPodX = 0.0f,
		posPodY = 0.0f,
		posPodZ = 0.0f,
		rotPodX = 0.0f,
		rotPodY = 0.0f,
		rotPodZ = 0.0f,
		giroTurbina = 0.0f;
float	posPodXInc = 0.0f,
		posPodYInc = 0.0f,
		posPodZInc = 0.0f,
		rotPodXInc = 0.0f,
		rotPodYInc = 0.0f,
		rotPodZInc = 0.0f,
		giroTurbinaInc = 0.0f;

bool	animacion = false,
		avanza = true;
int		movimientoYoda = 0;	
		
#define MAX_FRAMES 10
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float giroR2D2;
	float giro2R2D2;
	float giro3R2D2;
	float movPataIzq;
	float movPataDer;
	float movPataCh;
	float movCuerpo;
	float movCabeza;

	// PodRacer
	float posPodX;		//Variable para PosicionX
	float posPodY;		//Variable para PosicionY
	float posPodZ;		//Variable para PosicionZ
	float rotPodX;
	float rotPodY;
	float rotPodZ;
	float giroTurbina;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 10;		//numero de keyframes 
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;
	KeyFrame[FrameIndex].giroR2D2 = giroR2D2;
	KeyFrame[FrameIndex].giro2R2D2 = giro2R2D2;
	KeyFrame[FrameIndex].giro3R2D2 = giro3R2D2;
	KeyFrame[FrameIndex].movPataIzq = movPataIzq;
	KeyFrame[FrameIndex].movPataDer = movPataDer;
	KeyFrame[FrameIndex].movPataCh = movPataCh;
	KeyFrame[FrameIndex].movCuerpo = movCuerpo;
	KeyFrame[FrameIndex].movCabeza = movCabeza;

	// PodRacer
	KeyFrame[FrameIndex].posPodX = posPodX;
	KeyFrame[FrameIndex].posPodY = posPodY;
	KeyFrame[FrameIndex].posPodZ = posPodZ;
	KeyFrame[FrameIndex].rotPodX = rotPodX;
	KeyFrame[FrameIndex].rotPodY = rotPodY;
	KeyFrame[FrameIndex].rotPodZ = rotPodZ;
	KeyFrame[FrameIndex].giroTurbina = giroTurbina;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;
	//R2D2
	giroR2D2 = KeyFrame[0].giroR2D2;
	giro2R2D2 = KeyFrame[0].giro2R2D2;
	giro3R2D2 = KeyFrame[0].giro3R2D2;
	movPataIzq = KeyFrame[0].movPataIzq;
	movPataDer = KeyFrame[0].movPataDer;
	movPataCh = KeyFrame[0].movPataCh;
	movCuerpo = KeyFrame[0].movCuerpo;
	movCabeza = KeyFrame[0].movCabeza;

	// PodRacer
	posPodX = KeyFrame[0].posPodX;
	posPodY = KeyFrame[0].posPodY;
	posPodZ = KeyFrame[0].posPodZ;
	rotPodX = KeyFrame[0].rotPodX;
	rotPodY = KeyFrame[0].rotPodY;
	rotPodZ = KeyFrame[0].rotPodZ;
	giroTurbina = KeyFrame[0].giroTurbina;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	giroR2D2Inc = (KeyFrame[playIndex + 1].giroR2D2 - KeyFrame[playIndex].giroR2D2) / i_max_steps;
	giro2R2D2Inc = (KeyFrame[playIndex + 1].giro2R2D2 - KeyFrame[playIndex].giro2R2D2) / i_max_steps;
	giro3R2D2Inc = (KeyFrame[playIndex + 1].giro3R2D2 - KeyFrame[playIndex].giro3R2D2) / i_max_steps;
	movPataIzqInc = (KeyFrame[playIndex + 1].movPataIzq - KeyFrame[playIndex].movPataIzq) / i_max_steps;
	movPataDerInc = (KeyFrame[playIndex + 1].movPataDer - KeyFrame[playIndex].movPataDer) / i_max_steps;
	movPataChInc = (KeyFrame[playIndex + 1].movPataCh - KeyFrame[playIndex].movPataCh) / i_max_steps;
	movCuerpoInc = (KeyFrame[playIndex + 1].movCuerpo - KeyFrame[playIndex].movCuerpo) / i_max_steps;
	movCabezaInc = (KeyFrame[playIndex + 1].movCabeza - KeyFrame[playIndex].movCabeza) / i_max_steps;	

	// PodRacer
	posPodXInc = (KeyFrame[playIndex + 1].posPodX - KeyFrame[playIndex].posPodX) / i_max_steps;
	posPodYInc = (KeyFrame[playIndex + 1].posPodY - KeyFrame[playIndex].posPodY) / i_max_steps;
	posPodZInc = (KeyFrame[playIndex + 1].posPodZ - KeyFrame[playIndex].posPodZ) / i_max_steps;
	rotPodXInc = (KeyFrame[playIndex + 1].rotPodX - KeyFrame[playIndex].rotPodX) / i_max_steps;
	rotPodYInc = (KeyFrame[playIndex + 1].rotPodY - KeyFrame[playIndex].rotPodY) / i_max_steps;
	rotPodZInc = (KeyFrame[playIndex + 1].rotPodZ - KeyFrame[playIndex].rotPodZ) / i_max_steps;
	giroTurbinaInc = (KeyFrame[playIndex + 1].giroTurbina - KeyFrame[playIndex].giroTurbina) / i_max_steps;
}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			giroR2D2 += giroR2D2Inc;
			giro2R2D2 += giro2R2D2Inc;
			giro3R2D2 += giro3R2D2Inc;
			movPataIzq += movPataIzqInc;
			movPataDer += movPataDerInc;
			movPataCh += movPataChInc;
			movCuerpo += movCuerpoInc;
			movCabeza += movCabezaInc;

			// PodRacer
			posPodX += posPodXInc;
			posPodY += posPodYInc;
			posPodZ += posPodZInc;
			rotPodX += rotPodXInc;
			rotPodY += rotPodYInc;
			rotPodZ += rotPodZInc;
			giroTurbina += giroTurbinaInc;

			i_curr_steps++;
		}
	}

	//
	if (movCabeza <= 5.0f)
	{
		movCabeza += 10.0f;
	}
	else
	{
		movCabeza -= 0.22f;
	}
	
	movGrafico += 10.0f;


	if (animacion)
	{
		//Cambiando el signo cambia la direccion del movimiento 
		if (avanza)
		{
			movYoda_x += 3.0f;
			if (movYoda_x >= 800)
				avanza = false;
		}
		else
		{
			movYoda_z -= 3.0f;
			orientaYoda = 90.0f;
			if (movYoda_z <= -500.0f)
				avanza = true;
		}
		//Nada
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

int main()
{
	//MUSICAFONDO
	//PlaySound(TEXT("ambienteTatooine.wav"), NULL, SND_ASYNC);
	
	// glfw: initialize and configure
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Proyecto Final", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right4.jpg",
		"resources/skybox/back4.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom2.jpg",
		"resources/skybox/left4.jpg",
		"resources/skybox/front5.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	Model piso("resources/objects/piso2/piso2.obj");
	Model CasaT("resources/objects/CasaTatooine/CasaTatooine.obj");
	Model Pataizq("resources/objects/Pata izquierda (defrente)/pata izquierda color 2.obj");
	Model Patader("resources/objects/Pata derecha (defrente)R2D2/pata derecha color 2.obj");
	Model Patachica("resources/objects/Pata Chica R2D2/pata chica color 2.obj");
	Model Cuerpo("resources/objects/Cuerpo R2D2/Cuerpo Color 2.obj");
	Model Cabeza("resources/objects/Cabeza R2D2/Cabeza Color 3.obj");
	Model Base("resources/objects/Base/basehlgmcolor.obj");
	Model Grafico("resources/objects/Grafico/graficohlgmcolor.obj");
	Model CasaT2("resources/objects/Casa2Tatooine/casa2color.obj");
	Model CasaT3("resources/objects/Casa3Tatooine/casa3color.obj");
	Model CasaAnt("resources/objects/CasaAntenas/antenascolor.obj");
	Model Rampa("resources/objects/Base/rampanave.obj");
	Model Cantina("resources/objects/cantina/cantina3.obj");
	Model Casas("resources/objects/Tatoot/Tatoot.obj");
	Model Caja("resources/objects/CajaImperial/caja.obj");
	Model Caja2("resources/objects/CajaImperial/caja2.obj");
	Model Caja3("resources/objects/CajaImperial/caja3.obj");
	Model Habitante("resources/objects/Tatooiano/tatooiano.obj");
	Model Nave("resources/objects/Arc170/Arc170.obj");
	Model Accesorios("resources/objects/Accesorios/accesorios.obj");

	// PodRacer
	Model PodRacer("resources/objects/PodRacer/PodRacer.obj");
	Model Turbina1("resources/objects/PodRacer/Turbina1.obj");
	Model Turbina2("resources/objects/PodRacer/Turbina2.obj");
	Model Turbina3("resources/objects/PodRacer/Turbina3.obj");

	ModelAnim Cayendo("resources/objects/PilotAnim/pilotanim.dae");
	Cayendo.initShaders(animShader.ID);
	ModelAnim Muriendo("resources/objects/DroideAnim/droideanim.dae");
	Muriendo.initShaders(animShader.ID);
	ModelAnim Caminando("resources/objects/YodaAnim/yodaanim.dae");
	Caminando.initShaders(animShader.ID);
	ModelAnim Elevacion("resources/objects/RavenAnim/ravenanim.dae");
	Elevacion.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	KeyFrame[0].posX = 0.0f;
	KeyFrame[0].posY = 0.0f;
	KeyFrame[0].posZ = 0.0f;
	KeyFrame[0].giroR2D2 = 0.0f;
	KeyFrame[0].giro2R2D2 = 0.0f;
	KeyFrame[0].giro3R2D2 = 0.0f;
	KeyFrame[0].movPataIzq = 0.0f;
	KeyFrame[0].movPataDer = 0.0f;
	KeyFrame[0].movPataCh = 0.0f;
	KeyFrame[0].movCabeza = -40.0f;

	KeyFrame[1].posX = 0.0f;
	KeyFrame[1].posY = 0.0f;
	KeyFrame[1].posZ = 40.0f;
	KeyFrame[1].giroR2D2 = -25.0f;
	KeyFrame[1].giro2R2D2 = 0.0f;
	KeyFrame[1].giro3R2D2 = 0.0f;
	KeyFrame[1].movPataIzq = -3.5f;
	KeyFrame[1].movPataDer = -3.5f;
	KeyFrame[1].movPataCh = -2.0f;
	KeyFrame[1].movCabeza = 40.0f;

	KeyFrame[2].posX = 0.0f;
	KeyFrame[2].posY = 0.0f;
	KeyFrame[2].posZ = 40.0f;
	KeyFrame[2].giroR2D2 = 0.0f;
	KeyFrame[2].giro2R2D2 = 75.0f;
	KeyFrame[2].giro3R2D2 = 0.0f;
	KeyFrame[2].movPataIzq = -3.5f;
	KeyFrame[2].movPataDer = -3.5f;
	KeyFrame[2].movPataCh = -2.0f;
	KeyFrame[2].movCabeza = -40.0f;

	KeyFrame[3].posX = 0.0f;
	KeyFrame[3].posY = 0.0f;
	KeyFrame[3].posZ = 40.0f;
	KeyFrame[3].giroR2D2 = 0.0f;
	KeyFrame[3].giro2R2D2 = 75.0f;
	KeyFrame[3].giro3R2D2 = -15.0f;
	KeyFrame[3].movPataIzq = -4.5f;
	KeyFrame[3].movPataDer = -4.5f;
	KeyFrame[3].movPataCh = -2.0f;
	KeyFrame[3].movCabeza = 40.0f;

	KeyFrame[4].posX = 200.0f;
	KeyFrame[4].posY = 0.0f;
	KeyFrame[4].posZ = 40.0f;
	KeyFrame[4].giroR2D2 = 0.0f;
	KeyFrame[4].giro2R2D2 = 75.0f;
	KeyFrame[4].giro3R2D2 = -15.0f;
	KeyFrame[4].movPataIzq = -4.5f;
	KeyFrame[4].movPataDer = -4.5f;
	KeyFrame[4].movPataCh = -2.5f;
	KeyFrame[4].movCabeza = -40.0f;

	KeyFrame[5].posX = 200.0f;
	KeyFrame[5].posY = 0.0f;
	KeyFrame[5].posZ = 40.0f;
	KeyFrame[5].giroR2D2 = 0.0f;
	KeyFrame[5].giro2R2D2 = 170.0f;
	KeyFrame[5].giro3R2D2 = 0.0f;
	KeyFrame[5].movPataIzq = 4.5f;
	KeyFrame[5].movPataDer = 4.5f;
	KeyFrame[5].movPataCh = 2.5f;
	KeyFrame[5].movCabeza = 40.0f;

	KeyFrame[6].posX = 200.0f;
	KeyFrame[6].posY = 0.0f;
	KeyFrame[6].posZ = 0.0f;
	KeyFrame[6].giroR2D2 = 0.0f;
	KeyFrame[6].giro2R2D2 = 170.0f;
	KeyFrame[6].giro3R2D2 = 0.0f;
	KeyFrame[6].movPataIzq = 4.5f;
	KeyFrame[6].movPataDer = 4.5f;
	KeyFrame[6].movPataCh = 2.5f;
	KeyFrame[6].movCabeza = -40.0f;

	KeyFrame[7].posX = 200.0f;
	KeyFrame[7].posY = 0.0f;
	KeyFrame[7].posZ = 0.0f;
	KeyFrame[7].giroR2D2 = 0.0f;
	KeyFrame[7].giro2R2D2 = 245.0f;
	KeyFrame[7].giro3R2D2 = -15.0f;
	KeyFrame[7].movPataIzq = 4.5f;
	KeyFrame[7].movPataDer = 4.5f;
	KeyFrame[7].movPataCh = 2.5f;
	KeyFrame[7].movCabeza = 40.0f;

	KeyFrame[8].posX = 0.0f;
	KeyFrame[8].posY = 0.0f;
	KeyFrame[8].posZ = 0.0f;
	KeyFrame[8].giroR2D2 = 0.0f;
	KeyFrame[8].giro2R2D2 = 260.0f;
	KeyFrame[8].giro3R2D2 = -15.0f;
	KeyFrame[8].movPataIzq = 4.5f;
	KeyFrame[8].movPataDer = 4.5f;
	KeyFrame[8].movPataCh = 2.5f;
	KeyFrame[8].movCabeza = -40.0f;

	KeyFrame[9].posX = 0.0f;
	KeyFrame[9].posY = 0.0f;
	KeyFrame[9].posZ = 0.0f;
	KeyFrame[9].giroR2D2 = 0.0f;
	KeyFrame[9].giro2R2D2 = 360.0f;
	KeyFrame[9].giro3R2D2 = 0.0f;
	KeyFrame[9].movPataIzq = 0.5f;
	KeyFrame[9].movPataDer = 0.5f;
	KeyFrame[9].movPataCh = 0.0f;
	KeyFrame[9].movCabeza = 40.0f;

	// PodRacer
	KeyFrame[0].posPodX = 0.0f;
	KeyFrame[0].posPodY = 0.0f;
	KeyFrame[0].posPodZ = 0.0f;
	KeyFrame[0].rotPodX = -90.0f;
	KeyFrame[0].rotPodY = 0.0f;
	KeyFrame[0].rotPodZ = 0.0f;
	KeyFrame[0].giroTurbina = 1000.0f;

	KeyFrame[1].posPodX = -50.0f;
	KeyFrame[1].posPodY = 300.0f;
	KeyFrame[1].posPodZ = 0.0f;
	KeyFrame[1].rotPodX = -60.0f;
	KeyFrame[1].rotPodY = 0.0f;
	KeyFrame[1].rotPodZ = 0.0f;
	KeyFrame[1].giroTurbina = 2000.0f;

	KeyFrame[2].posPodX = -100.0f;
	KeyFrame[2].posPodY = 400.0f;
	KeyFrame[2].posPodZ = 350.0f;
	KeyFrame[2].rotPodX = -30.0f;
	KeyFrame[2].rotPodY = 0.0f;
	KeyFrame[2].rotPodZ = 0.0f;
	KeyFrame[2].giroTurbina = 3000.0f;

	KeyFrame[3].posPodX = -200.0f;
	KeyFrame[3].posPodY = 400.0f;
	KeyFrame[3].posPodZ = 700.0f;
	KeyFrame[3].rotPodX = 0.0f;
	KeyFrame[3].rotPodY = 0.0f;
	KeyFrame[3].rotPodZ = 720.0f;
	KeyFrame[3].giroTurbina = 4000.0f;

	KeyFrame[4].posPodX = -200.0f;
	KeyFrame[4].posPodY = 400.0f;
	KeyFrame[4].posPodZ = 1050.0f;
	KeyFrame[4].rotPodX = 0.0f;
	KeyFrame[4].rotPodY = 0.0f;
	KeyFrame[4].rotPodZ = 720.0f;
	KeyFrame[4].giroTurbina = 5000.0f;

	KeyFrame[5].posPodX = -500.0f;
	KeyFrame[5].posPodY = 200.0f;
	KeyFrame[5].posPodZ = 1400.0f;
	KeyFrame[5].rotPodX = 0.0f;
	KeyFrame[5].rotPodY = -40.0f;
	KeyFrame[5].rotPodZ = 720.0f;
	KeyFrame[5].giroTurbina = 4000.0f;

	KeyFrame[6].posPodX = -800.0f;
	KeyFrame[6].posPodY = 100.0f;
	KeyFrame[6].posPodZ = 1500.0f;
	KeyFrame[6].rotPodX = 0.0f;
	KeyFrame[6].rotPodY = -60.0f;
	KeyFrame[6].rotPodZ = 720.0f;
	KeyFrame[6].giroTurbina = 3000.0f;

	KeyFrame[7].posPodX = -1100.0f;
	KeyFrame[7].posPodY = 50.0f;
	KeyFrame[7].posPodZ = 1550.0f;
	KeyFrame[7].rotPodX = 0.0f;
	KeyFrame[7].rotPodY = -80.0f;
	KeyFrame[7].rotPodZ = 720.0f;
	KeyFrame[7].giroTurbina = 2000.0f;

	KeyFrame[8].posPodX = -1300.0f;
	KeyFrame[8].posPodY = -10.0f;
	KeyFrame[8].posPodZ = 1600.0f;
	KeyFrame[8].rotPodX = 0.0f;
	KeyFrame[8].rotPodY = -75.0f;
	KeyFrame[8].rotPodZ = 720.0f;
	KeyFrame[8].giroTurbina = 1000.0f;

	KeyFrame[9].posPodX = -1350.0f;
	KeyFrame[9].posPodY = -10.0f;
	KeyFrame[9].posPodZ = 1600.0f;
	KeyFrame[9].rotPodX = 0.0f;
	KeyFrame[9].rotPodY = -80.0f;
	KeyFrame[9].rotPodZ = 720.0f;
	KeyFrame[9].giroTurbina = 500.0f;

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		lastFrame = SDL_GetTicks();

		// input
		//my_input(window);
		animate();
		
		// render
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		// Fuente de Luz Direccional - Sol
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		// Componente Ambiental: Color hacia el cual tienden las sombras
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		// Componente Difusa: Color Principal de los Objetos
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		// Componente Especular: Brillo de los Objetos
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		// Fuentes de Luz Posicionales
		staticShader.setVec3("pointLight[0].position", lightPosition);
		// Componente Ambiental: Color hacia el cual tienden las sombras
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		// Componente Difusa: Color Principal de los Objetos
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		// Componente Especular: Brillo de los Objetos
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		// Control de la Intensidad de las Fuentes de Luz
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		// Componente Ambiental: Color hacia el cual tienden las sombras
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		// Componente Difusa: Color Principal de los Objetos
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		// Componente Especular: Brillo de los Objetos
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		// Control de la Intensidad de las Fuentes de Luz
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		//personaje animado caminando
		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -32.0f, -850.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.45f));
		animShader.setMat4("model", model);
		Elevacion.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 40.0f, -700.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.40f));
		animShader.setMat4("model", model);
		Cayendo.Draw(animShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, -8.0f, 700.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(35.0f));
		animShader.setMat4("model", model);
		Muriendo.Draw(animShader);
		
		//model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -22.0f, 800.0f));
		model = glm::translate(model, glm::vec3(movYoda_x, -22.0f, movYoda_z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(orientaYoda), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		animShader.setMat4("model", model);
		Caminando.Draw(animShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 19.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//ESQUINA INFERIOR DERECHA (DEFRENTE)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(930.0f, -23.0f, 820.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.7f));
		staticShader.setMat4("model", model);
		CasaT.Draw(staticShader);

		//ESQUINA SUPERIOR IZQUIERDA (DEFRENTE)
		model = glm::translate(glm::mat4(1.0f), glm::vec3(940.0f, -23.0f, -940.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.7f));
		staticShader.setMat4("model", model);
		CasaT.Draw(staticShader);

		//DEFRENTE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(30.5f, -22.0f, -925.0f));
		model = glm::rotate(model, glm::radians(-126.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.7f));
		staticShader.setMat4("model", model);
		CasaT2.Draw(staticShader);

		//DERECHA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(890.0f, -31.5f, -80.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.7f));
		staticShader.setMat4("model", model);
		CasaT3.Draw(staticShader);

		//ANTENAS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1000.0f, -20.0f, -160.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.7f));
		staticShader.setMat4("model", model);
		CasaAnt.Draw(staticShader);

		//IZQUIERDA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-650.0f, -22.0f, -670.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.022f));
		staticShader.setMat4("model", model);
		Rampa.Draw(staticShader);

		//CANTINA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, -22.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-179.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.5f));
		staticShader.setMat4("model", model);
		Cantina.Draw(staticShader);

		//ACCESORIOS
		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, -22.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		Accesorios.Draw(staticShader);

		//CASA C/NAVE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, 15.0f, 750.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		Casas.Draw(staticShader);

		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -20.0f, 250.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja.Draw(staticShader);

		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -20.0f, -250.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja.Draw(staticShader);

		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, -20.0f, 500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja2.Draw(staticShader);

		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, -20.0f, -500.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Caja3.Draw(staticShader);

		//CAJA
		model = glm::translate(glm::mat4(1.0f), glm::vec3(500.0f, -20.0f, 800.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		Habitante.Draw(staticShader);

		//NAVE
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, -20.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		Nave.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// PodRacer
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(850.0f + posPodX, 20.0f + posPodY, -800.0f + posPodZ));
		tmp = model = glm::rotate(model, glm::radians(rotPodX), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotPodY), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(rotPodZ), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		PodRacer.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(71.0f, -16.0f, 112.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina1.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-71.0f, -16.0f, 112.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina1.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(71.0f, -16.0f, 51.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina2.Draw(staticShader);	//Izq central

		model = glm::translate(tmp, glm::vec3(-71.0f, -16.0f, 51.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina2.Draw(staticShader);	//Der central

		model = glm::translate(tmp, glm::vec3(17.1f, 16.0f, -147.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina3.Draw(staticShader);	//Izq trasera

		model = glm::translate(tmp, glm::vec3(-17.1f, 16.0f, -147.0f));
		model = glm::rotate(model, glm::radians(giroTurbina), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setMat4("model", model);
		Turbina3.Draw(staticShader);	//Der trasera
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Holograma
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -23.0f, -320.0f));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Base.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -16.0f, -320.0f));
		model = glm::rotate(model, glm::radians(movGrafico), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Grafico.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -23.0f, -1020.0f));
		tmp = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Base.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-660.0f, -16.0f, -1020.0f));
		model = glm::rotate(model, glm::radians(-movGrafico), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.30f));
		staticShader.setMat4("model", model);
		Grafico.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		//Cuerpo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f, -24.0f, 600.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroR2D2), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giro2R2D2), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(giro3R2D2), glm::vec3(1.5f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f));
		staticShader.setMat4("model", model);
		Cuerpo.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(-8.3f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPataIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f));
		staticShader.setMat4("model", model);
		Pataizq.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(10.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(movPataDer), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f));
		staticShader.setMat4("model", model);
		Patader.Draw(staticShader);

		//Pata Chica
		model = glm::translate(tmp, glm::vec3(0.0f, -0.8f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, movPataCh, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f));
		staticShader.setMat4("model", model);
		Patachica.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.25f, -10.4f, -0.65f));
		model = glm::rotate(model, glm::radians(movCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.2f));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		staticShader.setMat4("model", model);
		Cabeza.Draw(staticShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje GONK 2
		// -------------------------------------------------------------------------------------------------------------------------
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		animacion = true;
		movimientoYoda = 1;
		movYoda_x = 0.0f;
	}
	
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}