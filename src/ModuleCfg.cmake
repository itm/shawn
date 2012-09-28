#=============
# Shawn module configuration
#
# Scans for modules and creates an cmake option for each one,
# allowing the individual activation of single modules.
#=============

	set( enablesDefines  )
	set( moduleInitFuncs  )
	set( moduleCfgs  )


#=============
# Add option to individually activate or deactivate modules
#=============
	
	option( CONFIGURE_${appsType} "Configure ${appsType} (off = use default configuration)" OFF )


#=============
# Collect all module.cmake files
#=============

	file( GLOB_RECURSE moduleCfgs ${appsPath}/module.cmake )


#=============
# Loop over the list of found modules
#=============

	if ( moduleCfgs )
		foreach( moduleCfg ${moduleCfgs} )
	
			include( ${moduleCfg} )
			string( TOLOWER ${moduleName} moduleNameLower )
			string( TOUPPER ${moduleName} moduleNameUpper )

			# Only let the user select individual module activation 
			# if the CONFIGURE_(LEGACY)APPS option is ON.

			if ( CONFIGURE_${appsType} )
				option( MODULE_${appsType}_${moduleName} "Activate module ${moduleName}" ${moduleStatus} )
			else ( CONFIGURE_${appsType} )
				set ( MODULE_${appsType}_${moduleName} ${moduleStatus} )
			endif ( CONFIGURE_${appsType} )

			if ( MODULE_${appsType}_${moduleName} )
				set( enablesDefines ${enablesDefines} ENABLE_${moduleName} )
				set( moduleInitFuncs ${moduleInitFuncs} init_${moduleNameLower} )
			endif ( MODULE_${appsType}_${moduleName} )
	
		endforeach( moduleCfg )
	endif ( moduleCfgs )
	

#=============
# Create header file containing all #defines for module activation
#=============

	if ( enablesDefines )
		foreach( enableDefine ${enablesDefines} )
			file( APPEND ${CMAKE_BINARY_DIR}/${outFileEnabled} "#define ${enableDefine}\n" )
		endforeach( enableDefine )
	endif ( enablesDefines )
	
	file( APPEND ${CMAKE_BINARY_DIR}/${outFileInit} "\n" )

	
#=============
# Create header file containing init function macro for all modules
#=============

	if ( moduleInitFuncs )
		foreach( moduleInitFunc ${moduleInitFuncs} )
			file( APPEND ${CMAKE_BINARY_DIR}/${outFileInit} "extern \"C\" void ${moduleInitFunc}( shawn::SimulationController& ); \n" )
		endforeach( moduleInitFunc )
	endif ( moduleInitFuncs )
	
	file( APPEND ${CMAKE_BINARY_DIR}/${outFileInit} "\n#define INIT_STATIC_${appsType}_MODULES \\\n" )
	
	if ( moduleInitFuncs )
		foreach( moduleInitFunc ${moduleInitFuncs} )
			file( APPEND ${CMAKE_BINARY_DIR}/${outFileInit} "${moduleInitFunc}\(sc\); \\\n" )
		endforeach( moduleInitFunc )
	endif ( moduleInitFuncs )
	
	file( APPEND ${CMAKE_BINARY_DIR}/${outFileInit} "\n\n" )


#=============
# Check for a module depending on a library (e.g. Expat, CGAL..),
# that is currently not specified 
#=============

	if ( moduleCfgs )
	
		set ( neededLibsMsg  )

		foreach( moduleCfg ${moduleCfgs} )

			set ( moduleLibs  )
			include( ${moduleCfg} )
			
			if ( MODULE_${appsType}_${moduleName} AND moduleLibs )
				foreach( moduleLib ${moduleLibs} )

					string( TOUPPER ${moduleLib} moduleLibUpper )
					if ( NOT LIB_PATH_${moduleLibUpper} )
						set ( neededLibsMsg "${neededLibsMsg} Module '${moduleName}' needs library '${moduleLib}'\n" )
					endif ( NOT LIB_PATH_${moduleLibUpper} )

				endforeach( moduleLib )
			endif ( MODULE_${appsType}_${moduleName} AND moduleLibs )
			
		endforeach( moduleCfg )

		if ( neededLibsMsg )
			message ( "Warning! The following modules depend on libraries that were not specified:\n\n${neededLibsMsg}\nPlease set the lib and include paths or deactivate the affected modules." )
		endif ( neededLibsMsg )

	endif ( moduleCfgs )
