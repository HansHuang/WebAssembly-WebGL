# WebAssembly WebGL Demo

This is example for desktop C++ app compiles to WebAssembly & WebGL to get it running on web browser.

One time coding, multiple compilation.

### Build & Run for Desktop
```sh
cd src
# make sure OpenGL3, ES3, GLFW libraries are installed
make native 

./app_opengl3
```

### Build & Run for Web
```sh
cd src
# build by Emscripten
make web

cd ..
# run any static web server, or install below one
# npm i -g reed-mock-server
imock start mock-server.json 
# browser http://localhost:3000
```

