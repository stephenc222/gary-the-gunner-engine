#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "main.h"
#include <glm/glm.hpp>
using namespace glm;

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
  glClear(GL_COLOR_BUFFER_BIT);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, *vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );
  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
  glDisableVertexAttribArray(0);

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
  glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

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

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  // NOTE: continue on from here:
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };

  // This will identify our vertex buffer
  GLuint vertexbuffer;
  // Generate 1 buffer, put the resulting identifier in vertexbuffer
  glGenBuffers(1, &vertexbuffer);
  // The following commands will talk about our 'vertexbuffer' buffer
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  // Give our vertices to OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  Game* game = newGame();

  // set up basic player events, will eventually be scene-dependent
  glfwSetKeyCallback(window, handleInput);

  /* Loop until the user closes the window */

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

  cleanUp(window, game);
  return 0;
}

int main(void)
{
  initGame();
}