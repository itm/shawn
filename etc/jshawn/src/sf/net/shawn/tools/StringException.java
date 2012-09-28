/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.tools;

/**
 * Simple excption class with a string message constructor
 * 
 * @author Dennis Pfisterer, 2005
 */
public class StringException extends Exception {
	private static final long serialVersionUID = 1L;

	/** The contained message */
	String msg = "";

	/**
	 * @param msg
	 */
	public StringException(String msg) {
		this.msg = msg;
	}

	/**
	 * 
	 */
	public String toString() {
		return msg;
	}

}

