/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.tools;

import java.net.URL;

import org.apache.log4j.Category;
import org.apache.log4j.Level;
import org.apache.log4j.PropertyConfigurator;

/**
 * Logging facility. For configuration, a file named log4j.properties will be
 * searched in the classpath in the package specified in "propFileLocation". The
 * syntax is available at the Apache log4j homepage
 * (http://jakarta.apache.org/log4j/).
 * 
 * @author Dennis Pfisterer, 2002
 */
public class Logging {
	private static final String propFileLocation = "sf.net.shawn.tools/log4j.properties";
	private static final Category log = get(Logging.class);

	static {
		URL configUrl = Logging.class.getClassLoader().getResource(propFileLocation);

		if (configUrl != null)
			PropertyConfigurator.configure(configUrl);
		else
			System.err.println("WARNING: log4j properties file [" + propFileLocation + "] not found in classpath");

	}

	/** No instantiation allowed */
	private Logging() {
	}

	/**
	 * Returns a log4j (http://jakarta.apache.org/log4j/) object for the
	 * supplied class name. The log4j framework is configured by a property file
	 * which can be customized by the user.
	 * 
	 * @param _name
	 *            The class name that wants to log messages.
	 * @return A logging object
	 */
	public static final Category get(String _name) {

		try {
			return get(Class.forName(_name));
		} catch (ClassNotFoundException cnfe) {
			return Category.getInstance("UNKNOWN");
		}
	}

	/**
	 * Returns a log4j (http://jakarta.apache.org/log4j/) object for the
	 * supplied class. The log4j framework is configured by a propertiy file
	 * which can be customized by the user.
	 * 
	 * @param _obj
	 *            The object that wants to log messages.
	 * @return A logging object
	 */
	public static final Category get(Object _obj) {
		return get(_obj.getClass());
	}

	/**
	 * Returns a log4j (http://jakarta.apache.org/log4j/) object for the
	 * supplied class. The log4j framework is configured by a propertiy file
	 * which can be customized by the user.
	 * 
	 * @param _class
	 *            The class that wants to log messages.
	 * @return A logging object
	 */
	@SuppressWarnings("unchecked")
	public static final Category get(Class _class) {
		return Category.getInstance(_class.getName());
	}

	/**
	 * Set the actual root log level. Can be overridden down in the hierarchy
	 * 
	 * @param l
	 *            the log level for the root category
	 */
	public static final void setRootLevel(Level l) {
		log.info("Root logging level set to: " + l.toString());
		Category.getRoot().setLevel(l);
	}

	/**
	 * Enable / disable root level verbosity
	 * 
	 * @param _enable
	 */
	public static final void setVerbose(boolean _enable) {
		log.info("Setting verbose logging to: " + _enable);
		if (_enable)
			setRootLevel(Level.DEBUG);
		else
			setRootLevel(Level.WARN);
	}

}
