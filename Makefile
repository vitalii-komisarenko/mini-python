OUTPUT_FILE = mini-python
DEV_TOOLS_FILE = dev-tools

all: app devtools

app:
	make pre-build
	make app_files
	g++ mini-python.cpp build/common/*.o -o "${OUTPUT_FILE}"

devtools: app_files
	g++ dev-tools.cpp build/common/*.o -I . -I variable -o "${DEV_TOOLS_FILE}"

clean:
	rm -rf build "${OUTPUT_FILE}" "${DEV_TOOLS_FILE}"

pre-build:
	mkdir -p build/common build/modules

mini_python_lib:
	make do_mini_python

LIB_SOURCES = Instruction.cpp LineLevelParser.cpp MiniPython.cpp Scope.cpp StandardFunctions.cpp Token.cpp TokenToVariable.cpp StringFormatting.cpp ../modules/os.cpp ../modules/sys.cpp ../modules/time.cpp ../modules/math.cpp
LIB_OBJECTS = $(LIB_SOURCES:%.cpp=build/common/%.o)

LIB_VARIABLE_SOURCES = Bool.cpp Float.cpp Function.cpp Int.cpp List.cpp None.cpp String.cpp GenericVariable.cpp File.cpp Set.cpp Dict.cpp Complex.cpp Iterable.cpp ObjectNotFound.cpp
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
	-I . \
	-I .. \
	-o $@
