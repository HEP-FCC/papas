#----------------------------------------------------------------------
# Copyright (c) 2010-11, Pierre-Antoine Delsart, Kurtis Geerlings, Joey Huston,
#                 Brian Martin, and Christopher Vermilion
#
#----------------------------------------------------------------------
# This file is part of SpartyJet.
#
#  SpartyJet is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  SpartyJet is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SpartyJet; if not, write to the Free Software
#  Foundation, Inc.:
#      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#----------------------------------------------------------------------
#
# Module to find the fastjet library and headers. It defines
#
#  fastjet_USER_LIB_PATH     : allows the user to specify additional library path
#  fastjet_USER_INCLUDE_PATH : allows the user to specify/override include path
#  fastjet_LIBRARIES         : the libraries to link against
#  fastjet_INCLUDE_DIRS      : where to find fastjet/*.h headers
#  FASTJET_FOUND             : TRUE if everything was found
#
# Since we want to have separate messages for found libraries and headers we
# set the FASTJET_FOUND variable ourselves and use find_package_message directly
# instead of using the find_package_handle_standard_args function
#-------------------------------------------------------------------------------

include (FindPackageMessage)

#set (fastjet_USER_LIB_PATH ${fastjet_USER_LIB_PATH} CACHE PATH "Additional paths to first search for FastJet library.")
#set (fastjet_USER_INCLUDE_PATH ${fastjet_USER_INCLUDE_PATH} CACHE PATH "Additional paths to first search for FastJet headers.")

# Is fastjet-config available?  If so, use it to set FastJet paths
#  These paths can still be overridden by user
execute_process (COMMAND which fastjet-config
   OUTPUT_VARIABLE fastjet_CONFIG
)
if (fastjet_CONFIG)
   execute_process (COMMAND fastjet-config --prefix
      OUTPUT_VARIABLE fastjet_PREFIX
   )
   if (fastjet_PREFIX)
      string (STRIP ${fastjet_PREFIX} fastjet_PREFIX)
      message (STATUS "Found fastjet-config, FastJet location is ${fastjet_PREFIX}")
      set (fastjet_USER_LIB_PATH ${fastjet_USER_LIB_PATH} ${fastjet_PREFIX}/lib)
      set (fastjet_USER_INCLUDE_PATH ${fastjet_USER_INCLUDE_PATH} ${fastjet_PREFIX}/include)
   endif (fastjet_PREFIX)

   execute_process(COMMAND fastjet-config --version
      OUTPUT_VARIABLE fastjet_VERSION
   )
   if (NOT fastjet_VERSION VERSION_GREATER "2.9")
      message (STATUS "SpartyJet now requires FastJet 3.0+")
   endif (NOT fastjet_VERSION VERSION_GREATER "2.9")
else (fastjet_CONFIG)
   set (fastjet_PREFIX ${fastjet_LOCAL})
   set (fastjet_VERSION 3.0)
endif (fastjet_CONFIG)

# include dir
find_path (fastjet_INCLUDE_DIR fastjet/PseudoJet.hh PATHS ${fastjet_USER_INCLUDE_PATH} NO_DEFAULT_PATH)
find_path (fastjet_INCLUDE_DIR fastjet/PseudoJet.hh)

# Start with the assumption that everything was found
set (FASTJET_FOUND TRUE)

set (lib_names fastjet fastjettools)
set (opt_lib_names siscone siscone_spherical fastjetplugins)

# check if we found all libraries
foreach (lib_name ${lib_names})
   unset (lib CACHE)
   find_library (lib ${lib_name} PATHS ${fastjet_USER_LIB_PATH} NO_DEFAULT_PATH)
   find_library (lib ${lib_name})
   if (lib)
      set (fastjet_LIBRARIES ${fastjet_LIBRARIES} ${lib})
      find_package_message (
         fastjet
         "Found FastJet library: ${lib}"
         "[${fastjet_LIBRARIES}]"
      )
   else (lib)
      set (FASTJET_FOUND FALSE)
      if (NOT fastjet_FIND_QUIETLY)
         message (STATUS "Could NOT find FastJetlibrary ${lib_name}.")
      endif (NOT fastjet_FIND_QUIETLY)
   endif (lib)
endforeach(lib_name)

# check which, if any, optional libs were found
foreach (lib_name ${opt_lib_names})
   unset (lib CACHE)
   find_library (lib ${lib_name} PATHS ${fastjet_USER_LIB_PATH} NO_DEFAULT_PATH)
   find_library (lib ${lib_name})
   if (lib)
      set (fastjet_LIBRARIES ${fastjet_LIBRARIES} ${lib})
      find_package_message (
         fastjet
         "Found FastJet library: ${lib}"
         "[${fastjet_LIBRARIES}]"
      )
   else (lib)
      if (NOT fastjet_FIND_QUIETLY)
         message (STATUS "Could NOT find FastJetlibrary ${lib_name}.")
      endif (NOT fastjet_FIND_QUIETLY)
   endif (lib)
endforeach(lib_name)

# check if we found the headers
if (fastjet_INCLUDE_DIR)
   set (fastjet_INCLUDE_DIRS ${fastjet_INCLUDE_DIR})
   find_package_message (
      fastjet
      "Found FASTJET headers in ${fastjet_INCLUDE_DIR}"
      "[${fastjet_LIBRARIES}][${fastjet_INCLUDE_DIRS}]"
   )
else (fastjet_INCLUDE_DIR)
   set (FASTJET_FOUND FALSE)
endif (fastjet_INCLUDE_DIR)

mark_as_advanced(
   fastjet_LIBRARY
   fastjet_INCLUDE_DIR
)

