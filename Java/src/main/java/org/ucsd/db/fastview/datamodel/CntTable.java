package org.ucsd.db.fastview.datamodel;

import java.util.ArrayList;
import java.util.List;

public class CntTable {
    
    private List<JoinTuple> joinTuples = new ArrayList<>();
    private int relationIndex = -1;
    private TableDefinition tableDefinition;
    
    public void add(JoinTuple t) {
        joinTuples.add(t);
        if(relationIndex == -1) {
            relationIndex = t.getIndex();
        }
    }

    public JoinTuple get(int i) {
        return joinTuples.get(i);
    }

    public void remove(int i) {
        joinTuples.set(i, null);
    }

    public int size() {
        return joinTuples.size();
    }

    public List<JoinTuple> getTuples() {
        return joinTuples;
    }
//    public HashSet<JoinTuple> getFirstTuple() {
//        return set;
//    }

    public void setTableDefinition(TableDefinition tableDefinition) {
        this.tableDefinition = tableDefinition;
    }

    public TableDefinition getTableDefinition() {
        return tableDefinition;
    }    
}
