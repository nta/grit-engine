# ============================
# Makefile for building luaimg
# ============================

# Disable  built-in rules, in particular they interfere with C++ headers that have no file
# extension.
.SUFFIXES:


# -----------------------
# User-overrideable parts
# -----------------------

CXX?= g++
CC?= gcc
OPT?=-O3 -DNDEBUG
ARCH?= -march=native -mtune=native


# -----------------
# Compilation input
# -----------------

include engine/grit.mk
include gtasa/grit.mk

include dependencies/grit-bullet/grit.mk
include dependencies/grit-lua/grit.mk
include dependencies/grit-ogre/grit.mk
include dependencies/grit-util/grit.mk
include dependencies/jsonxx_grit.mk
include dependencies/quex-0.34.1/grit.mk
include dependencies/recastnavigation_grit.mk


PKGCONFIG_DEPS = freetype2 gl glu glew xaw7 zlib zziplib xrandr xrender


FREEIMAGE_LDFLAGS= \
    -lfreeimage \

ICU_LDLIBS= \
    /usr/lib/x86_64-linux-gnu/libicui18n.a \
    /usr/lib/x86_64-linux-gnu/libicuuc.a \
    /usr/lib/x86_64-linux-gnu/libicudata.a \
    -ldl \

OPENAL_LDLIBS   ?= -lopenal -lvorbisfile

VORBIS_LDLIBS   ?= -lvorbisfile



GRIT_WEAK_C_SRCS= \
    $(addprefix dependencies/grit-bullet/,$(BULLET_WEAK_C_SRCS)) \
    $(addprefix dependencies/grit-lua/,$(LUA_WEAK_C_SRCS)) \
    $(addprefix dependencies/grit-ogre/,$(OGRE_WEAK_C_SRCS)) \
    $(addprefix dependencies/grit-util/,$(UTIL_WEAK_C_SRCS)) \
    $(addprefix dependencies/jsonxx/,$(JSONXX_WEAK_C_SRCS)) \
    $(addprefix dependencies/quex-0.34.1/,$(QUEX_WEAK_C_SRCS)) \
    $(addprefix dependencies/recastnavigation/,$(RECAST_WEAK_C_SRCS)) \
    $(FREEIMAGE_WEAK_C_SRCS) \
    $(ICU_WEAK_C_SRCS) \
    $(OPENAL_WEAK_C_SRCS) \
    $(VORBIS_WEAK_C_SRCS) \

GRIT_WEAK_CPP_SRCS= \
    $(addprefix dependencies/grit-bullet/,$(BULLET_WEAK_CPP_SRCS)) \
    $(addprefix dependencies/grit-lua/,$(LUA_WEAK_CPP_SRCS)) \
    $(addprefix dependencies/grit-ogre/,$(OGRE_WEAK_CPP_SRCS)) \
    $(addprefix dependencies/grit-util/,$(UTIL_WEAK_CPP_SRCS)) \
    $(addprefix dependencies/jsonxx/,$(JSONXX_WEAK_CPP_SRCS)) \
    $(addprefix dependencies/quex-0.34.1/,$(QUEX_WEAK_CPP_SRCS)) \
    $(addprefix dependencies/recastnavigation/,$(RECAST_WEAK_CPP_SRCS)) \
    $(FREEIMAGE_WEAK_CPP_SRCS) \
    $(ICU_WEAK_CPP_SRCS) \
    $(OPENAL_WEAK_CPP_SRCS) \
    $(VORBIS_WEAK_CPP_SRCS) \

GRIT_C_SRCS= \
    $(addprefix dependencies/grit-bullet/,$(BULLET_C_SRCS)) \
    $(addprefix dependencies/grit-lua/,$(LUA_C_SRCS)) \
    $(addprefix dependencies/grit-ogre/,$(OGRE_C_SRCS)) \
    $(addprefix dependencies/grit-util/,$(UTIL_C_SRCS)) \
    $(addprefix dependencies/jsonxx/,$(JSONXX_C_SRCS)) \
    $(addprefix dependencies/quex-0.34.1/,$(QUEX_C_SRCS)) \
    $(addprefix dependencies/recastnavigation/,$(RECAST_C_SRCS)) \
    $(FREEIMAGE_C_SRCS) \
    $(ICU_C_SRCS) \
    $(OPENAL_C_SRCS) \
    $(VORBIS_C_SRCS) \

