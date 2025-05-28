################################################################################
# ltn makefile
#						$Revision: 1.7 $
#						$Date: 2024/03/04 19:16:51 $
################################################################################
#CROSS=mipsel-linux-uclibc-
GCC=$(CROSS)gcc
GXX=$(CROSS)g++
CC=$(CROSS)gcc
AS=$(CROSS)as
AR=$(CROSS)ar
STRIP=$(CROSS)strip
LD=$(CROSS)ld
RANLIB=$(CROSS)ranlib
#CFLAGS= -g -O3 -Wall -Dlinux -pthread
CFLAGS= -g -O3 -Wall -Dlinux -D_DEBUG -pthread -std=c++11
#CFLAGS= -O1 -Wall -Dlinux -pthread
#CFLAGS= -g -Wall -Dlinux -D_DEBUG
RM=rm

# Makefile

# プログラム名とオブジェクトファイル名
program = lutino
objs = ltn_main.o   \
	ltn_param.o  \
	ltn_listen.o \
	ltn_http.o   \
	ltn_send_file.o      \
	ltn_proxy.o  \
	ltn_cgi.o    \
	ltn_tools.o  \
	ltn_script.o \
	ltn_String.o \
	ssdp.o	\
	TinyJS.o      \
	TinyJS_Functions.o    \
	TinyJS_MathFunctions.o        \
	ltn_db.o       \
	utf8tbl.o     \
	dregex.o

# 定義d済マクロの再定義
#CC = gcc
#CFLAGS = -g -Wall

# サフィックスルール適用対象の拡張子の定義
.SUFFIXES: .cpp .o

# プライマリターゲット
$(program): $(objs)
	$(GXX) $(CFLAGS) -o $(program) $^
	strip $(program)
	mv $(program) ..

# サフィックスルール
.cpp.o:
	$(GXX) $(CFLAGS) -c $<

# ファイル削除用ターゲット
.lutino: clean
clean:
	$(RM) -f $(program) $(objs)
# ターゲット 'hello' の定義
hello:
	echo "Hello, World"

# ターゲット 'install' の定義
install: ../lutino
	if [ -d /var/www/html ]; then
	echo "Directory already exists. Remove folder /var/www before execute this command."
	else
	mkdir -p /var/www
	mkdir -p /var/www/html
	-killall lutino
	cp ../lutino /var/www
	cp ../ltn.conf.linux /var/www/ltn.conf
	cp -r ../html /var/www
	cp -r ../database /var/www
	cp -r ../system /var/www
	cp -r ../skin /var/www
	cp ../README.md /var/www
	/var/www/lutino
	fi
# ヘッダーファイルの依存関係
