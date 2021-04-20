#                                               -*- cmake -*-
#
#  FindLibJsoncpp.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibJsoncpp
# Once done this will define
#  LIBJSONCPP_FOUND - System has LibXml2
#  LIBJSONCPP_INCLUDE_DIRS - The LibXml2 include directories
#  LIBJSONCPP_LIBRARIES - The libraries needed to use LibXml2
#  LIBJSONCPP_DEFINITIONS - Compiler switches required for using LibXml2

find_path(LIBJSONCPP_INCLUDE_DIR json
          HINTS /usr/include/jsoncpp/ /usr/local/include/ /opt/local/include/ ${PC_LIBJSONCPP_INCLUDEDIR} ${PC_LIBJSONCPP_INCLUDE_DIRS}
          PATH_SUFFIXES json )

find_library(LIBJSONCPP_LIBRARY NAMES jsoncpp libjsoncpp 
             HINTS /usr/local/lib/ /opt/local/lib/ ${PC_LIBJSONCPP_LIBDIR} ${PC_LIBJSONCPP_LIBRARY_DIRS} )

set(LIBJSONCPP_LIBRARIES ${LIBJSONCPP_LIBRARY} )
set(LIBJSONCPP_INCLUDE_DIRS ${LIBJSONCPP_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibJsoncpp  DEFAULT_MSG
                                  LIBJSONCPP_LIBRARY LIBJSONCPP_INCLUDE_DIR)

mark_as_advanced(LIBJSONCPP_INCLUDE_DIR LIBJSONCPP_LIBRARY )

if(LIBJSONCPP_FOUND)
	add_definitions(-DLIBJSONCPP_FOUND)
endif()
