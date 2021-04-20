#                                               -*- cmake -*-
#
#  FindLibSixaxis.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibSixaxis
# Once done this will define
#  LIBSIXAXIS_FOUND - System has LibXml2
#  LIBSIXAXIS_INCLUDE_DIRS - The LibXml2 include directories
#  LIBSIXAXIS_LIBRARIES - The libraries needed to use LibXml2
#  LIBSIXAXIS_DEFINITIONS - Compiler switches required for using LibXml2

#find_package(PkgConfig)
#pkg_check_modules(PC_LIBSIXAXIS libsixaxis)
#set(LIBSIXAXIS_DEFINITIONS ${PC_LIBSIXAXIS_CFLAGS_OTHER})

find_path(LIBSIXAXIS_INCLUDE_DIR sixaxis.h
          HINTS ${PC_LIBSIXAXIS_INCLUDEDIR} ${PC_LIBSIXAXIS_INCLUDE_DIRS}
          PATH_SUFFIXES libsixaxis )

find_library(LIBSIXAXIS_LIBRARY NAMES sixaxis libsixaxis
             HINTS ${PC_LIBSIXAXIS_LIBDIR} ${PC_LIBSIXAXIS_LIBRARY_DIRS} /usr/local/lib64)

set(LIBSIXAXIS_LIBRARIES ${LIBSIXAXIS_LIBRARY} )
set(LIBSIXAXIS_INCLUDE_DIRS ${LIBSIXAXIS_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibSixaxis  DEFAULT_MSG
                                  LIBSIXAXIS_LIBRARY LIBSIXAXIS_INCLUDE_DIR)

mark_as_advanced(LIBSIXAXIS_INCLUDE_DIR LIBSIXAXIS_LIBRARY )

#if(${LIBSIXAXIS_FOUND})
#	message("Found sixaxis library!")
#	message("-- sixaxis.h: ${LIBSIXAXIS_INCLUDE_DIR}")
#	message("-- libsixaxis: ${LIBSIXAXIS_LIBRARY}")
#endif(${LIBSIXAXIS_FOUND})
