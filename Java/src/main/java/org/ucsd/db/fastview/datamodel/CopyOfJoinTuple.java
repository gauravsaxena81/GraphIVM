package org.ucsd.db.fastview.datamodel;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.index.IntHashMap;

public class CopyOfJoinTuple implements Iterable<List<Object>>{
    public enum TYPE{
        JOIN_TUPLE,
        GROUPED_TUPLE,
        AGGREGATED_TUPLE;
    }
    public Object data;
    private final TYPE type;
    private VertexValue[] vertexValues;
	private int[] startPos;
	private int projectedTupleSize = -1;
    private int index = -1;
    //TODO use index of aggregation attribute rather than calculating it every time
    private List<String> projectedAttributeNamesList;
    private TableDefinition tableDefinition;
    private int localKnowledge = 0;
    private boolean globalKnowledge = true;
    private int leafTupleCount;
    private int newProjectedTupleIndex;
    private boolean lookAhead;
    private int[] leafTupleCounts;
    private List<Object> groupedAttributes;
    private IntHashMap<List<Object>> pMap = new IntHashMap<List<Object>>();
    
    public CopyOfJoinTuple(TYPE type, int index) {
        this.index = index; 
        this.type = type;
    }
    
    public CopyOfJoinTuple(int index) {
        this(TYPE.JOIN_TUPLE, index);
    }
    
	public VertexValue[] getVertexValues() {
        return vertexValues;
    }
    public void setVertexValues(VertexValue[] vertexValues) {
        this.vertexValues = vertexValues;
    }
    public void addLeafTuple(List<Object> projectedAttributeValues) {
        switch(type) {
            case JOIN_TUPLE:
                addLeafTupleJoin(projectedAttributeValues);
                break;
            case AGGREGATED_TUPLE:
                addLeafTupleAggregtated(projectedAttributeValues);
                break;
            case GROUPED_TUPLE:
                addLeafTupleGrouped(projectedAttributeValues);
                break;
        }
    }
    private void addLeafTupleJoin(List<Object> projectedAttributeValues) {
		if(pMap.increment(projectedAttributeValues, 1) == -1)
		    leafTupleCount++;
	}

    private void addLeafTupleGrouped(List<Object> projectedAttributeValues) {
        leafTupleCount++;
    }
    private void addLeafTupleAggregtated(List<Object> projectedAttributeValues) {
        projectedTupleSize = projectedAttributeValues.size();
        List<Object> groupedAttributes = getGroupedAttributes(projectedAttributeValues);
        for(int i = 0; i < projectedAttributeValues.size(); i++) {
            AggregatedAttribute aggregatedAttribute = tableDefinition.getAggregatedAttribute(projectedAttributeNamesList.get(i));
            if(aggregatedAttribute != null) {
                switch(aggregatedAttribute.function) {
                    case SUM:
                        doSum(groupedAttributes, projectedAttributeValues, i, 1);
                        break;
                    case COUNT:
                        doCount(groupedAttributes, projectedAttributeValues, i, 1);
                        break;
                }
            }
        }
    }

    private boolean doCount(List<Object> groupedAttributes, List<Object> projectedAttributeValues, int aggregatedColumnIndex, int posNeg) {
        int count = pMap.increment(groupedAttributes, posNeg);
        if(count == -1) {
            leafTupleCount++;
            return true;
        } else {
            if(count == 1)
                return removeLeafTuple(projectedAttributeValues);
            else
                return false;
        }
    }

    private boolean doSum(List<Object> groupedAttributes, List<Object> projectedAttributeValues, int aggregatedColumnIndex, int posNeg) {
        int count = pMap.increment(groupedAttributes, posNeg * (int)projectedAttributeValues.get(aggregatedColumnIndex));
        if(count == -1) {
            leafTupleCount++;
            return true;
        } else {
            if(count == 1)
                return removeLeafTuple(groupedAttributes);
            else
                return false;
        }
    }

