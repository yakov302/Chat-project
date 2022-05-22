
SRC_DIR = ../src
INC_DIR = ../inc

CXX = gcc
CC = $(CXX)

WARNINGS = -ansi -pedantic -g 
CXXFLAGS = $(WARNINGS) 

CPPFLAGS = -I$(INC_DIR)

TARGET = runs
OBJS = ./users_manager.o  ./server_manager.o ./protocol.o ./server_net.o ./user.o ./server_main.o ./groups_manager.o ./group.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)  -L. -lgenDS -lpthread 

./usersManage.o: $(INC_DIR)/users_manager.h $(INC_DIR)/user.h $(INC_DIR)/hash.map.h	
./serverManager.o: $(INC_DIR)/server_manager.h $(INC_DIR)/protocol.h $(INC_DIR)/server_net.h $(INC_DIR)/users_manager.h $(INC_DIR)/groups_manager.h	
./protocol.o: $(INC_DIR)/server_net.h $(INC_DIR)/protocol.h
./server.o: $(INC_DIR)/list.h $(INC_DIR)/gen.list.h $(INC_DIR)/server_net.h
./user.o: $(INC_DIR)/user.h $(INC_DIR)/protocol.h $(INC_DIR)/list.h
./serverMain.o: $(INC_DIR)/server_manager.h	
./groupsManager.o: $(INC_DIR)/groups_manager.h $(INC_DIR)/group.h $(INC_DIR)/queue.h $(INC_DIR)/hash.map.h $(INC_DIR)/list.h
./group.o: $(INC_DIR)/group.h $(INC_DIR)/protocol.h $(INC_DIR)/gen.list.h $(INC_DIR)/list.h $(INC_DIR)/hash.map.h $(INC_DIR)/queue.h

check: $(TARGET)
	./$(TARGET) 

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean

# Before running the executable file write in the terminal the commands below :
# 1. LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
# 2. export LD_LIBRARY_PATH
