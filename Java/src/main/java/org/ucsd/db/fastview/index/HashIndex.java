package org.ucsd.db.fastview.index;

import java.util.HashMap;

import org.ucsd.db.fastview.datamodel.VertexValue;

public class HashIndex implements IIndex {
    
    private HashMap<VertexValue, IndexLeaf> index = new HashMap<>();

    @Override
    public IndexLeaf getLeaf(VertexValue vertexValue) {
        return index.get(vertexValue);
    }

    @Override
    public IndexLeaf add(VertexValue vertexValue, String relationName, int joinTupleIndex) {
        IndexLeaf indexLeaf = index.get(vertexValue);
        if(indexLeaf == null) {
            indexLeaf = new IndexLeaf();
            index.put(vertexValue, indexLeaf);
        }
        indexLeaf.add(relationName, joinTupleIndex);
        return indexLeaf;
    }

    @Override
    public IndexLeaf delete(VertexValue vertexValue, String relationName, int joinTupleIndex) {
        IndexLeaf indexLeaf = index.get(vertexValue);
        indexLeaf.delete(relationName, joinTupleIndex);
        if(indexLeaf.getTupleIndexes(relationName).size() == 0 &&
                indexLeaf.getTupleIndexesOfOtherRelation(relationName).size() == 0)
            index.remove(vertexValue);
        return indexLeaf;
    }
}