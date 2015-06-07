package org.ucsd.db.fastview.datamodel;

import java.util.ArrayList;
import java.util.List;

import org.ucsd.db.fastview.delta.FastViewUtil;

public class JoinTuple implements Iterable<List<Object>>{
    public enum TYPE{
        JOIN_TUPLE,
        GROUPED_TUPLE,
        AGGREGATED_TUPLE;
    }
    class Wrapper{
        private int h;
        private int startPosition;
        private int size;
        private int projectedTupleIndex;
        
        Wrapper(){}
        Wrapper(Wrapper tempWrapper){
            this.startPosition = tempWrapper.startPosition;
            this.size = tempWrapper.size;
            this.h = tempWrapper.h;
        }
        private void init(int startPosition, int size) {
            this.startPosition = startPosition;
            this.size = size;
            h =  31 + JoinTuple.this.h;
            for(int i = startPosition, max = i + size; i < max; i++)
                h =  h * 31 + MemoryManager.tables[index].get(i).hashCode(); 
        }
        JoinTuple getParent() {
            return JoinTuple.this;
        }
        private int getSize() {
            return size;
        }
        private int getStartPosition() {
            return startPosition;
        }
        public int hashCode() {
            return h;
        }
        public boolean equals(Object o) {
            Wrapper w = (Wrapper)o;
            if((JoinTuple.this == w.getParent()) && size == w.getSize()) {
                List<Object> table = MemoryManager.tables[index];
                for(int start1 = startPosition, start2 = w.getStartPosition(), max = start1 + size; start1 < max; start1++, start2++)
                    if(!table.get(start1).equals(table.get(start2)))
                        return false;
                return true;
            } else
                return false;
        }
    }
    public Object data;
    private static int x = 0;
    private final int h = x++;
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
    List<Object> groupedAttributes;
    
    public JoinTuple(TYPE type, int index) {
        this.index = index; 
        this.type = type;
    }
    
