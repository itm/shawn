public class _ShawnTest
{
	public static void main( String argv[] )
	{
		System.loadLibrary( "../../buildfiles/release/shawn" );
		
		Message msg = new Message();
		
		System.out.println( msg.timestamp_simulation_round() );
		
		System.out.println( "ready." );
	}
	
}