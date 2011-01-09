LBITS := $(shell getconf LONG_BIT)
CXX = g++
DEFINES = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CXXFLAGS = -m$(LBITS) -pipe -mtune=generic -O3 -pipe -Wall -W -D_REENTRANT $(DEFINES)
INCPATH = -I/usr/share/qt/mkspecs/linux-g++-$(LBITS) -I. -I/usr/include/QtCore -I/usr/include/QtGui -I/usr/include -I. -I.
LINK = g++
LFLAGS = -m$(LBITS) -Wl,-O3
LIBS = -L/usr/lib -L/usr/lib -L/usr/X11R6/lib$(LBITS) -pthread -pthread -pthread -pthread -pthread -pthread -pthread -pthread -lQtGui -pthread -lpng -lfreetype -lgobject-2.0 -lSM -lICE -pthread -pthread -lXrender -lfontconfig -lXext -lX11 -lQtCore -lz -lm -pthread -lgthread-2.0 -lrt -lglib-2.0 -ldl -lpthread
OBJECTS = main.o mainwindow.o qticonloader.o $@.o moc_mainwindow.o moc_$@.o

all: yellowcot

yellowcot:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@.o $@.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mainwindow.o mainwindow.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o qticonloader.o qticonloader.cpp
	/usr/bin/moc $(DEFINES) $(INCPATH) $@.h -o moc_$@.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_$@.o moc_$@.cpp
	/usr/bin/moc $(DEFINES) $(INCPATH) mainwindow.h -o moc_mainwindow.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mainwindow.o moc_mainwindow.cpp
	$(LINK) $(LFLAGS) -o $@ $(OBJECTS) $(LIBS)

install:
	install -m 755 yellowcot /usr/bin
	if test -d /usr/share/yellowcot; then rm -r /usr/share/yellowcot; fi
	mkdir /usr/share/yellowcot
	cp yellowcot.svg /usr/share/yellowcot/.
	cp yellowcot.desktop /usr/share/applications/.
