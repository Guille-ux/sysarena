TARGET = sysarena
VERSION = $(shell cat VERSION)
ARCH = $(shell dpkg --print-architecture)

SRC_DIR = ../src
SRC_INCLUDE = ../include
BUILD_DIR = build
DEBIAN_DIR = debian

INCLUDE_DIR = $(DEBIAN_DIR)/usr/local/include
LIB_DIR = $(DEBIAN_DIR)/usr/local/lib

CFLAGS = -Wall -Wextra -O2
ARFLAGS = rcs

all: $(BUILD_DIR)/lib$(TARGET).a

$(BUILD_DIR)/lib$(TARGET).a: $(SRC_DIR)/sysarena.c
	mkdir -p $(BUILD_DIR)
	gcc $(CFLAGS) -c $(SRC_DIR)/sysarena.c -o $(BUILD_DIR)/$(TARGET).o
	ar $(ARFLAGS) $(BUILD_DIR)/lib$(TARGET).a $(BUILD_DIR)/$(TARGET).o

install: all
	mkdir -p $(INCLUDE_DIR)
	mkdir -p $(LIB_DIR)
	cp $(SRC_INCLUDE)/sysarena.h $(INCLUDE_DIR)/
	cp $(BUILD_DIR)/lib$(TARGET).a $(LIB_DIR)/

prepare-control:
	mkdir -p $(DEBIAN_DIR)/DEBIAN
	echo "Package: $(TARGET)" > $(DEBIAN_DIR)/DEBIAN/control
	echo "Version: $(VERSION)" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Section: libs" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Priority: optional" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Architecture: $(ARCH)" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Maintainer: Guille guillermoleiratemes@protonmail.com" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Description: Arena based memory management." >> $(DEBIAN_DIR)/DEBIAN/control
	echo " Arena-based memory management library created from scratch. Very lightweight, no malloc." >> $(DEBIAN_DIR)/DEBIAN/control

deb: install prepare-control
	dpkg-deb --build $(DEBIAN_DIR) $(TARGET)_$(VERSION)_$(ARCH).deb

clean:
	rm -rf $(BUILD_DIR)
	rm -f *.deb
	rm -rf $(DEBIAN_DIR)/usr
	rm -rf $(DEBIAN_DIR)/DEBIAN/control

.PHONY: all install prepare-control deb clean
