CC = gcc
CPP_FLAGS = -Wall -g
UUID_LINKER_FLAG = -luuid
# Order is matter
SDL_FLAGS = -lSDL -lSDL_ttf `sdl-config --cflags --libs`

# $^ - means get all from, for example, after `server:` -  ie udp-server.c network.o
# $@ - means get `server`
# if `-c` has specified we get `.o` file, but if `-c` not specified we get exectuable file

all: client server

server: src/server.c \
	build/serv_network.o \
	build/utils.o \
	build/network.o | build
	$(CC) $(CPP_FLAGS) $^  $(SDL_FLAGS) ${UUID_LINKER_FLAG}  -o $@

client: src/client.c \
	build/cl_game_state.o \
	build/cl_game_object.o \
	build/cl_graph.o \
	build/cl_events.o \
	build/cl_fonts.o \
	build/cl_player.o \
	build/network.o \
	build/utils.o \
	build/cl_network.o | build
	$(CC) $(CPP_FLAGS) $^ $(SDL_FLAGS) $(UUID_LINKER_FLAG) -o $@

build/cl_game_state.o: src/cl_game_state.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/cl_game_object.o: src/cl_game_object.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/cl_player.o: src/cl_player.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/cl_events.o: src/cl_events.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/cl_graph.o: src/cl_graph.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/cl_fonts.o: src/cl_fonts.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/cl_network.o: src/cl_network.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@

build/serv_network.o: src/serv_network.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@ 

build/network.o: src/network.c | build
	$(CC) $(CPP_FLAGS) -c $^ $(SDL_FLAGS) ${UUID_LINKER_FLAG} -o $@ 

build/utils.o: src/utils.c | build
	$(CC) $(CPP_FLAGS) -c $^ ${UUID_LINKER_FLAG} -o $@

build:
	mkdir build


clean:
	@echo "Cleaning..."
	@rm -rf build ./client ./server
	@echo "OK"
