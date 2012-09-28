/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.module;

import sf.net.shawn.shawn.IShawnProcess;
import sf.net.shawn.shawn.ShawnResult;

public abstract class JShawnModule {
	private IShawnProcess p = null;

	public final void internalStart(IShawnProcess p) throws Exception {
		this.p = p;
		start();
	}

	public final void internalEnd() throws Exception {
		ShawnResult r = end();
		if (r != null)
			addResult(end());
	}

	protected final void addResult(ShawnResult res) {
		p.addResult(res);
	}

	protected final void notifyTaskDone() {
		p.notifyTaskDone();
	}

	/**
	 * @return
	 */
	public abstract String getName();

	/**
	 * 
	 *
	 */
	public abstract void start() throws Exception;

	/**
	 * @param buffer
	 */
	public abstract String receive(String buffer) throws Exception;

	/**
	 * 
	 *
	 */
	public abstract ShawnResult end() throws Exception;

}
