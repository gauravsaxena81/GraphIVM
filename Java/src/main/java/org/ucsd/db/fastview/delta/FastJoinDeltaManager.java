package org.ucsd.db.fastview.delta;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Stack;

import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.JoinTuple;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.ProjectedTupleIterator;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.VertexValue;

public class FastJoinDeltaManager {
    
    public <E extends Enum<E>> List<Object[]> insertTuple(CntTable cntTable, VertexValue<E>[] vertexValues
        , List<Object> projectedAttributeValues, int tableIndex, String relationName, TableDefinition tableDefinition
        , List<String> projectedAttributeNamesList) {
        return insertTuple(cntTable, true, vertexValues, projectedAttributeValues, tableIndex, relationName//delete
                , tableDefinition, projectedAttributeNamesList);
    }
    public <E extends Enum<E>> void insertTupleWithoutReturn(CntTable cntTable, VertexValue<E>[] vertexValues
        , List<Object> projectedAttributeValues, int tableIndex, String relationName/*delete*/, TableDefinition tableDefinition
        , List<String> projectedAttributeNamesList) {
        insertTuple(cntTable, false, vertexValues, projectedAttributeValues, tableIndex, relationName
                , tableDefinition, projectedAttributeNamesList);
    }
    private <E extends Enum<E>> List<Object[]> insertTuple(CntTable cntTable, boolean doReturn, VertexValue<E>[] vertexValues 
        , List<Object> projectedAttributeValues, int tableIndex, String relationName//TODO delete
        , TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        int[] offsetAttributes = cntTable.getTableDefinition().getOffsetAttributes();
        //Find the Integer which contains a row s with join attributes the same as t
        int[] t = new int[]{-1, -1};
        int[] lastRe = MemoryManager.map.putIfAbsent(vertexValues, t);
        //Integer lastRe = FastViewUtil.getJoinTupleIndex(tuple.getVertexValues(), relationName, vertexAliasToIndexMap);
        if(lastRe  != null && lastRe[tableIndex % 2] > -1) {
            //There can only be a single nested tuple which has the same join attribute as t
            JoinTuple joinTuple = (JoinTuple)cntTable.get(lastRe[tableIndex % 2]);
            joinTuple.addLeafTuple(projectedAttributeValues);
            if(doReturn)
                //return getTuplesIntArrayList(offsetAttributes,  joinTuple, tuple.getProjectedTupleList());
                //return getTuplesDfsArray(offsetAttributes,  joinTuple, Collections.singletonList(projectedAttributeValues));
                return getTuplesBfs(cntTable, offsetAttributes, joinTuple, projectedAttributeValues, true);
                //return getTuplesBfs1(cntTable, offsetAttributes, joinTuple, tuple.getProjectedTupleList());
                //return null;
            else
                return null;
        } else {//Otherwise, look for tuples of other relations which may join with t
            lastRe = t;
            boolean unJoinedTuple = false;
            JoinTuple tuple = new JoinTuple(tableIndex);
            tuple.setProjectedAttributeNames(projectedAttributeNamesList);
            tuple.setTableDefinition(tableDefinition);
            tuple.addLeafTuple(projectedAttributeValues);
            tuple.setVertexValues(vertexValues);
            for(VertexValue<E> i : vertexValues) {
                i = FastViewUtil.getOldVertexValue(i);
                unJoinedTuple |= (i.getJoinTuples((tableIndex + 1) % 2 )[0] == null);
                i.addJoinTuple(tableIndex, tuple);
            }
//            if(lastRe == null) {
//                lastRe = new int[]{-1, -1};
//                MemoryManager.map.put(vertexValues, lastRe);
//            }
            lastRe[tableIndex % 2] = cntTable.size();
            cntTable.add(tuple);
            if(unJoinedTuple) {
//                if(doReturn) {
//                    Object[] ret = new Object[offsetAttributes[offsetAttributes.length - 1]];
//                    copy(projectedAttributeValues, offsetAttributes[tableIndex], ret);
//                    return Collections.singletonList(ret);
//                } else
                    return null;
            } else if(doReturn)
//                return getTuplesDfsArray(offsetAttributes,  tuple, Collections.singletonList(projectedAttributeValues));
                return getTuplesBfs(cntTable, offsetAttributes, tuple, projectedAttributeValues, false);
            else
                return null;
        }
    }
    
//    public static final List<IndexLeaf> getIndexLeaves(List<VertexValue> vertexValues, String relationName) {
//        List<IndexLeaf> list = new ArrayList<>();
//        for(VertexValue i : vertexValues) {
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
        , List<Object> projectedAttributeValues, int tableIndex) {
        return deleteTuple(cntTable, vertexValues, tableIndex, projectedAttributeValues, true);
    }
    public <E extends Enum<E>> void deleteTupleWithoutReturn(CntTable cntTable, VertexValue<E>[] vertexValues
        , List<Object> projectedAttributeValues, int tableIndex) {
        deleteTuple(cntTable, vertexValues, tableIndex, projectedAttributeValues, false);
    }
    
    private <E extends Enum<E>> List<Object[]> deleteTuple(CntTable cntTable, VertexValue<E>[] vertexValues, int tableIndex
        , List<Object> projectedAttributeValues, boolean doReturn) {
        //Find the Integer which contains a row s with join attributes the same as t
        int[] lastRe = MemoryManager.map.get(vertexValues);
        if(lastRe != null && lastRe[tableIndex % 2] > -1) {
            //There can only be a single nested tuple which has the same join attribute as t
            JoinTuple joinTupleToBeRemoved = (JoinTuple) cntTable.get(lastRe[tableIndex % 2]);
            List<Object[]> returnValue = null;
            if(doReturn)
                returnValue = null;//getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes()
//                    , joinTupleToBeRemoved, projectedAttributeValues, true);
            //Case of slide?
            boolean allProjectedTuplesDeleted = joinTupleToBeRemoved.deleteLeafTuple(projectedAttributeValues);
            if(allProjectedTuplesDeleted) {
                cntTable.remove(lastRe[tableIndex % 2]);
                lastRe[tableIndex % 2] = -1;
                if(lastRe[(tableIndex + 1) % 2] == -1) {
                    MemoryManager.map.remove(vertexValues);
                }
                for(VertexValue<E> i : joinTupleToBeRemoved.getVertexValues()) {
                    //i = FastViewUtil.getOldVertexValue(i);
                    i.removeJoinTuple(tableIndex, joinTupleToBeRemoved);
                }
            }
            return returnValue;
        }
        return null;
    }

    
    public void updateTuple(CntTable cntTable
        , JoinTuple tuple) {
        
    }
    public void scan(CntTable cntTable) {
        int c = 0;
        for(JoinTuple i : cntTable.getTuples()) {
            if(i.getIndex() == 0) {
                for(ProjectedTupleIterator itr = i.iterator(); itr.hasNext();) {
                    List<Object> next = itr.next();
                    List<Object[]> t = getTuplesBfs(cntTable, cntTable.getTableDefinition().getOffsetAttributes(), i, next, true);
                    c += t.size();
                }
            }
        }
        System.out.println(c);
    }

