#                                               -*- cmake -*-
#
#  FindLibCryptopp.cmake
#
#  Copyright (C) 2016 Mogi LLC
#
#  This file is part of LibMogi.
#
#  LibMogi is free software; you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License
#  version 2.1 as published by the Free Software Foundation;
#

# - Try to find LibCryptopp
# Once done this will define
#  LIBCRYPTOPP_FOUND - System has Libcryptopp
#  LIBCRYPTOPP_INCLUDE_DIRS - The Libcryptopp include directories
#  LIBCRYPTOPP_LIBRARIES - The libraries needed to use Libcryptopp

find_package(PkgConfig)
pkg_check_modules(PC_LIBCRYPTOPP libcryptopp QUIET)

find_path(LIBCRYPTOPP_INCLUDE_DIR cryptopp
          HINTS /usr/local/include/ /opt/local/include/ ${PC_LIBCRYPTOPP_INCLUDEDIR} ${PC_LIBCRYPTOPP_INCLUDE_DIRS}
          PATH_SUFFIXES cryptopp)

find_library(LIBCRYPTOPP_LIBRARY NAMES cryptopp libcryptopp
             HINTS /usr/local/lib/ ${PC_LIBCRYPTOPP_LIBDIR} ${PC_LIBCRYPTOPP_LIBRARY_DIRS} )

set(LIBCRYPTOPP_LIBRARIES ${LIBCRYPTOPP_LIBRARY} )
set(LIBCRYPTOPP_INCLUDE_DIRS ${LIBCRYPTOPP_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibCryptopp  DEFAULT_MSG
                                  LIBCRYPTOPP_LIBRARY LIBCRYPTOPP_INCLUDE_DIR)

mark_as_advanced(LIBCRYPTOPP_INCLUDE_DIR LIBCRYPTOPP_LIBRARY )
