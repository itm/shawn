/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn;

import jargs.gnu.CmdLineParser;
import jargs.gnu.CmdLineParser.IllegalOptionValueException;
import jargs.gnu.CmdLineParser.UnknownOptionException;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintWriter;

import org.apache.log4j.Category;

import sf.net.shawn.shawn.IShawnProcess;
import sf.net.shawn.shawn.ShawnProcessFactory;
import sf.net.shawn.tools.Logging;

import bsh.Interpreter;

public class Main {
	static Category log = Logging.get(Main.class);

	/**
	 * @param args
	 * @return
	 * @throws FileNotFoundException
	 * @throws Exception
	 */
	public static void main(String args[]) throws FileNotFoundException {
		log.info("------------------------------------------------------------");
		log.info("New jshawn instance");
		log.info("------------------------------------------------------------");

		Runtime.getRuntime().addShutdownHook(new Thread() {
			public void run() {
				ShawnProcessFactory.destroy();
			}
		});

		log.debug("Creating new environment");
		EnvironmentConfig ec = parseOptions(args);
		ShawnProcessFactory.setDefaultValues(ec);
		ec.debug();

		log.debug("Running script");
		try {
			runBeanScript(ec);
		} catch (Throwable e) {
			log.fatal("Unable to parse the bean shell script: " + e, e);
			System.err.println("Unable to parse the bean shell script: " + e);
			e.printStackTrace(System.err);
		}

		log.debug("Done, exit now.");
		System.exit(0);
	}

	/**
	 * @param scriptFile
	 * @throws Exception
	 * @throws Exception
	 */
	private static void runBeanScript(EnvironmentConfig ec) throws Exception {
		Interpreter i = new Interpreter();

		ShawnProcessFactory.setDefaultValues(ec);

		if (ec.isCreateDefaultShawn()) {
			IShawnProcess s = ShawnProcessFactory.create();
			i.set("shawn", s);
		}

		i.eval("import de.farberg.jshawn.shawn.*;");
		i.eval("import de.farberg.tools.*;");
		i.eval("import de.farberg.jshawn.*;");
		
		i.eval("import sf.net.shawn.shawn.*;");
		i.eval("import sf.net.shawn.tools.*;");
		i.eval("import sf.net.shawn.*;");

		i.source(ec.getBeanShellScript());
	}

	/**
	 * @return
	 */
	private static EnvironmentConfig defaultOptions() {
		EnvironmentConfig ec = new EnvironmentConfig();
		ec.setShawnExecutable("shawn");
		ec.setWorkDir(System.getProperty("user.dir"));
		ec.setErrStream(System.err);
		ec.setOutStream(System.out);
		ec.setCreateDefaultShawn(true);
		ec.setDryRun(false);
		return ec;
	}

	private static void usage() {
		PrintWriter out = new PrintWriter(System.out);

		out.println("Usage:");
		out.println("\t [-s,--shawn]      Path to the shawn executable");
		out.println("\t [-b,--beanshell]  Bean shell script to execute");
		out.println("\t [-d,--workdir]    The default work dir for shawn instances (default: .)");
		out.println("\t [-e,--errstream]  The file where the error stream should be written to");
		out.println("\t [-o,--outstream]  The file where the stdout stream should be written to");
		out.println("\t [--dryrun]     	  The commands are not actually passed to shawn, but a trace history is written to a conf file");
		out.println("\t [--nodefault]     No default shawn instance is created");
		out.println("\t [--verbose]       Enable debugging output");
		out.flush();
	}

	/**
	 * @param args
	 * @return
	 * @throws IllegalOptionValueException
	 * @throws UnknownOptionException
	 * @throws FileNotFoundException
	 */
	private static EnvironmentConfig parseOptions(String args[]) throws FileNotFoundException {
		String tmp = null;
		EnvironmentConfig ec = defaultOptions();

		CmdLineParser parser = new CmdLineParser();
		CmdLineParser.Option opt_shawn = parser.addStringOption('s', "shawn");
		CmdLineParser.Option opt_bean = parser.addStringOption('b', "beanshell");
		CmdLineParser.Option opt_workdir = parser.addStringOption('d', "workdir");
		CmdLineParser.Option opt_errOut = parser.addStringOption('e', "errstream");
		CmdLineParser.Option opt_stdOut = parser.addStringOption('o', "outstream");
		CmdLineParser.Option opt_nodefault = parser.addBooleanOption("nodefault");
		CmdLineParser.Option opt_verbose = parser.addBooleanOption("verbose");
		CmdLineParser.Option opt_dryrun = parser.addBooleanOption("dryrun");

		try {
			parser.parse(args);
		} catch (IllegalOptionValueException e) {
			log.warn("Illegal option: " + e);
			usage();
			System.exit(1);
		} catch (UnknownOptionException e) {
			log.warn("Unknown option: " + e);
			usage();
			System.exit(1);
		}

		if ((tmp = (String) parser.getOptionValue(opt_shawn, null)) != null)
			ec.setShawnExecutable(tmp);

		if ((tmp = (String) parser.getOptionValue(opt_workdir, null)) != null)
			ec.setWorkDir(tmp);

		if ((tmp = (String) parser.getOptionValue(opt_bean, null)) != null)
			ec.setBeanShellScript(tmp);

		if ((tmp = (String) parser.getOptionValue(opt_errOut, null)) != null)
			ec.setOutStream(new FileOutputStream(new File(tmp)));

		if ((tmp = (String) parser.getOptionValue(opt_stdOut, null)) != null)
			ec.setOutStream(new FileOutputStream(new File(tmp)));

		Boolean def = (Boolean) parser.getOptionValue(opt_nodefault);
		if (def != null)
			ec.setCreateDefaultShawn(false);
		else
			ec.setCreateDefaultShawn(ec.isCreateDefaultShawn());

		Boolean verbose = (Boolean) parser.getOptionValue(opt_verbose, new Boolean(false));
		if (verbose.booleanValue())
			Logging.setVerbose(true);

		Boolean dry_run = (Boolean) parser.getOptionValue(opt_dryrun, new Boolean(false));
		ec.setDryRun(dry_run.booleanValue());
		
		return ec;
	}

}
