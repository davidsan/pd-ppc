#
# Generic PureData makefile for building externals
# Should be able to compile on the following platforms
#   - Android
#   - Cygwin
#   - MacOSX
#   - IPhoneOS
#   - Linux
#   - Windows
#
# Author : Philippe Esling <esling@ircam.fr>
#

# add your .c source files, one object per file
SOURCES = horloge.c multipouet.c

# If you need to override the shitty Apple-LLVM CC (like me)
CC = /usr/bin/gcc

# these can be set from outside without (usually) breaking the build
CFLAGS = -Wall -W -g
LDFLAGS =
LIBS =

# You need to edit this to have the PD includes !
PD_PATH = /Applications/Pd-extended.app/Contents/Resources/
PD_INCLUDE = $(PD_PATH)/src

# where to install the library, overridden below depending on platform
prefix = /usr/local
libdir = $(prefix)/lib
pkglibdir = $(libdir)/pd-externals
objectsdir = $(pkglibdir)

# If you want multiple-compile
ALLSOURCES := $(SOURCES) $(SOURCES_android) $(SOURCES_cygwin) $(SOURCES_macosx) \
	         $(SOURCES_iphoneos) $(SOURCES_linux) $(SOURCES_windows)

# If you want to create
DISTDIR=$(LIBRARY_NAME)-$(LIBRARY_VERSION)
ORIGDIR=pd-$(LIBRARY_NAME:~=)_$(LIBRARY_VERSION)

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
  CPU := $(shell uname -p)
  ifeq ($(CPU),arm) # iPhone/iPod Touch
    SOURCES += $(SOURCES_iphoneos)
    EXTENSION = pd_darwin
    SHARED_EXTENSION = dylib
    OS = iphoneos
    IPHONE_BASE=/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin
    CC=$(IPHONE_BASE)/gcc
    CPP=$(IPHONE_BASE)/cpp
    CXX=$(IPHONE_BASE)/g++
    ISYSROOT = -isysroot /Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS3.0.sdk
    IPHONE_CFLAGS = -miphoneos-version-min=3.0 $(ISYSROOT) -arch armv6
    OPT_CFLAGS =  -funroll-loops -fomit-frame-pointer
    ALL_CFLAGS := $(IPHONE_CFLAGS) $(ALL_CFLAGS)
    ALL_LDFLAGS += -arch armv6 -bundle -undefined dynamic_lookup $(ISYSROOT)
    SHARED_LDFLAGS += -arch armv6 -dynamiclib -undefined dynamic_lookup $(ISYSROOT)
    ALL_LIBS += -lc $(LIBS_iphoneos)
    STRIP = strip -x
    DISTBINDIR=$(DISTDIR)-$(OS)
  else # Mac OS X
    SOURCES += $(SOURCES_macosx)
    EXTENSION = pd_darwin
    SHARED_EXTENSION = dylib
    OS = macosx
    OPT_CFLAGS = -ftree-vectorize
    ALL_CFLAGS += -fPIC
    # if the 'pd' binary exists, check the linking against it to aid with stripping
    BUNDLE_LOADER = $(shell test ! -e $(PD_PATH)/bin/pd || echo -bundle_loader $(PD_PATH)/bin/pd)
    ALL_LDFLAGS += -undefined dynamic_lookup
    SHARED_LDFLAGS += $(FAT_FLAGS) -dynamiclib -undefined dynamic_lookup \
	-install_name @loader_path/$(SHARED_LIB) -compatibility_version 1 -current_version 1.0
    ALL_LIBS += -lc $(LIBS_macosx)
    STRIP = strip -x
    DISTBINDIR=$(DISTDIR)-$(OS)
  endif
