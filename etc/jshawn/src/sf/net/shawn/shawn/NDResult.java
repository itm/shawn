/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
package sf.net.shawn.shawn;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.PrintStream;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;
import java.util.Vector;

import org.apache.log4j.Category;

import sf.net.shawn.module.ShawnNDResultModule;
import sf.net.shawn.tools.Logging;

/**
 * @author lipphardt
 */
public class NDResult extends ShawnResult {
	private Category log = Logging.get(this);

	// /
	String axis_label_ = "default_axis";

	// /
	String description_ = "";

	// /
	boolean is_value_ = false;

	// /
	double value_ = 0.0;

	// /
	// HashMap<Double, NDResult> values_ = new HashMap<Double, NDResult>();
	TreeMap<Double, NDResult> values_ = new TreeMap<Double, NDResult>();

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public void addValue(double value, double[] indices) throws Exception {
		int size = indices.length;
		if (size == 0) {
			// equals the "=" assign operator
			if (values_.size() > 0)
				throw new Exception("add_value: An index can not be assigned a value");

			is_value_ = true;
			value_ = value;
			return;

		} else {

			if (is_value_ == true) {
				throw new Exception("add_value: A value cannot be indexed");
			}
			// getting remaining indices (without the first)
			double remaining_indices[] = new double[size - 1];

			for (int i = 1; i < size; i++) {
				remaining_indices[i - 1] = indices[i];
			}
			try {
				NDResult n = values_.get(indices[0]);
				// As far as it is not initialized... let's do so...
				if (n == null) {
					n = new NDResult();
					values_.put(indices[0], n);
				}
				n.addValue(value, remaining_indices);
			} catch (Exception e) {
				System.out.println("add_value: failed: with current indices_size of " + size + ": " + e.getMessage());
			}
		}

	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public void addValue(double value) throws Exception {
		double indices[] = {};
		addValue(value, indices);
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public double getValue(double[] indices) throws Exception {
		int size = indices.length;
		if (size == 0) {
			if (is_value_ == false)
				throw new Exception("get_value: Not a value type");
			else
				return value_;
		} else {
			if (is_value_ == true)
				throw new Exception("get_value: Too many dimensions");

			// getting remaining indices (without the first)
			double remaining_indices[] = new double[size - 1];

			for (int i = 1; i < size; i++)
				remaining_indices[i - 1] = indices[i];

			try {
				return values_.get(indices[0]).getValue(remaining_indices);
			} catch (Exception e) {
				System.out.println("get_value: failed: with current indices_size of " + size + ": " + e.getMessage());
			}
		}
		return -1;
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public double getValue() throws Exception {
		double indices[] = {};
		return getValue(indices);
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public void addAxis(NDResult ndr, double[] indices) throws Exception {
		int size = indices.length;
		if (size == 0) {
			throw new Exception("add_dimension: no key given - don't use empty array");
		} else {
			if (size == 1) {
				values_.put(indices[0], ndr);
			} else { // size >1
				double remaining_indices[] = new double[size - 1];

				for (int i = 1; i < size; i++)
					remaining_indices[i - 1] = indices[i];

				try {
					// As far as it is not initialized... let's do so...
					NDResult r = values_.get(indices[0]);

					if (r == null) {
						r = new NDResult();
						values_.put(indices[0], r);
					}
					values_.get(indices[0]).addAxis(ndr, remaining_indices);

				} catch (Exception e) {
					System.out.println("add_dimension: failed: with current indices_size of " + size + ": " + e.getMessage());
				}
			}

		}

	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public NDResult getAxis(double[] indices) throws Exception {
		int size = indices.length;
		if (size == 0) {
			throw new Exception("get_axis: no key given - don't use empty array");
		} else {
			if (size == 1) {
				return (NDResult) values_.get(new Double(indices[0]));
			} else { // size >1
				double remaining_indices[] = new double[size - 1];

				for (int i = 1; i < size; i++)
					remaining_indices[i - 1] = indices[i];

				try {
					return values_.get(indices[0]).getAxis(remaining_indices);
				} catch (Exception e) {
					System.out.println("get_axis: failed: with current indices_size of " + size + ": " + e.getMessage());
				}
			}
		}
		return null;
	}

	/**
	 * @param indices
	 * @return
	 * @throws Exception
	 */
	public NDResult getAxisByIndex(int[] indices) throws Exception {
		int size = indices.length;
		if (size == 0) {
			throw new Exception("getAxisByIndex(int[]): no index given - don't use empty array");
		} else {
			if (size == 1) {
				return getAxisByIndex(indices[0]);
			} else {
				int remaining_indices[] = new int[size - 1];

				for (int i = 1; i < size; i++)
					remaining_indices[i - 1] = indices[i];

				try {
					return getAxisByIndex(indices[0]).getAxisByIndex(remaining_indices);
				} catch (Exception e) {
					System.out.println("getAxisByIndex(int[]) failed: with current indices_size of " + size + ": " + e.getMessage());
				}
			}
		}
		return null;
	}

	/**
	 * @param index
	 * @return
	 * @throws Exception
	 */
	public NDResult getAxisByIndex(int index) throws Exception {
		int mapsize = values_.size();
		if (index >= mapsize) {
			throw new Exception("getAxisByIndex(int): index out of bounds");
		}
		Iterator keyValuePairs = values_.entrySet().iterator();
		Map.Entry entry = null;
		// Iterating, till iterator is on the entry with the given index
		for (int i = 0; i <= index; i++) {
			entry = (Map.Entry) keyValuePairs.next();
		}
		return (NDResult) entry.getValue();
	}

	/**
	 * @param indices
	 * @return
	 * @throws Exception
	 */
	public double[] getKeyByIndex(int[] indices) throws Exception {
		int size = indices.length;
		double result[] = new double[size];
		if (size == 0) {
			throw new Exception("getKeyByIndex(int[]): no index given - don't use empty array");
		} else {

			result[0] = getKeyByIndex(indices[0]);

			if (size > 1) {
				double temp[] = new double[size - 1];
				int remaining_indices[] = new int[size - 1];

				for (int i = 1; i < size; i++)
					remaining_indices[i - 1] = indices[i];

				try {
					temp = getAxisByIndex(indices[0]).getKeyByIndex(remaining_indices);
				} catch (Exception e) {
					System.out.println("getKeyByIndex(int[]) failed: with current indices_size of " + size + ": " + e.getMessage());
				}
				for (int i = 1; i < size; i++) {
					result[i] = temp[i - 1];
				}
			}
		}
		return null;
	}

	/**
	 * @param index
	 * @return
	 * @throws Exception
	 */
	public double getKeyByIndex(int index) throws Exception {
		int mapsize = values_.size();
		if (index >= mapsize) {
			throw new Exception("getKeyByIndex(int): index out of bounds");
		}
		Iterator keyValuePairs = values_.entrySet().iterator();
		Map.Entry entry = null;
		// Iterating, till iterator is on the entry with the given index
		for (int i = 0; i <= index; i++) {
			entry = (Map.Entry) keyValuePairs.next();
		}
		return ((Double) entry.getKey()).doubleValue();
	}

	/**
	 * @return
	 */
	public int getMapSize() {
		if (is_value_ == true)
			return 0;
		return values_.size();
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public void setAxisLabel(String label) {
		axis_label_ = label;
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public String getAxisLabel() {
		return axis_label_;
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public void setDescription(String description) {
		description_ = description;
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public String description() {
		return description_;
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public int dimension() {
		if (is_value_ == true)
			return 1;

		int mapsize = values_.size();
		int max_dim = 0;

		Iterator keyValuePairs = values_.entrySet().iterator();
		for (int i = 0; i < mapsize; i++) {
			Map.Entry entry = (Map.Entry) keyValuePairs.next();
			// receiving the pairs from the map
			int dim = ((NDResult) entry.getValue()).dimension();
			if (dim > max_dim) {
				max_dim = dim;
			}
		}
		return 1 + max_dim;
	}

	// -----------------------------------------------------------------------
	/**
	 * The getAllAxisLabels-method retrieves the axis labels from all first elements in every dimension and puts them
	 * into a String sperated by the given seperator
	 */
	public String getAllAxisLabels(String seperator) {
		if (is_value_ == true) {
			return axis_label_;
		}
		Iterator keyValuePairs = values_.entrySet().iterator();
		Map.Entry entry = (Map.Entry) keyValuePairs.next(); // getting the first element
		NDResult value = (NDResult) entry.getValue();
		return axis_label_ + seperator + value.getAllAxisLabels(seperator);
	}

	// ------------------------------------------------------------------------
	/**
	 * Returns the NDResult in form of an XMLString
	 */
	public String toXmlString() {
		String xml_string = "";
		String lineSep = System.getProperty("line.separator");

		xml_string = xml_string + "<axis name=\"" + axis_label_ + "\" ";
		if (is_value_ == true) {
			xml_string = xml_string + "is_value=\"true\" value=\"" + value_ + "\">" + lineSep;
		} else {
			xml_string = xml_string + "is_value=\"false\">" + lineSep;

			int mapsize = values_.size();

			Iterator keyValuePairs = values_.entrySet().iterator();
			for (int i = 0; i < mapsize; i++) {
				Map.Entry entry = (Map.Entry) keyValuePairs.next();
				// receiving the pairs from the map
				double key = ((Double) entry.getKey()).doubleValue();
				NDResult value = (NDResult) entry.getValue();
				// proceeding with generating output
				xml_string = xml_string + "<index value=\"" + key + "\">" + lineSep;

				// rekursuve call:
				xml_string = xml_string + value.toXmlString();

				xml_string = xml_string + "</index>" + lineSep;
			}

		}
		xml_string = xml_string + "</axis>" + lineSep;

		return xml_string;
	}

	// ------------------------------------------------------------------------
	/**
	 * Returns a String representing the NDResult in form of a table, each column seperated by the seperator
	 */
	public String toTableString(String seperator) {
		String table_string = "";
		String lineSep = System.getProperty("line.separator");

		String column_names = getAllAxisLabels(seperator);

		column_names = column_names + lineSep;

		Vector<String> table_string_vector = toTableStringHelper(seperator);

		int vectorsize = table_string_vector.size();
		for (int j = 0; j < vectorsize; j++) {
			table_string = table_string + table_string_vector.elementAt(j) + lineSep;
		}

		return column_names + table_string;
	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	protected Vector<String> toTableStringHelper(String seperator) {
		Vector<String> result_string_vector = new Vector<String>();

		if (is_value_ == true) {
			String result = new Double(value_).toString();
			result_string_vector.add(result);
		}

		else { // getting into "<index....> level" see xml

			int mapsize = values_.size();

			Iterator keyValuePairs = values_.entrySet().iterator();
			for (int i = 0; i < mapsize; i++) {
				Map.Entry entry = (Map.Entry) keyValuePairs.next();
				// receiving the pairs from the map
				double key = ((Double) entry.getKey()).doubleValue();
				NDResult value = (NDResult) entry.getValue();

				// recursion
				// receive a stringvector where the first String is always the columnnames
				Vector<String> temp_string_vector = value.toTableStringHelper(seperator);

				int vectorsize = temp_string_vector.size();
				for (int j = 0; j < vectorsize; j++) { // The first argument was already used
					result_string_vector.add(key + seperator + temp_string_vector.elementAt(j));
				}

			}

		}

		return result_string_vector;
	}

	/**
	 * @param filename
	 */

	public void saveToXmlFile(String filename) {
		FileOutputStream out; // declare a file output object
		PrintStream p; // declare a print stream object

		try {
			out = new FileOutputStream(filename);
			p = new PrintStream(out);

			p.println(toXmlString());

			p.close();
		} catch (Exception e) {
			log.error("Error while writing to file" + e, e);
		}
	}

	/**
	 * @param filename
	 */

	public void saveToCSVFile(String filename) {
		FileOutputStream out; // declare a file output object
		PrintStream p; // declare a print stream object

		try {
			out = new FileOutputStream(filename);
			p = new PrintStream(out);

			p.println(toTableString(", "));

			p.close();
		} catch (Exception e) {
			log.error("Error while writing to file" + e, e);
		}
	}

	/**
	 * @param filename
	 * @return
	 */

	public static NDResult readFromXmlFile(String filename) {
		ShawnNDResultModule tempModule = new ShawnNDResultModule();
		tempModule.start();
		try {
			BufferedReader input = new BufferedReader(new FileReader(filename));

			// Continue to read lines while
			// there are still some left to read
			String line = null;
			while ((line = input.readLine()) != null) {
				tempModule.receive(line);
			}

			input.close();
		} catch (Exception e) {
			System.err.println("Error while reading file" + e.getMessage());

		}
		return (NDResult) tempModule.end();
	}

	/**
	 * @param ndr
	 */
	public void add(NDResult ndr) {
		int size = ndr.getMapSize();
		if (size == 0) {
			System.out.println("ERROR SIZE == 0"); // TODO
		}
		setAxisLabel(ndr.getAxisLabel());
		for (int index = 0; index < size; index++) {
			double keys[] = new double[1];
			int depth = 1;
			try {
				keys[0] = ndr.getKeyByIndex(index);
				add(ndr.getAxis(keys), keys, depth);
			} catch (Exception e) {
				log.error("Error while adding" + e, e);
			}
		}
	}

	private void add(NDResult ndr, double[] keys, int depth) throws Exception {

		int size = ndr.getMapSize();
		if (size == 0) { // equals is_value==true
			addValue(ndr.getValue(), keys); // end of recursion
			getAxis(keys).setAxisLabel(ndr.getAxisLabel()); // setting the label after the adding of the Element
		} else {

			double deepKeys[] = new double[(depth + 1)];
			for (int d = 0; d < depth; d++) {
				deepKeys[d] = keys[d];
			}
			for (int index = 0; index < size; index++) {
				double singleKey[] = new double[1];
				singleKey[0] = ndr.getKeyByIndex(index);

				deepKeys[depth] = singleKey[0];
				add(ndr.getAxis(singleKey), deepKeys, depth + 1);
				getAxis(keys).setAxisLabel(ndr.getAxisLabel());
			}
		}

	}

	// ------------------------------------------------------------------------
	/**
	 * 
	 */
	public static void main(String[] args) {
		System.out.println("main:");
		double indices[] = { 0, 0 };
		NDResult nr = new NDResult();

		// FILLING THE MATRIX
		try {
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 4; j++) {
					indices[0] = i;
					indices[1] = j;
					nr.addValue(i * j, indices);
				}
			}
		} catch (Exception e) {
			System.out.println("Exception thrown: " + e.getMessage());
		}

		// OUTPUT
		try {
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 4; j++) {
					indices[0] = i;
					indices[1] = j;
					System.out.println("Index (" + i + "," + j + ") : " + nr.getValue(indices));
				}
			}
		} catch (Exception e) {
			System.out.println("Exception thrown: " + e.getMessage());
		}

		System.out.println("XML:   " + nr.toXmlString());

		// Saving and reading from file
		nr.saveToXmlFile("MyNDResult.xml");

		NDResult nd = NDResult.readFromXmlFile("MyNDResult.xml");
		System.out.println("Loaded file: " + nd.toTableString("; "));

		// FILLING THE MATRIX FOR adding
		NDResult b = new NDResult();
		try {
			for (int i = 0; i < 3; i++) {
				for (int j = 4; j < 6; j++) {
					indices[0] = i;
					indices[1] = j;
					b.addValue(i * j, indices);
				}
			}
		} catch (Exception e) {
			System.out.println("Exception thrown: " + e.getMessage());
		}
		System.out.println("nd: " + nd.toTableString("   "));
		nd.add(b);
		System.out.println("b : " + b.toTableString("   "));
		System.out.println("nd+b: " + nd.toTableString("   "));

	}
}
