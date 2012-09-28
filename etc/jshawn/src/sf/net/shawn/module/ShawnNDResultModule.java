/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.module;

import java.util.Vector;

import org.apache.log4j.Category;

import sf.net.shawn.shawn.NDResult;
import sf.net.shawn.shawn.ShawnResult;
import sf.net.shawn.tools.Logging;

/**
 * @author lipphardt TODO To change the template for this generated type comment go to Window - Preferences - Java -
 *         Code Style - Code Templates
 */
public class ShawnNDResultModule extends JShawnModule {
	// public class ShawnNDResultModule { //for testing purpose
	private Category log = Logging.get(this);

	NDResult result_object_;

	String current_string_;

	int depth_;

	Vector<Double> indices_;

	boolean is_closing_tag_;

	String tag_name_prefix_;

	boolean is_axis_tag_;

	/**
	 * @return
	 */
	public String getName() {
		return "n_dimensional_result";
	}

	/**
	 * 
	 *
	 */
	public void start() {
		indices_ = new Vector<Double>();
		depth_ = 0;
	}

	/**
	 * @param buffer
	 */
	public String receive(String buffer) throws Exception {
		current_string_ = buffer;
		if (!(current_string_ == null || current_string_.equals(""))) {
			try {
				evaluate_string();
			} catch (Exception e) {
				log.error("receive failed, buffer[" + buffer + "]:" + e, e);
			}
		}
		return null;
	}

	/**
	 * 
	 *
	 */
	public ShawnResult end() {
		if (result_object_ != null)
			log.info("Done. Returning result object");
		else
			log.warn("Done. No result object available. Parsing failed.");
		return result_object_;
	}

	private void evaluate_string() throws Exception {
		evaluateTagState();
		evaluateTag();
		if (is_axis_tag_ == true) {
			if (is_closing_tag_ == false) {
				evaluateAxisTag();
			}
		} else {
			evaluateIndexTag();
		}

	}

	private void evaluateTagState() throws Exception {
		if (current_string_.startsWith("<")) {
			is_closing_tag_ = false;
			tag_name_prefix_ = "<";
		} else {
			throw new Exception("ShawnNDResultModule.evaluate_tag_state: bad line - doesn't begin with opening tag");
		}
		if (current_string_.startsWith("</")) {
			is_closing_tag_ = true;
			tag_name_prefix_ = "</";
		}
	}

	private void evaluateTag() throws Exception {
		if (current_string_.startsWith(tag_name_prefix_ + "axis")) {
			is_axis_tag_ = true;
		} else {
			if (current_string_.startsWith(tag_name_prefix_ + "index")) {
				is_axis_tag_ = false;
			} else {
				throw new Exception("ShawnNDResultModule.evaluate_tag: wrong tag name - accepts only \"axis\" or \"index\"");
			}
		}
	}

	private void evaluateAxisTag() throws Exception {
		NDResult ndr = new NDResult();
		log.debug("Created new Result");
		ndr.setAxisLabel(evaluateName());
		if (evaluateIsAxisValue()) {
			ndr.addValue(evaluateValue());
			if (log.isDebugEnabled())
				log.debug("Adding value v=" + evaluateValue());
		}
		addAxisToResultObject(ndr);
	}

	private void evaluateIndexTag() throws Exception {
		if (is_closing_tag_ == true) {
			depth_--;
			indices_.removeElementAt(indices_.size() - 1);
		} else {
			depth_++;
			indices_.add(new Double(evaluateValue()));
		}

	}

	private boolean evaluateIsAxisValue() throws Exception {
		if (current_string_.contains("is_value=\"true\"")) {
			current_string_.replace("is_value=\"true\"", "");
			return true;
		}
		if (current_string_.contains("is_value=\"false\"")) {
			current_string_.replace("is_value=\"false\"", "");
			return false;
		}
		throw new Exception("ShawnNDResultModule.evaluate_is_axis_value: no value definition given in axis tag");
	}

	private double evaluateValue() throws Exception {
		if (current_string_.contains(" value=\"")) {
			int i = current_string_.indexOf(" value=\""); // <axis .... value="123">
			String temp = current_string_.substring(i + 8); // 123">

			i = temp.indexOf("\"");
			temp = temp.substring(0, i);

			return (new Double(temp)).doubleValue();

		}
		// else
		throw new Exception("ShawnNDResultModule.evaluate_value: no value given in assigned value tag");
	}

	private String evaluateName() {
		if (current_string_.contains(" name=\"")) {
			int i = current_string_.indexOf(" name=\""); // <axis .... name="123">
			String temp = current_string_.substring(i + 7); // 123">
			i = temp.indexOf("\"");
			return temp.substring(0, i);
		}
		return ("default_name");

	}

	private void addAxisToResultObject(NDResult axis) {
		// If we are on the upper layer
		if (depth_ == 0) {
			if (log.isDebugEnabled())
				log.debug("Axis: " + axis.toXmlString());
			result_object_ = axis;
			if (log.isDebugEnabled())
				log.debug("Set result in depth " + depth_);
			return;
		}
		// else
		try {
			if (log.isDebugEnabled())
				log.debug("Added Axis in depth " + depth_);
			result_object_.addAxis(axis, getIndices());
		} catch (Exception e) {
			log.error("Failed in depth " + depth_ + ": " + e, e);
		}

	}

	private double[] getIndices() {
		int size = indices_.size();
		double[] result = new double[size];
		for (int i = 0; i < size; i++) {
			result[i] = ((Double) indices_.elementAt(i)).doubleValue();
			if (log.isDebugEnabled())
				log.debug("result[" + i + "] : " + result[i]);
		}
		return result;
	}

	public static void main(String[] args) throws Exception {
		ShawnNDResultModule srm = new ShawnNDResultModule();
		srm.start();
		srm.receive("<axis name=\"rootelement\" is_value=\"true\" value=\"12345\">");
	}
}
