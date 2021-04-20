#                                               -*- cmake -*-
#
#  FindLibMogi.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibMogi
# Once done this will define
#  LIBMOGI_FOUND - System has LibXml2
#  LIBMOGI_INCLUDE_DIRS - The LibXml2 include directories
#  LIBMOGI_LIBRARIES - The libraries needed to use LibXml2
#  LIBMOGI_DEFINITIONS - Compiler switches required for using LibXml2

#find_package(PkgConfig)
#pkg_check_modules(PC_LIBMOGI libmogi)
#set(LIBMOGI_DEFINITIONS ${PC_LIBMOGI_CFLAGS_OTHER})

find_path(LIBMOGI_INCLUDE_DIR mogi.h
          HINTS ${PC_LIBMOGI_INCLUDEDIR} ${PC_LIBMOGI_INCLUDE_DIRS}
          PATH_SUFFIXES libmogi )

find_library(LIBMOGI_LIBRARY NAMES mogi libmogi
             HINTS ${PC_LIBMOGI_LIBDIR} ${PC_LIBMOGI_LIBRARY_DIRS} /usr/local/lib64)

set(LIBMOGI_LIBRARIES ${LIBMOGI_LIBRARY} )
set(LIBMOGI_INCLUDE_DIRS ${LIBMOGI_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibMogi  DEFAULT_MSG
                                  LIBMOGI_LIBRARY LIBMOGI_INCLUDE_DIR)

mark_as_advanced(LIBMOGI_INCLUDE_DIR LIBMOGI_LIBRARY )

#if(${LIBMOGI_FOUND})
#	message("Found mogi library!")
#	message("-- mogi.h: ${LIBMOGI_INCLUDE_DIR}")
#	message("-- libmogi: ${LIBMOGI_LIBRARY}")
#endif(${LIBMOGI_FOUND})

find_package (Threads REQUIRED)

# also add the libjsoncpp libraries as they are required:
find_package (LibJsoncpp)
if(LIBJSONCPP_FOUND)
	add_definitions(-DLIBJSONCPP_FOUND)
	set(LIBMOGI_INCLUDE_DIRS ${LIBMOGI_INCLUDE_DIRS} ${LIBJSONCPP_INCLUDE_DIRS} )
	set(LIBMOGI_LIBRARIES ${LIBMOGI_LIBRARIES} ${LIBJSONCPP_LIBRARIES})
endif()

# Currently required for Dynamical stuff
find_package (LibFtdi)
if(LIBFTDI_FOUND)
	add_definitions(-DLIBFTDI_FOUND)
	set(LIBMOGI_INCLUDE_DIRS ${LIBMOGI_INCLUDE_DIRS} ${LIBFTDI_INCLUDE_DIRS} )
	set(LIBMOGI_LIBRARIES ${LIBMOGI_LIBRARIES} ${LIBFTDI_LIBRARIES})
endif()

# I guess this may require OpenCV as well.. dammit…
# also add the MBmath libraries as they are required:
#find_package (OpenCV)
if(OpenCV_FOUND)
	add_definitions(-DOPENCV_EXISTS)
	set(LIBMOGI_INCLUDE_DIRS ${LIBMOGI_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} )
	set(LIBMOGI_LIBRARIES ${LIBMOGI_LIBRARIES} ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
endif(OpenCV_FOUND)

find_package (LibMBsimulation)
if(LIBMBSIMULATION_FOUND)
	add_definitions(-DOPENGL_EXISTS)
#	message ("akjsdfhiausdhflkajshflaksjdhflaskjdhfalksjdfasdf!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ! ! ! ! !")
	set(LIBMOGI_INCLUDE_DIRS ${LIBMOGI_INCLUDE_DIRS} ${LIBMBSIMULATION_INCLUDE_DIRS} )
	set(LIBMOGI_LIBRARIES ${LIBMOGI_LIBRARIES} ${LIBMBSIMULATION_LIBRARIES} )
endif()
