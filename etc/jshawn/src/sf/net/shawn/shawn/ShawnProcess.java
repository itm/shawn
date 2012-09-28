/************************************************************************
 * This file is part of the network simulator Shawn. Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project
 * Shawn is free software; you can redistribute it and/or modify it under the terms of the BSD License. Refer to the
 * shawn-licence.txt ** file in the root of the Shawn source tree for further details. **
 ************************************************************************/
package sf.net.shawn.shawn;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.LinkedList;

import org.apache.log4j.Category;

import sf.net.shawn.tools.Logging;
import sf.net.shawn.tools.TimeDiff;
import sf.net.shawn.tools.Tools;

public class ShawnProcess extends IShawnProcess {

	private Category log = Logging.get(this);

	private Process shawn = null;

	private PrintWriter stdinStream = null;

	private BufferedInputStream stdoutStream = null;

	private BufferedInputStream stderrStream = null;

	private boolean taskRunning = false;

	private LinkedList<ShawnResult> currentResults = new LinkedList<ShawnResult>();

	// --------------------------------------------------------------------------------
	/**
	 * @param executablePath
	 * @throws IOException
	 */
	ShawnProcess(String executablePath, String workingDir) throws IOException {
		executablePath += " -jshawn";
		log.debug("Starting shawn: " + executablePath + ", workdir: " + workingDir);

		Runtime rt = Runtime.getRuntime();

		shawn = rt.exec(executablePath, null, new File(workingDir));

		if (!isRunning()) {
			log.error("Shawn executable not found.");
			return;
		}

		stdinStream = new PrintWriter(new OutputStreamWriter(shawn.getOutputStream()));

		new Thread() {
			public void run() {
				int available = 0;
				byte[] buf = new byte[256];

				//
				stderrStream = new BufferedInputStream(shawn.getErrorStream());
				while (true) {
					try {
						// Check if the error stream has bytes available and sleep if not
						available = stderrStream.available();
						if (available == 0) {
							Tools.sleep(100);
							continue;
						}

						available = stderrStream.read(buf);

						// Check if we have reached the end of the stream
						if (available == -1) {
							log.warn("End of std-error stream reached");
							break;
						} else if (available > 0) {
							System.err.write(buf, 0, available);
							System.err.flush();
						}

					} catch (IOException e) {
						log.debug("Shawn terminated: Error while reading from std-error stream: " + e, e);
						notifyTaskDone();
						killProcess();
						break;
					}
				}
			}

		}.start();

		stdoutStream = new BufferedInputStream(shawn.getInputStream());
		new StreamParser(this, stdoutStream, System.out);
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#setGlobalVariable(java.lang.String, java.lang.String)
	 */
	public void setGlobalVariable(String name, String value) {
		String cmdLine = toGlobalVariableLine(name, value);
		log.debug("Setting global variable: " + cmdLine);
		// Save the command in the history
		storeCommandInHistory(cmdLine);

		// Send to Shawn
		stdinStream.println(cmdLine);
		stdinStream.flush();
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#runCommand(java.lang.String)
	 */
	public synchronized LinkedList<ShawnResult> runCommand(String cmd) {
		return runCommand(cmd, "");
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#runCommand(java.lang.String, java.lang.String)
	 */
	public synchronized LinkedList<ShawnResult> runCommand(String cmd, String params) {
		taskRunning = true;
		currentResults = new LinkedList<ShawnResult>();
		String cmdLine = toCommandLine(cmd, params);

		// Save the command in the history
		storeCommandInHistory(cmdLine);

		// Send the command to Shawn
		log.debug("Sending line to shawn: " + cmdLine);
		stdinStream.println(cmdLine);
		stdinStream.flush();

		// Wait for the task to finish
		while (taskRunning) {
			try {
				wait();
			} catch (InterruptedException e) {
				log.error("While waiting for task done: " + e, e);
				e.printStackTrace();
			}
		}

		log.info("Task done: " + cmd);
		return currentResults;
	}

	// --------------------------------------------------------------------------------
	/**
	 * @param res
	 */
	public synchronized void addResult(ShawnResult res) {
		if (log.isDebugEnabled())
			log.debug("New result: " + res);
		currentResults.add(res);
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public synchronized void notifyTaskDone() {
		log.debug("Task done. Number of results: " + currentResults.size());
		taskRunning = false;
		notifyAll();
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#isRunning()
	 */
	public boolean isRunning() {
		try {
			shawn.exitValue();
			return false;
		} catch (IllegalThreadStateException itse) {
			return true;
		}
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#killProcess()
	 */
	public void killProcess() {
		log.info("Destroying shawn process.");
		try {

			// Close the stdin stream of shawn to indicate eof
			if (stdinStream != null) {
				this.stdinStream.flush();
				this.stdinStream.close();
			}

			// Wait for termination (exit value available)
			TimeDiff timer = new TimeDiff();
			while (timer.s() < 5) {
				try {
					log.debug("Exit value of shawn: " + shawn.exitValue());
					break;
				} catch (IllegalThreadStateException e) {
					log.debug("Shawn process is still alive");
					Tools.sleep(100);
				}
			}

			super.killProcess();

			// Close the out streams
			try {
				if (stderrStream != null)
					stderrStream.close();
			} catch (IOException e) {
				log.error("Error while closing stream: " + e, e);
			}

			try {
				if (stdoutStream != null)
					stdoutStream.close();
			} catch (IOException e) {
				log.error("Error while closing stream: " + e, e);
			}

			// Now forcibly terminate shawn if still running
			shawn.destroy();

			log.debug("Terminated shawn process, isRunning[" + isRunning() + "]");
		} catch (Throwable t) {
			log.error("Error while destroying shawn process: " + t, t);
		}
	}



}