endif
# Tho Android uses Linux, we use this fake uname to provide an easy way to
# setup all this things needed to cross-compile for Android using the NDK
ifeq ($(UNAME),ANDROID)
  CPU := arm
  SOURCES += $(SOURCES_android)
  EXTENSION = pd_linux
  SHARED_EXTENSION = so
  OS = android
  NDK_BASE := /usr/local/android-ndk
  NDK_PLATFORM_VERSION := 5
  NDK_SYSROOT=$(NDK_BASE)/platforms/android-$(NDK_PLATFORM_VERSION)/arch-arm
  NDK_UNAME := $(shell uname -s | tr '[A-Z]' '[a-z]')
  NDK_TOOLCHAIN_BASE=$(NDK_BASE)/toolchains/arm-linux-androideabi-4.4.3/prebuilt/$(NDK_UNAME)-x86
  CC := $(NDK_TOOLCHAIN_BASE)/bin/arm-linux-androideabi-gcc --sysroot=$(NDK_SYSROOT)
  OPT_CFLAGS = -O6 -funroll-loops -fomit-frame-pointer
  CFLAGS +=
  LDFLAGS += -rdynamic -shared
  SHARED_LDFLAGS += -Wl,-soname,$(SHARED_LIB) -shared
  LIBS += -lc $(LIBS_android)
  STRIP := $(NDK_TOOLCHAIN_BASE)/bin/arm-linux-androideabi-strip \
	--strip-unneeded -R .note -R .comment
  DISTBINDIR=$(DISTDIR)-$(OS)-$(shell uname -m)
endif
ifeq ($(UNAME),Linux)
  CPU := $(shell uname -m)
  SOURCES += $(SOURCES_linux)
  EXTENSION = pd_linux
  SHARED_EXTENSION = so
  OS = linux
  OPT_CFLAGS = -O6 -funroll-loops -fomit-frame-pointer
  ALL_CFLAGS += -fPIC
  ALL_LDFLAGS += -rdynamic -shared -fPIC -Wl,-rpath,"\$$ORIGIN",--enable-new-dtags
  SHARED_LDFLAGS += -Wl,-soname,$(SHARED_LIB) -shared
  ALL_LIBS += -lc $(LIBS_linux)
  STRIP = strip --strip-unneeded -R .note -R .comment
  DISTBINDIR=$(DISTDIR)-$(OS)-$(shell uname -m)
endif
ifeq ($(UNAME),GNU)
  # GNU/Hurd, should work like GNU/Linux for basically all externals
  CPU := $(shell uname -m)
  SOURCES += $(SOURCES_linux)
  EXTENSION = pd_linux
  SHARED_EXTENSION = so
  OS = linux
  PD_PATH = /usr
  OPT_CFLAGS = -O6 -funroll-loops -fomit-frame-pointer
  ALL_CFLAGS += -fPIC
  ALL_LDFLAGS += -rdynamic -shared -fPIC -Wl,-rpath,"\$$ORIGIN",--enable-new-dtags
  SHARED_LDFLAGS += -shared -Wl,-soname,$(SHARED_LIB)
  ALL_LIBS += -lc $(LIBS_linux)
  STRIP = strip --strip-unneeded -R .note -R .comment
  DISTBINDIR=$(DISTDIR)-$(OS)-$(shell uname -m)
endif
ifeq ($(UNAME),GNU/kFreeBSD)
  # Debian GNU/kFreeBSD, should work like GNU/Linux for basically all externals
  CPU := $(shell uname -m)
  SOURCES += $(SOURCES_linux)
  EXTENSION = pd_linux
  SHARED_EXTENSION = so
  OS = linux
  PD_PATH = /usr
  OPT_CFLAGS = -O6 -funroll-loops -fomit-frame-pointer
  ALL_CFLAGS += -fPIC
  ALL_LDFLAGS += -rdynamic -shared -fPIC -Wl,-rpath,"\$$ORIGIN",--enable-new-dtags
  SHARED_LDFLAGS += -shared -Wl,-soname,$(SHARED_LIB)
  ALL_LIBS += -lc $(LIBS_linux)
  STRIP = strip --strip-unneeded -R .note -R .comment
  DISTBINDIR=$(DISTDIR)-$(OS)-$(shell uname -m)
endif
ifeq (CYGWIN,$(findstring CYGWIN,$(UNAME)))
  CPU := $(shell uname -m)
  SOURCES += $(SOURCES_cygwin)
  EXTENSION = dll
  SHARED_EXTENSION = dll
  OS = cygwin
  PD_PATH = $(shell cygpath $$PROGRAMFILES)/pd
  OPT_CFLAGS = -O6 -funroll-loops -fomit-frame-pointer
  ALL_CFLAGS +=
  ALL_LDFLAGS += -rdynamic -shared -L"$(PD_PATH)/src" -L"$(PD_PATH)/bin"
  SHARED_LDFLAGS += -shared -Wl,-soname,$(SHARED_LIB)
  ALL_LIBS += -lc -lpd $(LIBS_cygwin)
  STRIP = strip --strip-unneeded -R .note -R .comment
  DISTBINDIR=$(DISTDIR)-$(OS)
