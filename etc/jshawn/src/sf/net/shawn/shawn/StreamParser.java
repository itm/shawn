/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.shawn;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.Stack;

import org.apache.log4j.Category;

import sf.net.shawn.module.JShawnModule;
import sf.net.shawn.module.ModuleManager;
import sf.net.shawn.tools.Logging;

public class StreamParser extends Thread {
	static final String trigger = "jshawn_internal:";

	private Category log = Logging.get(this);

	private PrintWriter bw = null;

	private BufferedReader br = null;

	private boolean running = true;

	private IShawnProcess shawn = null;

	private Stack<JShawnModule> moduleStack = new Stack<JShawnModule>();

	/**
	 * @param br
	 */
	public StreamParser(IShawnProcess p, InputStream is, OutputStream os) {
		this.bw = new PrintWriter(new OutputStreamWriter(os));
		this.br = new BufferedReader(new InputStreamReader(is), 65536);
		shawn = p;
		start();
	}

	/**
	 * 
	 *
	 */
	public void terminate() {
		log.debug("Terminate requested.");
		running = false;
	}

	/**
	 * 
	 */
	public void run() {
		while (running) {

			try {
				String line = br.readLine();

				if (line == null) {
					log.debug("Line is null. End of processing.");
					break;
				}

				if (log.isDebugEnabled())
					log.debug("Read line: " + line);

				if (line.startsWith(trigger)) {
					JShawnLine jl = new JShawnLine(line);

					if (jl.getCommand() == JShawnLine.Command.START_MODULE) {
						JShawnModule m = ModuleManager.get(jl.getModule());
						moduleStack.push(m);
						m.internalStart(shawn);
						log.debug("Pushed new module on top of the stack: " + jl.getModule());
					} else if (jl.getCommand() == JShawnLine.Command.END_MODULE) {
						JShawnModule m = moduleStack.pop();
						m.internalEnd();
						log.debug("Removed module from the top of the stack: " + jl.getModule());
					}
				} else {

					if (moduleStack.size() > 0) {
						moduleStack.peek().receive(line);
					} else {
						bw.println(line);
						bw.flush();
					}
				}

			} catch (Exception e) {
				log.warn("Error while reading. Terminating stream reading: " + e, e);
				terminate();
			}
		}

		bw.flush();
		log.debug("Exiting listening thread");
	}

}

class JShawnLine {
	public enum Command {
		START_MODULE, END_MODULE
	};

	private Category log = Logging.get(this);

	private String line = "";

	private Command command;

	private String module = "";

	public JShawnLine(String line_) throws Exception {
		line = line_;

		if (line == null || "".equals(line))
			throw new Exception("line[" + line + "] is null or empty");

		if (!line.startsWith(StreamParser.trigger))
			throw new Exception("line[" + line + "] does not start with prefix[" + StreamParser.trigger + "]");

		line = this.line.substring(StreamParser.trigger.length());

		int idx = this.line.indexOf(":");

		if (idx < 0)
			throw new Exception("line[" + line + "] has no further :");

		String cmd = line.substring(0, idx);

		if ("start_module".equals(cmd))
			command = Command.START_MODULE;
		else if ("end_module".equals(cmd))
			command = Command.END_MODULE;
		else
			throw new Exception("command[" + cmd + "] is not valid");

		module = line.substring(idx + 1);
		log.debug("line[" + line_ + "] --> command[" + command + "], module[" + module + "]");
	}

	public String getLine() {
		return line;
	}

	public Command getCommand() {
		return command;
	}

	public String getModule() {
		return module;
	}

}
