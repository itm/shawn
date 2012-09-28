package de.swarmnet.shawn;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.zip.*;

/**
 * ShawnMakeDist (Shawn Make Distribution) is an command line application
 * that is able to create a package out of the sources of the Shawn
 * project. Please see the <code>printUsage()</code> method for help on
 * using this program.
 * 
 */
public class ShawnMakeDist
{
	private static File					shawnDir	= null;		// Shawn root directory
	private static File					outDir		= null;		// Output directory for distribution file
	private static String				packageName	= "";		// Filename of the package to be created
	private static ArrayList< String >	ignoreList	= null;		// Arraylist containing the list of all files that belong to the Shawn package.
	private static ShawnFileFilter		fileFilter	= null;		// Filter filter for ignoring cetain files and directories 

	/**
	 * Main program, reads command line parameters and invokes the recursive
	 * file finding and package generation.
	 */
	public static void main( String[] args )
	{
		// Check command line arguments

		if ( args.length < 1 )
		{
			printUsage();
			return;
		}

		shawnDir = new File( args[0] );
		if ( !shawnDir.isDirectory() )
		{
			System.err.println( "'" + shawnDir.toString() + "' is not a valid directory." );
			return;
		}
		

		if ( args.length > 1 )
		{
			outDir = new File( args[1] );
			if ( !outDir.isDirectory() )
			{
				System.err.println( "'" + outDir.toString() + "' is not a valid directory." );
				return;
			}
		}
		else
			outDir = new File( "." );

		
		// Read ignore list
		ignoreList = readIgnoreList( new File( "ignore.txt" ) );
		if ( ignoreList == null )
		{
			System.out.println( "Warning: Can't open ignore list 'ingore.txt', all sources will be packed." );
			ignoreList = new ArrayList< String >();
		}

		// Print out information
		System.out.println( "Shawn root directory: " + shawnDir.getPath() );
		System.out.println( "Output directory:     " + outDir.getPath() );
		System.out.print( "Ignoring: " );
		for ( String s : ignoreList )
			System.out.print( s + " " );
		
		System.out.println();
		System.out.println( "Collecting files...\n" );

		// Set package name
		SimpleDateFormat df = new SimpleDateFormat( "yyyy-MM-dd" );
		packageName = "Shawn-" + df.format( new Date() ) + "_source.zip";
		ignoreList.add( packageName );

		// Create the file filter
		ShawnMakeDist.fileFilter = new ShawnFileFilter( ignoreList );

		// Collect package files
		ArrayList< File > packageFiles = findFiles( shawnDir );
		System.out.println( packageFiles.size() + " files found, creating zip file..." );
		
		
		// Create the zip file
		try
		{
			createZipFile( packageFiles );
		}
		catch ( IOException e )
		{
			e.printStackTrace();
		}
		
		
		//for ( File f : packageFiles )
		//	System.out.println( getZipPath( f.getPath() ) );
		
		//System.out.println( "\nFinished.\n(" + outDir.getAbsolutePath() + "/" + packageName + ")" );
		System.out.println( "\nFinished." );

		return;
	}
	
	/**
	 * Creates a zip file out of the privided collection of File objects.
	 * 
	 * @param	packageFiles	An ArrayList containing all files to zip
	 * @throws	IOException
	 */
	private static void createZipFile( ArrayList< File > packageFiles ) throws IOException
	{
		int				read		= 0;
		final int		BUFFER_SIZE	= 1024;
		byte[]			data		= new byte[ BUFFER_SIZE ];

		
		ZipEntry		entry		= null;
		ZipOutputStream zout		= null;
		FileInputStream	in			= null;

		// Create the zip file and set the zip - mode (compression of files)
		zout = new ZipOutputStream( new FileOutputStream ( outDir.getPath() + "/" + packageName ) );
		zout.setMethod( ZipOutputStream.DEFLATED );

		long startTime = System.currentTimeMillis();
		int fileNum = 0;
		
		for ( File file : packageFiles )
		{
			in		= new FileInputStream( file );
			entry	= new ZipEntry( getRelativePath( file.getPath() ) );

			zout.putNextEntry( entry );
			
			while ( (read = in.read( data, 0, BUFFER_SIZE ) ) != -1 )
				zout.write( data, 0, read );

			zout.closeEntry();
			in.close();
		
			fileNum++;

			// Display progress
			if ( System.currentTimeMillis() - startTime > 2000 )
			{
				startTime = System.currentTimeMillis();
				int percent = (int)( ( (float)fileNum / (float)packageFiles.size() ) * 100.0f );
				System.out.print( percent + "% " );
			}
		}
		
		// Make sure there is an empty buildfiles directory in the zip file
		entry = new ZipEntry( getRelativePath( shawnDir.getPath() + "/buildfiles/" ) );
		zout.putNextEntry( entry );
		zout.closeEntry();

		zout.close();
		
		System.out.println( "100%" );
	}
	
	
	/**
	 * Returns the relativ path of a filename to the shawn root dir
	 */
	private static String getRelativePath( String filename )
	{
		int l = shawnDir.getPath().length();
		return filename.substring( l+1, filename.length() );
	}

	/**
	 * Collects all files recursively with subdirectories. It ignores
	 * directories with the names found in the <code>ingoreList</code>
	 * of this application.
	 * 
	 * @param dir Where to start
	 * @return ArrayList containing File objects representing the files
	 */
	private static ArrayList< File > findFiles( File dir )
	{
		if ( !dir.isDirectory() )
			return null;

		ArrayList< File > files = new ArrayList< File >(); 

		File[] currentFiles = dir.listFiles( ShawnMakeDist.fileFilter );

		if ( currentFiles != null )
			for ( File file : currentFiles )
				if ( file.isDirectory() )
					files.addAll( findFiles( file ) );
				else
					files.add( file );

		return files;
	}
	
	/**
	 * Reads the contents of the ignore list. Those directories will be ignored by
	 * the <code>findFiles</code> method.
	 * 
	 * @param	ignoreFile	File object representing the directory and filename of the ignore file
	 * @return	ArrayList containing the directory names of the ignore file as Strings.
	 */
	private static ArrayList< String > readIgnoreList( File ignoreFile )
	{
		String				buffer		= "";
		ArrayList< String > ignoreList	= new ArrayList< String >();

		try
		{
			BufferedReader in = new BufferedReader( new InputStreamReader( new FileInputStream ( ignoreFile ) ) );
			while ( ( buffer = in.readLine() ) != null )
			{
				buffer = buffer.trim();
				if ( !buffer.equals( "" ) && !buffer.startsWith( "//" ) )
					ignoreList.add( buffer );
			}
		}
		catch ( IOException e )
		{
			//e.printStackTrace();
			return null;
		}

		return ignoreList;
	}

	/**
	 * Prints a help text how to use this program.
	 */
	private static void printUsage()
	{
		System.out.println(
			"ShawnMakeDist usage:\n" +  
			"   java -jar ShawnMakeDist.jar <shawn root dir> [<out dir>]\n\n" +
	
			"   <shawn source dir>  The directory of the Shawn source tree.\n" +
			"   <out dir>           Optional. Output directory for package.\n\n" +
	
			"Example:\n" +
			"    java MakeDist /projects/shawn /projects/shawn/dist\n\n"
		);
		
	
	}
}

class ShawnFileFilter implements FileFilter
{
	private ArrayList< String > ignoreList = null;

	public ShawnFileFilter( ArrayList< String > ignoreList )
	{
		this.ignoreList = ignoreList;
	}

	public boolean accept( File f )
	{
		// Ingore all directories and files of the ignore list
		return !ignoreList.contains( f.getName() );
	}
}