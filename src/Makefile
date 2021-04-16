CXX = emcc
OUTPUT = ../web/imgui.js
IMGUI_DIR:=./lib

SOURCES = main.cpp
SOURCES += imgui_impl_glfw.cpp imgui_impl_opengl2.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_widgets.cpp

LIBS = -lGL
WEBGL_VER = -s USE_WEBGL2=1 -s USE_GLFW=3 -s FULL_ES3=1
#WEBGL_VER = USE_GLFW=2
USE_WASM = -s WASM=1

all: $(SOURCES) $(OUTPUT)

$(OUTPUT): $(SOURCES) 
	$(CXX)  $(SOURCES) -std=c++11 -o $(OUTPUT) $(LIBS) $(WEBGL_VER) -O2 --preload-file data $(USE_WASM) -I$(IMGUI_DIR)

clean:
	rm -f $(OUTPUT)