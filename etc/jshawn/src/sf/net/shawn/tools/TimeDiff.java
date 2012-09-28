/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.tools;

public class TimeDiff {
	private long lastTouch = System.currentTimeMillis();

	public void touch() {
		lastTouch = System.currentTimeMillis();
	}

	public long ms() {
		return System.currentTimeMillis() - lastTouch;
	}

	public long s() {
		return Math.round(((double) ms()) / 1000.0);
	}

	public long m() {
		return Math.round(((double) s()) / 60.0);
	}

	public long h() {
		return Math.round(((double) m()) / 60.0);
	}

	public long d() {
		return Math.round(((double) h()) / 24.0);
	}

}
