
SRC_DIR = ../src
INC_DIR = ../inc

CXX = gcc
CC = $(CXX)

WARNINGS = -ansi -pedantic -g 
CXXFLAGS = $(WARNINGS) 

CPPFLAGS = -I$(INC_DIR)

TARGET = runs
OBJS = ./usersManager.o  ./serverManager.o ./protocol.o ./server.o ./userStruct.o ./serverMain.o ./groupsManager.o ./groupStruct.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)  -L. -lgenDS

./usersManage.o: $(INC_DIR)/usersManager.h $(INC_DIR)/userStruct.h $(INC_DIR)/hash.map.h	
./serverManager.o: $(INC_DIR)/serverManager.h $(INC_DIR)/protocol.h $(INC_DIR)/server.h $(INC_DIR)/usersManager.h $(INC_DIR)/groupsManager.h	
./protocol.o: $(INC_DIR)/server.h $(INC_DIR)/protocol.h
./server.o: $(INC_DIR)/list.h $(INC_DIR)/gen.list.h $(INC_DIR)/server.h
./userStruct.o: $(INC_DIR)/userStruct.h $(INC_DIR)/protocol.h $(INC_DIR)/list.h
./serverMain.o: $(INC_DIR)/serverManager.h	
./groupsManager.o: $(INC_DIR)/groupsManager.h $(INC_DIR)/groupStruct.h $(INC_DIR)/queue.h $(INC_DIR)/hash.map.h $(INC_DIR)/list.h
./groupStruct.o: $(INC_DIR)/groupStruct.h $(INC_DIR)/protocol.h $(INC_DIR)/gen.list.h $(INC_DIR)/list.h $(INC_DIR)/hash.map.h $(INC_DIR)/queue.h

check: $(TARGET)
	./$(TARGET) 

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean

# Before running the executable file write in the terminal the commands below :
# 1. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
# 2. export LD_LIBRARY_PATH
