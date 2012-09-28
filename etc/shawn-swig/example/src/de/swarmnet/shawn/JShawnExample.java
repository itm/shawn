package de.swarmnet.shawn;

public class JShawnExample
{
	public static void main( String[] args )
	{
		/**
		 * Load shawn library. This library must be compiled beforehand from the shawn
		 * source as a windows DLL oder unix .so file.
		 */
		
		System.loadLibrary( "../../buildfiles/release/shawn" );

		ShawnSimulationController sc = Shawn.master_init();
		

		sc.execute_command_line( "prepare_world edge_model=simple comm_model=disk_graph range=1 transm_model=reliable" );
		
		sc.execute_command_line( "simulation max_iterations=5" );
		
		System.out.println( "stop." );
		
	}

}