endif
ifeq (MINGW,$(findstring MINGW,$(UNAME)))
  CPU := $(shell uname -m)
  SOURCES += $(SOURCES_windows)
  EXTENSION = dll
  SHARED_EXTENSION = dll
  OS = windows
  PD_PATH = $(shell cd "$$PROGRAMFILES/pd" && pwd)
  # MinGW doesn't seem to include cc so force gcc
  CC=gcc
  OPT_CFLAGS = -O3 -funroll-loops -fomit-frame-pointer
  ALL_CFLAGS += -mms-bitfields
  ALL_LDFLAGS += -s -shared -Wl,--enable-auto-import
  SHARED_LDFLAGS += -shared
  ALL_LIBS += -L"$(PD_PATH)/src" -L"$(PD_PATH)/bin" -L"$(PD_PATH)/obj" \
	-lpd -lwsock32 -lkernel32 -luser32 -lgdi32 -liberty $(LIBS_windows)
  STRIP = strip --strip-unneeded -R .note -R .comment
  DISTBINDIR=$(DISTDIR)-$(OS)
endif

# in case somebody manually set the HELPPATCHES above
HELPPATCHES ?= $(SOURCES:.c=-help.pd) $(PDOBJECTS:.pd=-help.pd)

ALL_CFLAGS := $(ALL_CFLAGS) $(CFLAGS) $(OPT_CFLAGS)
ALL_LDFLAGS := $(LDFLAGS) $(ALL_LDFLAGS)
ALL_LIBS := $(LIBS) $(ALL_LIBS)

SHARED_SOURCE ?= $(wildcard lib$(LIBRARY_NAME).c)
SHARED_HEADER ?= $(shell test ! -e $(LIBRARY_NAME).h || echo $(LIBRARY_NAME).h)
SHARED_LIB ?= $(SHARED_SOURCE:.c=.$(SHARED_EXTENSION))
SHARED_TCL_LIB = $(wildcard lib$(LIBRARY_NAME).tcl)

.PHONY = install libdir_install single_install install-doc install-examples install-manual install-unittests clean distclean dist etags $(LIBRARY_NAME)

all: $(SOURCES:.c=.$(EXTENSION)) $(SHARED_LIB)

%.o: %.c
	$(CC) $(ALL_CFLAGS) -o "$*.o" -c "$*.c"

%.$(EXTENSION): %.o $(SHARED_LIB)
	$(CC) $(ALL_LDFLAGS) -o "$*.$(EXTENSION)" "$*.o"  $(ALL_LIBS) $(SHARED_LIB)
	chmod a-x "$*.$(EXTENSION)"

# this links everything into a single binary file
$(LIBRARY_NAME): $(SOURCES:.c=.o) $(LIBRARY_NAME).o lib$(LIBRARY_NAME).o
	$(CC) $(ALL_LDFLAGS) -o $(LIBRARY_NAME).$(EXTENSION) $(SOURCES:.c=.o) \
		$(LIBRARY_NAME).o lib$(LIBRARY_NAME).o $(ALL_LIBS)
	chmod a-x $(LIBRARY_NAME).$(EXTENSION)

$(SHARED_LIB): $(SHARED_SOURCE:.c=.o)
	$(CC) $(SHARED_LDFLAGS) -o $(SHARED_LIB) $(SHARED_SOURCE:.c=.o) $(ALL_LIBS)

install: libdir_install

