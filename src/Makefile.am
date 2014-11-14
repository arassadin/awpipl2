##
#
# This file is the part of AWP Image Processing Library V2 project 
# made by AWP Group 
#
# $Id: Makefile.am,v 1.15 2004/03/03 08:56:32 eugene Exp $
#
##

lib_LTLIBRARIES = libawpipl2.la

libawpipl2_la_SOURCES   = awpBaseAnalysis.c \
			  awpchannels.c \
			awpcommon.c \
			awpcontour.c \
			awpcontrast.c \
			awpconvert.c \
			awpcopypaste.c \
			awpdraw.c \
			awpEdge.c \
			awperror.h \
			awpfft.c \
			awpframeprocess.c \
			awpgeometry.c \
			_awpHSL.c \
			_awpHSL.h \
			_awpHSV.c \
			_awpHSV.h \
			_awpio.c \
			awpio.c \
			awpio.h \
			_awpipl.h \
			awpipl.h \
			awpmaskconvolution.c \
			awpPointUtils.c \
			awpstroke.c \
			awpthreshold.c


pkginclude_HEADERS      = awperror.h \
                          awpipl.h

libawpipl2_la_LIBADD = $(M_LIBS)

if HAVE_JPEG
libawpipl2_la_LIBADD += $(JPEG_LIBS)
endif

if HAVE_FFTW3
libawpipl2_la_LIBADD += $(FFTW3_LIBS)
endif

