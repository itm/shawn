/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.module;

import java.util.HashMap;

public class ModuleManager {
	private static HashMap<String, JShawnModule> modules = new HashMap<String, JShawnModule>();

	static {
		register( new RunTaskModule() );
		register( new ShawnNDResultModule() );
	}
	
	public static void register(JShawnModule m) {
		modules.put(m.getName(), m);
	}

	public static JShawnModule get(String name) throws Exception {
		JShawnModule m = modules.get(name);
		if (m == null)
			throw new Exception("module[" + name + "] not found");
		return m;
	}

}
