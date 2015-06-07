package org.ucsd.db.deltaivm.datamodel;

import java.util.ArrayList;
import java.util.List;

import org.ucsd.db.fastview.datamodel.ArrayHashMap;
import org.ucsd.db.fastview.index.IntArrayList;

public final class RelationalTable {
    private List<Object[]> table = new ArrayList<>();
    private List<IndexedVertex<? extends Enum<?>>> indexedVertices;
    private IntArrayList projectedAttributeIndices;
    private ArrayHashMap<Object[], IntArrayList> projectedTupleIndex = new ArrayHashMap<>();
    
    public List<Object[]> getTable() {
        return table;
    }
    public List<IndexedVertex<? extends Enum<?>>> getIndexedVertices() {
        return indexedVertices;
    }
    public void setIndexedVertices(List<IndexedVertex<? extends Enum<?>>> indexedVertices) {
        this.indexedVertices = indexedVertices;
    }
    public IntArrayList getProjectedAttributeIndices() {
        return projectedAttributeIndices;
    }
    public void setProjectedAttributeIndices(IntArrayList projectedAttributeIndices) {
        this.projectedAttributeIndices = projectedAttributeIndices;
    }
    public ArrayHashMap<Object[], IntArrayList> getProjectedTupleIndex() {
        return projectedTupleIndex;
    }
}