    public JoinTuple(int index) {
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
    Wrapper tempWrapper = new Wrapper();
    private void addLeafTupleJoin(List<Object> projectedAttributeValues) {
        projectedTupleSize = projectedAttributeValues.size();
        int tempStartPos = MemoryManager.addToTable(index, projectedAttributeValues);
        Wrapper wrapper = new Wrapper();
        wrapper.init(tempStartPos, projectedAttributeValues.size());
		int projectedTupleIndex = MemoryManager.mapOfProjectedTupleToIndex.putIfAbsent(wrapper, newProjectedTupleIndex);
		if(projectedTupleIndex == -1) {
		    //Wrapper wrapper = new Wrapper(tempWrapper);
		    wrapper.projectedTupleIndex = newProjectedTupleIndex;
			//MemoryManager.mapOfProjectedTupleToIndex.put(wrapper, newProjectedTupleIndex);
			if(leafTupleCounts == null || newProjectedTupleIndex >= leafTupleCounts.length) {
                startPos = MemoryManager.ensureArraySize(startPos, newProjectedTupleIndex);
                leafTupleCounts = MemoryManager.ensureArraySize(leafTupleCounts, newProjectedTupleIndex);
            }
			startPos[newProjectedTupleIndex] = tempStartPos;
			leafTupleCounts[newProjectedTupleIndex]++;
			newProjectedTupleIndex++;
			leafTupleCount++;
		} else {
	        leafTupleCounts[projectedTupleIndex]++;
		    MemoryManager.deleteFromTable(index, tempStartPos, projectedTupleSize);
		}
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
        //TODO why not make the map an arrayhashmap and use jointuple, projectedAttributeValues as keys
        if(!groupedAttributes.isEmpty()) {
            int tempStartPos = MemoryManager.addToTable(index, groupedAttributes);
            //tempWrapper.init(tempStartPos, groupedAttributes.size());
            Wrapper wrapper = new Wrapper();
            wrapper.init(tempStartPos, groupedAttributes.size());
            Wrapper origKey = MemoryManager.mapOfProjectedTupleToIndex.putIfAbsentKey(wrapper, newProjectedTupleIndex);
            MemoryManager.deleteFromTable(index, tempStartPos, groupedAttributes.size());
            if(origKey == null) {
                //Wrapper wrapper = new Wrapper(tempWrapper);
                wrapper.projectedTupleIndex = newProjectedTupleIndex;
                //MemoryManager.mapOfProjectedTupleToIndex.put(wrapper, newProjectedTupleIndex);
                projectedAttributeValues.set(aggregatedColumnIndex, 1);
                if(startPos == null || newProjectedTupleIndex >= leafTupleCounts.length) {
                    startPos = MemoryManager.ensureArraySize(startPos, newProjectedTupleIndex);
                    leafTupleCounts = MemoryManager.ensureArraySize(leafTupleCounts, newProjectedTupleIndex);
                }
                startPos[newProjectedTupleIndex] = MemoryManager.addToTable(index, projectedAttributeValues);
                leafTupleCounts[newProjectedTupleIndex]++;//delete code doesn't go here
                newProjectedTupleIndex++;
                leafTupleCount++;
                return true;
            } else {
                int aggregatedLeafTupleIndex = origKey.projectedTupleIndex;
                List<Object> table = MemoryManager.tables[index];
                //TODO this call is superfluous when delete is called on a non-repeated projected tuple
                int index = startPos[aggregatedLeafTupleIndex] + aggregatedColumnIndex;
                table.set(index, (int)table.get(index) + posNeg);
                return leafTupleUpdated(posNeg, origKey, aggregatedLeafTupleIndex);
            }
        } else {
            if(startPos == null || startPos[0] == -1) {
                tempWrapper = new Wrapper();
                tempWrapper.init(MemoryManager.addToTable(index, groupedAttributes), 0);
                tempWrapper.projectedTupleIndex = newProjectedTupleIndex;
                MemoryManager.mapOfProjectedTupleToIndex.put(tempWrapper, newProjectedTupleIndex);
                projectedAttributeValues.set(aggregatedColumnIndex, 1);
                if(startPos == null || newProjectedTupleIndex >= leafTupleCounts.length) {
                    startPos = MemoryManager.ensureArraySize(startPos, newProjectedTupleIndex);
                    leafTupleCounts = MemoryManager.ensureArraySize(leafTupleCounts, newProjectedTupleIndex);
                }
                startPos[newProjectedTupleIndex] = MemoryManager.addToTable(index, projectedAttributeValues);
                leafTupleCounts[newProjectedTupleIndex]++;//delete code doesn't go here
                newProjectedTupleIndex++;
                leafTupleCount++;
                return true;
            } else {
                int aggregatedLeafTupleIndex = tempWrapper.projectedTupleIndex;
                List<Object> table = MemoryManager.tables[index];
                //TODO this call is superfluous when delete is called on a non-repeated projected tuple
                int index = startPos[aggregatedLeafTupleIndex] + aggregatedColumnIndex;
                table.set(index, (int)table.get(index) + posNeg);
                return leafTupleUpdated(posNeg, tempWrapper, aggregatedLeafTupleIndex);
            }
        }
    }

    private boolean doSum(List<Object> groupedAttributes, List<Object> projectedAttributeValues, int aggregatedColumnIndex, int posNeg) {
        int tempStartPos = MemoryManager.addToTable(index, groupedAttributes);
        //Wrapper wrapper = new Wrapper(tempStartPos, groupedAttributes.size());
        tempWrapper.init(tempStartPos, groupedAttributes.size());
        Wrapper wrapper1 = MemoryManager.mapOfProjectedTupleToIndex.getKey(tempWrapper);
        if(wrapper1 == null) {
            Wrapper wrapper = new Wrapper();
            wrapper.init(tempStartPos, groupedAttributes.size());
            wrapper.projectedTupleIndex = newProjectedTupleIndex;
            MemoryManager.mapOfProjectedTupleToIndex.put(wrapper, newProjectedTupleIndex);
            if(startPos == null || newProjectedTupleIndex >= leafTupleCounts.length) {
                startPos = MemoryManager.ensureArraySize(startPos, newProjectedTupleIndex);
                leafTupleCounts = MemoryManager.ensureArraySize(leafTupleCounts, newProjectedTupleIndex);
            }
            startPos[newProjectedTupleIndex] = MemoryManager.addToTable(this.index, projectedAttributeValues);
            leafTupleCounts[newProjectedTupleIndex]++;//because delete code doesn't go here
            newProjectedTupleIndex++;
            leafTupleCount++;
            return true;
        } else {
            MemoryManager.deleteFromTable(index, tempStartPos, groupedAttributes.size());
            List<Object> table = MemoryManager.tables[this.index];
            int aggregatedLeafTupleIndex = wrapper1.projectedTupleIndex;
            //TODO This is superfluous when a delete is called on a non-repeated projected tuple
            table.set(startPos[aggregatedLeafTupleIndex] + aggregatedColumnIndex
                    , (int)table.get(startPos[aggregatedLeafTupleIndex] + aggregatedColumnIndex) + 
                    posNeg * (int)table.get(startPos[aggregatedLeafTupleIndex] + aggregatedColumnIndex));
            //List<Object> prevProjectedAttributeValues = projectedTupleList.get(aggregatedLeafTuple);
            ///prevProjectedAttributeValues.set(index, (int)prevProjectedAttributeValues.get(index) + 
            //    posNeg * (int)projectedAttributeValues.get(index));
            return leafTupleUpdated(posNeg, wrapper1, aggregatedLeafTupleIndex);
        }
    }
	private boolean leafTupleUpdated(int posNeg, Wrapper origKey, int aggregatedLeafTupleIndex) {
	    if(posNeg > 0) {
	        leafTupleCounts[aggregatedLeafTupleIndex]++;
	        return true;
	    } else {
	        if(--leafTupleCounts[aggregatedLeafTupleIndex] == 0) {
	            return removeLeafTuple(origKey);
	        } else {
	            return true;
	        }
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
        int tempStartPos = MemoryManager.addToTable(index, projectedAttributeValues);
        tempWrapper.init(tempStartPos, projectedAttributeValues.size());
        //Wrapper wrapper = new Wrapper(tempStartPos, projectedAttributeValues.size());
        //Integer projectedTupleIndex = MemoryManager.mapOfTupleToTuple.get(wrapper);
        Wrapper origKey = MemoryManager.mapOfProjectedTupleToIndex.getKey(tempWrapper);
        MemoryManager.deleteFromTable(index, tempStartPos, projectedAttributeValues.size());
        leafTupleCounts[origKey.projectedTupleIndex]--;
        if(leafTupleCounts[origKey.projectedTupleIndex] == 0) {
            return removeLeafTuple(origKey);
        } else {
            return false;
        }
	}

    private boolean removeLeafTuple(Wrapper origKey) {
        int aggregatedLeafTupleIndex = MemoryManager.mapOfProjectedTupleToIndex.remove(origKey);
        //if(leafTupleCounts != null)
        //    leafTupleCounts[aggregatedLeafTupleIndex] = -1;
        //projectedTupleList.set(aggregatedLeafTupleIndex, null);
        MemoryManager.deleteFromTable(index, origKey.getStartPosition(), origKey.getSize());
        MemoryManager.mapOfProjectedTupleToIndex.remove(origKey);
        startPos[aggregatedLeafTupleIndex] = -1;
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
    private void setLookAhead() {
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
    public int getLeafTupleCount() {
        return leafTupleCount;
    }
    ProjectedTupleIterator y;
    @Override
    public ProjectedTupleIterator iterator() {
        if(y == null)
            y = new ProjectedTupleIterator(MemoryManager.tables[index], startPos, projectedTupleSize, leafTupleCount, leafTupleCounts);
        else
            y.reset(leafTupleCount, startPos, leafTupleCounts);
        return y;
    }

    public List<Object> getProjectedTuple(int index) {
        return MemoryManager.tables[index].subList(startPos[index], startPos[index] + projectedTupleSize);
    }
}