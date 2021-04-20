#                                               -*- cmake -*-
#
#  FindLibMBsimulation.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibMbsimulation
# Once done this will define
#  LIBMBSIMULATION_FOUND - System has LibXml2
#  LIBMBSIMULATION_INCLUDE_DIRS - The LibXml2 include directories
#  LIBMBSIMULATION_LIBRARIES - The libraries needed to use LibXml2
#  LIBMBSIMULATION_DEFINITIONS - Compiler switches required for using LibXml2

#find_package(PkgConfig)
#pkg_check_modules(PC_LIBMBSIMULATION libmbsimulation)
#set(LIBMBSIMULATION_DEFINITIONS ${PC_LIBMBSIMULATION_CFLAGS_OTHER})

find_path(LIBMBSIMULATION_INCLUDE_DIR mogi/simulation/simulation.h
          HINTS ${PC_LIBMBSIMULATION_INCLUDEDIR} ${PC_LIBMBSIMULATION_INCLUDE_DIRS}
          PATH_SUFFIXES libmbsimulation )

find_library(LIBMBSIMULATION_LIBRARY NAMES mbsimulation libmbsimulation
             HINTS ${PC_LIBMBSIMULATION_LIBDIR} ${PC_LIBMBSIMULATION_LIBRARY_DIRS} /usr/local/lib64)

set(LIBMBSIMULATION_LIBRARIES ${LIBMBSIMULATION_LIBRARY} )
set(LIBMBSIMULATION_INCLUDE_DIRS ${LIBMBSIMULATION_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibMbsimulation  DEFAULT_MSG
                                  LIBMBSIMULATION_LIBRARY LIBMBSIMULATION_INCLUDE_DIR)

mark_as_advanced(LIBMBSIMULATION_INCLUDE_DIR LIBMBSIMULATION_LIBRARY )

if(LIBMBSIMULATION_FOUND)
	message("Found mbsimulation library!")
	message("-- mbsimulation.h: ${LIBMBSIMULATION_INCLUDE_DIR}")
	message("-- libmbsimulation: ${LIBMBSIMULATION_LIBRARY}")
endif(LIBMBSIMULATION_FOUND)

# also add the MBmath libraries as they are required:
#find_package (LibFtdi REQUIRED)
#set(LIBMBSIMULATION_INCLUDE_DIRS ${LIBMBSIMULATION_INCLUDE_DIRS} ${LIBFTDI_INCLU9595
#DE_DIRS} )
#set(LIBMBSIMULATION_LIBRARIES ${LIBMBSIMULATION_LIBRARIES} ${LIBFTDI_LIBRARIES})


#find_package (LibPng REQUIRED)
#set(LIBMBSIMULATION_INCLUDE_DIRS ${LIBMBSIMULATION_INCLUDE_DIRS} ${PNG_INCLUDE_DIR} )
#set(LIBMBSIMULATION_LIBRARIES ${LIBMBSIMULATION_LIBRARIES} ${PNG_LIBRARY})
