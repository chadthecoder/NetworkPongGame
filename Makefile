cc = g++

gameName = Pong

main = main.cpp

linksWin = -lglew32 -lglfw3 -lgdi32 -lopengl32 -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

linksLin = -lSDL2main -lSDL2 -lSDL2_image -pthread
fun = -lGLEW -lglfw -lGL

includeDepsWin = -I "libsWin\glew\include" -I "libsWin\glfw\include" -I "libsWin\SDL2\i686-w64-mingw32\include" -I "libsWin\SDL2_image-2.0.3\i686-w64-mingw32\include"

includeDepsLin = -I "libsLin\glew\include" -I "libsLin\glfw\include" -I "lib/SDL/include" -I "lib/SDL_image/include"  -Iasio-1.28.0/include 

linkingDepsWin = -L "libsWin\glew\lib\Release\Win32" -L "libsWin\glfw\lib-mingw" -L "libsWin\SDL2\i686-w64-mingw32\lib" -L "libsWin\SDL2_image-2.0.3\i686-w64-mingw32\lib"

linkingDepsLin = -L "libsLin\glew\lib\Release\Win32" -L "libsLin\glfw\lib-mingw" -L "lib/SDL/build/build" -L "lib/SDL_image/build" -L "usr/local/lib"

libsWin = $(includeDepsWin) $(linkingDepsWin) $(linksWin)

libsLin = $(includeDepsLin) $(linkingDepsLin) $(linksLin)

dotOWin = windows/main.o windows/$(gameName).o

dotOLin = linux/main.o linux/$(gameName).o

iconO = windows/windows.res

outputWin = -o "windows/$(gameName)"

outputLin = -o "linux/$(gameName)"

tempFlag = -fno-stack-protector

win: $(dotOWin)
	windres windows/windows.rc -O coff -o $(iconO)
	$(cc) $(outputWin) $(dotOWin) $(iconO) $(libsWin)

windows/main.o: main.cpp
	$(cc) -c main.cpp -o windows/main.o

windows/$(gameName).o: $(gameName).cpp
	$(cc) -c $(gameName).cpp -o windows/$(gameName).o

runWin:
	"windows/$(gameName)"

lin: $(dotOLin)
	$(cc) $(outputLin) $(dotOLin) $(libsLin)

linux/main.o: main.cpp
	$(cc) $(libsLin) -c main.cpp -o linux/main.o

linux/$(gameName).o: $(gameName).cpp
	$(cc) $(libsLin) -c $(gameName).cpp -o linux/$(gameName).o
	g++ $(libsLin) server.cpp -o linux/server

runLinServer:
	./linux/server

runLin:
	linux/$(gameName) "127.0.0.1"

cleanWin:
	rm windows/*.o
	rm windows/*.exe

cleanLin:
	rm linux/*.o
	rm linux/"$(gameName)"
	rm linux/server