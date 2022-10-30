
SRC_DIR = .
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = gcc
CC = $(CXX)
WARNINGS = -pedantic #-ansi  -Wall -Werror
DEBUG = -g3
LIBRARIES = -pthread
CXXFLAGS = $(WARNINGS) $(DEBUG)

CFLAGS = -I$(INC_DIR) -I$(PROTOCOL_DIR)

TARGET = router 
OBJS = $(SRC_DIR)/main.o $(PROTOCOL_DIR)/protocol.o $(PROTOCOL_DIR)/args.o $(SRC_DIR)/socket.o $(SRC_DIR)/list.o $(SRC_DIR)/list_iterator.o
OBJS += $(SRC_DIR)/tcp.o $(SRC_DIR)/mutex.o $(SRC_DIR)/thread.o $(SRC_DIR)/router.o $(SRC_DIR)/action_in.o $(SRC_DIR)/user.o 
OBJS += $(SRC_DIR)/hash_map.o $(SRC_DIR)/users_manager.o $(SRC_DIR)/sha256.o $(SRC_DIR)/subscribs_manager.o $(SRC_DIR)/group.o
OBJS += $(SRC_DIR)/groups_manager.o $(SRC_DIR)/queue.o $(SRC_DIR)/action_out.o $(SRC_DIR)/server_manager.o

$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBRARIES)

main.o: main.c $(INC_DIR)/server_manager.h
	$(CC) $(CXXFLAGS) -c main.c

protocol.o: $(PROTOCOL_DIR)/protocol.c $(PROTOCOL_DIR)/protocol.h
	$(CC) $(CXXFLAGS) -c $(PROTOCOL_DIR)/protocol.c

args.o: $(PROTOCOL_DIR)/args.c $(PROTOCOL_DIR)/args.h
	$(CC) $(CXXFLAGS) -c $(PROTOCOL_DIR)/args.c

socket.o: socket.c $(INC_DIR)/socket.h
	$(CC) $(CXXFLAGS) -c socket.c

list.o: list.c $(INC_DIR)/list.h
	$(CC) $(CXXFLAGS) -c list.c

list_iterator.o: list_iterator.c $(INC_DIR)/list_iterator.h
	$(CC) $(CXXFLAGS) -c list_iterator.c

tcp.o: tcp.c $(INC_DIR)/tcp.h
	$(CC) $(CXXFLAGS) -c tcp.c

mutex.o: mutex.c $(INC_DIR)/mutex.h
	$(CC) $(CXXFLAGS) -c mutex.c

thread.o: thread.c $(INC_DIR)/thread.h
	$(CC) $(CXXFLAGS) -c thread.c

router.o: router.c $(INC_DIR)/router.h
	$(CC) $(CXXFLAGS) -c router.c

action_in.o: action_in.c $(INC_DIR)/action_in.h
	$(CC) $(CXXFLAGS) -c action_in.c

user.o: user.c $(INC_DIR)/user.h
	$(CC) $(CXXFLAGS) -c user.c

hash_map.o: hash_map.c $(INC_DIR)/hash_map.h
	$(CC) $(CXXFLAGS) -c hash_map.c

users_manager.o: users_manager.c $(INC_DIR)/users_manager.h
	$(CC) $(CXXFLAGS) -c users_manager.c

sha256.o: sha256.c $(INC_DIR)/sha256.h
	$(CC) $(CXXFLAGS) -c sha256.c

subscribs_manager.o: subscribs_manager.c $(INC_DIR)/subscribs_manager.h
	$(CC) $(CXXFLAGS) -c subscribs_manager.c

group.o: group.c $(INC_DIR)/group.h
	$(CC) $(CXXFLAGS) -c group.c

groups_manager.o: groups_manager.c $(INC_DIR)/groups_manager.h
	$(CC) $(CXXFLAGS) -c groups_manager.c

queue.o: queue.c $(INC_DIR)/queue.h
	$(CC) $(CXXFLAGS) -c queue.c

action_out.o: action_out.c $(INC_DIR)/action_out.h
	$(CC) $(CXXFLAGS) -c action_out.c

server_manager.o: server_manager.c $(INC_DIR)/server_manager.h
	$(CC) $(CXXFLAGS) -c server_manager.c

check: $(TARGET)
	./$(TARGET)  

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean