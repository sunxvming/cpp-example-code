TARGET = main
OBJ_PATH = objs


PROTOBUF_DIR = ../../ebpc/protobuf
PROTOBUF_TARGET = /src/.libs/libprotobuf.a
PROTOBUF_LIB = $(PROTOBUF_DIR)/$(PROTOBUF_TARGET)

CC = gcc
CPP = g++ -std=c++11
INCLUDES += -I ../include -I$(PROTOBUF_DIR)/src
LIBS = /usr/lib64/libpthread.so  /usr/lib64/librt.so $(PROTOBUF_LIB)
GFLAGS = -g
CFLAGS :=-Wall -Werror $(GFLAGS)
LINKFLAGS = -ldl

SRCDIR =. ./include ./ca 


C_SRCDIR = $(SRCDIR)
C_SOURCES = $(foreach d,$(C_SRCDIR),$(wildcard $(d)/*.c) )
C_OBJS = $(patsubst %.c, $(OBJ_PATH)/%.o, $(C_SOURCES))

CC_SRCDIR = $(SRCDIR)
CC_SOURCES = $(foreach d,$(CC_SRCDIR),$(wildcard $(d)/*.cc) )
CC_OBJS = $(patsubst %.cc, $(OBJ_PATH)/%.o, $(CC_SOURCES))

#CPP_SOURCES = $(wildcard *.cpp)
CPP_SRCDIR = $(SRCDIR)
CPP_SOURCES = $(foreach d,$(CPP_SRCDIR),$(wildcard $(d)/*.cpp) )
CPP_OBJS = $(patsubst %.cpp, $(OBJ_PATH)/%.o, $(CPP_SOURCES))

default:init compile

$(C_OBJS):$(OBJ_PATH)/%.o:%.c
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CC_OBJS):$(OBJ_PATH)/%.o:%.cc
	$(CPP) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(CPP_OBJS):$(OBJ_PATH)/%.o:%.cpp
	$(CPP) -c $(CFLAGS) $(INCLUDES) $< -o $@

init:
	$(foreach d,$(SRCDIR), mkdir -p $(OBJ_PATH)/$(d);)

test:
	@echo "C_SOURCES: $(C_SOURCES)"
	@echo "CC_SOURCES: $(CC_SOURCES)"
	@echo "CPP_SOURCES: $(CPP_SOURCES)"
	@echo "C_OBJS: $(C_OBJS)"
	@echo "CC_OBJS: $(CC_OBJS)"
	@echo "CPP_OBJS: $(CPP_OBJS)"


compile:$(C_OBJS) $(CC_OBJS) $(CPP_OBJS)
	$(CPP)  $^ -o $(TARGET)  $(LINKFLAGS) $(LIBS)

clean:
	rm -rf $(OBJ_PATH)
	rm -rf $(TARGET)

