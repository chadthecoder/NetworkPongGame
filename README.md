# graphicsProject

C++ Pong game using SDL2, woriking on implementing ASIO now, and eventually maybe OPENGL using GLEW and GLM. Following multiple tutorials to learn how to code graphics and games in C++.

Meant to be cross platform for Windows and Linux.

Multiplayer in dev branch.

# Clone

git clone --recurse-submodules ${ssh name}

# Build SDL2

cd lib/SDL
mkdir build
cd build
../configure
make
sudo make install

# Build SDL2 Image

cd lib/SDL_image
mkdir build
cd build
../configure
make
sudo make install

# Resources:

.gitignore started by https://github.com/deysanjeeb

README.md started by https://github.com/anubhav1206

pong icon by Icons8 (icons8.com)

Score images by Chad Mc Adams(chadthecoder on Github)

Game Programming in C++ by Sanjay Madhav  
 https://www.youtube.com/watch?v=W3gAzLwfIP0&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2  
 https://www.youtube.com/watch?v=QQzAHcojEKg&list=PLhfAbcv9cehhkG7ZQK0nfIGJC_C-wSLrx  
 https://stackoverflow.com/questions/708238/how-do-i-add-an-icon-to-a-mingw-gcc-compiled-executable

# install packages on ubunbtu

sudo apt-get install libglew-dev  
sudo apt-get install libglfw3  
sudo apt-get install libglfw3-dev

# Read Makefilw in order to install on Linux

make lin  
make runLinServer
make runLin (In new terminal window)  
make cleanLin (to clean when developing)

# Read Makefilw in order to install on Windows (Out of date, needs to be tested on Windows and updated)

make win  
make runWin  
make cleanWin
