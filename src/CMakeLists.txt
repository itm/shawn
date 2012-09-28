#==================================
#
# SHAWN build system
#
# Main input file for CMake
# 
#==================================

#=============
# Configure CMake
#=============

cmake_minimum_required(VERSION 2.4)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#=============
# Set the project's name
#
# This makes the following cmake vars available:
#   ${SHAWN_SOURCE_DIR}
#   ${SHAWN_BINARY_DIR}
#
#=============

	project( SHAWN )

#=============
# Options
#=============

	OPTION(BUILD_SINGLE_LIB "Disable to build apps into their own libs" ON)
	OPTION(OPT_ENABLE_CGAL "Enable CGAL library support" OFF)
	OPTION(OPT_ENABLE_CAIRO "Enable Cairo library support" OFF)
	OPTION(OPT_ENABLE_BOOST "Enable boost library support" OFF)
	OPTION(OPT_ENABLE_GLUT "Enable GLUT library used by vis Liveview" OFF)
	OPTION(OPT_ENABLE_PROTOBUF "Enable Protobuf library support" OFF)
	OPTION(OPT_ENABLE_GIT_SUPPORT "Enable support for git version control" OFF)

#=============
# Set the default paths for binary and lib build
#=============

	get_filename_component( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../buildfiles ABSOLUTE CACHE )
	get_filename_component( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../buildfiles ABSOLUTE CACHE )

#=============
# Set the default build type to "Release"
#=============

	IF(NOT CMAKE_BUILD_TYPE)
		SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
	ENDIF(NOT CMAKE_BUILD_TYPE)

	
