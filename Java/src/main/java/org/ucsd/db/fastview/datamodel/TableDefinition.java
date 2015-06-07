package org.ucsd.db.fastview.datamodel;

import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;

public class TableDefinition {
    private Map<String, AggregatedAttribute> columnNameToAggregatedAttributeMap;
    private int[] offsetAttributes;
    private HashMap<String, JoinTupleDefinition> joinDefMap = new HashMap<String, JoinTupleDefinition>();
    private List<Integer> groupByAttributes;
    private Collection<AggregatedAttribute> aggregatedAttributes;
    
    public void setAggregatedAttributes(Map<String, AggregatedAttribute> columnNameToAggregatedAttributeMap) {
        this.columnNameToAggregatedAttributeMap = columnNameToAggregatedAttributeMap;
    }
    public int[] getOffsetAttributes() {
        return offsetAttributes;
    }
    public void setOffsetAttributes(int[] offsetAttributes) {
        this.offsetAttributes = offsetAttributes;
    }
    public AggregatedAttribute getAggregatedAttribute(String aggregatedAttribute) {
        return columnNameToAggregatedAttributeMap.get(aggregatedAttribute);
    }
    public Collection<AggregatedAttribute> getAggregatedAttributes() {
        return aggregatedAttributes != null ? aggregatedAttributes : 
            (aggregatedAttributes = columnNameToAggregatedAttributeMap.values());
    }
    public HashMap<String, JoinTupleDefinition> getJoinDefinitionMap() {
        return joinDefMap;
    }
    public List<Integer> getGroupByAttributes() {
        return groupByAttributes;
    }
    public void setGroupByAttributes(List<Integer> groupByAttributes) {
        this.groupByAttributes = groupByAttributes;
    }
}