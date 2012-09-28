/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.module;

import java.io.BufferedReader;
import java.io.StringReader;

import org.apache.log4j.Category;

import sf.net.shawn.shawn.ShawnResult;
import sf.net.shawn.tools.Logging;

public class RunTaskModule extends JShawnModule {
	private Category log = Logging.get(this);

	StringBuffer sb = null;

	@Override
	public String getName() {
		return "jshawn_run_command";
	}

	@Override
	public void start() throws Exception {
		log.debug("Starting[" + getName() + "]");
		sb = new StringBuffer();
	}

	@Override
	public String receive(String buffer) throws Exception {
		log.debug("Reveived: " + buffer);
		sb.append(buffer);
		return buffer;
	}

	@Override
	public ShawnResult end() throws Exception {
		BufferedReader r = new BufferedReader(new StringReader(sb.toString()));
		String line = r.readLine();

		log.debug("Received" + line);

		if (line != null && line.startsWith("end")) {
			log.debug("Detected end of task line[" + line + "]");
			notifyTaskDone();
		}

		log.debug("End[" + getName() + "]");
		return null;
	}

}
