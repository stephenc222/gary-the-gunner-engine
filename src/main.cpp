#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "main.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loaders.h"

using namespace glm;
using namespace std;
GLuint programID;
GLuint vertexbuffer;
GLuint colorbuffer;
GLuint uvbuffer;
GLuint MatrixID;
glm::mat4 mvp;

void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

GameInput* newGameInput() {
  
  GameInput* gameInput = new GameInput;
  gameInput->isKeyDown = 0;
  gameInput->mouseX = 0;
  gameInput->mouseY = 0;
  gameInput->whichKey = 0;
  gameInput->isMouseDown = 0;

  return gameInput;
}

void destroyGameInput(GameInput* gameInput) {
  if (gameInput) {
    delete gameInput;
    gameInput = 0;
  }
}


void destroyGame(Game* game) {
  if (game->gameInput) {
    destroyGameInput(game->gameInput);
  }
  delete game;
  game = 0;
}

Game* newGame(void) {
  Game* game = new Game;
  game->gameInput = newGameInput();
  game->updateFunc = 0;
  game->renderFunc = 0;
  return game;
}

void render(GLFWwindow* window, Game* game, GLuint* vertexbuffer) {
   /* Render here */
  // Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		// glfwSwapBuffers(window);
  /* Swap front and back buffers */
  glfwSwapBuffers(window);
}

void update(Game* game, float dt) {
  // printf("dt: %f", dt);
}

void cleanUp(GLFWwindow* window, Game* game) {
  destroyGame(game);
  glfwDestroyWindow(window);
  glfwTerminate();
}

