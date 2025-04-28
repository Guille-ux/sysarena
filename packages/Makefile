TARGET = sysarena
VERSION = $(shell cat VERSION)
ARCH = $(shell dpkg --print-architecture)
PACKAGE_NAME = lib$(TARGET)

SRC_DIR = ../src
SRC_INCLUDE = ../include
BUILD_DIR = build
DEBIAN_DIR = debian
DEBIAN_DIR_S = debian

CFLAGS = -Wall -Wextra -O2 -fPIC
ARFLAGS = rcs
LDFLAGS = -shared
MSG = $(shell cat message)

all: $(BUILD_DIR)/lib$(TARGET).a $(BUILD_DIR)/lib$(TARGET).so

$(BUILD_DIR)/lib$(TARGET).a: $(SRC_DIR)/$(TARGET).c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@.o
	$(AR) $(ARFLAGS) $@ $@.o

$(BUILD_DIR)/lib$(TARGET).so: $(SRC_DIR)/$(TARGET).c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

install: all
	mkdir -p $(DEBIAN_DIR_S)/usr/include
	mkdir -p $(DEBIAN_DIR_S)/usr/lib
	mkdir -p $(DEBIAN_DIR)/source
	cp $(SRC_INCLUDE)/$(TARGET).h $(DEBIAN_DIR_S)/usr/include/
	cp $(SRC_INCLUDE)/types.h $(DEBIAN_DIR_S)/usr/include/
	cp $(BUILD_DIR)/lib$(TARGET).a $(DEBIAN_DIR_S)/usr/lib/
	cp $(BUILD_DIR)/lib$(TARGET).so $(DEBIAN_DIR_S)/usr/lib/
	echo "3.0 (native)" > $(DEBIAN_DIR)/source/format

prepare-control:
	mkdir -p $(DEBIAN_DIR)
	cat template >> $(DEBIAN_DIR)/control

prepare-changelog:
	mkdir -p $(DEBIAN_DIR)
	PACKAGE_VERSION="$(PACKAGE_NAME) ($(VERSION)-1) unstable; urgency=low"
	echo "$(PACKAGE_NAME) ($(VERSION)-1) unstable; urgency=low" >> $(DEBIAN_DIR)/changelog
	echo "" >> $(DEBIAN_DIR)/changelog
	echo "	[ Guille ]" >> $(DEBIAN_DIR)/changelog
	echo "	* " $(MSG) >> $(DEBIAN_DIR)/changelog
	echo "" >> $(DEBIAN_DIR)/changelog
	echo "-- Guille <guillermoleiratemes@protonmail.com>  $(shell date -R)" >> $(DEBIAN_DIR)/changelog

deb: install prepare-control prepare-changelog
	dpkg-deb --build $(DEBIAN_DIR) $(TARGET)_$(VERSION)-1_$(ARCH).deb

source-deb:
	dpkg-buildpackage -b -uc -us

clean:
	rm -rf $(BUILD_DIR)
	rm -f *.deb
	rm -rf $(DEBIAN_DIR_S)/usr
	rm -rf $(DEBIAN_DIR)/control

.PHONY: all install prepare-control deb clean source-deb