//    public List<Object[]> scan(CntTable cntTable
//            ) {
//        List<Object[]> returnValue = new ArrayList<>();
//        for(JoinTuple i : cntTable.getFirstTuple())
//            returnValue.addAll(getTuples(cntTable.getTableDefinition().getOffsetAttributes(), (JoinTuple) i
//                    , ((JoinTuple) i).getProjectedTupleList()));
//        return returnValue;
//    }
//    public List<Object[]> scan(CntTable cntTable
//        , JoinTuple me) {
//        Integer lastRe = MemoryManager.map.get(me.getVertexValues());
//        if(lastRe != null)
//            return getTuplesDfsArray(cntTable.getTableDefinition().getOffsetAttributes()
//                , (JoinTuple) cntTable.get(lastRe), me.getProjectedTupleList());
//        else
//            return null;
//    }
//    public List<Object[]> scan(CntTable cntTable
//        , JoinTuple me, List<List<Object>> projectedTuples) {
//        Integer lastRe = MemoryManager.map.get(me.getVertexValues());
//        if(lastRe != null)
//            return getTuplesDfsArray(cntTable.getTableDefinition().getOffsetAttributes(), (JoinTuple) cntTable.get(lastRe)
//                    , me.getProjectedTupleList());
//        else
//            return null;
//    }
    int count = 0;
    private <E extends Enum<E>> List<Object[]> getTuplesBfs(CntTable cntTable, int[] offsetAttributes, JoinTuple jt
        , List<Object> list2, boolean useGlobalKnowledge) {
        count = 0;
        LinkedList<JoinTuple> queue = new LinkedList<>();
        List<Object[]> table1 = new ArrayList<Object[]>();
        int columnCount = offsetAttributes[offsetAttributes.length - 1];
        Object[] joinedTuple = new Object[columnCount];
        copy(list2, offsetAttributes[jt.getIndex()], joinedTuple);
        table1.add(joinedTuple);
        queue.push(jt);
        HashMap<JoinTuple, int[]> startTuplesMap = new HashMap<>();
        startTuplesMap.put(jt, new int[]{0, 1});
        List<Object[]> tableOut = table1;
        int[] coveredRelations = new int[offsetAttributes.length - 1];//number of relations
        Arrays.fill(coveredRelations, -1);
        while((coveredRelations[0] == -1 || coveredRelations[coveredRelations.length - 1] == -1) && !queue.isEmpty()) {
            JoinTuple srcJt = queue.poll();
            VertexValue[] vertexValues = srcJt.getVertexValues();
            int currentRelationIndex = srcJt.getIndex();
            if(coveredRelations[jt.getIndex()] == -1 || vertexValues.length > 1) {
                if(coveredRelations[currentRelationIndex] == -1 && !tableOut.isEmpty()) {
                    table1 = tableOut;
                    tableOut = new ArrayList<Object[]>();
                }
                List<Object[]> tempTable1 = table1;
                int joinTupleSeen = 0;
                int[] s = startTuplesMap.get(srcJt);
                for(VertexValue<E> v : vertexValues) {
                    JoinTuple[] i = v.getJoinTuples(srcJt.getIndex() + 1);
                    /**
                     * This is required because of the case o----p-----q. If there is a diff of p, and o is joined first and 
                     * as a result, the set of tuples for q increase but we have not moved out of the loop above. So if we
                     * did not do this, the only the old subset of table1 is considered, which is clearly wrong.
                     */
                    if(i[0] != null && coveredRelations[i[0].getIndex()] == -1) {
                        if(joinTupleSeen == 1) {
                            tempTable1 = tableOut;
                            tableOut = new ArrayList<Object[]>();
                        }
                        boolean isAnyJoinTupleComplete = false;
                        for(int m = 0; m < i.length && i[m] != null; m++) {
                            JoinTuple nextJoinTuple = i[m];
                            if(!useGlobalKnowledge)
                                nextJoinTuple.setGlobalKnowledge(true);
                            if(nextJoinTuple.isComplete()) {
                                isAnyJoinTupleComplete = true;
                                queue.offer(nextJoinTuple);
                                int initialTableOutSize = tableOut.size();
                                for(int jIndex = s[0], max = s[1]; jIndex < max; jIndex++) {
                                    Object[] j = tempTable1.get(jIndex);
                                    for(ProjectedTupleIterator p = nextJoinTuple.iterator(); p.hasNext(); ) {
                                        count++;
                                        joinedTuple = Arrays.copyOf(j, j.length);
                                        copy1(p.next(), joinedTuple, offsetAttributes[nextJoinTuple.getIndex()]);
                                        //copy1(p.next(), j, offsetAttributes[nextJoinTuple.getIndex()]);
                                        tableOut.add(joinedTuple);
                                        for(int n = 1, count = p.getLeafTupleRepeatCount(); n < count; n++)
                                            tableOut.add(joinedTuple);
                                    }
                                }
                                startTuplesMap.put(nextJoinTuple, new int[]{initialTableOutSize, tableOut.size()});
                            }
                        }
                        srcJt.setGlobalKnowledge(isAnyJoinTupleComplete);
                        joinTupleSeen = 1;
                    }
                }
            }
            coveredRelations[currentRelationIndex] = 1;
        }
//        System.out.println("count:" + count);
        if(coveredRelations[coveredRelations.length - jt.getIndex() - 1] == -1)
            return Collections.EMPTY_LIST;
        /*else if(tableOut.size() < table1.size()) {
            if(table1.isEmpty())
                return Collections.EMPTY_LIST;
            else
                return table1;
        }*/ else
            return tableOut;
    }
    private void copy1(List<Object> l, Object[] joinedTuple, int offset) {
        for(int i = 0, k = offset; i < l.size(); i++, k++)
            joinedTuple[k] = l.get(i);
    }
    private int[] indexes = new int[10000];
    private List<Object[]> getTuplesDfsArray(int[] offsetAttributes, JoinTuple jt
            , List<List<Object>> clone) {
        List<Object[]> r = new ArrayList<>();
        
//        if(jt.getNumberOfJoinedTuples() == 0) {
//            Object[] tuple = new Object[offsetAttributes[offsetAttributes.length - 1]];
//            r.add(tuple);
//            copy(clone.get(0), offsetAttributes[jt.getIndex()], tuple);
//            return r;
//        }
        
        int c = 0;
        HashMap<Object, Integer> indexMap = new HashMap<Object, Integer>();
        indexes[c] = 0;
        indexMap.put(jt, c++);
        Stack<JoinTuple> stack = new Stack<>();
        Object[] tuple = null;
        boolean isFurtherPossible = true;
        B: while(isFurtherPossible && indexes[indexMap.get(jt)] < 1) {
            stack.clear();
            stack.push(jt);
            isFurtherPossible = true;
            int last = -1;
            int relationsTouched = 0;
            while(!stack.isEmpty()) {
                JoinTuple srcJt = stack.pop();
                if(!indexMap.containsKey(srcJt)) {
                    indexes[c] = 0;
                    indexMap.put(srcJt, c++);
                }
                boolean gotInside = false;
                for(VertexValue j : srcJt.getVertexValues()) {
                    if(!indexMap.containsKey(j)) {
                        indexes[c] = 0;
                        indexMap.put(j, c++);
                    }
                    JoinTuple[] joinedTuples = j.getJoinTuples(srcJt.getIndex() + 1);
                    if((srcJt == jt || srcJt.getVertexValues().length > 1) && joinedTuples[0] != null) {
                        if(srcJt != jt) {
                            if(joinedTuples[0].getIndex() == last) {
                                continue;
                            }
                        }
                        gotInside = true;
                        JoinTuple joinTuple = (JoinTuple) joinedTuples[indexes[indexMap.get(j)]];
                        if(!indexMap.containsKey(joinTuple)) {
                            indexes[c] = 0;
                            indexMap.put(joinTuple, c++);
                        }
                        if(indexes[indexMap.get(joinTuple)] == joinTuple.getProjectedTupleListSize()) {
                            indexes[indexMap.get(joinTuple)] = 0;
                            indexes[indexMap.get(j)]++;
                            if(indexes[indexMap.get(j)] >= joinedTuples.length) {
                                indexes[indexMap.get(j)] = 0;
                                indexes[indexMap.get(srcJt)]++;
                            }
                            if(isFurtherPossible)
                                continue B;//roll back
                            else
                                break B;
                        }
                        stack.push(joinTuple);
                    }
                    if(indexes[indexMap.get(srcJt)] == srcJt.getProjectedTupleListSize() - 1
                        && indexes[indexMap.get(j)] == srcJt.getVertexValues().length - 1)
                         isFurtherPossible |= false;
                     else
                         isFurtherPossible |= true;
                }
                if(tuple == null)
                    tuple = new Object[offsetAttributes[offsetAttributes.length - 1]];
                if(srcJt == jt) {
                    copy(clone.get(indexes[indexMap.get(srcJt)])
                        , offsetAttributes[srcJt.getIndex()], tuple);
                    relationsTouched++;
                } else {
                    copy(srcJt.getProjectedTuple(indexes[indexMap.get(srcJt)])
                        , offsetAttributes[srcJt.getIndex()], tuple);
                    //count += srcJt.getLeafTuples().get(indexes[indexMap.get(srcJt)]).getCount();
                    //if(srcJt.getVertexValues().length == 1)
                    if(!gotInside)
                        indexes[indexMap.get(srcJt)]++;
                    relationsTouched++;
                }
                last = srcJt.getIndex();
            }
            //for(int i = 0; i < count; i++)
            if(relationsTouched == offsetAttributes.length - 1) {
                r.add(tuple);
                //return r;//time to first diff
            }
            tuple = null;
        }
        return r;
    }
    
