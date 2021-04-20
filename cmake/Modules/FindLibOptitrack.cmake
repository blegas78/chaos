#                                               -*- cmake -*-
#
#  FindLibOptitrack.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibOPTITRACK
# Once done this will define
#  LIBOPTITRACK_FOUND - System has LibXml2
#  LIBOPTITRACK_INCLUDE_DIRS - The LibXml2 include directories
#  LIBOPTITRACK_LIBRARIES - The libraries needed to use LibXml2
#  LIBOPTITRACK_DEFINITIONS - Compiler switches required for using LibXml2

#find_package(PkgConfig)
#pkg_check_modules(PC_LIBOPTITRACK liboptitrack)
#set(LIBOPTITRACK_DEFINITIONS ${PC_LIBOPTITRACK_CFLAGS_OTHER})

find_path(LIBOPTITRACK_INCLUDE_DIR optitrack.h
          HINTS ${PC_LIBOPTITRACK_INCLUDEDIR} ${PC_LIBOPTITRACK_INCLUDE_DIRS}
          PATH_SUFFIXES liboptitrack )

find_library(LIBOPTITRACK_LIBRARY NAMES optitrack liboptitrack
             HINTS ${PC_LIBOPTITRACK_LIBDIR} ${PC_LIBOPTITRACK_LIBRARY_DIRS} /usr/local/lib64)

set(LIBOPTITRACK_LIBRARIES ${LIBOPTITRACK_LIBRARY} )
set(LIBOPTITRACK_INCLUDE_DIRS ${LIBOPTITRACK_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibOptitrack  DEFAULT_MSG
                                  LIBOPTITRACK_LIBRARY LIBOPTITRACK_INCLUDE_DIR)

mark_as_advanced(LIBOPTITRACK_INCLUDE_DIR LIBOPTITRACK_LIBRARY )

#if(${LIBOPTITRACK_FOUND})
#	message("Found optitrack library!")
#	message("-- optitrack.h: ${LIBOPTITRACK_INCLUDE_DIR}")
#	message("-- liboptitrack: ${LIBOPTITRACK_LIBRARY}")
#endif(${LIBOPTITRACK_FOUND})
