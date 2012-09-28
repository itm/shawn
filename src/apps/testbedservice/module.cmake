#=============
# Shawn module configuration for cmake build system
#=============

OPTION(OPT_ENABLE_SHAWN_REALTIME "Enable Realtime in Shawn with 1 round = 1 second" OFF)
if ( OPT_ENABLE_SHAWN_REALTIME )
 add_definitions( -DWITH_NONAMESPACES -DMULTITHREADED_EVENT_SCHEDULER )
endif ( OPT_ENABLE_SHAWN_REALTIME )

# Name of this module

   set ( moduleName TESTBEDSERVICE )

# Default status (ON/OFF)

   set ( moduleStatus OFF )

# List of libraries needed by this module, seperated by white space

   set ( moduleLibs  )
