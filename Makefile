default:
	mkdir bin
	g++ -g src/*.cpp src/glad.c -I include -I /opt/assimp/include -I /opt/assimp/build/include -lglfw -lassimp -o bin/space

clean:
	rm -rf bin
