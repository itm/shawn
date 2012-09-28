/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.tools;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

import org.apache.log4j.Category;

/**
 * MD5 helper class.
 */
public class MD5 {
	private static Category log = Logging.get(MD5.class); // /< Logging member

	private static final char kHexChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	// ------------------------------------------------------------------
	/**
	 * @param _input
	 * @return
	 * @throws NoSuchAlgorithmException
	 */
	public static String getMD5(String _input) throws NoSuchAlgorithmException {
		if (_input == null) {
			log.error("getMD5: Input is null");
			return "";
		}

		MessageDigest md5 = MessageDigest.getInstance("MD5");
		md5.update(_input.getBytes());
		return bufferToHex(md5.digest());
	}

	// ------------------------------------------------------------------
	/**
	 * @param _user
	 * @param _password
	 * @return
	 * @throws NoSuchAlgorithmException
	 */
	public static String getMD5(String _user, String _password) throws NoSuchAlgorithmException {
		if (_user == null || _password == null) {
			log.error("getMD5: Input is null user[" + _user + "], pass[" + _password + "]");
			return "";
		}

		MessageDigest md5 = MessageDigest.getInstance("MD5");
		md5.update(_user.getBytes());
		md5.update(_password.getBytes());
		return bufferToHex(md5.digest());
	}

	// ------------------------------------------------------------------
	/**
	 * Just a simplified wrapper for the bufferToHex(byte buffer[], int startOffset, int length) method.
	 * 
	 * @param stringBytes
	 *            A buffer to convert to hex
	 * @return The hex string version of the input buffer
	 */
	public static String bufferToHex(byte[] stringBytes) {
		return bufferToHex(stringBytes, 0, stringBytes.length);
	}

	// ------------------------------------------------------------------
	/**
	 * Returns a string containing the hexadecimal representation of the input byte array. Each byte in the input array
	 * is converted to a two-digit hexadecimal value. Thus the returned string is twice the length of the specified
	 * amount of the input byte array. The output hex characters are upper case.
	 * 
	 * @param buffer
	 *            A buffer to convert to hex
	 * @param startOffset
	 *            The offset of the first byte in the buffer to process
	 * @param length
	 *            The number of bytes in the buffer to process
	 * @return The hex string version of the input buffer
	 */
	public static String bufferToHex(byte buffer[], int startOffset, int length) {
		StringBuffer hexString = new StringBuffer(2 * length);
		int endOffset = startOffset + length;

		for (int i = startOffset; i < endOffset; i++)
			appendHexPair(buffer[i], hexString);

		return hexString.toString();
	}

	// ------------------------------------------------------------------
	/**
	 * Appends a hexadecimal representation of a particular char value to a string buffer. That is, two hexadecimal
	 * digits are appended to the string.
	 * 
	 * @param b
	 *            A byte whose hex representation is to be obtained
	 * @param hexString
	 *            The string to append the hex digits to
	 */
	private static void appendHexPair(byte b, StringBuffer hexString) {
		char highNibble = kHexChars[(b & 0xF0) >> 4];
		char lowNibble = kHexChars[b & 0x0F];

		hexString.append(highNibble);
		hexString.append(lowNibble);
	}

	// ------------------------------------------------------------------
	/**
	 * @param args
	 * @throws NoSuchAlgorithmException
	 */
	public static void main(String args[]) throws NoSuchAlgorithmException {
		String test = "test";

		for (int i = 1; i < 10; i++) {
			System.out.println("md5[" + (test + i) + "] = " + MD5.getMD5((test + i)));
		}

	}

}