//    private List<Object[]> getTuplesDfsIntArrayList(int[] offsetAttributes, JoinTuple jt, List<List<Object>> clone) {
//                List<Object[]> r = new ArrayList<>();
//                
////                if(jt.getNumberOfJoinedTuples() == 0) {
////                    Object[] tuple = new Object[offsetAttributes[offsetAttributes.length - 1]];
////                    r.add(tuple);
////                    copy(clone.get(0), offsetAttributes[jt.getIndex()], tuple);
////                    return r;
////                }
//                
//                int c = 0;
//                HashMap<Object, Integer> indexMap = new HashMap<Object, Integer>();
//                indexMap.put(jt, c++);
//                IntArrayList indexes = new IntArrayList();
//                Stack<JoinTuple> stack = new Stack<>();
//                Object[] tuple = null;
//                boolean isFurtherPossible = true;
//                B: while(isFurtherPossible && getValue(indexes, indexMap.get(jt)) < clone.size()) {
//                    stack.clear();
//                    stack.push(jt);
//                    isFurtherPossible = true;
//                    String last = null;
//                    int relationsTouched = 0;
//                    while(!stack.isEmpty()) {
//                        JoinTuple srcJt = stack.pop();
//                        if(!indexMap.containsKey(srcJt))
//                            indexMap.put(srcJt, c++);
//                        boolean gotInside = false;
//                        for(VertexValue j : srcJt.getVertexValues()) {
//                            if(!indexMap.containsKey(j))
//                                indexMap.put(j, c++);
//                            List<JoinTuple> joinedTuples = j.getJoinTuples(srcJt.getIndex() + 1);
//                            if((srcJt == jt || srcJt.getVertexValues().length > 1) && !joinedTuples.isEmpty()) {
//                                if(srcJt != jt) {
//                                    if(joinedTuples.get(0).getRelationName().equals(last)) {
//                                        continue;
//                                    }
//                                }
//                                gotInside = true;
//                                JoinTuple joinTuple = (JoinTuple) joinedTuples.get(getValue(indexes, indexMap.get(j)));
//                                if(!indexMap.containsKey(joinTuple))
//                                    indexMap.put(joinTuple, c++);
//                                if(getValue(indexes, indexMap.get(joinTuple)) == joinTuple.getProjectedTupleListSize()) {
//                                    indexes.set(indexMap.get(joinTuple), 0);
//                                    indexes.set(indexMap.get(j), getValue(indexes, indexMap.get(j)) + 1);
//                                    if(getValue(indexes, indexMap.get(j)) >= joinedTuples.size()) {
//                                        indexes.set(indexMap.get(j), 0);
//                                        indexes.set(indexMap.get(srcJt), getValue(indexes, indexMap.get(srcJt)) + 1);
//                                    }
//                                    if(isFurtherPossible)
//                                        continue B;//roll back
//                                    else
//                                        break B;
//                                }
//                                stack.push(joinTuple);
//                            }
//                            if(getValue(indexes, indexMap.get(srcJt)) == srcJt.getProjectedTupleListSize() - 1
//                                && getValue(indexes, indexMap.get(j)) == srcJt.getVertexValues().length - 1)
//                                 isFurtherPossible |= false;
//                             else
//                                 isFurtherPossible |= true;
//                        }
//                        if(tuple == null)
//                            tuple = new Object[offsetAttributes[offsetAttributes.length - 1]];
//                        if(srcJt == jt) {
//                            copy(clone.get(getValue(indexes, indexMap.get(srcJt)))
//                                , offsetAttributes[srcJt.getIndex()], tuple);
//                            relationsTouched++;
//                        } else {
//                            copy(srcJt.getProjectedTupleList().get(getValue(indexes, indexMap.get(srcJt)))
//                                , offsetAttributes[srcJt.getIndex()], tuple);
//                            //count += srcJt.getLeafTuples().get(indexes[indexMap.get(srcJt)]).getCount();
//                            //if(srcJt.getVertexValues().length == 1)
//                            if(!gotInside)
//                                indexes.set(indexMap.get(srcJt), getValue(indexes, indexMap.get(srcJt)) + 1);
//                            relationsTouched++;
//                        }
//                        last = srcJt.getRelationName();
//                    }
//                    //for(int i = 0; i < count; i++)
//                    if(relationsTouched == offsetAttributes.length - 1)
//                        r.add(tuple);
//                    tuple = null;
//                }
//                return r;
//            }

//    private int getValue(IntArrayList indexes, Integer index) {
//        if(index == indexes.size()) {
//            indexes.add(0);
//            return 0;
//        } else
//            return indexes.elementData[index];
//    }

    private void copy(List<Object> list, int offset, Object[] tuple) {
        for(Object j : list)
            tuple[offset++] = j;
    }
}