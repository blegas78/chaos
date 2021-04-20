#                                               -*- cmake -*-
#
#  FindLibFtdi.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibFtdi
# Once done this will define
#  LIBFTDI_FOUND - System has LibXml2
#  LIBFTDI_INCLUDE_DIRS - The LibXml2 include directories
#  LIBFTDI_LIBRARIES - The libraries needed to use LibXml2
#  LIBFTDI_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_LIBFTDI libftdi1 QUIET)
set(LIBFTDI_DEFINITIONS ${PC_LIBFTDI_CFLAGS_OTHER})

find_path(LIBFTDI_INCLUDE_DIR ftdi.h
          HINTS /usr/local/include/libftdi1/ /opt/local/include/libftdi1/ ${PC_LIBFTDI_INCLUDEDIR} ${PC_LIBFTDI_INCLUDE_DIRS}
          PATH_SUFFIXES libftdi1 )

find_library(LIBFTDI_LIBRARY NAMES ftdi1 libftdi1
             HINTS /usr/local/lib/ ${PC_LIBFTDI_LIBDIR} ${PC_LIBFTDI_LIBRARY_DIRS} )

set(LIBFTDI_LIBRARIES ${LIBFTDI_LIBRARY} )
set(LIBFTDI_INCLUDE_DIRS ${LIBFTDI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibFtdi  DEFAULT_MSG
                                  LIBFTDI_LIBRARY LIBFTDI_INCLUDE_DIR)

mark_as_advanced(LIBFTDI_INCLUDE_DIR LIBFTDI_LIBRARY )

if(LIBFTDI_FOUND)
	add_definitions(-DLIBFTDI_FOUND)
endif()
