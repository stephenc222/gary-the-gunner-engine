
gamemake: ./src/main.c
	clang ./src/main.c -MMD -MP -g -L/usr/local/lib -lglfw -lGLEW -framework OpenGL -o ./bin/game