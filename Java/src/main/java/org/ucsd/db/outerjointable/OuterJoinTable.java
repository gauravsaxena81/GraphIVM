package org.ucsd.db.outerjointable;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

public class OuterJoinTable {
    public static final String UNIQUE = "UNIQUE";
    private List<Object[]> tuples = new ArrayList<Object[]>();
    private List<String> attributes = new ArrayList<>();
    private JoinGraph joinGraph;
    private HashMap<String, Integer> indexOffset = new HashMap<>();
    private HashMap<List<Object>, Integer> uniqueTupleMap = new HashMap<>();
    
    public OuterJoinTable() {
        this(512);
    }
    
    public OuterJoinTable(int numRows) {
        tuples = new ArrayList<>(numRows);
    }
    //TODO Consider not exposing HashMap. Instead, expose an API to get offset
    public HashMap<String, Integer> getIndexOffset() {
        return indexOffset;
    }

    public void setIndexOffset(HashMap<String, Integer> indexOffset) {
        this.indexOffset = indexOffset;
    }
    public List<Object[]> getTuples() {
        return tuples;
    }
    public void setTuples(List<Object[]> tuples) {
        this.tuples = tuples;
    }
    public List<String> getAttributes() {
        return attributes;
    }
    public void setAttributes(List<String> attributes) {
        this.attributes = attributes;
    }

    public JoinGraph getJoinGraph() {
        return joinGraph;
    }

    public void setJoinGraph(JoinGraph joinGraph) {
        this.joinGraph = joinGraph;
    }

    public void add(Object[] attributeValues) {
        tuples.add(attributeValues);
        for(String otherRelationName : indexOffset.keySet()) {
            Integer index = indexOffset.get(otherRelationName);
            uniqueTupleMap.put(Arrays.asList(Arrays.copyOfRange(attributeValues, index
                , index + joinGraph.getJoinAttributeValues(otherRelationName).size() + 
                joinGraph.getProjectedAttributeValues(otherRelationName).size())), tuples.size() - 1);
        }
    }

    public Integer getUniqueIndex(int tupleIndex, String relationName) {
        Object[] attributeValues = tuples.get(tupleIndex);
        Integer index = indexOffset.get(relationName);
        return uniqueTupleMap.get(Arrays.asList(Arrays.copyOfRange(attributeValues, index
                        , index + joinGraph.getJoinAttributeValues(relationName).size() + 
                        joinGraph.getProjectedAttributeValues(relationName).size())));
    }
}
