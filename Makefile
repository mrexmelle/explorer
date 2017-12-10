#CROSS_COMPILE=ppc_6xx-

#compiler
CXX = g++
CC = gcc
LDXX = g++
LD = gcc
AR = ar

OUTPUT = posix-explorer
MAIN_C_SOURCES=$(shell find src -name "*.c")
MAIN_C_OBJECTS=$(MAIN_C_SOURCES:.c=.o)
MAIN_CPP_SOURCES=$(shell find src -name "*.cpp")
MAIN_CPP_OBJECTS=$(MAIN_CPP_SOURCES:.cpp=.o)

MAIN_APP_OBJECTS=${MAIN_C_OBJECTS} ${MAIN_CPP_OBJECTS}

INCLUDE= -I. -Isrc -Isrc/types -Isrc/utils

LINK=

DEFINES=

CPPFLAGS=

all : $(OUTPUT)

${OUTPUT}: ${MAIN_APP_OBJECTS}
	$(CROSS_COMPILE)$(CXX) ${MAIN_APP_OBJECTS} $(DEFINES) -o ${OUTPUT} $(LINK)

%.o : %.c
	$(CROSS_COMPILE)$(CC) $(CFLAGS) $(INCLUDE) $(DEFINES) -c $< -o $@

%.o : %.cpp
	$(CROSS_COMPILE)$(CXX) $(CPPFLAGS) $(INCLUDE) $(DEFINES) -c $< -o $@


clean:
	rm -f ${MAIN_APP_OBJECTS} $(OUTPUT)
	
