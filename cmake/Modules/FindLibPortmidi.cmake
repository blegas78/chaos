#                                               -*- cmake -*-
#
#  FindLibPortmidi.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibPortmidi
# Once done this will define
#  LIBPORTMIDI_FOUND - System has LibXml2
#  LIBPORTMIDI_INCLUDE_DIRS - The LibXml2 include directories
#  LIBPORTMIDI_LIBRARIES - The libraries needed to use LibXml2
#  LIBPORTMIDI_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_LIBPORTMIDI QUIET libportmidi libporttime)
set(LIBPORTMIDI_DEFINITIONS ${PC_LIBPORTMIDI_CFLAGS_OTHER})

find_path(LIBPORTMIDI_INCLUDE_DIR portmidi.h
          HINTS /usr/local/include/ ${PC_LIBPORTMIDI_INCLUDEDIR} ${PC_LIBPORTMIDI_INCLUDE_DIRS}
          PATH_SUFFIXES libportmidi )

find_library(LIBPORTMIDI_LIBRARY NAMES portmidi libportmidi
             HINTS ${PC_LIBPORTMIDI_LIBDIR} ${PC_LIBPORTMIDI_LIBRARY_DIRS} )

find_library(LIBPORTTIME_LIBRARY NAMES porttime libporttime
             HINTS ${PC_LIBPORTMIDI_LIBDIR} ${PC_LIBPORTMIDI_LIBRARY_DIRS} )

set(LIBPORTMIDI_LIBRARIES ${LIBPORTMIDI_LIBRARY} ${LIBPORTTIME_LIBRARY} )
set(LIBPORTMIDI_INCLUDE_DIRS ${LIBPORTMIDI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibPortMIDI  DEFAULT_MSG
                                  LIBPORTMIDI_LIBRARY LIBPORTMIDI_INCLUDE_DIR)

mark_as_advanced(LIBPORTMIDI_INCLUDE_DIR LIBPORTMIDI_LIBRARY LIBPORTTIME_LIBRARY)
