/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

package sf.net.shawn.tools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.poi.hssf.usermodel.HSSFCell;
import org.apache.poi.hssf.usermodel.HSSFRow;
import org.apache.poi.hssf.usermodel.HSSFSheet;
import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.poifs.filesystem.POIFSFileSystem;

import sf.net.shawn.shawn.NDResult;

import com.Ostermiller.util.CSVParser;


public class ExcelExport {
	private File excelFile = null;

	private HSSFWorkbook wb = null;

	private HSSFSheet activeSheet = null;

	/**
	 * 
	 * @param f
	 * @throws FileNotFoundException
	 * @throws IOException
	 */
	public ExcelExport(File f) throws FileNotFoundException, IOException {
		excelFile = f;

		if (f.exists()) {
			POIFSFileSystem fs = new POIFSFileSystem(new FileInputStream(excelFile));
			wb = new HSSFWorkbook(fs);
		} else {
			wb = new HSSFWorkbook();
		}

	}

	/**
	 * 
	 * @param sheetName
	 */
	public void selectWorksheet(String sheetName) {
		activeSheet = wb.getSheet(sheetName);

		if (activeSheet == null)
			activeSheet = wb.createSheet(sheetName);
	}

	/**
	 * 
	 * @throws IOException
	 */
	public void save() throws IOException {
		FileOutputStream fileOut = new FileOutputStream(excelFile);
		wb.write(fileOut);
		fileOut.close();

	}

	/**
	 * 
	 * @param row
	 * @param col
	 * @param val
	 */
	public void setValue(short row, short col, String val) {
		HSSFCell cell = getOrCreateCell(row, col);

		try {
			String tmp = val.replace(',', '.');
			cell.setCellValue(Double.parseDouble(tmp));
		} catch (NumberFormatException nfe) {
			cell.setCellValue(val);
		}
	}

	/**
	 * 
	 * @param row
	 * @param col
	 * @param formula
	 */
	public void setFormula(short row, short col, String formula) {
		HSSFCell cell = getOrCreateCell(row, col);
		cell.setCellFormula(formula);
	}

	/**
	 * 
	 * @param rowOffset
	 * @param colOffset
	 * @param cvsFile
	 * @throws IOException 
	 */
	public void addCsvFile(int rowOffset, int colOffset, File csvFile, char delim, String comments) throws IOException {
		CSVParser shredder = new CSVParser(new InputStreamReader(new FileInputStream(csvFile)));
		shredder.setCommentStart(comments);
		shredder.changeDelimiter(delim);
		short row = (short) rowOffset;
		String[] vals = null;

		while ((vals = shredder.getLine()) != null) {
			for (short col = 0; col < vals.length; col++)
				setValue(row, (short) (col + colOffset), vals[col]);
			row++;
		}
	}

	/**
	 * 
	 * @param row
	 * @param col
	 * @return
	 */
	public HSSFCell getOrCreateCell(short row, short col) {
		HSSFRow exrow = activeSheet.getRow(row);

		if (exrow == null)
			exrow = activeSheet.createRow(row);

		HSSFCell excell = exrow.getCell(col);

		if (excell == null)
			excell = exrow.createCell(col);

		return excell;
	}

	/**
	 * 
	 * @param rowOffset
	 * @param colOffset
	 * @param cvsFile
	 * @throws IOException 
	 */
	public void addNDResult(int rowOffset, int colOffset, NDResult res) throws IOException {

		//TODO machen 
		throw new IOException();

	}
}

/*+---------------------------------------------------------------+
 *| Source  $Source: /cvs/jshawn/src/de/farberg/tools/ExcelExport.java,v $  
 *| Version $Revision: 1.3 $ modified by $Author: pfister $
 *| Date    $Date: 2007/01/11 12:47:14 $
 *+---------------------------------------------------------------
 *| $Log: ExcelExport.java,v $
 *| Revision 1.3  2007/01/11 12:47:14  pfister
 *| *** empty log message ***
 *|
 *| Revision 1.2  2006/01/26 09:23:44  lipphardt
 *| addNDResult implemented
 *|
 *| Revision 1.1  2005/12/01 08:06:54  pfister
 *| *** empty log message ***
 *|
 *+---------------------------------------------------------------*/