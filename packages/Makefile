TARGET = sysarena
VERSION = $(shell cat VERSION)
ARCH = $(shell dpkg --print-architecture)
PACKAGE_NAME = lib$(TARGET)

SRC_DIR = ../src
SRC_INCLUDE = ../include
BUILD_DIR = build
DEBIAN_DIR = debian

CFLAGS = -Wall -Wextra -O2 -fPIC
ARFLAGS = rcs
LDFLAGS = -shared

all: $(BUILD_DIR)/lib$(TARGET).a $(BUILD_DIR)/lib$(TARGET).so

$(BUILD_DIR)/lib$(TARGET).a: $(SRC_DIR)/$(TARGET).c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@.o
	$(AR) $(ARFLAGS) $@ $@.o

$(BUILD_DIR)/lib$(TARGET).so: $(SRC_DIR)/$(TARGET).c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

install: all
	mkdir -p $(DEBIAN_DIR)/usr/include
	mkdir -p $(DEBIAN_DIR)/usr/lib
	cp $(SRC_INCLUDE)/$(TARGET).h $(DEBIAN_DIR)/usr/include/
	cp $(SRC_INCLUDE)/types.h $(DEBIAN_DIR)/usr/include/
	cp $(BUILD_DIR)/lib$(TARGET).a $(DEBIAN_DIR)/usr/lib/
	cp $(BUILD_DIR)/lib$(TARGET).so $(DEBIAN_DIR)/usr/lib/

prepare-control:
	mkdir -p $(DEBIAN_DIR)/DEBIAN
	echo "Source: $(TARGET)" > $(DEBIAN_DIR)/DEBIAN/control
	echo "Section: libs" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Priority: optional" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Maintainer: Guille guillermoleiratemes@protonmail.com" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Version: $(VERSION)-1" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Architecture: any" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Package: lib$(TARGET)0" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Description: Arena based memory management library." >> $(DEBIAN_DIR)/DEBIAN/control
	echo " Lightweight, no malloc arena-based memory management." >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Package: lib$(TARGET)-dev" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Section: libdevel" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Priority: optional" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Architecture: any" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Depends: lib$(TARGET)0 (= $(VERSION)-1)" >> $(DEBIAN_DIR)/DEBIAN/control
	echo "Description: Development files for the lib$(TARGET) library." >> $(DEBIAN_DIR)/DEBIAN/control
	echo " Includes header files and static library for lib$(TARGET)." >> $(DEBIAN_DIR)/DEBIAN/control

prepare-changelog:
	mkdir -p $(DEBIAN_DIR)
	echo "$(PACKAGE_NAME) ($(VERSION)-1) unstable; urgency=low" > $(DEBIAN_DIR)/changelog
	echo "" >> $(DEBIAN_DIR)/changelog
	echo "  * Initial release." >> $(DEBIAN_DIR)/changelog
	echo "" >> $(DEBIAN_DIR)/changelog
	date -R | sed 's/^\([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \([^ ]*\) \(.*\)/\1, \3 \2 \6 \5/' | awk '{print "-- Guille <guillermoleiratemes@protonmail.com>  "$0}' >> $(DEBIAN_DIR)/changelog

deb: install prepare-control prepare-changelog
	dpkg-deb --build $(DEBIAN_DIR) $(TARGET)_$(VERSION)-1_$(ARCH).deb

source-deb:
	dpkg-source --build $(DEBIAN_DIR)

clean:
	rm -rf $(BUILD_DIR)
	rm -f *.deb
	rm -rf $(DEBIAN_DIR)/usr
	rm -rf $(DEBIAN_DIR)/DEBIAN
	rm -f $(DEBIAN_DIR)/changelog

.PHONY: all install prepare-control deb clean source-deb