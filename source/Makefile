################################################################################
# Lutino Makefile
################################################################################

CROSS ?=
CXX := $(CROSS)g++
RM ?= rm -f

TARGET := ../lutino
INSTALL_DIR ?= /var/www/html9

CPPFLAGS += -DBR_AES_X86NI=0 \
	-DBR_SSE2=0 \
	-DBR_RDRAND=0 \
	-DBR_POWER8=0
CXXFLAGS ?= -g -O3
CXXFLAGS += -Wall -pthread -std=c++11
LDLIBS += -pthread

CORE_SOURCES := \
	ltn_main.cpp \
	ltn_param.cpp \
	ltn_listen.cpp \
	ltn_http.cpp \
	ltn_send_file.cpp \
	ltn_proxy.cpp \
	ltn_cgi.cpp \
	ltn_tools.cpp \
	ltn_script.cpp \
	ltn_String.cpp \
	ltn_transport.cpp \
	ltn_tls.cpp \
	ssdp.cpp \
	TinyJS.cpp \
	TinyJS_Functions.cpp \
	TinyJS_MathFunctions.cpp \
	ltn_db.cpp \
	utf8tbl.cpp \
	dregex.cpp \
	alt_time.cpp

BEARSSL_SOURCES := $(wildcard bearssl/*.cpp)
SOURCES := $(CORE_SOURCES) $(BEARSSL_SOURCES)
OBJECTS := $(SOURCES:.cpp=.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

.PHONY: all clean hello install

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	$(RM) $(TARGET) $(OBJECTS) $(DEPENDENCIES)

install: $(TARGET)
	@set -eu; \
	if [ ! -f ../ltn.conf.linux ]; then \
		echo "Error: ../ltn.conf.linux was not found." >&2; \
		exit 1; \
	fi; \
	for dir in html database skin system; do \
		if [ ! -d "../$$dir" ]; then \
			echo "Error: ../$$dir was not found." >&2; \
			exit 1; \
		fi; \
	done; \
	if [ -d "$(INSTALL_DIR)" ] && \
		find "$(INSTALL_DIR)" -mindepth 1 -maxdepth 1 -print -quit | grep -q .; then \
		echo "Warning: $(INSTALL_DIR) is not empty; installation aborted." >&2; \
		exit 1; \
	fi; \
	mkdir -p "$(INSTALL_DIR)"; \
	install -m 755 "$(TARGET)" "$(INSTALL_DIR)/lutino"; \
	awk -v root="$(INSTALL_DIR)" \
		'$$1 == "document_root" { printf "document_root           %s/html\n", root; next } \
		 $$1 == "skin_root" { printf "skin_root               %s/skin\n", root; next } \
		 $$1 == "alias" && $$2 == "/html/system" { printf "alias /html/system %s/system\n", root; next } \
		 { print }' \
		../ltn.conf.linux > "$(INSTALL_DIR)/.ltn.conf.tmp"; \
	mv "$(INSTALL_DIR)/.ltn.conf.tmp" "$(INSTALL_DIR)/ltn.conf"; \
	cp -a ../html "$(INSTALL_DIR)/html"; \
	cp -a ../database "$(INSTALL_DIR)/database"; \
	cp -a ../skin "$(INSTALL_DIR)/skin"; \
	cp -a ../system "$(INSTALL_DIR)/system"

hello:
	@echo "Hello, World"

-include $(DEPENDENCIES)
