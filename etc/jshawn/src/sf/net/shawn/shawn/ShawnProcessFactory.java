/************************************************************************
 ** This file is part of the network simulator Shawn. ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de)
 * project ** Shawn is free software; you can redistribute it and/or modify it ** under the terms of the BSD License.
 * Refer to the shawn-licence.txt ** file in the root of the Shawn source tree for further details. **
 ************************************************************************/
package sf.net.shawn.shawn;

import java.util.Vector;

import org.apache.log4j.Category;

import sf.net.shawn.EnvironmentConfig;
import sf.net.shawn.tools.Logging;

public class ShawnProcessFactory {
	private static Category log = Logging.get(ShawnProcessFactory.class);

	private static Vector<IShawnProcess> shawns = new Vector<IShawnProcess>();

	private static EnvironmentConfig defaultValues = null;

	public static IShawnProcess create() throws Exception {
		log.debug("Creating shawn instance with default values");

		IShawnProcess s = null;

		if (defaultValues.isDryRun()) {
			log.info("Creating dry run shawn instance");
			s = new DryRunShawnProcess();
		} else {
			log.info("Creating local process shawn instance");
			s = new ShawnProcess(defaultValues.getShawnExecutable(), defaultValues.getWorkDir());
		}
		shawns.add(s);
		return s;
	}

	public static void destroy() {

		log.debug("Destroying " + shawns.size() + " shawn processes.");
		for (IShawnProcess proc : shawns)
			proc.killProcess();
		log.debug("Done.");
	}

	public static EnvironmentConfig getDefaultValues() {
		return defaultValues;
	}

	public static void setDefaultValues(EnvironmentConfig defaultValues) {
		ShawnProcessFactory.defaultValues = defaultValues;
	}

}
