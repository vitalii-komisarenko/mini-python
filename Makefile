OUTPUT_FILE = mini-python
DEV_TOOLS_FILE = dev-tools

CPP_FLAGS = \
	-std=c++23 \
	-I . \
	-I .. \
	-I src \
	-I variable \
	-I export

all: app devtools

app:
	make pre-build
	make app_files
	g++ mini-python.cpp build/common/*.o build/modules/*.o -o "${OUTPUT_FILE}"

devtools: app_files
	g++ dev-tools.cpp build/common/*.o build/modules/*.o -I . -I variable -o "${DEV_TOOLS_FILE}"

clean:
	rm -rf build "${OUTPUT_FILE}" "${DEV_TOOLS_FILE}"

pre-build:
	mkdir -p build/common build/modules

mini_python_lib:
	make do_mini_python

LIB_SOURCES = \
	CallContext.cpp \
	FunctionParamatersParsing.cpp \
	Instruction.cpp \
	LineLevelParser.cpp \
	MiniPython.cpp \
	Parser.cpp \
	RaiseException.cpp \
	Scope.cpp \
	StandardFunctions.cpp \
	StringFormatting.cpp \
	Token.cpp \
	TokenToVariable.cpp \
	Utils.cpp \
	../modules/array.cpp \
	../modules/base64.cpp \
	../modules/binascii.cpp \
	../modules/gc.cpp \
	../modules/ipaddress.cpp \
	../modules/math.cpp \
	../modules/os.cpp \
	../modules/sys.cpp \
	../modules/time.cpp

LIB_OBJECTS = $(LIB_SOURCES:%.cpp=build/common/%.o)

LIB_VARIABLE_SOURCES = \
    Array.cpp \
    Bool.cpp \
    Bytes.cpp \
    Complex.cpp \
    Dict.cpp \
    File.cpp \
    Float.cpp \
    Function.cpp \
    GenericVariable.cpp \
    GenericVariableImpl.cpp \
    Int.cpp \
    Iterable.cpp \
    List.cpp \
    None.cpp \
    ObjectNotFound.cpp \
    Set.cpp \
    String.cpp

LIB_VARIABLE_OBJECTS = $(LIB_VARIABLE_SOURCES:%.cpp=build/common/%.o)

app_files: $(LIB_OBJECTS) $(LIB_VARIABLE_OBJECTS)

$(LIB_OBJECTS): build/common/%.o: src/%.cpp
	g++ -c \
	$< \
	${CPP_FLAGS} \
	-o $@

$(LIB_VARIABLE_OBJECTS): build/common/%.o: variable/%.cpp
	g++ -c \
	${CPP_FLAGS} \
	$< \
	-o $@
