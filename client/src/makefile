
SRC_DIR = .
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = gcc
CC = $(CXX)
WARNINGS = -pedantic #-ansi -Wall -Werror
DEBUG = -g3
LIBRARIES = -pthread
CXXFLAGS = $(WARNINGS) $(DEBUG)

CFLAGS = -I$(INC_DIR) -I$(PROTOCOL_DIR)

TARGET = main 
OBJS = $(SRC_DIR)/main.o $(PROTOCOL_DIR)/protocol.o $(PROTOCOL_DIR)/args.o $(SRC_DIR)/socket.o $(SRC_DIR)/action_out.o 
OBJS += $(SRC_DIR)/tcp.o $(SRC_DIR)/mutex.o $(SRC_DIR)/thread.o $(SRC_DIR)/router.o $(SRC_DIR)/action_in.o $(SRC_DIR)/list.o
OBJS += $(SRC_DIR)/user.o $(SRC_DIR)/group.o $(SRC_DIR)/list_iterator.o  $(SRC_DIR)/app.o $(SRC_DIR)/io.o  $(SRC_DIR)/client_manager.o

$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBRARIES)

main.o: main.c $(INC_DIR)/client_manager.h
	$(CC) $(CXXFLAGS) -c main.c

protocol.o: $(PROTOCOL_DIR)/protocol.c $(PROTOCOL_DIR)/protocol.h 
	$(CC) $(CXXFLAGS) -c $(PROTOCOL_DIR)/protocol.c

args.o: $(PROTOCOL_DIR)/args.c $(INC_DIR)/args.h 
	$(CC) $(CXXFLAGS) -c $(PROTOCOL_DIR)/args.c

socket.o: socket.c $(INC_DIR)/socket.h 
	$(CC) $(CXXFLAGS) -c socket.c

action_out.o: action_out.c $(INC_DIR)/action_out.h 
	$(CC) $(CXXFLAGS) -c action_out.c

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

list.o: list.c $(INC_DIR)/list.h 
	$(CC) $(CXXFLAGS) -c list.c

user.o: user.c $(INC_DIR)/user.h 
	$(CC) $(CXXFLAGS) -c user.c

group.o: group.c $(INC_DIR)/group.h 
	$(CC) $(CXXFLAGS) -c group.c

list_iterator.o: list_iterator.c $(INC_DIR)/list_iterator.h 
	$(CC) $(CXXFLAGS) -c list_iterator.c

app.o: app.c $(INC_DIR)/app.h 
	$(CC) $(CXXFLAGS) -c app.c

io.o: io.c $(INC_DIR)/io.h 
	$(CC) $(CXXFLAGS) -c io.c

client_manager.o: client_manager.c $(INC_DIR)/client_manager.h 
	$(CC) $(CXXFLAGS) -c client_manager.c

check: $(TARGET)
	gcc -pedantic -g3 pop_up_message.c -o pop_up_message -pthread
	gcc -pedantic -g3 chat_window.c -o chat_window -pthread
	gcc -pedantic -g3 text_bar.c -o text_bar -pthread
	./$(TARGET) 

debug: $(TARGET)
	gcc -pedantic -g3 pop_up_message.c -o pop_up_message -pthread
	gcc -pedantic -g3 chat_window.c -o chat_window -pthread
	gcc -pedantic -g3 text_bar.c -o text_bar -pthread
	gdb ./$(TARGET)

clean:
	@$(RM) $(TARGET) $(OBJS) pop_up_message chat_window text_bar

.PHONY: check clean
