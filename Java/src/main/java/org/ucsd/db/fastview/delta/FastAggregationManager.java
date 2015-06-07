package org.ucsd.db.fastview.delta;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

import org.ucsd.db.deltaivm.delta.GroupByOperator;
import org.ucsd.db.fastview.datamodel.AggregatedAttribute;
import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.JoinTuple;
import org.ucsd.db.fastview.datamodel.JoinTuple.TYPE;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.ProjectedTupleIterator;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.index.IntHashMap;

public class FastAggregationManager {
    private GroupByOperator groupByOperator = new GroupByOperator();
    
    public <E extends Enum<E>> List<Object[]> insertTuple(CntTable cntTable, VertexValue<E>[] vertexValues, List<Object> projectedAttributeValues
            , TYPE type, int tableIndex, String relationName, TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        return insertTuple(cntTable, true, vertexValues, projectedAttributeValues, type, tableIndex, relationName//delete
                , tableDefinition, projectedAttributeNamesList);
    }
    public <E extends Enum<E>> void insertTupleWithoutReturn(CntTable cntTable, VertexValue<E>[] vertexValues, List<Object> projectedAttributeValues
        , TYPE type, int tableIndex, String relationName, TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        insertTuple(cntTable, false, vertexValues, projectedAttributeValues, type, tableIndex, relationName//delete
                , tableDefinition, projectedAttributeNamesList);
    }
    private <E extends Enum<E>> List<Object[]> insertTuple(CntTable cntTable, boolean doReturn, VertexValue<E>[] vertexValues 
        , List<Object> projectedAttributeValues, TYPE type, int tableIndex, String relationName//delete
        , TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        //Find the Integer which contains a row s with join attributes the same as t
        int[] t = new int[]{-1, -1};
        int[] lastRe = MemoryManager.map.putIfAbsent(vertexValues, t);
        int index = 0;
        if(lastRe != null && (index = lastRe[tableIndex & 1]) > -1) {
            //There can only be a single nested tuple which has the same join attribute as t
            JoinTuple joinTuple = cntTable.get(index);
            joinTuple.addLeafTuple(projectedAttributeValues);
            if(doReturn) {
//                if(type.equals(TYPE.JOIN_TUPLE)) {
//                    return getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes()
//                        , joinTuple, projectedAttributeValues, true);
//                } else {
                    //groupByOperator.convertToGroupedTuple(projectedAttributeValues, tableDefinition, projectedAttributeNamesList);
                    return getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes(), joinTuple
                        , projectedAttributeValues, true);
//                }
            } else
                return null;
        } else {//Otherwise, look for tuples of other relations which may join with t
            lastRe = t;
            boolean unJoinedTuple = false;
            int isAnyLocalKnowledgeFalse = 0;
            JoinTuple tuple = new JoinTuple(type, tableIndex);
            tuple.setProjectedAttributeNames(projectedAttributeNamesList);
            tuple.setTableDefinition(tableDefinition);
            tuple.addLeafTuple(projectedAttributeValues);
            tuple.setVertexValues(vertexValues);
            for(VertexValue<E> i : vertexValues) {
                i = FastViewUtil.getOldVertexValue(i);
                unJoinedTuple |= (i.getJoinTuples((tableIndex + 1) % 2 )[0] == null);
                i.addJoinTuple(tuple.getIndex(), tuple);
            }
            if(isAnyLocalKnowledgeFalse == 1)
                tuple.setGlobalKnowledge(false);
//            if(lastRe == null) {
//                lastRe = new int[]{-1, -1};
//                MemoryManager.map.put(vertexValues, lastRe);
//            }
            lastRe[tableIndex % 2] = cntTable.size();
            cntTable.add(tuple);
            if(unJoinedTuple) {
//                if(doReturn) {
//                    int[] offsetAttributes = cntTable.getTableDefinition().getOffsetAttributes();
//                    Object[] ret = new Object[offsetAttributes[offsetAttributes.length - 1]];
//                    copy(tuple.iterator().next(), offsetAttributes[tuple.getIndex()], ret);
//                    return Collections.singletonList(ret);
//                } else
                    return null;
            } else if(doReturn) {
                if(tuple.isJoinTuple() || tuple.isAggregatedTuple()) {
                    return getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes()
                        , tuple, projectedAttributeValues, false);
                } else {
                    return getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes()
                        , tuple, projectedAttributeValues, false);
                }
            }
            else
                return null;
        }
    }
    
