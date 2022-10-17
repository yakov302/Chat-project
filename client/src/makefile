
SRC_DIR = .
INC_DIR = ../inc
PROTOCOL_DIR = ../../protocol

CXX = gcc
CC = $(CXX)
WARNINGS = -ansi -pedantic -Wall -Werror
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

check: $(TARGET)
	gcc -pedantic -g3 pop_up_message.c -o pop_up_message -pthread
	gcc -pedantic -g3 chat_window.c -o chat_window -pthread
	gcc -pedantic -g3 text_bar.c -o text_bar -pthread
	./$(TARGET) 

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	@$(RM) $(TARGET) $(OBJS) pop_up_message chat_window text_bar

.PHONY: check clean