GRIT_CPP_SRCS= \
    $(addprefix dependencies/grit-bullet/,$(BULLET_CPP_SRCS)) \
    $(addprefix dependencies/grit-lua/,$(LUA_CPP_SRCS)) \
    $(addprefix dependencies/grit-ogre/,$(OGRE_CPP_SRCS)) \
    $(addprefix dependencies/grit-util/,$(UTIL_CPP_SRCS)) \
    $(addprefix dependencies/jsonxx/,$(JSONXX_CPP_SRCS)) \
    $(addprefix dependencies/quex-0.34.1/,$(QUEX_CPP_SRCS)) \
    $(addprefix dependencies/recastnavigation/,$(RECAST_CPP_SRCS)) \
    $(FREEIMAGE_CPP_SRCS) \
    $(ICU_CPP_SRCS) \
    $(OPENAL_CPP_SRCS) \
    $(VORBIS_CPP_SRCS) \
	$(addprefix engine/,$(ENGINE_CPP_SRCS)) \


# TODO: remove EXTRACT_INCLUDE_DIRS from here, refactor to put stuff in dependencies/
INCLUDE_DIRS= \
    $(addprefix gtasa/,$(EXTRACT_INCLUDE_DIRS)) \
    $(addprefix dependencies/grit-bullet/,$(BULLET_INCLUDE_DIRS)) \
    $(addprefix dependencies/grit-lua/,$(LUA_INCLUDE_DIRS)) \
    $(addprefix dependencies/grit-ogre/,$(OGRE_INCLUDE_DIRS)) \
    $(addprefix dependencies/grit-util/,$(UTIL_INCLUDE_DIRS)) \
    $(addprefix dependencies/jsonxx/,$(JSONXX_INCLUDE_DIRS)) \
    $(addprefix dependencies/quex-0.34.1/,$(QUEX_INCLUDE_DIRS)) \
    $(addprefix dependencies/recastnavigation/,$(RECAST_INCLUDE_DIRS)) \
    $(FREEIMAGE_INCLUDE_DIRS) \
    $(ICU_INCLUDE_DIRS) \
    $(OPENAL_INCLUDE_DIRS) \
    $(VORBIS_INCLUDE_DIRS) \

CFLAGS= \
	$(INCLUDE_DIRS:%=-isystem%)  \
	$(BULLET_DEFS:%=-D%) \
	$(LUA_DEFS:%=-D%) \
	$(OGRE_DEFS:%=-D%) \
	$(UTIL_DEFS:%=-D%) \
	$(JSONXX_DEFS:%=-D%) \
	$(QUEX_DEFS:%=-D%) \
	$(FREEIMAGE_DEFS:%=-D%) \
	$(ICU_DEFS:%=-D%) \
	$(OPENAL_DEFS:%=-D%) \
	$(VORBIS_DEFS:%=-D%) \
	$(shell pkg-config $(PKGCONFIG_DEPS) --cflags) \

LDFLAGS= \
	$(BULLET_LDFLAGS) \
	$(LUA_LDFLAGS) \
	$(OGRE_LDFLAGS) \
	$(UTIL_LDFLAGS) \
	$(JSONXX_LDFLAGS) \
	$(QUEX_LDFLAGS) \
	$(FREEIMAGE_LDFLAGS) \
	$(ICU_LDFLAGS) \
	$(OPENAL_LDFLAGS) \
	$(VORBIS_LDFLAGS) \
	$(shell pkg-config $(PKGCONFIG_DEPS) --libs-only-L --libs-only-other) \

LDLIBS= \
	$(BULLET_LDLIBS) \
	$(LUA_LDLIBS) \
	$(OGRE_LDLIBS) \
	$(UTIL_LDLIBS) \
	$(JSONXX_LDLIBS) \
	$(QUEX_LDLIBS) \
	$(FREEIMAGE_LDLIBS) \
	$(ICU_LDLIBS) \
	$(OPENAL_LDLIBS) \
	$(VORBIS_LDLIBS) \
	$(shell pkg-config $(PKGCONFIG_DEPS) --libs-only-l) \
	-lreadline \
	-lm \

COL_CONV_TARGETS= \
	$(addprefix build/engine/,$(COL_CONV_STANDALONE_CPP_SRCS)) \

EXTRACT_TARGETS= \
	$(addprefix build/gtasa/,$(EXTRACT_CPP_SRCS)) \
	$(addprefix build/dependencies/grit-ogre/,$(OGRE_WEAK_CPP_SRCS:%.cpp=%.weak_cpp)) \
	$(addprefix build/dependencies/grit-ogre/,$(OGRE_WEAK_C_SRCS:%.c=%.weak_c)) \
	$(addprefix build/dependencies/grit-ogre/,$(OGRE_CPP_SRCS)) \
	$(addprefix build/dependencies/grit-ogre/,$(OGRE_C_SRCS)) \

GRIT_TARGETS= \
	$(addprefix build/,$(GRIT_WEAK_CPP_SRCS:%.cpp=%.weak_cpp)) \
	$(addprefix build/,$(GRIT_WEAK_C_SRCS:%.c=%.weak_c)) \
	$(addprefix build/,$(GRIT_CPP_SRCS)) \
	$(addprefix build/,$(GRIT_C_SRCS)) \

