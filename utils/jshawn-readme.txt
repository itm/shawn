------------------------------------------------------------------------------------------
JShawn is a control application for Shawn (http://www.swarmnet.de/shawn) written in Java.  
------------------------------------------------------------------------------------------

Features
------------------------------------------------------------------------------------------
    - Run multiple instances of Shawn simultaneously
    - Set global variables, run simulation tasks
    - Use all features of Java to control the simulation control flow
    - Using BeanShell (http://www.beanshell.org), an easy to use scripting interface 
      in pure Java language can be used to avoid additional compilation steps
    - Helper class to easily write and modify MS Excel Files with Apaches POI
    
Usage & Requirements
------------------------------------------------------------------------------------------

    Requirements
    ------------------------------------------------------------------------------------------
        - Java 1.4 or later (http://java.sun.com)
        
    Usage (from the shawn main directory)
    ------------------------------------------------------------------------------------------
        General usage:
            
            java -jar scripts/jshawn/jshawn-allinone.jar <COMMAND LINE OPTIONS>
        
        Command line options:
            [-s,--shawn]      Path to the shawn executable
            [-b,--beanshell]  Bean shell script to execute
            [-d,--workdir]    The default work dir for shawn instances (default: .)
            [-e,--errstream]  The file where the error stream should be written to
            [-o,--outstream]  The file where the stdout stream should be written to
            [--nodefault]     No default shawn instance is created
            [--verbose]       Enable debugging output        
            
        Standard usage example (Windows):
            Windows:
            java -jar scripts/jshawn/jshawn-allinone.jar -s Release\shawn.exe -b yourscript.bsh
            
            Linux:
            java -jar scripts/jshawn/jshawn-allinone.jar -s bin/conapp-default -b yourscript.bsh

Documentation
------------------------------------------------------------------------------------------
The bean shell script that you supply on the command line will be executed (nearly) as a standard java method. By default, one instance of Shawn will be magically available. This instance is available by the name 'shawn' and of type ShawnInterface. The following methods are available on the ShawnInterface:

    public interface ShawnInterface {

        public void setGlobalVariable(String name, String value);
        
        public void runCommand(String cmd, String params);
        
        public boolean isRunning();
        public void destroy();
        
        public String getInstanceName();
        public void setInstanceName(String name);
    }


Examples
------------------------------------------------------------------------------------------

    ---------------------------------------------
    Simple one-shawn-instance helloworld example
    Run with -s Release\shawn.exe -b helloworld.bsh
    ---------------------------------------------
    
        shawn.setGlobalVariable("test_name", "test_value");
        
        shawn.runCommand("prepare_world", "edge_model=simple comm_model=disk_graph range=3");
        shawn.runCommand("rect_world", "width=25 height=25 count=10 processors=helloworld");
        shawn.runCommand("simulation", "max_iterations=30");

    ---------------------------------------------
    100 parallel helloworld shawn instances, no default instance
    If you change "ShawnProcessFactory.create(true);" to "ShawnProcessFactory.create(false);"
    runCommand will block until the command has been executed.
    Run with -s Release\shawn.exe -b parallel.bsh --nodefault
    ---------------------------------------------
        import java.util.*;
        
        Vector shawns = new Vector();
        
        //Create a number of Shawn instances and add them to the vector
        //The parameter true will create the shawn instances in a new thread.
        for(int i = 0; i < 100; i++) {
            ShawnInterface s = ShawnProcessFactory.create(true);
            s.setInstanceName("Shawn instance #"+i);
            shawns.add(s);
        }
        
        //Run all the simulations in parallel
        for(Iterator it = shawns.iterator(); it.hasNext(); ) {
            ShawnInterface s = (ShawnInterface)it.next();
            
            s.runCommand("prepare_world", "edge_model=simple comm_model=disk_graph range=1");
            s.runCommand("rect_world", "width=25 height=25 count=10 processors=helloworld");
            s.runCommand("simulation", "max_iterations=100");
        }

    ---------------------------------------------
    Simple Excelfile example
    ---------------------------------------------
        //If the file exists, it will be modified
        ExcelExport ee = new ExcelExport(new File("test.xls"));
        //Set the current active worksheet
        ee.selectWorksheet("csv file sheet");
        //Import a csv file with offset (0,0)
        //Delimiter is ; and comment character #
        ee.addCsvFile(0,0, new File("test.csv"), ';', "#");
        //Save the modified file
        ee.save();

FAQ
------------------------------------------------------------------------------------------
Q: Why do I get "java.lang.OutOfMemoryError int Thread-x"?
A: Because the current implementation relies on parsing the std-out from Shawn
   If you use stdout with excessively long lines, the buffer will overflow
   Just insert some line breaks in your stdout stream.