# The meta and help files are explicitly installed to make sure they are
# actually there.  Those files are not optional, then need to be there.
libdir_install: $(SOURCES:.c=.$(EXTENSION)) $(SHARED_LIB) install-doc install-examples install-manual install-unittests
	$(INSTALL_DIR) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	$(INSTALL_DATA) $(LIBRARY_NAME)-meta.pd \
		$(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	test -z "$(strip $(SOURCES))" || (\
		$(INSTALL_PROGRAM) $(SOURCES:.c=.$(EXTENSION)) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME) && \
		$(STRIP) $(addprefix $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/,$(SOURCES:.c=.$(EXTENSION))))
	test -z "$(strip $(SHARED_LIB))" || \
		$(INSTALL_DATA) $(SHARED_LIB) \
			$(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	test -z "$(strip $(wildcard $(SOURCES:.c=.tcl)))" || \
		$(INSTALL_DATA) $(wildcard $(SOURCES:.c=.tcl)) \
			$(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	test -z "$(strip $(PDOBJECTS))" || \
		$(INSTALL_DATA) $(PDOBJECTS) \
			$(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	test -z "$(strip $(SHARED_TCL_LIB))" || \
		$(INSTALL_DATA) $(SHARED_TCL_LIB) \
			$(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)

# install library linked as single binary
single_install: $(LIBRARY_NAME) install-doc install-examples install-manual install-unittests
	$(INSTALL_DIR) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	$(INSTALL_PROGRAM) $(LIBRARY_NAME).$(EXTENSION) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	$(STRIP) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/$(LIBRARY_NAME).$(EXTENSION)

install-doc:
	$(INSTALL_DIR) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	test -z "$(strip $(SOURCES) $(PDOBJECTS))" || \
		$(INSTALL_DATA) $(HELPPATCHES) \
			$(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)
	$(INSTALL_DATA) README.txt $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/README.txt
	$(INSTALL_DATA) LICENSE.txt $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/LICENSE.txt

install-examples:
	test -z "$(strip $(EXAMPLES))" || \
		$(INSTALL_DIR) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/examples && \
		for file in $(EXAMPLES); do \
			$(INSTALL_DATA) examples/$$file $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/examples; \
		done

install-manual:
	test -z "$(strip $(MANUAL))" || \
		$(INSTALL_DIR) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/manual && \
		for file in $(MANUAL); do \
			$(INSTALL_DATA) manual/$$file $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/manual; \
		done

install-unittests:
	test -z "$(strip $(UNITTESTS))" || \
		$(INSTALL_DIR) $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/unittests && \
		for file in $(UNITTESTS); do \
			$(INSTALL_DATA) unittests/$$file $(DESTDIR)$(objectsdir)/$(LIBRARY_NAME)/unittests; \
		done

clean:
	-rm -f -- $(SOURCES:.c=.o) $(SOURCES_LIB:.c=.o) $(SHARED_SOURCE:.c=.o)
	-rm -f -- $(SOURCES:.c=.$(EXTENSION))
	-rm -f -- $(LIBRARY_NAME).o
	-rm -f -- $(LIBRARY_NAME).$(EXTENSION)
	-rm -f -- $(SHARED_LIB)

distclean: clean
	-rm -f -- $(DISTBINDIR).tar.gz
	-rm -rf -- $(DISTBINDIR)
	-rm -f -- $(DISTDIR).tar.gz
	-rm -rf -- $(DISTDIR)
	-rm -f -- $(ORIGDIR).tar.gz
	-rm -rf -- $(ORIGDIR)


$(DISTBINDIR):
	$(INSTALL_DIR) $(DISTBINDIR)

libdir: all $(DISTBINDIR)
	$(INSTALL_DATA) $(LIBRARY_NAME)-meta.pd  $(DISTBINDIR)
	$(INSTALL_DATA) $(SOURCES) $(SHARED_SOURCE) $(SHARED_HEADER) $(DISTBINDIR)
	$(INSTALL_DATA) $(HELPPATCHES) $(DISTBINDIR)
	test -z "$(strip $(EXTRA_DIST))" || \
		$(INSTALL_DATA) $(EXTRA_DIST)    $(DISTBINDIR)
#	tar --exclude-vcs -czpf $(DISTBINDIR).tar.gz $(DISTBINDIR)

$(DISTDIR):
	$(INSTALL_DIR) $(DISTDIR)

$(ORIGDIR):
	$(INSTALL_DIR) $(ORIGDIR)

dist: $(DISTDIR)
	$(INSTALL_DATA) Makefile  $(DISTDIR)
	$(INSTALL_DATA) README.txt $(DISTDIR)
	$(INSTALL_DATA) LICENSE.txt $(DISTDIR)
	$(INSTALL_DATA) $(LIBRARY_NAME)-meta.pd  $(DISTDIR)
	test -z "$(strip $(ALLSOURCES))" || \
		$(INSTALL_DATA) $(ALLSOURCES)  $(DISTDIR)
	test -z "$(strip $(wildcard $(ALLSOURCES:.c=.tcl)))" || \
		$(INSTALL_DATA) $(wildcard $(ALLSOURCES:.c=.tcl))  $(DISTDIR)
	test -z "$(strip $(wildcard $(LIBRARY_NAME).c))" || \
		$(INSTALL_DATA) $(LIBRARY_NAME).c  $(DISTDIR)
	test -z "$(strip $(SHARED_HEADER))" || \
		$(INSTALL_DATA) $(SHARED_HEADER)  $(DISTDIR)
	test -z "$(strip $(SHARED_SOURCE))" || \
		$(INSTALL_DATA) $(SHARED_SOURCE)  $(DISTDIR)
	test -z "$(strip $(SHARED_TCL_LIB))" || \
		$(INSTALL_DATA) $(SHARED_TCL_LIB)  $(DISTDIR)
	test -z "$(strip $(PDOBJECTS))" || \
		$(INSTALL_DATA) $(PDOBJECTS)  $(DISTDIR)
	test -z "$(strip $(HELPPATCHES))" || \
		$(INSTALL_DATA) $(HELPPATCHES) $(DISTDIR)
	test -z "$(strip $(EXTRA_DIST))" || \
		$(INSTALL_DATA) $(EXTRA_DIST)    $(DISTDIR)
	test -z "$(strip $(EXAMPLES))" || \
		$(INSTALL_DIR) $(DISTDIR)/examples && \
		for file in $(EXAMPLES); do \
			$(INSTALL_DATA) examples/$$file $(DISTDIR)/examples; \
		done
	test -z "$(strip $(MANUAL))" || \
		$(INSTALL_DIR) $(DISTDIR)/manual && \
		for file in $(MANUAL); do \
			$(INSTALL_DATA) manual/$$file $(DISTDIR)/manual; \
		done
	test -z "$(strip $(UNITTESTS))" || \
		$(INSTALL_DIR) $(DISTDIR)/unittests && \
		for file in $(UNITTESTS); do \
			$(INSTALL_DATA) unittests/$$file $(DISTDIR)/unittests; \
		done
	tar --exclude-vcs -czpf $(DISTDIR).tar.gz $(DISTDIR)

# make a Debian source package
dpkg-source:
	debclean
	make distclean dist
	mv $(DISTDIR) $(ORIGDIR)
	tar --exclude-vcs -czpf ../$(ORIGDIR).orig.tar.gz $(ORIGDIR)
	rm -f -- $(DISTDIR).tar.gz
	rm -rf -- $(DISTDIR) $(ORIGDIR)
	cd .. && dpkg-source -b $(LIBRARY_NAME)

etags: TAGS

TAGS: $(wildcard $(PD_INCLUDE)/*.h) $(SOURCES) $(SHARED_SOURCE) $(SHARED_HEADER)
	etags $(wildcard $(PD_INCLUDE)/*.h)
	etags -a *.h $(SOURCES) $(SHARED_SOURCE) $(SHARED_HEADER)
	etags -a --language=none --regex="/proc[ \t]+\([^ \t]+\)/\1/" *.tcl

showsetup:
	@echo "CC: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "LIBS: $(LIBS)"
	@echo "ALL_CFLAGS: $(ALL_CFLAGS)"
	@echo "ALL_LDFLAGS: $(ALL_LDFLAGS)"
	@echo "ALL_LIBS: $(ALL_LIBS)"
	@echo "PD_INCLUDE: $(PD_INCLUDE)"
	@echo "PD_PATH: $(PD_PATH)"
	@echo "objectsdir: $(objectsdir)"
	@echo "LIBRARY_NAME: $(LIBRARY_NAME)"
	@echo "LIBRARY_VERSION: $(LIBRARY_VERSION)"
	@echo "SOURCES: $(SOURCES)"
	@echo "SHARED_HEADER: $(SHARED_HEADER)"
	@echo "SHARED_SOURCE: $(SHARED_SOURCE)"
	@echo "SHARED_LIB: $(SHARED_LIB)"
	@echo "SHARED_TCL_LIB: $(SHARED_TCL_LIB)"
	@echo "PDOBJECTS: $(PDOBJECTS)"
	@echo "ALLSOURCES: $(ALLSOURCES)"
	@echo "ALLSOURCES TCL: $(wildcard $(ALLSOURCES:.c=.tcl))"
	@echo "UNAME: $(UNAME)"
	@echo "CPU: $(CPU)"
	@echo "pkglibdir: $(pkglibdir)"
	@echo "DISTDIR: $(DISTDIR)"
	@echo "ORIGDIR: $(ORIGDIR)"
