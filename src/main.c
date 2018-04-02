#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct t_Game {
  int numSceneRects;
  // GameMap* gameMap;
  // GameInput* gameInput;
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

int initGame(void);
void update(Game* game, float dt);
void render(GLFWwindow* window, Game* game);
void cleanUp(GLFWwindow* window, Game* game);

Game* newGame(void);
void destroyGame(Game* game);


void destroyGame(Game* game) {
  game = 0;
}

Game* newGame(void) {
  Game* game = malloc(sizeof *game);

  game->updateFunc = 0;
  game->renderFunc = 0;
  return game;
}

void render(GLFWwindow* window, Game* game) {
  // printf("rendering...");
   /* Render here */
  glClear(GL_COLOR_BUFFER_BIT);

  /* Swap front and back buffers */
  glfwSwapBuffers(window);
}

void update(Game* game, float dt) {
  // printf("dt: %f", dt);
}

void cleanUp(GLFWwindow* window, Game* game) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

int initGame(void) {
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
  Game* game = newGame();


  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  { 
    /* Poll for and process events */
    glfwPollEvents();

    float dt = 0;
    double lastTime = 0, currentTime;

    // handleInput(renderer,game, &loop);
    currentTime = glfwGetTime();
    if ((currentTime - lastTime) < 1000) {
      dt = (currentTime - lastTime);
      update(game, dt);
    }
    lastTime = currentTime;
    render( window, game);
  }

  cleanUp(window, game);
  // glfwTerminate();
  return 0;
}

int main(void)
{
  initGame();
}