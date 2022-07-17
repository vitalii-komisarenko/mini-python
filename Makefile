mini_python_app: mini_python_lib
	g++ mini-python.cpp -L build -lMiniPython -o mini-python

mini_python_lib:
	mkdir -p build
	make do_mini_python_lib

LIB_SOURCES = Instruction.cpp LineLevelParser.cpp MiniPython.cpp Scope.cpp StandardFunctions.cpp Token.cpp TokenToVariable.cpp
LIB_OBJECTS = $(LIB_SOURCES:.cpp=.o)

LIB_VARIABLE_SOURCES = Bool.cpp Float.cpp Function.cpp Int.cpp List.cpp None.cpp String.cpp
LIB_VARIABLE_OBJECTS = $(LIB_VARIABLE_SOURCES:.cpp=.o)

do_mini_python_lib: $(LIB_OBJECTS) $(LIB_VARIABLE_OBJECTS)
	g++ -fPIC -shared build/*.o -o build/libMiniPython.so

$(LIB_OBJECTS): %.o: src/%.cpp
	g++ -c \
	-fPIC \
	$< \
	-I src \
	-I variable \
	-I . \
	-I export \
	-o build/$@

$(LIB_VARIABLE_OBJECTS): %.o: variable/%.cpp
	g++ -c \
	-fPIC \
	$< \
	-o build/$@