//    public static final List<IndexLeaf> getIndexLeaves(List<VertexValue<E>> vertexValues, String relationName) {
//        List<IndexLeaf> list = new ArrayList<>();
//        for(VertexValue<E> i : vertexValues) {
//            IIndex index = vertexAliasToIndexMap.get(i.getAlias());
//            if(index == null)
//                return null;
//            //gets the rowId for other relation which joins with R on a join attribute a_indexOfAttribute. There can be only one
//            IndexLeaf leaf = index.getLeaf(i);
//            if(leaf == null)
//                return null;
//            else
//                list.add(leaf);
//        }
//        return list;
//    }

    public <E extends Enum<E>> List<Object[]> deleteTuple(CntTable cntTable, VertexValue<E>[] vertexValues
        , List<Object> projectedAttributeValues, int tableIndex, TableDefinition tableDefinition
        , List<String> projectedAttributeNamesList) {
        //Find the Integer which contains a row s with join attributes the same as t
        int[] lastRe = MemoryManager.map.get(vertexValues);
        if(lastRe != null && lastRe[tableIndex % 2] > -1) {
            //There can only be a single nested tuple which has the same join attribute as t
            JoinTuple joinTupleToBeRemoved = cntTable.get(lastRe[tableIndex % 2]);
            groupByOperator.convertToGroupedTuple(projectedAttributeValues, tableDefinition, projectedAttributeNamesList);
            List<Object[]> returnValue = getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes()
                    , joinTupleToBeRemoved, projectedAttributeValues, true);
            boolean isProjectedTupleLeft = joinTupleToBeRemoved.deleteLeafTuple(projectedAttributeValues);
            if(!isProjectedTupleLeft) {
                cntTable.remove(lastRe[tableIndex % 2]);
                lastRe[tableIndex] = -1;
                if(lastRe[(tableIndex + 1) % 2] == -1) {
                    MemoryManager.map.remove(vertexValues);
                }
                for(VertexValue<E> i : vertexValues) {
                    i = FastViewUtil.getOldVertexValue(i);
                    i.removeJoinTuple(tableIndex, joinTupleToBeRemoved);
                }
            }
            return returnValue;
        }
        return null;
    }
    public <E extends Enum<E>> void deleteTupleWithoutReturn(CntTable cntTable, VertexValue<E>[] vertexValues
        , List<Object> projectedAttributeValues, TYPE type, int tableIndex) {
            //Find the Integer which contains a row s with join attributes the same as t
            int[] lastRe = MemoryManager.map.get(vertexValues);
            if(lastRe != null && lastRe[tableIndex % 2] > -1) {
                //There can only be a single nested tuple which has the same join attribute as t
                JoinTuple joinTupleToBeRemoved = cntTable.get(lastRe[tableIndex % 2]);
                boolean isProjectedTupleLeft;
                isProjectedTupleLeft = joinTupleToBeRemoved.deleteLeafTuple(projectedAttributeValues);
                if(!isProjectedTupleLeft) {
                    JoinTuple joinTuple = cntTable.get(lastRe[tableIndex % 2]);
                    cntTable.remove(lastRe[tableIndex % 2]);
                    lastRe[tableIndex % 2] = -1;
                    if(lastRe[(tableIndex + 1) % 2] == -1) {
                        MemoryManager.map.remove(vertexValues);
                    }
                    for(VertexValue<E> i : vertexValues) {
                        i = FastViewUtil.getOldVertexValue(i);
                        i.removeJoinTuple(joinTuple.getIndex(), joinTuple);
                    }
                }
            }
        }
    public void scan(CntTable cntTable) {
//        int c = 0;
        for(JoinTuple i : cntTable.getTuples()) {
            if(i.getIndex() == 0) {
                for(ProjectedTupleIterator itr = i.iterator(); itr.hasNext();)
                    getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes(), i, itr.next(), true);
            }
        }