GSL_TARGETS= \
	$(addprefix build/engine/,$(GSL_STANDALONE_CPP_SRCS)) \

XMLCONVERTER_TARGETS= \
	$(addprefix build/dependencies/grit-ogre/,$(XMLCONVERTER_WEAK_CPP_SRCS:%.cpp=%.weak_cpp)) \
	$(addprefix build/dependencies/grit-ogre/,$(XMLCONVERTER_WEAK_C_SRCS:%.c=%.weak_c)) \
	$(addprefix build/dependencies/grit-ogre/,$(XMLCONVERTER_CPP_SRCS)) \
	$(addprefix build/dependencies/grit-ogre/,$(XMLCONVERTER_C_SRCS)) \

ALL_TARGETS= \
	$(COL_CONV_TARGETS) \
	$(EXTRACT_TARGETS) \
	$(GRIT_TARGETS) \
	$(GSL_TARGETS) \
	$(XMLCONVERTER_TARGETS) \

CODEGEN= \
    $(OPT) \
    $(ARCH) \
    -Wno-type-limits \
    -Wno-deprecated \
    -g \
    -ffast-math \


# -----------
# Build rules
# -----------

COMPUTING_DEPENDENCIES= echo -e '\e[0mComputing dependencies: \e[33m$<\e[0m'
COMPILING= echo -e '\e[0mCompiling: \e[32m$<\e[0m'
LINKING= echo -e '\e[0mLinking: \e[1;32m$@\e[0m'


build/%.cpp.o: %.cpp
	@$(COMPILING)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -c $(CODEGEN) -std=c++11 -pedantic -Wall -Wextra $(CFLAGS) $< -o $@

build/%.c.o: %.c
	@$(COMPILING)
	@mkdir -p $(shell dirname $@)
	@$(CC) -c $(CODEGEN) -std=c99 -pedantic -Wall -Wextra $(CFLAGS) $< -o $@

build/%.weak_cpp.o: %.cpp
	@$(COMPILING)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -c $(CODEGEN) -std=c++11 $(CFLAGS) $< -o $@

# Hack to also support dependencies that use .cc
build/%.weak_cpp.o: %.cc
	@$(COMPILING)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -c $(CODEGEN) -std=c++11 $(CFLAGS) $< -o $@

build/%.weak_c.o: %.c
	@$(COMPILING)
	@mkdir -p $(shell dirname $@)
	@$(CC) -c $(CODEGEN) $(CFLAGS) $< -o $@

ALL_EXECUTABLES= extract grit gsl grit_col_conv GritXMLConverter
all: $(ALL_EXECUTABLES)

extract: $(addsuffix .o,$(EXTRACT_TARGETS))
	@$(LINKING)
	@$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

grit: $(addsuffix .o,$(GRIT_TARGETS))
	@$(LINKING)
	@$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

gsl: $(addsuffix .o,$(GSL_TARGETS))
	@$(LINKING)
	@$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

grit_col_conv: $(addsuffix .o,$(COL_CONV_TARGETS))
	@$(LINKING)
	@$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@

GritXMLConverter: $(addsuffix .o,$(XMLCONVERTER_TARGETS))
	@$(LINKING)
	@$(CXX) $^ $(LDFLAGS) $(LDLIBS) -o $@


# ---------
# Dev stuff
# ---------

build/%.cpp.d: %.cpp
	@$(COMPUTING_DEPENDENCIES)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -MM -std=c++11 $(CFLAGS) $< -o $@ -MT $(@:%.d=%.o)

build/%.c.d: %.c
	@$(COMPUTING_DEPENDENCIES)
	@mkdir -p $(shell dirname $@)
	@$(CC) -MM -std=c99 $(CFLAGS) $< -o $@ -MT $(@:%.d=%.o)

build/%.weak_cpp.d: %.cpp
	@$(COMPUTING_DEPENDENCIES)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -MM -std=c++11 $(CFLAGS) $< -o $@ -MT $(@:%.d=%.o)

# Hack to also support dependencies that use .cc
build/%.weak_cpp.d: %.cc
	@$(COMPUTING_DEPENDENCIES)
	@mkdir -p $(shell dirname $@)
	@$(CXX) -MM -std=c++11 $(CFLAGS) $< -o $@ -MT $(@:%.d=%.o)

build/%.weak_c.d: %.c
	@$(COMPUTING_DEPENDENCIES)
	@mkdir -p $(shell dirname $@)
	@$(CC) -MM $(CFLAGS) $< -o $@ -MT $(@:%.d=%.o)

ALL_DEPS = $(addsuffix .d,$(ALL_TARGETS))

clean_depend:
	@rm -f $(ALL_DEPS)
	@echo Dependencies cleaned.

clean:
	rm -rfv $(ALL_EXECUTABLES) build

-include $(ALL_DEPS)
