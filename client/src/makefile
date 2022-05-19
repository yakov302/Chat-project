
SRC_DIR = ../src
INC_DIR = ../inc

CXX = gcc
CC = $(CXX)

WARNINGS = -ansi -pedantic -g 
CXXFLAGS = $(WARNINGS) 

CPPFLAGS = -I$(INC_DIR)

TARGET = runc
OBJS = ./client_net.o  ./client_app.o ./protocol.o ./ui.o ./client_main.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)  -L. -lgenDS

./client_app.o: $(INC_DIR)/client_app.h 
./client_net.o: $(INC_DIR)/client_net.h	
./client_app.o: $(INC_DIR)/client_net.h  $(INC_DIR)/client_app.h $(INC_DIR)/protocol.h $(INC_DIR)/ui.h	
./protocol.o: $(INC_DIR)/protocol.h
./ui.o: $(INC_DIR)/protocol.h	
./clientMain.o: $(INC_DIR)/protocol.h $(INC_DIR)/client_app.h $(INC_DIR)/ui.h


check: $(TARGET)
	./$(TARGET) 

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean

# Before running the executable file write in the terminal the commands below :
# 1. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
# 2. export LD_LIBRARY_PATH