//        System.out.println(c);
    }
//    //@Override
//    public List<List<Object>> scan1(CntTable cntTable
//            ) {
//        long tm = System.nanoTime();
//        Set<JoinTuple> copyJoinTuples = new HashSet<>(cntTable.getFirstTuple());
//        List<List<Object>> returnValue = new ArrayList<>();
//        List<List<List<Object>>> lll = new ArrayList<>();
//        while(!copyJoinTuples.isEmpty()) {
//            lll.clear();
//            JoinTuple me = copyJoinTuples.iterator().next();
//            copyJoinTuples.remove(me);
//            lll.add(me.getProjectedTupleList());
//            for(JoinTuple t : me.getConn()) {
//                lll.add(t.getProjectedTupleList());
//                copyJoinTuples.remove(t);
//            }
//            returnValue.addAll(cartesianProduct(lll));
//        }
//        return returnValue;
//    }
//    public List<Object[]> scan(CntTable cntTable
//            ) {
//        List<Object[]> returnValue = new ArrayList<>();
//        for(JoinTuple i : cntTable.getFirstTuple())
//            returnValue.addAll(getTuplesDfs(cntTable.getOffsetAttributes(), i, i.getProjectedTupleList()));
//        return returnValue;
//    }
//    public List<Object[]> scan(CntTable cntTable
//        , JoinTuple me) {
//        int lastRe = FastViewUtil.getJoinTupleIndex(me.getVertexValues(), me.getRelationName()
//                        );
//        if(lastRe > -1) {
//            JoinTuple tuple = getTuple(cntTable, me);
//            if(tuple.isAggregatedTuple() || tuple.isGroupedTuple())
//                return getTuplesDfs(cntTable.getTableDefinition().getOffsetAttributes(), cntTable.get(lastRe)
//                                , tuple.getProjectedTupleList());
//            else
//                return getTuplesDfs(cntTable.getTableDefinition().getOffsetAttributes(), cntTable.get(lastRe)
//                                , null);
//        } else
//            return null;
//    }
//    public List<Object[]> scan(CntTable cntTable
//        , JoinTuple me, List<List<Object>> projectedTuples) {
//        int lastRe = MemoryManager.map.get(me.getVertexValues());
//        if(lastRe > -1)
//            return getTuplesDfs(cntTable.getTableDefinition().getOffsetAttributes(), cntTable.get(lastRe), me.getProjectedTupleList());
//        else
//            return null;
//    }
    private <E extends Enum<E>>List<Object[]> getTuplesBfs(CntTable cntTable, int[] offsetAttributes, JoinTuple jt, List<Object> list2
        , boolean useGlobalKnowledge) {
        List<Object[]> table1 = new ArrayList<Object[]>();
        int columnCount = offsetAttributes[offsetAttributes.length - 1];
        Object[] joinedTuple = new Object[columnCount];
        table1.add(joinedTuple);
        IntHashMap<JoinTuple> factorMap = new IntHashMap<>();
//        if(jt.isGroupedTuple()) {
//            factorMap.put(jt, jt.getLeafTupleCount());
//        } else//Grouped Tuple
            copy(list2, offsetAttributes[jt.getIndex()], joinedTuple);
        LinkedList<JoinTuple> queue = new LinkedList<>();
        queue.push(jt);
        HashMap<JoinTuple, int[]> startTuplesMap = new HashMap<>();
        startTuplesMap.put(jt, new int[]{0, 1});
        int[] coveredRelations = new int[offsetAttributes.length - 1];//number of relations
        Arrays.fill(coveredRelations, -1);
        List<Object[]> tableOut = table1;
        //ArrayList<JoinTuple[]> list  = new ArrayList<JoinTuple[]>();
        int needsScaling = -2;
        ArrayList<JoinTuple> completeTuples = new ArrayList<JoinTuple>();
        A: while((coveredRelations[0] == -1 || coveredRelations[coveredRelations.length - 1] == -1) && !queue.isEmpty()) {
                JoinTuple srcJt = queue.poll();
                boolean gk = false;
                VertexValue[] vertexValues = srcJt.getVertexValues();
                if(coveredRelations[jt.getIndex()] == -1 || vertexValues.length > 1) {
                    int currentRelationIndex = srcJt.getIndex();
                    if(coveredRelations[currentRelationIndex] == -1 && !tableOut.isEmpty()) {
                        table1 = tableOut;
                        tableOut = new ArrayList<Object[]>();
                    }
                    List<Object[]> tempTable1 = table1;
                    int joinTupleSeen = 0;
                    int initialSize = tableOut.size();
                    completeTuples.clear();
                    int[] s = startTuplesMap.get(srcJt);
                    int start = s[0];
                    int end = s[1];
                    boolean nextJoinExists = true;
                    for(VertexValue<E> v : vertexValues) {
                        JoinTuple[] i = v.getJoinTuples(srcJt.getIndex() + 1);
                        /**
                         * This is required because of the case o----p-----q. If there is a diff of p, and o is joined first and 
                         * as a result, the set of tuples for q increase but we have not moved out of the loop above. So if we
                         * did not do this, the only the old subset of table1 is considered, which is clearly wrong.
                         */
                        if(i[0] != null && coveredRelations[i[0].getIndex()] == -1) {
                            if(joinTupleSeen == 1) {
                                start = initialSize;
                                end = tableOut.size();
                                tempTable1 = tableOut;
                                tableOut = new ArrayList<Object[]>();
                            }
                            gk = false;
                            for(int m = 0; m < i.length && i[m] != null; m++) {
                                JoinTuple nextJoinTuple = i[m];
                                if(!useGlobalKnowledge)
                                    nextJoinTuple.setGlobalKnowledge(true);
                                if(nextJoinTuple.isComplete()) {
                                    gk = true;
                                    queue.offer(nextJoinTuple);
                                    nextJoinExists = nextJoinTuple.getVertexValues().length > 1;
                                    /*if(nextJoinTuple.isGroupedTuple()) {
                                        if(joinTupleSeen == 0) {
                                            int srcJtFactor = Math.abs(factorMap.remove(srcJt));
                                            int k = srcJtFactor * nextJoinTuple.getLeafTupleCount();
                                            if(k > 1)
                                                needsScaling = factorMap.put(nextJoinTuple, k);
                                        }
                                        startTuplesMap.put(nextJoinTuple, s);
                                    } else*/ {
                                        completeTuples.add(nextJoinTuple);
                                        if(!startTuplesMap.containsKey(nextJoinTuple)) {
                                            int srcJtFactor = Math.abs(factorMap.remove(srcJt));
                                            if(srcJtFactor > 1)
                                                needsScaling = factorMap.put(nextJoinTuple, srcJtFactor);
                                            for(int jIndex = start; jIndex < end; jIndex++) {
                                                Object[] j = tempTable1.get(jIndex);
                                                ProjectedTupleIterator p = nextJoinTuple.iterator();
                                                while(p.hasNext()) {
                                                    joinedTuple = getJoinedTuple(j, p.next(), columnCount
                                                        , offsetAttributes[nextJoinTuple.getIndex()]);
                                                    tableOut.add(joinedTuple);
                                                    int leafTupleRepeatCount = p.getLeafTupleRepeatCount();
                                                    if(leafTupleRepeatCount > 1)
                                                        scalePrjectedTuple(cntTable.getTableDefinition().getAggregatedAttributes()
                                                            , joinedTuple, s, leafTupleRepeatCount);
                                                }
                                            }
                                        } else {//always greater than 1
                                            int srcJtFactor = Math.abs(factorMap.remove(srcJt));
                                            int l = Math.abs(factorMap.get(nextJoinTuple));
                                            needsScaling = factorMap.put(nextJoinTuple, l + srcJtFactor);
                                        }
                                    }
                                }
                                joinTupleSeen = 1;
                            }
                            if(!nextJoinExists && queue.size() == 1) {
                                if(needsScaling > -2 && initialSize < tableOut.size()) {
                                    int[] x = new int[]{initialSize, tableOut.size()};//This could create problem if join tuples seen = 1 and s is updated
                                    for(JoinTuple k: completeTuples)
                                        startTuplesMap.put(k, x);
                                }
                                break A;
                            }
                        }
//                    if(!completeTuples.isEmpty())
//                        System.out.println();
                    if(/*(nextJoinExists || needsScaling > -2) &&*/ initialSize < tableOut.size()) {
                        int[] x = new int[]{initialSize, tableOut.size()};//This could create problem if join tuples seen = 1 and s is updated
                        for(JoinTuple k: completeTuples)
                            startTuplesMap.put(k, x);
                    }
                    coveredRelations[currentRelationIndex] = 1;
                }
                srcJt.setGlobalKnowledge(gk);
            }
        }
        if(table1.size() > tableOut.size()) {
            if(table1.isEmpty())
                return null;
            else {
                if(needsScaling > -2)
                    scale1(cntTable.getTableDefinition().getAggregatedAttributes(), table1, startTuplesMap, factorMap);
                return table1;
            }
        } else {
            if(needsScaling > -2)
                scale1(cntTable.getTableDefinition().getAggregatedAttributes(), tableOut, startTuplesMap, factorMap);
            return tableOut;
        }
    }
    private void scalePrjectedTuple(Collection<AggregatedAttribute> aggregatedAttributes, Object[] joinedTuple
            , int[] startEnd, int leafTupleRepeatCount) {
        for(AggregatedAttribute k : aggregatedAttributes)
            for(int j = startEnd[0]; j < startEnd[1]; j++) {
                joinedTuple[k.viewIndex] = (int)joinedTuple[k.viewIndex] * leafTupleRepeatCount; 
            }        
    }
    private void scale1(Collection<AggregatedAttribute> aggregatedAttributes, List<Object[]> table, HashMap<JoinTuple, int[]> startTuplesMap
        , IntHashMap<JoinTuple> factorMap) {
        for(IntHashMap.Entry<JoinTuple> i : factorMap.entrySet()) {
            for(AggregatedAttribute k : aggregatedAttributes) {
                int[] s = startTuplesMap.get(i.getKey());
                for(int j = s[0]; j < s[1]; j++) {
                    table.get(j)[k.viewIndex] = (Integer)table.get(j)[k.viewIndex] * i.getValue(); 
                }
            }
        }
    }
    private Object[] getJoinedTuple(Object[] j, List<Object> l, int columnCount, int offset) {
        Object[] joinedTuple = Arrays.copyOf(j, j.length); 
        copy1(l, joinedTuple, offset);
        return joinedTuple;
    }
    private void copy1(List<Object> l, Object[] joinedTuple, int offset) {
        for(int i = 0, k = offset; i < l.size(); i++, k++)
            joinedTuple[k] = l.get(i);
    }
