package org.ucsd.db.fastview.index;


public class IndexLeaf {
    private static final IntArrayList EMPTY_LIST = new IntArrayList();
    private IntArrayList[] leafEntries = new IntArrayList[2];
    private String firstRelation = null;
    private String secondRelation = null;
	
	public IntArrayList getTupleIndexes(String relationName) {
	    IntArrayList ret = null; 
	    if(relationName.equals(firstRelation))
	        ret = leafEntries[0];
	    else if(leafEntries[1] != null)
	        ret = leafEntries[1];
	    
	    if(ret == null)
	        return EMPTY_LIST;
	    else
	        return ret;
	}
	
	public IntArrayList getTupleIndexesOfOtherRelation(String relationName) {
	    IntArrayList ret = null;
	    if(relationName.equals(firstRelation))
            ret = leafEntries[1];
        else if(leafEntries[0] != null)
            ret = leafEntries[0];
        if(ret == null)
            return EMPTY_LIST;
        else
            return ret;
	}

	public void add(String relationName, int joinTupleIndex) {
		IntArrayList rowEntries = getTupleIndexes(relationName);
		if(rowEntries == EMPTY_LIST) {
		    rowEntries = new IntArrayList();
		    if(firstRelation == null) {
		        firstRelation = relationName;
		        leafEntries[0] = rowEntries;
		    } else {
		        secondRelation = relationName;
	            leafEntries[1] = rowEntries;
		    }
		}
		rowEntries.add(joinTupleIndex);
	}

    public void delete(String relationName, int rowId) {
        IntArrayList rowEntries = getTupleIndexes(relationName);
        if(rowEntries != null)
            rowEntries.removeValue(rowId);
    }
    
    public String getOtherRelationName(String relationName) {
        if(relationName.equals(firstRelation))
            return secondRelation;
        else
            return firstRelation;
    }
}