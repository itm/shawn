/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
#include <cstring>

#include "sys/shawn_sys_init.h"
#include "apps/apps_init.h"
#include "revision.h"

#ifdef HAVE_LEGACYAPPS
	#include "legacyapps/legacyapps_init.h"
#endif

#include "sys/sys_tasks_init.h"
#include "sys/simulation/shawn_simulation_controller.h"
#include "sys/simulation/jshawn/jshawn_simulation_controller.h"

#ifdef WITH_READLINE_SUPPORT
#include <readline/readline.h>
#include <readline/history.h>
#endif

using namespace std;
using namespace shawn;

ShawnSimulationController* master_init( bool jshawn_support )
   throw()
{
   ShawnSimulationController* sc = NULL;

   if(!jshawn_support)
	   sc = shawn_init_sys();
   else
	   sc = new JShawnSimulationController;

   init_sys_tasks(*sc);
   init_apps(*sc);

	#ifdef HAVE_LEGACYAPPS
		init_legacyapps(*sc);
	#endif

	return sc;
}

/** The method reads the given stream and searches include_file commands.
 *  Those commands allow the integration of multiple files into one
 *  configuration file. Hence, if this method finds a
 *  \code
 *    include_file filename
 *  \endcode
 *  exactly at the beginning of a line, the file with the given filename is
 *  included into the configuration file (stream).
 *
 *  To simplify matters, recursive inclusions (that is, an
 *  <em>include_file</em>-command in an included file) are not supported.
 *
 *  \param istream& The stream that is ought to be preprocessed.
 *  \return The resulting stream with all correct written input commands
 *    replaced by the appropriate file content.
 */
istream* preprocess_file( istream& in )
{
   stringstream* tmp_stream = new stringstream();
   char linebuf[1000];

   while( in.getline( linebuf, 1000 ) )
   {
      std::string line( linebuf );

      if ( line.find( "include_file", 0 ) == 0 )
      {
         // The following searches for a correct "include_file" statement.
         // Correct means, that the line starts (exactly at position 0) with
         // 'include_file', followed by one or more spaces or tabs, followed
         // by the filename that is not allowed to contain spaces, tabs, or
         // other blanks.
         std::string::size_type fn_beg, fn_end;
         // find end of the 'include_file' statement
         fn_beg = line.find_first_of( " ", 0 );
         // find beginning of the filename
         fn_beg = line.find_first_not_of( " \t", fn_beg );
         if ( fn_beg != std::string::npos )
         {
            fn_end = line.find_first_of( " \t\n\r", fn_beg );

            std::string filename;
            if ( fn_end == std::string::npos )
               filename = line.substr( fn_beg );
            else
               filename = line.substr( fn_beg, fn_end - fn_beg );

            ifstream* file = new ifstream( filename.c_str() );
            if( !(*file) )
            {
               perror( filename.c_str() );
               delete file;
               continue;
            }
            // If everything has worked fine, add the wanted file line by
            // line to the resulting stream.
            char fbuf[1000];
            while( file->getline( fbuf, 1000 ) )
               *tmp_stream << fbuf << std::endl;
            delete file;

            // The file has been included, so continue with the next line in
            // the original file (and thus avoid adding the include-statement
            // to tmp_stream).
            continue;
         }
      }

      // If the read line (in the buffer) is not a correct "include_file"
      // statement, just append the line to the resulting stream
      *tmp_stream << linebuf << std::endl;
   }

   return tmp_stream;
}

bool parse_args( int argc, char **argv,
                 shawn::SimulationController& sc,
                 std::string& filename)
{
   char *pos;

   for( int arg = 1; arg<argc; ++arg )
      {
         if( strcmp(argv[arg], "-f") == 0 )
            {
               if( arg<argc-1 )
                  {
                     filename = argv[arg+1];
                     ++arg;
                  }
               else
                  {
                     cerr << "ERROR: -f needs an argument." << endl;
                     return false;
                  }
            }

         else if( (pos=strchr(argv[arg],'=')) != NULL )
            {
               std::string parm( argv[arg], pos-argv[arg] );
               std::string val( pos+1 );
               //cout << "P{" << parm << "} V{" << val << "}" << endl;
               sc.environment_w().add_parameter_high_prio( parm, val );
            }
		 else if( strcmp(argv[arg], "-jshawn") == 0 )
		    {
				//Do nothin, but accept the command line option
		    }

         else
            {
               cerr << "ERROR: undefined argument '" << argv[arg] << "'" << endl;
               return false;
            }
      }

   return true;
}

int main( int argc, char *argv[] )
{
   char linebuf[1000];
   std::string filen("-");

   bool jshawn_support = false;
   for(int i = 1; i < argc; ++i)
      {
	if( strcmp(argv[i], "-jshawn") == 0 )
		jshawn_support = true;

	if( strcmp(argv[i], "--git-version") == 0 )
	{
		cout << git::revision << endl;
		return 0;    // no further execution
	}
      }

   shawn::ShawnSimulationController* sc = master_init(jshawn_support);

   if( !parse_args( argc, argv, *sc, filen ) )
      exit(1);

   istream* in;
   if( filen==std::string("-") )
      {
		  if(!jshawn_support) {
			cout << "[ NOW READING CONFIGURATION AND COMMANDS FROM STDIN --        ]" << endl
				<< "[ IF YOU ARE CONFUSED WHAT'S GOING ON HERE, TRY PIPING THE    ]" << endl
				<< "[ FILE shawn/apps/helloworld/randomhello.conf INTO THIS BINARY. ]" << endl;
			}
         in = &cin;
      }
   else
      {
         ifstream* file=new ifstream(filen.c_str());
         if( !(*file) )
            {
               perror(filen.c_str());
               delete file;
               return 1;
            }
         in = preprocess_file( *file );
		 delete file;
      }


#ifdef WITH_READLINE_SUPPORT
   if( in!=&cin ) {
#endif
   try {
      while( in->getline(linebuf,1000) )
         sc->execute_command_line(linebuf);
   }
   catch( std::runtime_error& re ) {
      cerr << "ERROR: " << re.what() << endl;
      if( in != &cin )
         delete in;
      return 1;
   }
#ifdef WITH_READLINE_SUPPORT
   }
   else
      {
         char* line;
         do {
            line = readline("Task> ");
            if( line != NULL )
               {
                  if( *line != 0 )
                     {
                        add_history(line);
                        try {
                           sc->execute_command_line(line);
                        }
                        catch( std::runtime_error& re ) {
                           cerr << "ERROR: " << re.what() << endl;
                        }
                     }
                  free(line);
               }
         }
         while(line!=NULL);
      }
#endif

   if( in != &cin )
      delete in;

   delete sc;
   return 0;
}







/*-----------------------------------------------------------------------
* Source  $Source: /cvs/shawn/shawn/frontend/console/con_main.cpp,v $
* Version $Revision: 462 $
* Date    $Date: 2010-09-21 16:20:58 +0200 (Tue, 21 Sep 2010) $
*-----------------------------------------------------------------------
* $Log: con_main.cpp,v $
 *-----------------------------------------------------------------------*/