//    private List<Object[]> getTuplesDfs(int[] offsetAttributes, JoinTuple jt
//            , List<List<Object>> clone) {
//        List<Object[]> r = new ArrayList<>();
//        
////        if(jt.getNumberOfJoinedTuples() == 0) {
////            return null;
////        }
//        
//        int c = 0;
//        HashMap<Object, Integer> indexMap = new HashMap<Object, Integer>();
//        indexMap.put(jt, c++);
//        int[] indexes = new int[1000];
//        Stack<JoinTuple> stack = new Stack<>();
//        Object[] tuple = null;
//        boolean isFurtherPossible = true;
//        B: while(isFurtherPossible && indexes[indexMap.get(jt)] < clone.size()) {
//            stack.clear();
//            stack.push(jt);
//            isFurtherPossible = true;
//            String last = null;
//            int factor = 1;
//            while(!stack.isEmpty()) {
//                JoinTuple srcJt = stack.pop();
//                if(!indexMap.containsKey(srcJt))
//                    indexMap.put(srcJt, c++);
//                for(VertexValue<E> j : srcJt.getVertexValues()) {
//                    if(!indexMap.containsKey(j))
//                        indexMap.put(j, c++);
//                    List<JoinTuple> joinedTuples = j.getJoinTuples(srcJt.getIndex() + 1);
//                    if((srcJt == jt || srcJt.getVertexValues().length > 1) && !joinedTuples.isEmpty()) {
//                        if(srcJt != jt) {
//                            if(joinedTuples.get(0).getRelationName().equals(last)) {
//                                continue;
//                            }
//                        }
//                        JoinTuple joinTuple = joinedTuples.get(indexes[indexMap.get(j)]);
//                        if(!indexMap.containsKey(joinTuple))
//                            indexMap.put(joinTuple, c++);
//                        if(indexes[indexMap.get(joinTuple)] == joinTuple.getProjectedTupleListSize()) {
//                            indexes[indexMap.get(joinTuple)] = 0;
//                            indexes[indexMap.get(j)]++;
//                            if(indexes[indexMap.get(j)] >= joinedTuples.size()) {
//                                indexes[indexMap.get(j)] = 0;
//                                indexes[indexMap.get(srcJt)]++;
//                            }
//                            if(isFurtherPossible)
//                                continue B;//roll back
//                            else
//                                break B;
//                        }
//                        stack.push(joinTuple);
//                    }
//                    if(indexes[indexMap.get(srcJt)] == srcJt.getProjectedTupleListSize() - 1
//                        && indexes[indexMap.get(j)] == srcJt.getVertexValues().length - 1)
//                         isFurtherPossible |= false;
//                     else
//                         isFurtherPossible |= true;
//                }
//                if(tuple == null)
//                    tuple = new Object[offsetAttributes[offsetAttributes.length - 1]];
//                if(srcJt == jt) {
//                    if(jt.isGroupedTuple())
//                        factor *= srcJt.getLeafTupleCount();
//                    else// Join Tuple
//                        copy(clone.get(indexes[indexMap.get(srcJt)]), offsetAttributes[srcJt.getIndex()], tuple);
//                } else {
//                    if(srcJt.isGroupedTuple())
//                        factor *=  srcJt.getLeafTupleCount();
//                    else
//                        copy(srcJt.getProjectedTupleList().get(indexes[indexMap.get(srcJt)])
//                            , offsetAttributes[srcJt.getIndex()], tuple);
//                    if(srcJt.getVertexValues().length == 1)
//                        indexes[indexMap.get(srcJt)]++;
//                }
//                last = srcJt.getRelationName();
//            }
//            r.add(tuple);
//            for(int jIndex = 0; jIndex < tuple.length; jIndex++) {
//                Object j = tuple[jIndex];
//                if(j instanceof Long)
//                    tuple[jIndex] = ((long)j * factor);
//            }
//            tuple = null;
//        }
//        return r;
//    }
    
    private void copy(List<Object> list, int offset, Object[] tuple) {
        for(Object j : list) {
            tuple[offset++] = j;
        }
    }
}