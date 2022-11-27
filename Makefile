OUTPUT_FILE = mini-python

app:
	make pre-build
	make app_files
	g++ mini-python.cpp build/common/*.o -o "${OUTPUT_FILE}"

clean:
	rm -rf build "${OUTPUT_FILE}"

pre-build:
	mkdir -p build/common

mini_python_lib:
	make do_mini_python

LIB_SOURCES = Instruction.cpp LineLevelParser.cpp MiniPython.cpp Scope.cpp StandardFunctions.cpp Token.cpp TokenToVariable.cpp
LIB_OBJECTS = $(LIB_SOURCES:%.cpp=build/common/%.o)

LIB_VARIABLE_SOURCES = Bool.cpp Float.cpp Function.cpp Int.cpp List.cpp None.cpp String.cpp
LIB_VARIABLE_OBJECTS = $(LIB_VARIABLE_SOURCES:%.cpp=build/common/%.o)

app_files: $(LIB_OBJECTS) $(LIB_VARIABLE_OBJECTS)

$(LIB_OBJECTS): build/common/%.o: src/%.cpp
	g++ -c \
	$< \
	-I src \
	-I variable \
	-I . \
	-I export \
	-o $@

$(LIB_VARIABLE_OBJECTS): build/common/%.o: variable/%.cpp
	g++ -c \
	$< \
	-o $@
