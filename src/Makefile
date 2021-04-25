#CXX = g++
#CXX = clang++

EXE = app_opengl3
LIB_DIR = libs
SOURCES = main.cpp
# SOURCES += blotter.cpp
SOURCES += $(LIB_DIR)/imgui.cpp $(LIB_DIR)/imgui_demo.cpp $(LIB_DIR)/imgui_draw.cpp $(LIB_DIR)/imgui_tables.cpp $(LIB_DIR)/imgui_widgets.cpp
SOURCES += $(LIB_DIR)/backends/imgui_impl_glfw.cpp $(LIB_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I$(LIB_DIR) -I$(LIB_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## Emscripten WebGL + WebAssembly
##---------------------------------------------------------------------

WEB_OUTPUT = ../web/app.js
WEBGL_VER = -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1
#WEBGL_VER = USE_GLFW=2
USE_WASM = -s WASM=1

CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(LIB_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(LIB_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

native: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

web:
	em++ $(SOURCES) -s ASSERTIONS=1 -std=c++14 -o $(WEB_OUTPUT) -lGL $(LIBS) $(WEBGL_VER) -Os --preload-file ../misc $(USE_WASM) $(CXXFLAGS)
	@echo Build complete for Web

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
