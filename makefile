include ../MODULE_CONFIG

CXX_CFLAGS 		=
CXX_LDFLAGS 	=
LIBRARIES 		= $(CXX_SYSLIBS)

MODULE_NAME		= ChannelBot
OBJECTS			= $(MODULE_NAME).o Channel.o Auth.o
OUTPUT			= $(MODULE_NAME).so

default: $(OUTPUT)
Debug: all
Release: all
all: default


$(OUTPUT): $(SRC_DIR)$(MODULE_NAME).o $(SRC_DIR)Channel.o $(SRC_DIR)Auth.o
	$(CXX_LINK) -o $(LIB_DIR)$@ $(OBJECTS) $(LIBRARIES)

update:
	git pull

cleanDebug: clean
cleanRelease: clean
clean:
	rm -f $(OBJECTS) $(LIB_DIR)$(OUTPUT)

cleanall: clean
