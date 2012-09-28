/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn;

import java.io.OutputStream;
import java.util.Iterator;
import java.util.Vector;

import org.apache.log4j.Category;

import sf.net.shawn.tools.Logging;

public class EnvironmentConfig {
	private Category log = Logging.get(this);

	private String shawnExecutable = null;

	private String workDir = null;

	private String beanShellScript = null;

	private OutputStream errStream = null;

	private OutputStream outStream = null;

	private boolean createDefaultShawn = true;

	private boolean dryRun = false;

	public String getShawnExecutable() {
		return shawnExecutable;
	}

	public void setShawnExecutable(String shawnExecutable) {
		this.shawnExecutable = shawnExecutable;
	}

	public String getWorkDir() {
		return workDir;
	}

	public void setWorkDir(String workDir) {
		this.workDir = workDir;
	}

	public OutputStream getErrStream() {
		return errStream;
	}

	public void setErrStream(OutputStream errStream) {
		this.errStream = errStream;
	}

	public OutputStream getOutStream() {
		return outStream;
	}

	public void setOutStream(OutputStream outStream) {
		this.outStream = outStream;
	}

	public String getBeanShellScript() {
		return beanShellScript;
	}

	public void setBeanShellScript(String beanShellScript) {
		this.beanShellScript = beanShellScript;
	}

	public boolean isCreateDefaultShawn() {
		return createDefaultShawn;
	}

	public void setCreateDefaultShawn(boolean createDefaultShawn) {
		this.createDefaultShawn = createDefaultShawn;
	}

	@SuppressWarnings("unchecked")
	private Vector description() {
		Vector<String> v = new Vector<String>();

		v.add("Bean shell script: " + getBeanShellScript());
		v.add("Shawn executable: " + getShawnExecutable());
		v.add("Default shawn instance: " + isCreateDefaultShawn());
		v.add("Working dir: " + getWorkDir());
		v.add("Err stream: " + getErrStream());
		v.add("Out stream: " + getOutStream());
		v.add("Dry run: " + isDryRun());

		return v;
	}

	@SuppressWarnings("unchecked")
	public void debug() {
		if (!log.isDebugEnabled())
			return;

		Vector v = description();
		for (Iterator it = v.iterator(); it.hasNext();)
			log.debug((String) it.next());
	}

	@SuppressWarnings("unchecked")
	public String toString() {
		Vector v = description();
		String s = "";
		for (Iterator it = v.iterator(); it.hasNext();)
			s += (String) it.next();
		return s;
	}

	public void setDryRun(boolean dryRun) {
		this.dryRun = dryRun;
	}

	public boolean isDryRun() {
		return dryRun;
	}
}
