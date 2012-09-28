#=============
# Shawn - CMake code for creating a new module
#=============


#=============
# Provide new CMake options for creating a new module
#=============

	set( CREATE_NEW_MODULE OFF CACHE BOOL "Set to ON to create a new module" )
	if ( CREATE_NEW_MODULE )
		set( NEW_MODULE_NAME   CACHE STRING "Name for the new module" )
		set( NEW_MODULE_LIBDEP   CACHE STRING "Libs needed by this module, white space seperated" )
	endif ( CREATE_NEW_MODULE )


	# If the name for the new module is given, generate the uppercase,
	# lowercase and capitalized versions

	if ( NEW_MODULE_NAME )

		string( TOUPPER ${NEW_MODULE_NAME} NEW_MODULE_NAME_UPPER )
		string( TOLOWER ${NEW_MODULE_NAME} NEW_MODULE_NAME_LOWER )

		# Generate capitalized version of the module name

		string( LENGTH ${NEW_MODULE_NAME} tmp_length )
		MATH( EXPR tmp_length "${tmp_length} - 1" )
		string( SUBSTRING ${NEW_MODULE_NAME_LOWER} 0 1 tmp_head )
		string( TOUPPER ${tmp_head} tmp_head )
		string( SUBSTRING ${NEW_MODULE_NAME_LOWER} 1 ${tmp_length} tmp_trail )

		set ( NEW_MODULE_NAME_CAPITALIZED ${tmp_head}${tmp_trail} )

	endif ( NEW_MODULE_NAME )


#=============
# Create the new module
#=============

	#
	# Macro to replace a string in a file
	#

	macro( ReplaceInFile filename regex replace )
		file( READ ${filename} tmp_filecontent )
			string( REGEX REPLACE ${regex} ${replace} tmp_filecontent "${tmp_filecontent}" )
		file( WRITE ${filename} "${tmp_filecontent}" )
	endmacro( ReplaceInFile )
	
	#
	# Macro that copies a file
	#

	macro( CopyFile filenameFrom filenameTo )
		if ( NOT filenameFrom STREQUAL filenameTo )
			file( READ ${filenameFrom} tmp_filecontent )
			file( WRITE ${filenameTo} "${tmp_filecontent}" )
		endif ( NOT filenameFrom STREQUAL filenameTo )
	endmacro( CopyFile )


	# An internal cache var "moduleCreated_NEW_MODULE_NAME_UPPER" is 
	# been created for each new module, so the module will not be re-
	# created on a new configure-run.
	
	if ( CREATE_NEW_MODULE AND NEW_MODULE_NAME AND NOT moduleCreated_${NEW_MODULE_NAME_UPPER} )
	
		set ( templateModuleName helloworld )
		string( TOUPPER ${templateModuleName} templateModuleName_upper )
		string( TOLOWER ${templateModuleName} templateModuleName_lower )
		set ( templateModuleFullPath ${SHAWN_SOURCE_DIR}/apps/${templateModuleName_lower} )
		set ( newModuleFullPath ${SHAWN_SOURCE_DIR}/apps/${NEW_MODULE_NAME_LOWER} )
	
		# Check if module already exist
		if ( NOT EXISTS ${newModuleFullPath} )
	
			#
			# Create new module directory
			#
			
			file( MAKE_DIRECTORY ${newModuleFullPath} )
	
			#
			# Copy and edit module.cmake
			#
			
			CopyFile( ${templateModuleFullPath}/module.cmake ${newModuleFullPath}/module.cmake )
			ReplaceInFile( ${newModuleFullPath}/module.cmake HELLOWORLD ${NEW_MODULE_NAME_UPPER} )
			ReplaceInFile( ${newModuleFullPath}/module.cmake moduleLibs "moduleLibs ${NEW_MODULE_LIBDEP}" )

			#
			# Copy template files to new module and change the
			# content (module declarations, namespace etc...)
			#
	
			set ( templateFiles _init.cpp
								_init.h
								_message.cpp
								_message.h
								_processor.cpp
								_processor.h
								_processor_factory.cpp
								_processor_factory.h )

			foreach( templateFile ${templateFiles} )

				set ( tmp_currentFile ${newModuleFullPath}/${NEW_MODULE_NAME_LOWER}${templateFile} )
				CopyFile( ${templateModuleFullPath}/${templateModuleName}${templateFile} ${tmp_currentFile} )

				ReplaceInFile( ${tmp_currentFile} HELLOWORLD ${NEW_MODULE_NAME_UPPER} )
				ReplaceInFile( ${tmp_currentFile} helloworld ${NEW_MODULE_NAME_LOWER} )
				ReplaceInFile( ${tmp_currentFile} Helloworld ${NEW_MODULE_NAME_CAPITALIZED} )

			endforeach( templateFile )


			# Remeber that this new module was created, so re-creating it with another configure-run is impossible
			set( moduleCreated_${NEW_MODULE_NAME_UPPER} TRUE CACHE INTERNAL moduleCreated_${NEW_MODULE_NAME_UPPER} )

			# Tell the user, that and where the new module has been created. The user
			# has to do another configure-run to add these files to the buildsystem due to the limitation,
			# that files written with the FILE command can't be used as cmake input files (module.cmake!)
			message( "A new module has been created in 'src/apps/${NEW_MODULE_NAME_LOWER}'\n\nTo add the new module files to the buildsystem, you must do another configure-run by pressing [c] (Linux/MacOSX) or clicking on 'configure' (Windows)." )
	
		else ( NOT EXISTS ${newModuleFullPath} )

			# If the module name already exists, warn the user and don't do anything.
			message( "The module '${newModuleFullPath}' already exist." )

		endif ( NOT EXISTS ${newModuleFullPath} )

	endif ( CREATE_NEW_MODULE AND NEW_MODULE_NAME AND NOT moduleCreated_${NEW_MODULE_NAME_UPPER} )