#=============
# Shawn module configuration for cmake build system
#=============

set ( INCLUDE_PATH_WISELIB_STABLE CACHE PATH "Path to Wiselib Stable" )
set ( INCLUDE_PATH_WISELIB_TESTING CACHE PATH "Path to Wiselib Testing" )
include_directories ( ${INCLUDE_PATH_WISELIB_TESTING} )
include_directories ( ${INCLUDE_PATH_WISELIB_STABLE} )


# Name of this module

   set ( moduleName WISELIB )

# Default status (ON/OFF)

   set ( moduleStatus OFF )

# List of libraries needed by this module, seperated by white space

   set ( moduleLibs  )
