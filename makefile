SRC_DIR:=src
OBJ_DIR:=obj
INCLUDES:=-Iinc/ -Ilib/include/
LIBRARIES:=`pkg-config --static --libs glfw3` -lfreeimage -lnanogui
OUTDIR:=build
GAMEFILE:=game

CFLAGS:=$(INCLUDES) -O2 $(LIBRARIES) -Llib/x64/ -lgl3w 
LFLAGS:=$(INCLUDES) -O2 $(LIBRARIES) -Llib/x64/ -lgl3w 

CPPS:=$(shell find $(SRC_DIR) -name *.cpp)
OBJS:=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPPS))

build: objdir $(GAMEFILE)

$(GAMEFILE): $(OBJS)
	g++ -o $@ $(OBJS) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	if [ ! -d "$(dir $@)" ]; then \
		mkdir -p $(dir $@); \
	fi
	
	g++ -o $@ -c $? $(CFLAGS)

.PHONY: objdir clean test

clean:
	if [ -d "$(OUTDIR)" ]; then \
		rm -rf $(OUTDIR); \
	fi
	
	if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR); \
	fi

test:
	echo $(CPPS)
	echo $(OBJS)

objdir:
	if [ ! -d "$(OBJ_DIR)" ]; then \
		mkdir $(OBJ_DIR); \
	fi