#=============
# Collect all source files recursively to be compiled
#=============

	file( GLOB_RECURSE shawnAutocastCPP apps/autocast/*.cpp )
	file( GLOB_RECURSE shawnAutocastH apps/autocast/*.h )

	file( GLOB_RECURSE shawnDistestCPP apps/distest/*.cpp )
	file( GLOB_RECURSE shawnDistestH apps/distest/*.h )

	file( GLOB_RECURSE shawnExamplesCPP apps/examples/*.cpp )
	file( GLOB_RECURSE shawnExamplesH apps/examples/*.h )

	file( GLOB_RECURSE shawnIsenseCPP apps/isense/*.cpp )
	file( GLOB_RECURSE shawnIsenseH apps/isense/*.h )

	file( GLOB_RECURSE shawnLocalizationCPP apps/localization/*.cpp )
	file( GLOB_RECURSE shawnLocalizationH apps/localization/*.h )

	file( GLOB_RECURSE shawnMotionEventTaskCPP apps/motion_event_task/*.cpp )
	file( GLOB_RECURSE shawnMotionEventTaskH apps/motion_event_task/*.h )

	file( GLOB_RECURSE shawnReadingCPP apps/reading/*.cpp )
	file( GLOB_RECURSE shawnReadingH apps/reading/*.h )

	file( GLOB_RECURSE shawnRoutingCPP apps/routing/*.cpp )
	file( GLOB_RECURSE shawnRoutingH apps/routing/*.h )

	file( GLOB_RECURSE shawnSpyglassCPP apps/spyglass/*.cpp )
	file( GLOB_RECURSE shawnSpyglassH apps/spyglass/*.h )

	file( GLOB_RECURSE shawnSumoCPP apps/sumo/*.cpp )
	file( GLOB_RECURSE shawnSumoH apps/sumo/*.h )

	file( GLOB_RECURSE shawnTestbedserviceCPP apps/testbedservice/*.cpp )
	file( GLOB_RECURSE shawnTestbedserviceH apps/testbedservice/*.h )

	file( GLOB_RECURSE shawnTcpipCPP apps/tcpip/*.cpp )
	file( GLOB_RECURSE shawnTcpipgH apps/tcpip/*.h )

	file( GLOB_RECURSE shawnTopologyCPP apps/topology/*.cpp )
	file( GLOB_RECURSE shawnTopologyH apps/topology/*.h )

	file( GLOB_RECURSE shawnTraciClientCPP apps/traci_client/*.cpp )
	file( GLOB_RECURSE shawnTraciClientH apps/traci_client/*.h )

	file( GLOB_RECURSE shawnVanetCPP apps/vanet/*.cpp )
	file( GLOB_RECURSE shawnVanetH apps/vanet/*.h )

	file( GLOB_RECURSE shawnVisCPP apps/vis/*.cpp )
	file( GLOB_RECURSE shawnVisH apps/vis/*.h )

	file( GLOB_RECURSE shawnWiselibCPP apps/wiselib/*.cpp )
	file( GLOB_RECURSE shawnWiselibH apps/wiselib/*.h )

	file( GLOB_RECURSE shawnWiseMlCPP apps/wiseml/*.cpp )
	file( GLOB_RECURSE shawnWiseMlH apps/wiseml/*.h )

	file( GLOB_RECURSE shawnExternalAppCPP apps/external_application/*.cpp )
	file( GLOB_RECURSE shawnExternalAppH apps/external_application/*.h )

	file( GLOB_RECURSE shawnSourcesFrontendCPP frontend/*.cpp )
	file( GLOB_RECURSE shawnSourcesFrontendH frontend/*.h )
	
	file( GLOB_RECURSE shawnSourcesSysCPP sys/*.cpp sys/*.c )
	file( GLOB_RECURSE shawnSourcesSysH sys/*.h )
	
	#file( GLOB_RECURSE shawnSourcesConfig shawn_config.h )

	#
	# Collect LEGACYAPPS sources if present
	#
	
	set( LEGACYAPPS_PATH "" CACHE PATH "Path to legacy apps. Leave blank to not use legacy apps." )
	if ( LEGACYAPPS_PATH )
		file( GLOB_RECURSE legacyappsSourcesCPP ${LEGACYAPPS_PATH}/*.cpp )
		file( GLOB_RECURSE legacyappsSourcesH ${LEGACYAPPS_PATH}/*.h )
	endif ( LEGACYAPPS_PATH )

	#
	# Let the user choosse whether to build the shawn-with-main lib
	#

	OPTION(BUILD_LIB_WITH_MAIN "Additionally build a library containing main()" OFF)

#=============
# Add collected source files to common source variables
#=============

	# shawnSources - Sources for executable build
		set( shawnSources ${shawnSources} ${shawnSourcesFrontendCPP} )
		set( shawnSources ${shawnSources} ${shawnSourcesFrontendH} )
		set( shawnSources ${shawnSources} shawn_config.h )
		set( shawnSources ${shawnSources} apps/apps_init.h )
		set( shawnSources ${shawnSources} apps/apps_init.cpp )
 		set( shawnSources ${shawnSources} revision.h )

	# shawnSourcesLib - Sources for library build
		set( shawnSourcesLib ${shawnSourcesLib} ${shawnSourcesSysCPP} )
		set( shawnSourcesLib ${shawnSourcesLib} ${shawnSourcesSysH} )
		set( shawnSourcesLib ${shawnSourcesLib} shawn_config.h )

	# LegacyApps
		set( shawnLegacyLib ${shawnLegacyLib} ${legacyappsSourcesCPP} )
		set( shawnLegacyLib ${shawnLegacyLib} ${legacyappsSourcesH} )
	# shawnAutocast - Source for autocast library build
		set( shawnAutocast ${shawnAutocast} ${shawnAutocastCPP} )
		set( shawnAutocast ${shawnAutocast} ${shawnAutocastH} )

	# shawnDistest - Source for Examples library build
		set( shawnDistest ${shawnDistest} ${shawnDistestCPP} )
		set( shawnDistest ${shawnDistest} ${shawnDistestH} )

	# shawnExamples - Source for Examples library build
		set( shawnExamples ${shawnExamples} ${shawnExamplesCPP} )
		set( shawnExamples ${shawnExamples} ${shawnExamplesH} )

	# shawnIsense - Source for Examples library build
		set( shawnIsense ${shawnIsense} ${shawnIsenseCPP} )
		set( shawnIsense ${shawnIsense} ${shawnIsenseH} )

	# shawnLocalization - Source for Examples library build
		set( shawnLocalization ${shawnLocalization} ${shawnLocalizationCPP} )
		set( shawnLocalization ${shawnLocalization} ${shawnLocalizationH} )

	# shawnMotionEventTask - Source for Examples library build
		set( shawnMotionEventTask ${shawnMotionEventTask} ${shawnMotionEventTaskCPP} )
		set( shawnMotionEventTask ${shawnMotionEventTask} ${shawnMotionEventTaskH} )

	# shawnReading - Source for Reading library build
		set( shawnReading ${shawnReading} ${shawnReadingCPP} )
		set( shawnReading ${shawnReading} ${shawnReadingH} )

	# shawnRouting - Source for Routing library build
		set( shawnRouting ${shawnRouting} ${shawnRoutingCPP} )
		set( shawnRouting ${shawnRouting} ${shawnRoutingH} )

	# shawnSpyglass - Source for Spyglass library build
		set( shawnSpyglass ${shawnSpyglass} ${shawnSpyglassCPP} )
		set( shawnSpyglass ${shawnSpyglass} ${shawnSpyglassH} )

	# shawnSumo - Source for Sumo library build
		set( shawnSumo ${shawnSumo} ${shawnSumoCPP} )
		set( shawnSumo ${shawnSumo} ${shawnSumoH} )

	# shawnTcpip - Source for Testbedservice library build
		set( shawnTestbedservice ${shawnTestbedservice} ${shawnTestbedserviceCPP} )
		set( shawnTestbedservice ${shawnTestbedservice} ${shawnTestbedserviceH} )

	# shawnTcpip - Source for Tcpip library build
		set( shawnTcpip ${shawnTcpip} ${shawnTcpipCPP} )
		set( shawnTcpip ${shawnTcpip} ${shawnTcpipH} )

	# shawnTopology - Source for Topology library build
		set( shawnTopology ${shawnTopology} ${shawnTopologyCPP} )
		set( shawnTopology ${shawnTopology} ${shawnTopologyH} )

	# shawnTraciClient - Source for TraciClient library build
		set( shawnTraciClient ${shawnTraciClient} ${shawnTraciClientCPP} )
		set( shawnTraciClient ${shawnTraciClient} ${shawnTraciClientH} )

	# shawnVanet - Source for Vanet library build
		set( shawnVanet ${shawnVanet} ${shawnVanetCPP} )
		set( shawnVanet ${shawnVanet} ${shawnVanetH} )

	# shawnVis - Source for Vis library build
		set( shawnVis ${shawnVis} ${shawnVisCPP} )
		set( shawnVis ${shawnVis} ${shawnVisH} )

	# shawnWiselib - Source for Wiselib library build
		set( shawnWiselib ${shawnWiselib} ${shawnWiselibCPP} )
		set( shawnWiselib ${shawnWiselib} ${shawnWiselibH} )

	# shawnWiseMl - Source for WiseMl library build
		set( shawnWiseMl ${shawnWiseMl} ${shawnWiseMlCPP} )
		set( shawnWiseMl ${shawnWiseMl} ${shawnWiseMlH} )

	# shawnExternalApp - Source for ExternalApplication library build
		set( shawnExternalApp ${shawnExternalApp} ${shawnExternalAppCPP} )
		set( shawnExternalApp ${shawnExternalApp} ${shawnExternalAppH} )
		

#=============
# Add include and library directories for the compiler/linker
#=============

	include_directories (
		./
		${CMAKE_BINARY_DIR}/../buildfiles
	)

	if ( OPT_ENABLE_CGAL )
		set( DEFAULT_CGAL_ROOT )
		set( LIB_PATH_CGAL CACHE PATH "Path to CGAL library" )
		set( INCLUDE_PATH_CGAL CACHE PATH "Path to CGAL includes" )
	        set( INCLUDE_PATH_CGAL_COMPILER_CONFIG CACHE PATH "Path to CGAL compiler config" )
		link_directories( ${LIB_PATH_CGAL} )
		include_directories ( 
			${INCLUDE_PATH_CGAL}
			${INCLUDE_PATH_CGAL_COMPILER_CONFIG}
		)
	endif ( OPT_ENABLE_CGAL )

	if( OPT_ENABLE_BOOST )
		set ( LIB_PATH_BOOST   CACHE PATH "Boost lib path" )
		set ( INCLUDE_PATH_BOOST   CACHE PATH "Boost include path" )
		link_directories( ${LIB_PATH_BOOST} )
		include_directories ( ${INCLUDE_PATH_BOOST} )
	endif( OPT_ENABLE_BOOST )

	if ( OPT_ENABLE_CAIRO )
		set ( DEFAULT_CAIRO_ROOT )
		set ( LIB_PATH_CAIRO   CACHE PATH "Path to CAIRO library" )
		set ( INCLUDE_PATH_CAIRO   CACHE PATH "Path to CAIRO includes" )
		link_directories( ${LIB_PATH_CAIRO} )
		include_directories ( ${INCLUDE_PATH_CAIRO} )
	endif ( OPT_ENABLE_CAIRO )	

	if ( OPT_ENABLE_GLUT )
		set ( LIB_PATH_GLUT   CACHE PATH "Path to GLUT library" )
		set ( INCLUDE_PATH_GLUT   CACHE PATH "Path to GLUT includes" )
		link_directories( ${LIB_PATH_GLUT} )
		include_directories ( ${INCLUDE_PATH_GLUT} )
	endif ( OPT_ENABLE_GLUT )

	if ( OPT_ENABLE_PROTOBUF )
		set ( INCLUDE_PATH_GOOGLE_PROTOBUF CACHE PATH "Path to Protobuf 2.3.0 includes" )
		set ( LIB_PATH_GOOGLE_PROTOBUF CACHE PATH "Path to Protobuf 2.3.0 libs" )
		include_directories ( ${INCLUDE_PATH_GOOGLE_PROTOBUF} )
		link_directories( ${LIB_PATH_GOOGLE_PROTOBUF} )
	endif ( OPT_ENABLE_PROTOBUF )


#=============
# Configure logging libraries
#=============

	set ( LOGGER_TYPE "STDOUT" CACHE STRING "Logger type to use. Choose STDOUT (default), LOG4CXX or NONE." )
	string( TOUPPER ${LOGGER_TYPE} LOGGER_TYPE_UPPERCASE )

	if ( LOGGER_TYPE_UPPERCASE STREQUAL "STDOUT" )
		add_definitions( -DLOGGER_STDOUT )
		
		# DEBUG and INFO configuration only for STDOUT. Otherwise
		# the logging library configuration should be used.
		set( LOG_DEBUG "AUTO" CACHE STRING "Output debug messages. Choose ON, OFF or AUTO." )
		string( TOUPPER ${LOG_DEBUG} LOG_DEBUG_UPPERCASE ) 
		if( LOG_DEBUG_UPPERCASE STREQUAL "OFF" )
			add_definitions( -DLOG_DEBUG_DISABLED )
			
		elseif ( LOG_DEBUG_UPPERCASE STREQUAL "AUTO" )
			if ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
				add_definitions( -DLOG_DEBUG_DISABLED )
			endif ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" )
		endif ( LOG_DEBUG_UPPERCASE STREQUAL "OFF" )

		set( LOG_INFO "AUTO" CACHE STRING "Output info messages. Choose ON, OFF or AUTO." )
		string( TOUPPER ${LOG_INFO} LOG_INFO_UPPERCASE )
		if( LOG_INFO_UPPERCASE STREQUAL "OFF" )
			add_definitions( -DLOG_INFO_DISABLED )
			
		elseif ( LOG_INFO_UPPERCASE STREQUAL "AUTO" )
			if ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
				add_definitions( -DLOG_INFO_DISABLED )
			endif ( NOT CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
		endif ( LOG_INFO_UPPERCASE STREQUAL "OFF" )

	elseif ( LOGGER_TYPE_UPPERCASE STREQUAL "LOG4CXX" )
		unset( LOG_DEBUG CACHE )
		unset( LOG_INFO CACHE )

		add_definitions( -DLOG4CXX_STATIC -DLOGGER_LOG4CXX )
		set ( INCLUDE_PATH_LOG4CXX CACHE PATH "Path to LOG4CXX includes" )
		set ( LIB_PATH_LOG4CXX CACHE PATH "Path to LOG4CXX libs" )
		link_directories ( ${LIB_PATH_LOG4CXX} )
		include_directories ( ${INCLUDE_PATH_LOG4CXX} )
		
	elseif ( LOGGER_TYPE_UPPERCASE STREQUAL "NONE")
		unset( LOG_DEBUG CACHE )
		unset( LOG_INFO CACHE )
		
		add_definitions( -DLOGGER_NONE )
	   
	else ( LOGGER_TYPE_UPPERCASE STREQUAL "STDOUT" )
		message( FATAL_ERROR "Unsupportet logging library" )
	   
	endif ( LOGGER_TYPE_UPPERCASE STREQUAL "STDOUT" )


#=============
# Compiler definitions
#=============

##############################################################################
#   add_definitions( -DSHAWN_EV_SCHED_ENABLE_RATE_ADAPTATION -DWITH_NONAMESPACES -DMULTITHREADED_EVENT_SCHEDULER )
##############################################################################

	if( OPT_ENABLE_CGAL )
		add_definitions( -DHAVE_CGAL )
	endif( OPT_ENABLE_CGAL )

	if( OPT_ENABLE_BOOST )
		add_definitions(-DHAVE_BOOST -DHAVE_BOOST_REGEX )
	endif( OPT_ENABLE_BOOST )

	if( OPT_ENABLE_CAIRO )
		add_definitions( -DHAVE_CAIRO )
	endif ( OPT_ENABLE_CAIRO )

	if( OPT_ENABLE_GLUT )
		add_definitions( -DHAVE_GLUT )
	endif ( OPT_ENABLE_GLUT )

	if ( OPT_ENABLE_PROTOBUF )
		add_definitions( -DHAVE_PROTOBUF )
	endif ( OPT_ENABLE_PROTOBUF )

	if( LEGACYAPPS_PATH )
		add_definitions( -DHAVE_LEGACYAPPS )
	endif ( LEGACYAPPS_PATH )

	# Add SHAWN flag, to distinguish code needed by Shawn from code
	# that should be excluded, when an App is used by another program

	add_definitions( -DSHAWN )

	# Add flags for expat
	
	add_definitions( -DHAVE_EXPAT_CONFIG_H )
	add_definitions( -DXML_STATIC )


#=============
# Configure shawn apps modules
#=============

	# create new module config header files
	file( WRITE ${CMAKE_BINARY_DIR}/_apps_init_cmake.h "" )
	file( WRITE ${CMAKE_BINARY_DIR}/_apps_enable_cmake.h "#include \"shawn_config.h\"\n" )

	set( appsType APPS )
	set( appsPath apps )
	set( outFileEnabled ../buildfiles/_apps_enable_cmake.h )
	set( outFileInit ../buildfiles/_apps_init_cmake.h )

	include( ModuleCfg.cmake )


#=============
# Configure shawn legacyapps modules
#=============
	
	if ( LEGACYAPPS_PATH )

		# create new module config header files
		file( WRITE ${CMAKE_BINARY_DIR}/_legacyapps_init_cmake.h "" )
		file( WRITE ${CMAKE_BINARY_DIR}/_legacyapps_enable_cmake.h "#include \"shawn_config.h\"\n" )

		set( appsType LEGACYAPPS )
		set( appsPath ${LEGACYAPPS_PATH} )
		set( outFileEnabled _legacyapps_enable_cmake.h )
		set( outFileInit _legacyapps_init_cmake.h )

		include( ModuleCfg.cmake )
		
		#=============
		# When the legacyapps are used, we need to give the compiler
		# an additional include directory, because legacyapps sources
		# are outside of the build tree. This way, the old include
		# directives like
		#
		#   #include "legacyapps/marathon_net/marathon_world_factory.h"
		#
		# can still be used.
		#
		# Important: legacyapps sources must not be in the root
		#            directory, at least in one subdir like
		#            "/legacyapps/*.*", not "/*.*"!
		#=============
		
		include_directories( ${LEGACYAPPS_PATH}/.. )
		
	endif ( LEGACYAPPS_PATH )


#=============
# Option for creating a new module
#=============

	include( NewModule.cmake )


#=============
# Build target
#=============

	IF(BUILD_SINGLE_LIB)
		IF(BUILD_LIB_WITH_MAIN)
			add_library( shawnlib ${shawnSources} ${shawnSourcesLib} 
				${shawnAutocast} ${shawnDistest} ${shawnExamples} ${shawnIsense}
				${shawnLocalization} ${shawnMotionEventTask} ${shawnReading} 
				${shawnRouting} ${shawnSpyglass} ${shawnTestbedservice} 
				${shawnSumo} ${shawnTcpip} ${shawnTopology} ${shawnTraciClient} 
				${shawnVanet} ${shawnVis} ${shawnWiselib} ${shawnWiseMl}
				${shawnExternalApp} ${shawnLegacyLib} )
		ELSE(BUILD_LIB_WITH_MAIN)
			add_library( shawnlib ${shawnSources} ${shawnSourcesLib} 
				${shawnAutocast} ${shawnDistest} ${shawnExamples} ${shawnIsense}
				${shawnLocalization} ${shawnMotionEventTask} ${shawnReading} 
				${shawnRouting} ${shawnSpyglass} ${shawnTestbedservice} 
				${shawnSumo} ${shawnTcpip} ${shawnTopology} ${shawnTraciClient} 
				${shawnVanet} ${shawnVis} ${shawnWiselib} ${shawnWiseMl}
				${shawnExternalApp} ${shawnLegacyLib} )
			add_executable( shawn ${shawnSources} )
		ENDIF(BUILD_LIB_WITH_MAIN)
	ELSE(BUILD_SINGLE_LIB)
		IF(BUILD_LIB_WITH_MAIN)
			add_library( shawnlib ${shawnSources} ${shawnSourcesLib} )
		ELSE(BUILD_LIB_WITH_MAIN)
			add_library( shawnlib ${shawnSourcesLib} )
			add_executable( shawn ${shawnSources} )
		ENDIF(BUILD_LIB_WITH_MAIN)
	
		if( LEGACYAPPS_PATH )	
			add_library( legacyapps ${shawnLegacyLib} )
		endif( LEGACYAPPS_PATH )
		add_library( apps_autocastlib ${shawnAutocast} )
		add_library( apps_distestlib ${shawnDistest} )
		add_library( apps_exampleslib ${shawnExamples} )
		add_library( apps_isenselib ${shawnIsense} )
		add_library( apps_localizationlib ${shawnLocalization} )
		add_library( apps_motioneventtasklib ${shawnMotionEventTask} )
		add_library( apps_readinglib ${shawnReading} )
		add_library( apps_routinglib ${shawnRouting} )
		add_library( apps_spyglasslib ${shawnSpyglass} )
		add_library( apps_sumolib ${shawnSumo} )
		add_library( apps_testbedservicelib ${shawnTestbedservice} )
		add_library( apps_tcpiplib ${shawnTcpip} )
		add_library( apps_topologylib ${shawnTopology} )
		add_library( apps_traciclientlib ${shawnTraciClient} )
		add_library( apps_vanetlib ${shawnVanet} )
		add_library( apps_vislib ${shawnVis} )
		add_library( apps_wisemllib ${shawnWiseMl} )
		add_library( apps_wiseliblib ${shawnWiselib} )
		add_library( apps_externalapplib ${shawnExternalApp} )
	ENDIF(BUILD_SINGLE_LIB)

#=============
# Link libraries to the shawn target
#=============

	if ( WIN32 AND NOT CYGWIN  )
		set ( SYSTEM_LIBS ws2_32.lib )
		add_definitions( -DNOMINMAX )
		add_definitions( -DNOGDI )

		if ( OPT_ENABLE_CGAL )
			set ( LIB_CGAL CGAL.lib )
	  	endif ( OPT_ENABLE_CGAL )

		if ( LIB_PATH_LOG4CXX )
			set ( LIB_LOGGING log4Cxx.lib apr.lib aprutil.lib xml.lib odbc32.lib mswsock.lib)
		endif ( LIB_PATH_LOG4CXX )

		if ( OPT_ENABLE_CAIRO )
			set (LIB_CAIRO cairo.lib)
		endif ( OPT_ENABLE_CAIRO )

		if ( OPT_ENABLE_GLUT )
			set ( LIB_GLUT OpenGL32.Lib glut32.lib )
		endif ( OPT_ENABLE_GLUT )

		if ( OPT_ENABLE_BOOST )
			set ( LIB_BOOST )
		endif ( OPT_ENABLE_BOOST )

	else ( WIN32 AND NOT CYGWIN  )
		set ( SYSTEM_LIBS dl )

		if ( MODULE_APPS_TESTBEDSERVICE )
		  set ( LIB_TESTBEDSERVICE pthread )
		endif ( MODULE_APPS_TESTBEDSERVICE )

		if ( OPT_ENABLE_CGAL )
			set ( LIB_CGAL CGAL)
		endif ( OPT_ENABLE_CGAL )

		if ( LIB_PATH_LOG4CXX )
			set ( LIB_LOGGING log4cxx )
		endif ( LIB_PATH_LOG4CXX )

		if ( OPT_ENABLE_CAIRO )
			if ( CYGWIN )
				set ( LIB_CAIRO cairo png )
			else ( CYGWIN )
				set ( LIB_CAIRO cairo pixman-1 png freetype fontconfig z )
			endif ( CYGWIN )
		endif ( OPT_ENABLE_CAIRO )

		if ( OPT_ENABLE_BOOST )
			if ( CYGWIN )
				set ( LIB_BOOST boost_regex-gcc-mt boost_date_time-gcc-mt boost_thread-gcc-mt )
			else ( CYGWIN )
				set ( LIB_BOOST boost_regex-mt boost_date_time-mt boost_thread-mt boost_system-mt )
			endif ( CYGWIN )
		endif ( OPT_ENABLE_BOOST )

		if ( OPT_ENABLE_GLUT AND APPLE)
			set ( LIB_GLUT GL glu glut )
                        SET_TARGET_PROPERTIES( shawn PROPERTIES LINK_FLAGS "-dylib_file /System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib" )
		endif ( OPT_ENABLE_GLUT AND APPLE )

		if ( OPT_ENABLE_GLUT AND NOT APPLE)
			set ( LIB_GLUT GL glut GLU )
		endif ( OPT_ENABLE_GLUT AND NOT APPLE )
		
	if ( OPT_ENABLE_PROTOBUF )
		set ( LIB_PROTOBUF protobuf )
	endif ( OPT_ENABLE_PROTOBUF )

	endif ( WIN32 AND NOT CYGWIN  )

	IF ( LEGACYAPPS_PATH )
		set ( LIB_LEGACYAPPS legacyapps )
	ELSEIF ( LEGACYAPPS_PATH )
		set ( LIB_LEGACYAPPS )
	ENDIF ( LEGACYAPPS_PATH )

	IF(BUILD_SINGLE_LIB)
	  IF(BUILD_LIB_WITH_MAIN)
		target_link_libraries( shawnlib 
			${SYSTEM_LIBS} ${LIB_TESTBEDSERVICE} ${LIB_CGAL} ${LIB_LOGGING} 
			${LIB_CAIRO} ${LIB_BOOST} ${LIB_GLUT} ${SHAWN_LINK_LIBS})
	  ELSE(BUILD_LIB_WITH_MAIN)
	    target_link_libraries( shawn shawnlib 
			${SYSTEM_LIBS} ${LIB_TESTBEDSERVICE} ${LIB_CGAL} ${LIB_LOGGING} 
			${LIB_CAIRO} ${LIB_BOOST} ${LIB_GLUT} ${SHAWN_LINK_LIBS})
	  ENDIF(BUILD_LIB_WITH_MAIN)
	ELSE(BUILD_SINGLE_LIB)
	  IF(BUILD_LIB_WITH_MAIN)
		target_link_libraries( ${LIB_LEGACYAPPS} apps_externalapplib apps_exampleslib apps_autocastlib apps_distestlib 
			apps_isenselib apps_localizationlib apps_motioneventtasklib apps_topologylib apps_readinglib apps_routinglib
			apps_spyglasslib apps_testbedservicelib apps_tcpiplib apps_vanetlib apps_vislib apps_wiseliblib apps_wisemllib 
			shawnlib apps_sumolib apps_traciclientlib
			${SYSTEM_LIBS} ${LIB_TESTBEDSERVICE} ${LIB_CGAL} ${LIB_LOGGING} 
			${LIB_CAIRO} ${LIB_BOOST} ${LIB_GLUT} ${LIB_PROTOBUF} ${SHAWN_LINK_LIBS} )
	  ELSE(BUILD_LIB_WITH_MAIN)
	    target_link_libraries( shawn ${LIB_LEGACYAPPS} apps_externalapplib apps_exampleslib apps_autocastlib apps_distestlib 
			apps_isenselib apps_localizationlib apps_motioneventtasklib apps_topologylib apps_readinglib apps_routinglib
			apps_spyglasslib apps_testbedservicelib apps_tcpiplib apps_vanetlib apps_vislib apps_wiseliblib apps_wisemllib 
			shawnlib apps_sumolib apps_traciclientlib
			${SYSTEM_LIBS} ${LIB_TESTBEDSERVICE} ${LIB_CGAL} ${LIB_LOGGING} 
			${LIB_CAIRO} ${LIB_BOOST} ${LIB_GLUT} ${LIB_PROTOBUF} ${SHAWN_LINK_LIBS} )
	  ENDIF(BUILD_LIB_WITH_MAIN)
	ENDIF(BUILD_SINGLE_LIB)

#=============
# Git revision information
#=============

	if( OPT_ENABLE_GIT_SUPPORT )
		include(FindPythonInterp)
		if ( PYTHONINTERP_FOUND )
			add_custom_command( OUTPUT revision.h COMMAND ${PYTHON_EXECUTABLE} ARGS ${CMAKE_SOURCE_DIR}/generateRevisionHeader.py --reset ${CMAKE_SOURCE_DIR}/revision.h revision git COMMENT "Generating git revision header" VERBATIM )
		endif ( PYTHONINTERP_FOUND )
	endif( OPT_ENABLE_GIT_SUPPORT )
