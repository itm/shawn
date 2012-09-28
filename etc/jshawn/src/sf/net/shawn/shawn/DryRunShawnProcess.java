package sf.net.shawn.shawn;

import java.util.LinkedList;

import org.apache.log4j.Category;

import sf.net.shawn.tools.Logging;

public class DryRunShawnProcess extends IShawnProcess {
	private Category log = Logging.get(this);

	private boolean running = true;

	@Override
	public void addResult(ShawnResult res) {
	}

	@Override
	public boolean isRunning() {
		return running;
	}

	@Override
	public void killProcess() {
		running = false;
		super.killProcess();
	}

	@Override
	public void notifyTaskDone() {
	}

	@Override
	public LinkedList<ShawnResult> runCommand(String cmd) {
		return runCommand(cmd, "");
	}

	@Override
	public LinkedList<ShawnResult> runCommand(String cmd, String params) {
		String cmdLine = toCommandLine(cmd, params);
		storeCommandInHistory(cmdLine);
		log.debug("Sending line to shawn: " + cmdLine);
		return new LinkedList<ShawnResult>();
	}

	@Override
	public void setGlobalVariable(String name, String value) {
		String cmdLine = toGlobalVariableLine(name, value);
		storeCommandInHistory(cmdLine);
		log.debug("Setting global variable: " + cmdLine);
	}

}
