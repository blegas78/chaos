#                                               -*- cmake -*-
#
#  FindLibSdl2.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# This module defines
#  SDL2_INCLUDE_DIR, where to find SDL.h, etc.
#  SDL2_FOUND, If false, do not try to use SDL.
# also defined, but not for general use are
#  SDL2_LIBRARY, where to find the SDL library.

FIND_PATH(
  SDL2_INCLUDE_DIR SDL2
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES 
    include
    include/SDL2
  NO_DEFAULT_PATH )

FIND_PATH(
  SDL2_INCLUDE_DIR SDL2
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES 
    include
    include/libSDL2 )

SET(SDL2_NAMES ${SDL2_NAMES} SDL2)


FIND_LIBRARY(
  SDL2_LIBRARY 
  NAMES 
    ${SDL2_NAMES}
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES
    /lib
  NO_DEFAULT_PATHS )

FIND_LIBRARY(
  SDL2_LIBRARY 
  NAMES 
    ${SDL2_NAMES}
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES
    /lib )

SET(SDL2_IMAGE_NAMES ${SDL2_IMAGE_NAMES} SDL2_image)

FIND_LIBRARY(
  SDL2_IMAGE_LIBRARY 
  NAMES 
    ${SDL2_IMAGE_NAMES}
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES
    /lib )

SET(SDL2_MAIN_NAMES ${SDL2_MAIN_NAMES} SDL2main)

FIND_LIBRARY(
  SDL2_MAIN_LIBRARY 
  NAMES 
    ${SDL2_MAIN_NAMES}
  PATHS
    ${SDL2_ROOT}
  PATH_SUFFIXES
    /lib )

set( SDL2_LIBRARY ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MAIN_LIBRARY} )

#SET(SDL2_TTF_NAMES ${SDL2_TTF_NAMES} SDL2_ttf)
#FIND_LIBRARY(
#  SDL2_TTF_LIBRARY 
#  NAMES 
#    ${SDL2_TTF_NAMES}
#  PATHS
#    ${SDL2_ROOT}
#  PATH_SUFFIXES
#    /lib )
#
#set( SDL2_LIBRARY ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY} )

# handle the QUIETLY and REQUIRED arguments and set SDL2_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)

#message(" —— Libraries for SDL2:    ${SDL2_LIBRARY}")
 
if(SDL2_FOUND)
			add_definitions(-DSDL2_FOUND)
endif()
