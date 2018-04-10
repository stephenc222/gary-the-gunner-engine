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


extern int initGame(void);
extern void update(Game* game, float dt);
extern void render(GLFWwindow* window, Game* game, GLuint* vertexbuffer);
extern void cleanUp(GLFWwindow* window, Game* game);
extern void handleInput(GLFWwindow* window, int key, int scancode, int action, int mods);
extern void destroyGameInput(GameInput* gameInput);
extern Game* newGame(void);
extern void destroyGame(Game* game);
extern GameInput* newGameInput();
extern GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);