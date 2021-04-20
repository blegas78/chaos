#                                               -*- cmake -*-
#
#  FindLibSdl.cmake
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
#  SDL_INCLUDE_DIR, where to find SDL.h, etc.
#  SDL_FOUND, If false, do not try to use SDL.
# also defined, but not for general use are
#  SDL_LIBRARY, where to find the SDL library.

FIND_PATH(
  SDL_INCLUDE_DIR SDL
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES 
    include
    include/SDL
  NO_DEFAULT_PATH )

FIND_PATH(
  SDL_INCLUDE_DIR SDL
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES 
    include
    include/libSDL )

SET(SDL_NAMES ${SDL_NAMES} SDL)


FIND_LIBRARY(
  SDL_LIBRARY 
  NAMES 
    ${SDL_NAMES}
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES
    /lib
  NO_DEFAULT_PATHS )

FIND_LIBRARY(
  SDL_LIBRARY 
  NAMES 
    ${SDL_NAMES}
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES
    /lib )

SET(SDL_IMAGE_NAMES ${SDL_IMAGE_NAMES} SDL_image)

FIND_LIBRARY(
  SDL_IMAGE_LIBRARY 
  NAMES 
    ${SDL_IMAGE_NAMES}
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES
    /lib )

SET(SDL_MAIN_NAMES ${SDL_MAIN_NAMES} SDLmain)

FIND_LIBRARY(
  SDL_MAIN_LIBRARY 
  NAMES 
    ${SDL_MAIN_NAMES}
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES
    /lib )

set( SDL_LIBRARY ${SDL_LIBRARY} ${SDL_IMAGE_LIBRARY} ${SDL_MAIN_LIBRARY} )

SET(SDL_TTF_NAMES ${SDL_TTF_NAMES} SDL_ttf)
FIND_LIBRARY(
  SDL_TTF_LIBRARY 
  NAMES 
    ${SDL_TTF_NAMES}
  PATHS
    ${SDL_ROOT}
  PATH_SUFFIXES
    /lib )

set( SDL_LIBRARY ${SDL_LIBRARY} ${SDL_TTF_LIBRARY} )

# handle the QUIETLY and REQUIRED arguments and set SDL_FOUND to TRUE if 
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL DEFAULT_MSG SDL_LIBRARY SDL_INCLUDE_DIR)

message(" —— Libraries for SDL:    ${SDL_LIBRARY}") 