int initGame(void) {
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

  GLFWwindow* window;
  /* Initialize the library */


  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Gary the Gunner - Alpha in C++", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glewExperimental=true; // Needed in core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  printf("MY VERSION: %s \n",glGetString(GL_VERSION));

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  // NOTE: continue on from here:
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
  // static const GLfloat g_vertex_buffer_data[] = {
  //   -1.0f, -1.0f, 0.0f,
  //   1.0f, -1.0f, 0.0f,
  //   0.0f,  1.0f, 0.0f,
  // };
  // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
  // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
  static const GLfloat g_vertex_buffer_data[] = {
      -1.0f,-1.0f,-1.0f, // triangle 1 : begin
      -1.0f,-1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f, // triangle 1 : end
      1.0f, 1.0f,-1.0f, // triangle 2 : begin
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f, // triangle 2 : end
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      -1.0f,-1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f,-1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f,-1.0f,
      1.0f,-1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f,-1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f,-1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f,-1.0f, 1.0f
  };

  // One color for each vertex. They were generated randomly.
  // static const GLfloat g_color_buffer_data[] = {
  //     0.583f,  0.771f,  0.014f,
  //     0.609f,  0.115f,  0.436f,
  //     0.327f,  0.483f,  0.844f,
  //     0.822f,  0.569f,  0.201f,
  //     0.435f,  0.602f,  0.223f,
  //     0.310f,  0.747f,  0.185f,
  //     0.597f,  0.770f,  0.761f,
  //     0.559f,  0.436f,  0.730f,
  //     0.359f,  0.583f,  0.152f,
  //     0.483f,  0.596f,  0.789f,
  //     0.559f,  0.861f,  0.639f,
  //     0.195f,  0.548f,  0.859f,
  //     0.014f,  0.184f,  0.576f,
  //     0.771f,  0.328f,  0.970f,
  //     0.406f,  0.615f,  0.116f,
  //     0.676f,  0.977f,  0.133f,
  //     0.971f,  0.572f,  0.833f,
  //     0.140f,  0.616f,  0.489f,
  //     0.997f,  0.513f,  0.064f,
  //     0.945f,  0.719f,  0.592f,
  //     0.543f,  0.021f,  0.978f,
  //     0.279f,  0.317f,  0.505f,
  //     0.167f,  0.620f,  0.077f,
  //     0.347f,  0.857f,  0.137f,
  //     0.055f,  0.953f,  0.042f,
  //     0.714f,  0.505f,  0.345f,
  //     0.783f,  0.290f,  0.734f,
  //     0.722f,  0.645f,  0.174f,
  //     0.302f,  0.455f,  0.848f,
  //     0.225f,  0.587f,  0.040f,
  //     0.517f,  0.713f,  0.338f,
  //     0.053f,  0.959f,  0.120f,
  //     0.393f,  0.621f,  0.362f,
  //     0.673f,  0.211f,  0.457f,
  //     0.820f,  0.883f,  0.371f,
  //     0.982f,  0.099f,  0.879f
  // };

  // Two UV coordinatesfor each vertex. They were created with Blender. You'll learn shortly how to do this yourself.
  // static const GLfloat g_uv_buffer_data[] = {
  //     0.000059f, 1.0f-0.000004f,
  //     0.000103f, 1.0f-0.336048f,
  //     0.335973f, 1.0f-0.335903f,
  //     1.000023f, 1.0f-0.000013f,
  //     0.667979f, 1.0f-0.335851f,
  //     0.999958f, 1.0f-0.336064f,
  //     0.667979f, 1.0f-0.335851f,
  //     0.336024f, 1.0f-0.671877f,
  //     0.667969f, 1.0f-0.671889f,
  //     1.000023f, 1.0f-0.000013f,
  //     0.668104f, 1.0f-0.000013f,
  //     0.667979f, 1.0f-0.335851f,
  //     0.000059f, 1.0f-0.000004f,
  //     0.335973f, 1.0f-0.335903f,
  //     0.336098f, 1.0f-0.000071f,
  //     0.667979f, 1.0f-0.335851f,
  //     0.335973f, 1.0f-0.335903f,
  //     0.336024f, 1.0f-0.671877f,
  //     1.000004f, 1.0f-0.671847f,
  //     0.999958f, 1.0f-0.336064f,
  //     0.667979f, 1.0f-0.335851f,
  //     0.668104f, 1.0f-0.000013f,
  //     0.335973f, 1.0f-0.335903f,
  //     0.667979f, 1.0f-0.335851f,
  //     0.335973f, 1.0f-0.335903f,
  //     0.668104f, 1.0f-0.000013f,
  //     0.336098f, 1.0f-0.000071f,
  //     0.000103f, 1.0f-0.336048f,
  //     0.000004f, 1.0f-0.671870f,
  //     0.336024f, 1.0f-0.671877f,
  //     0.000103f, 1.0f-0.336048f,
  //     0.336024f, 1.0f-0.671877f,
  //     0.335973f, 1.0f-0.335903f,
  //     0.667969f, 1.0f-0.671889f,
  //     1.000004f, 1.0f-0.671847f,
  //     0.667979f, 1.0f-0.335851f
  // };

  static const GLfloat g_uv_buffer_data[] = {
      1.000059f, 1.0f-0.000004f,
      1.000103f, 1.0f-0.336048f,
      1.335973f, 1.0f-0.335903f,
      1.000023f, 1.0f-0.000013f,
      1.667979f, 1.0f-0.335851f,
      1.999958f, 1.0f-0.336064f,
      1.667979f, 1.0f-0.335851f,
      1.336024f, 1.0f-0.671877f,
      1.667969f, 1.0f-0.671889f,
      1.000023f, 1.0f-0.000013f,
      1.668104f, 1.0f-0.000013f,
      1.667979f, 1.0f-0.335851f,
      1.000059f, 1.0f-0.000004f,
      0.335973f, 0.0f-0.335903f,
      0.336098f, 0.0f-0.000071f,
      0.667979f, 0.0f-0.335851f,
      0.335973f, 0.0f-0.335903f,
      0.336024f, 0.0f-0.671877f,
      1.000004f, 0.0f-0.671847f,
      0.999958f, 0.0f-0.336064f,
      0.667979f, 0.0f-0.335851f,
      0.668104f, 0.0f-0.000013f,
      0.335973f, 0.0f-0.335903f,
      0.667979f, 0.0f-0.335851f,
      0.335973f, 0.0f-0.335903f,
      0.668104f, 0.0f-0.000013f,
      0.336098f, 0.0f-0.000071f,
      0.000103f, 0.0f-0.336048f,
      0.000004f, 0.0f-0.671870f,
      0.336024f, 0.0f-0.671877f,
      0.000103f, 0.0f-0.336048f,
      0.336024f, 0.0f-0.671877f,
      0.335973f, 0.0f-0.335903f,
      0.667969f, 0.0f-0.671889f,
      1.000004f, 0.0f-0.671847f,
      0.667979f, 0.0f-0.335851f
  };

  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

  // This will identify our vertex buffer
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  // loading png to texture
  GLuint Texture = LoadPng("./assets/uvmaptexture_1024_1024.png", static_cast<GLsizei>(1));

  cout << "Texture: " << Texture << endl;


  Game* game = newGame();

  // set up basic player events, will eventually be scene-dependent
  glfwSetKeyCallback(window, handleInput);

  /* Loop until the user closes the window */
  programID = LoadShaders( "./assets/simple.vs", "./assets/simple.fs" );

  

  //glUseProgram(programID);

  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  float width = 4;
  float height = 4;
  glm::mat4 Projection = perspective(glm::radians(45.0f), (float) width / (float)height, 0.1f, 100.0f);
    
  // Or, for an ortho camera :
  //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
    
  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
      glm::vec3(0,0,0), // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
      );
    
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
  mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around

  // Get a handle for our "MVP" uniform
  // Only during the initialisation
  MatrixID = glGetUniformLocation(programID, "MVP");
    
  // Send our transformation to the currently bound shader, in the "MVP" uniform
  // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);


  // this is the main loop of the game application
  while (!glfwWindowShouldClose(window))
  { 
    /* Poll for and process events */
    glfwPollEvents();

    float dt = 0;
    double lastTime = 0, currentTime;

    currentTime = glfwGetTime();
    if ((currentTime - lastTime) < 1000) {
      dt = (currentTime - lastTime);
      update(game, dt);
    }
    lastTime = currentTime;
    render( window, game, &vertexbuffer);
  }
  glDeleteBuffers(0, &vertexbuffer);
  cleanUp(window, game);
  return 0;
}

int main(void)
{
  initGame();
}