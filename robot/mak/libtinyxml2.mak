TARGET = build/libtinyxml2
SRCS = src/tinyxml2/tinyxml2.cc
LINK_TYPE = static
INCLUDE = -Isrc
BUILD_DIR = build

include mak/main.mak

