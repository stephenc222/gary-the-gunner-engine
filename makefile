CC := g++
CFLAGS ?= $(shell pkg-config glew glm --cflags) -O0
LDFLAGS ?= $(shell pkg-config glew glm --libs) -Wl,-headerpad_max_install_names -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

TARGET_EXEC ?= game
BUILD_DIR ?= ./bin
SRC_DIRS ?= ./src
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -g

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) ./src/lib/libglfw.3.3.dylib

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)
MKDIR_P ?= mkdir -p