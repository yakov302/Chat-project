
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
OBJS += $(SRC_DIR)/user.o $(SRC_DIR)/group.o $(SRC_DIR)/list_iterator.o  $(SRC_DIR)/app.o  $(SRC_DIR)/io.o 

$(TARGET):$(OBJS)
	$(CC) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBRARIES)

check: $(TARGET)
	./$(TARGET)  

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean
