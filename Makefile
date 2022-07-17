app:
	make pre-build
	make app_files
	g++ mini-python.cpp build/common/*.o -o mini-python

pre-build:
	mkdir -p build/common

mini_python_lib:
	make do_mini_python

LIB_SOURCES = Instruction.cpp LineLevelParser.cpp MiniPython.cpp Scope.cpp StandardFunctions.cpp Token.cpp TokenToVariable.cpp
LIB_OBJECTS = $(LIB_SOURCES:.cpp=.o)

LIB_VARIABLE_SOURCES = Bool.cpp Float.cpp Function.cpp Int.cpp List.cpp None.cpp String.cpp
LIB_VARIABLE_OBJECTS = $(LIB_VARIABLE_SOURCES:.cpp=.o)

app_files: $(LIB_OBJECTS) $(LIB_VARIABLE_OBJECTS)

$(LIB_OBJECTS): %.o: src/%.cpp
	g++ -c \
	-fPIC \
	$< \
	-I src \
	-I variable \
	-I . \
	-I export \
	-o build/common/$@

$(LIB_VARIABLE_OBJECTS): %.o: variable/%.cpp
	g++ -c \
	-fPIC \
	$< \
	-o build/common/$@