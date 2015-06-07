package org.ucsd.db.fastview.tableloader;

import org.ucsd.db.fastview.datamodel.CntTable;

public class CntTableAndIndices {
    public CntTable cntTable = new CntTable();
    
	public CntTable getCntTable() {
        return cntTable;
    }
    public void setCntTable(CntTable cntTable) {
        this.cntTable = cntTable;
    }
}