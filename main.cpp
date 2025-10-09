#include <iostream>
#include <vector>
#include <string>
#include "json.hpp"
#include <fstream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderClass.h"
#include "Camera.h"
#include "Application.h"
//#include "Nodes.h"
// #include "Streets.h"
#include "WorldMap.h"

using json = nlohmann::json;
const int WIDTH = 1920, HEIGHT = 1080;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "1M spheres", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	Shader defShader("./shaders/default.vert", "./shaders/default.frag", "default");
	defShader.Activate();

	std::ifstream input_file("./map_data/cbe2.json");
	if(!input_file.is_open()){
		std::cerr<<"Could not open the json file\n";
		return 1;
	}

	json map_data;
	try{
		input_file >> map_data;
	}catch(json::parse_error &e){
		std::cerr << "Parse error: " << e.what() << std::endl;
		return 1;
	}
	
	//Nodes *nodes = new Nodes(map_data);
	//Streets *streets = new Streets(map_data);
	WorldMap* worldMap = new WorldMap(map_data);
	
	//streets->printWayNodes();
	// std::cout << nodes->min_lon << " " << nodes->max_lon << " " << nodes->min_lat << " " << nodes->max_lat << "\n";
	Camera camera;

	Application app(window, &camera);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);

	while (!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		camera.Inputs(window);
		camera.updateMatrix();

		defShader.Activate();

		// GLint proj_loc = glGetUniformLocation(defShader.ID, "mvpMatrix");
		glUniformMatrix4fv(glGetUniformLocation(defShader.ID, "mvpMatrix"), 1, GL_FALSE,
			glm::value_ptr(camera.cameraMatrix));

		// nodes->Draw();
		//streets->DrawWays();
		//streets->Draw();
		worldMap->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			std::cout << "OpenGL Error: " << err << std::endl;
		}
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}