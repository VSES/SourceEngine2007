#
# Base makefile for Source under x86 Linux
#
#
# compiler options

CC=gcc
CPLUS=g++
CLINK=gcc
CPP_LIB=

# -fpermissive is so gcc 3.4.x doesn't bitch about some template stuff
ARCH_FLAGS=-mtune=i686 -march=pentium -mmmx -msse -pipe

ifeq ($(MAKE_DEBUG),1)
 DEBUG_LINK_FLAGS=-g -ggdb
 OPTIMIZER_FLAGS=-g -ggdb -ffast-math -fforce-addr -malign-loops=4 -malign-jumps=4 -malign-functions=4
else
 OPTIMIZER_FLAGS=$(OLEVEL) -fomit-frame-pointer -ffast-math -fforce-addr -funroll-loops -malign-loops=4 -malign-jumps=4 -malign-functions=4 -ffast-math -finline-limit=1600
endif

BASE_CFLAGS_CS=-DSWDS -D_LINUX -DLINUX -DNDEBUG -fpermissive -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp
#BASE_CFLAGS_CS=-DALLOW_DEBUG_DEDICATED_SERVER_OUTSIDE_STEAM -DNO_STEAM -DSWDS -D_LINUX -DLINUX -DNDEBUG -fpermissive -Dstricmp=strcasecmp -D_stricmp=strcasecmp -D_strnicmp=strncasecmp -Dstrnicmp=strncasecmp -D_snprintf=snprintf -D_vsnprintf=vsnprintf -D_alloca=alloca -Dstrcmpi=strcasecmp
SHLIBEXT=so
SHLIBCFLAGS=-fPIC
SHLIBLDFLAGS=-Wl,-Map,$@_map.txt -Wl

#CFLAGS_CS=-g -ggdb
LDFLAGS= $(CFLAGS_CS)
LINKLIBS=-lm -ldl -lpthread
BASE_LIBS=/usr/local/lib/libstdc++.a /usr/local/lib/libgcc_eh.a
CFLAGS_CS=$(ARCH_FLAGS) $(OPTIMIZER_FLAGS)
CFLAGS_CS+=$(DEFINES) $(INCLUDES) $(INCLUDEDIRS) $(BASE_CFLAGS_CS) -Usprintf=use_Q_snprintf_instead_of_sprintf -Ustrncpy=use_Q_strncpy_instead -UPROTECTED_THINGS_ENABLE

#############################################################################
# The compiler command lne for each src code file to compile
#############################################################################

OBJ_DIR=$(PROJDIR)/obj

OBJS=$(addprefix $(OBJ_DIR)/, $(notdir $(CPPFILES:.cpp=.o) $(CXXFILES:.cxx=.oxx) $(CFILES:.c=.o)))

OLIB=$(SRCROOT)/lib/linux/$(NAME)_486.a
SHLIB=$(SRCROOT)/lib/linux/$(NAME)_486.a
TARGET_OBJ=
ifeq ($(CONFTYPE),lib)
  TARGET=$(OLIB)
  TARGET_OBJ=$(OLIB)
endif
ifeq ($(CONFTYPE),dll)
  TARGET=$(OUTPUT_SO_FILE)
endif
ifeq ($(CONFTYPE),exe)
  TARGET=$(OUTPUT_EXECUTABLE)
endif

CFLAGS_CS+=-fPIC

DO_CC=	\
	@if [ "$(MAKE_QUIET)" == "" ]; then \
		echo "----";\
		echo "---- $@ ----";\
		echo "----";\
		echo $(CPLUS) -w $(CFLAGS_CS) -o $@ -c $<;\
		echo "";\
	fi;\
	if [ "$(MAKE_QUIET)" != "" ]; then\
		echo "---- $@ ----";\
	fi;\
	$(CPLUS) -w $(CFLAGS_CS) -o $@ -c $<;

all: start_status dirs $(OBJS) $(TARGET)
	@echo $(TARGET)

# Verify the options.
start_status:
	@if [ "$(MAKE_DEBUG)" == "1" ]; then\
		echo ""; \
		echo ""; \
		echo "NOTE: Relevant environment variables detected.";\
		if [ "$(MAKE_DEBUG)" == "1" ]; then\
			echo "    MAKE_DEBUG was found so optimizations will be reduced.";\
		fi;\
	fi

dirs:
	@-mkdir -p $(OBJ_DIR)

objs: dirs $(OBJS) $(TARGET_OBJ)


%.o: %.cpp
	$(DO_CC)

%.oxx: $.cxx
	$(DO_CC)

$(OLIB) : $(OBJS)
	-p4 edit $(OLIB)
	ar rs $(OLIB) $(OBJS);


clean:
	rm -f $(OBJS) $(TARGET)


$(OUTPUT_SO_FILE) : $(OBJS) $(TARGET_OBJ)
	@if [ "$(MAKE_QUIET)" == "" ]; then \
		echo "----";\
		echo "---- LINKING ----";\
		echo "----";\
		echo $(CLINK) -shared $(DEBUG_LINK_FLAGS) $(SHLIBLDFLAGS) -o $(OUTPUT_SO_FILE) $(OBJS) $(LIBFILES) $(LIBFILES) $(BASE_LIBS);\
		echo "";\
	fi;\
	if [ "$(MAKE_QUIET)" != "" ]; then\
		echo "---- LINKING $@ ----";\
	fi;\
	#NOTE: YES we want to include the lib files 2 times. We've run into problems with the 1-pass linker not bringing in symbols it should.
	cd $(SRCROOT)/linux; $(CLINK) -shared $(DEBUG_LINK_FLAGS) $(SHLIBLDFLAGS) -o $(OUTPUT_SO_FILE) $(LINKLIBS) $(OBJS) $(LIBFILES) $(LIBFILES) $(BASE_LIBS);

$(OUTPUT_EXECUTABLE) : $(OBJS) $(TARGET_OBJ)
	@if [ "$(MAKE_QUIET)" == "" ]; then \
		echo "----";\
		echo "---- LINKING EXE ----";\
		echo "----";\
		echo $(CLINK) $(DEBUG_LINK_FLAGS) $(SHLIBLDFLAGS) $(LINKLIBS) -o $(OUTPUT_EXECUTABLE) $(OBJS) $(LIBFILES) $(LIBFILES) $(BASE_LIBS);\
		echo "";\
	fi;\
	if [ "$(MAKE_QUIET)" != "" ]; then\
		echo "---- LINKING EXE $@ ----";\
	fi;\
	#NOTE: YES we want to include the lib files two times. We've run into problems with the 1-pass linker not bringing in symbols it should.
	$(CLINK) $(DEBUG_LINK_FLAGS) $(SHLIBLDFLAGS) $(LINKLIBS) -o $(OUTPUT_EXECUTABLE) $(OBJS) $(LIBFILES) $(LIBFILES) $(BASE_LIBS);

tags:
	etags -a -C -o $(SRCROOT)/TAGS *.cpp *.cxx *.h
