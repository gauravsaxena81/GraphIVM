package org.ucsd.db.fastview.datamodel;

import java.util.Iterator;
import java.util.List;

public class ProjectedTupleIterator implements Iterator<List<Object>> {

    private int[] startPos;
    private int projectedTupleSize;
    private List<Object> table;
    private int leafTupleCount;
    private int j = 0;
    private int[] leafTupleCounts;

    public ProjectedTupleIterator(List<Object> table, int[] startPos, int projectedTupleSize, int leafTupleCount, int[] leafTupleCounts) {
        this.startPos = startPos;
        this.projectedTupleSize = projectedTupleSize;
        this.table = table;
        this.leafTupleCount = leafTupleCount;
        j = 0;
        this.leafTupleCounts = leafTupleCounts;
    }
    
    public void reset(int leafTupleCount, int[] startPos, int[] leafTupleCounts) {
        j = 0;
        this.leafTupleCount = leafTupleCount;
        this.startPos = startPos;
        this.leafTupleCounts = leafTupleCounts;
    }

    @Override
    public boolean hasNext() {
        return j < leafTupleCount;
    }

    @Override
    public List<Object> next() {
        while(j < leafTupleCount && startPos[j] < 0)
            j++;
        return table.subList(startPos[j], startPos[j++] + projectedTupleSize);
    }

    @Override
    public void remove() {
        
    }
    public int getLeafTupleRepeatCount() {
        return leafTupleCounts[j - 1];
    }

}
