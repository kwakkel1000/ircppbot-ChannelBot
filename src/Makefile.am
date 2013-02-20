## Process this file with automake to produce Makefile.in

GIT_VERSION = $(shell sh -c 'git describe --abbrev=4 --dirty --always')

ACLOCAL_AMFLAGS = -I ../m4

CHANNELBOTLIBRARY_VERSION=0:2:0

lib_LTLIBRARIES = libchannelbot.la

libchannelbot_la_SOURCES = \
        cauth.cpp \
        cchannel.cpp \
        channelbot.cpp

libchannelbot_la_LDFLAGS = --version-info $(CHANNELBOTLIBRARY_VERSION)