	public <E extends Enum<E>> void addJoinedTuple(VertexValue<E> i) {
	    int indexOf = FastViewUtil.indexOf(vertexValues, i);
	    if(indexOf > -1)
	        localKnowledge |= 1 << indexOf;
	    setLookAhead();
	}
	public boolean deleteLeafTuple(List<Object> projectedAttributeValues) {
	    switch(type) {
            case JOIN_TUPLE:
                return deleteLeafTupleJoin(projectedAttributeValues);
            case AGGREGATED_TUPLE:
                return deleteLeafTupleAggregtated(projectedAttributeValues);
            case GROUPED_TUPLE:
                return deleteLeafTupleGrouped(projectedAttributeValues);
        }
        return false;
	}
	private boolean deleteLeafTupleGrouped(List<Object> projectedAttributeValues) {
        return --leafTupleCount == 0;
    }

    private boolean deleteLeafTupleAggregtated(List<Object> projectedAttributeValues) {
	    List<Object> groupedAttributes = getGroupedAttributes(projectedAttributeValues);
        for(int i = 0; i < projectedAttributeValues.size(); i++) {
            AggregatedAttribute aggregatedAttribute = tableDefinition.getAggregatedAttribute(projectedAttributeNamesList.get(i));
            if(aggregatedAttribute != null) {
                switch(aggregatedAttribute.function) {
                    case SUM:
                        return doSum(groupedAttributes, projectedAttributeValues, i, -1);
                    case COUNT:
                        return doCount(groupedAttributes, projectedAttributeValues, i, -1);
                }
            }
        }
        return false;
    }

    private List<Object> getGroupedAttributes(List<Object> projectedAttributeValues) {
        if(groupedAttributes == null) {
            groupedAttributes = new ArrayList<Object>();
            for(int i = 0; i < projectedAttributeValues.size(); i++) {
                if(null == tableDefinition.getAggregatedAttribute(projectedAttributeNamesList.get(i)))
                    groupedAttributes.add(projectedAttributeValues.get(i));
            }
        }
        return groupedAttributes;
    }

    private boolean deleteLeafTupleJoin(List<Object> projectedAttributeValues) {
        if(pMap.increment(projectedAttributeValues, -1) == 1)
            return removeLeafTuple(projectedAttributeValues);
        else
            return false;
	}

    private boolean removeLeafTuple(List<Object> projectedAttributeValues) {
        pMap.remove(projectedAttributeValues);
        return --leafTupleCount == 0;
    }

    public void removeJoinedTuple(VertexValue vertexValue, boolean isListEmpty) {
        //List<JoinTuple> list = vertexValueToJoinedTuplesMap.get(vertexValue);
//        int indexOf = vertexValues.indexOf(vertexValue);
//        List<JoinTuple> list = joinTuplesArray[indexOf];
        if(isListEmpty) {
            localKnowledge &= ~(1 << FastViewUtil.indexOf(vertexValues, vertexValue));
            setLookAhead();
        }
    }
//    public List<List<Object>> getProjectedTupleList() {
//        return projectedTupleList;
//    }
    public int getIndex() {
        return index;
    }

    /**
     * Only being used by getTupleDfs and therefore not required
     * @return
     */
    public int getProjectedTupleListSize() {
        return leafTupleCount;
    }
    public void setProjectedAttributeNames(List<String> projectedAttributeNamesList) {
        this.projectedAttributeNamesList = projectedAttributeNamesList;
    }
    public List<String> getProjectedAttributeNames() {
        return this.projectedAttributeNamesList;
    }
    public void setTableDefinition(TableDefinition tableDefinition) {
        this.tableDefinition = tableDefinition;
    }
    public boolean isComplete() {
        return lookAhead;
    }
    public void setLookAhead() {
        lookAhead = (localKnowledge == ((1 << vertexValues.length) - 1)) & globalKnowledge;
    }
    public void setGlobalKnowledge(boolean globalKnowledge) {
        this.globalKnowledge = globalKnowledge;
        setLookAhead();
    }
    public boolean isJoinTuple() {
        return type == TYPE.JOIN_TUPLE;
    }
    public boolean isAggregatedTuple() {
        return type == TYPE.AGGREGATED_TUPLE;
    }
    public boolean isGroupedTuple() {
        return type == TYPE.GROUPED_TUPLE;
    }
    public int getLeafTupleCount() {
        return leafTupleCount;
    }
    @Override
    public Iterator<List<Object>> iterator() {
        return pMap.keySet().iterator();
    }
}