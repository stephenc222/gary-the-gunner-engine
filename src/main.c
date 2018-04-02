#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
  int isKeyDown;
  int isMouseDown;
  double mouseX;
  double mouseY;
  int whichKey;
} GameInput;

typedef struct t_Game {
  int numSceneRects;
  // GameMap* gameMap;
  GameInput* gameInput;
  // GameMenu* gameMenu;
  // Player* player;
  // Boss* firstBoss;
  // NPC* maleOne;
  // Inventory* inventory;
  // int fightDelay;
  // char* currentSceneName;
  void (*updateFunc)( struct t_Game* game, float dt);
  void (*renderFunc)( struct t_Game* game);
} Game;

// game is a global data structure
Game* game;

int initGame(void);
void update(Game* game, float dt);
void render(GLFWwindow* window, Game* game);
void cleanUp(GLFWwindow* window, Game* game);
void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
void destroyGameInput(GameInput* gameInput);
Game* newGame(void);
void destroyGame(Game* game);
GameInput* newGameInput();

void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

GameInput* newGameInput() {
  
  GameInput* gameInput = malloc(sizeof *gameInput);
  gameInput->isKeyDown = 0;
  gameInput->mouseX = 0;
  gameInput->mouseY = 0;
  gameInput->whichKey = 0;
  gameInput->isMouseDown = 0;

  return gameInput;
}

void destroyGameInput(GameInput* gameInput) {
  if (gameInput) {
    free(gameInput);
  }
}


void destroyGame(Game* game) {
  if (game->gameInput) {
    destroyGameInput(game->gameInput);
  }
  game = 0;
}

Game* newGame(void) {
  Game* game = malloc(sizeof *game);
  game->gameInput = newGameInput();
  game->updateFunc = 0;
  game->renderFunc = 0;
  return game;
}

void render(GLFWwindow* window, Game* game) {
   /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

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
  window = glfwCreateWindow(640, 480, "Gary the Gunner - Alpha", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glewExperimental=1; // Needed in core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);
  // NOTE: continue on from here:
  // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

  game = newGame();

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
    render( window, game);
  }

  cleanUp(window, game);
  return 0;
}

int main(void)
{
  initGame();
}