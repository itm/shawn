package sf.net.shawn.shawn;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedList;

import org.apache.log4j.Category;

import sf.net.shawn.tools.Logging;

public abstract class IShawnProcess {
	private Category log = Logging.get(this);

	private BufferedWriter historyTraceStream = null;

	private String name = "<noname>";

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	protected String toCommandLine(String cmd, String params) {
		return cmd + " " + params;
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	protected String toGlobalVariableLine(String name, String value) {
		return name + "=" + value;
	}

	// --------------------------------------------------------------------------------
	/**
	 * @param name
	 * @param value
	 */
	public abstract void setGlobalVariable(String name, String value);

	// --------------------------------------------------------------------------------
	/**
	 * @param cmd
	 * @param params
	 */
	public abstract LinkedList<ShawnResult> runCommand(String cmd);

	// --------------------------------------------------------------------------------
	/**
	 * @param cmd
	 * @param params
	 */
	public abstract LinkedList<ShawnResult> runCommand(String cmd, String params);

	// --------------------------------------------------------------------------------
	/**
	 * @return
	 */
	public abstract boolean isRunning();

	// --------------------------------------------------------------------------------
	/**
	 *
	 */
	public void killProcess() {
		// Close history trace stream
		if (historyTraceStream != null) {
			try {
				historyTraceStream.flush();
				historyTraceStream.close();
			} catch (Throwable t) {
				log.error("Error while closing historyTraceStream: " + t, t);
			}
		}

	}

	// --------------------------------------------------------------------------------
	/**
	 * Save the command in the history
	 * 
	 * @param cmdLine
	 *            The complete command string
	 */
	protected void storeCommandInHistory(String cmdLine) {
		if (historyTraceStream != null) {
			try {
				historyTraceStream.write(cmdLine + "\n");
				historyTraceStream.flush();
			} catch (IOException e) {
				log.error("Unable to write history stream: " + e, e);
			}
		}
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#traceHistory(java.lang.String)
	 */
	public synchronized void traceHistory(String outfile) throws IOException {
		if (historyTraceStream != null) {
			historyTraceStream.flush();
			historyTraceStream.close();
		}
		log.info("Tracing history to " + outfile);
		historyTraceStream = new BufferedWriter(new FileWriter(new File(outfile)));
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#getInstanceName()
	 */
	public String getInstanceName() {
		return name;
	}

	// --------------------------------------------------------------------------------
	/* (non-Javadoc)
	 * @see sf.net.shawn.shawn.IShawnProcess#setInstanceName(java.lang.String)
	 */
	public void setInstanceName(String name) {
		this.name = name;
	}

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public abstract void addResult(ShawnResult res);

	// --------------------------------------------------------------------------------
	/**
	 * 
	 */
	public abstract void notifyTaskDone();

}