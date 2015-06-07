package org.ucsd.db.fastview.delta;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;

import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.index.IIndex;
import org.ucsd.db.fastview.index.IndexLeaf;
import org.ucsd.db.fastview.index.IntArrayList;

public class FastViewUtil {
    public static final int getJoinTupleIndex(List<VertexValue> vertexValues, String relationName
        , HashMap<String, IIndex> vertexAliasToIndexMap) {
        VertexValue i = vertexValues.get(0);
        IIndex index = vertexAliasToIndexMap.get(i.getAlias());
        IndexLeaf leaf = index.getLeaf(i);
        if(leaf == null)
            return -1;
        //IntArrayList lastReList = leaf.getTupleIndexes(relationName);
        int[] array = leaf.getTupleIndexes(relationName).toArray();
        int size = array.length;
        if(array.length == 0)
            return -1;
        for(int iIndex = 1; iIndex < vertexValues.size(); iIndex++) {
            i = vertexValues.get(iIndex);
            index = vertexAliasToIndexMap.get(i.getAlias());
            //gets the rowId for other relation which joins with R on a join attribute a_indexOfAttribute. There can be only one
            leaf = index.getLeaf(i);
            if(leaf == null)
                return -1;
            //else 
            { 
                //IntArrayList lastReListNew = new IntArrayList();
                IntArrayList tupleIndexes2 = leaf.getTupleIndexes(relationName);
                int l = 0;
                for(int j = 0, k = 0; j < size && k < tupleIndexes2.size(); ) {
                    if(array[j] > tupleIndexes2.get(k))
                        k++;
                    else if(array[j] < tupleIndexes2.get(k))
                        j++;
                    else {
                        //lastReListNew.add(lastReList.get(j));
                        array[l++] = array[j];
                        j++;
                        k++;
                    }
                }
                if(l == 0)
                    return -1;
                else {
                    //lastReList = lastReListNew;
                    size = l;
                }
            }
        }
        return array[0];//lastReList.get(0);
    }
    public static boolean areAllIndexLeavesEmpty(List<IndexLeaf> indexLeaves, String relationName) {
        for(IndexLeaf i : indexLeaves) {
            if(!i.getTupleIndexes(relationName).isEmpty())
                return false;
            if(!i.getTupleIndexesOfOtherRelation(relationName).isEmpty())
                return false;
        }
        return true;
    }
    public static boolean isAggregated(TableDefinition tableDefiniton, Collection<String> projectedAttributeNames) {
        for(String i : projectedAttributeNames) {
            if(tableDefiniton.getAggregatedAttribute(i) != null)
                return true;
        }
        return false;
    }
//    public static <E extends Enum<E>> VertexValue<E> getOldVertexValue(Object... vertexValue) {
//        VertexValue<E> oldVertexValue = MemoryManager.vertexValueMap.get(vertexValue);
//        if(oldVertexValue == null) {
//            VertexValue newvertexValue = new VertexValue<>();
//            newvertexValue.setAlias((Enum)(vertexValue[0]));
//            List<Object> joinAttributeValues = new ArrayList<>(1);
//            joinAttributeValues.add(vertexValue[1]);//t_user_id - present
//            newvertexValue.setAttributeValues(joinAttributeValues);
//            MemoryManager.vertexValueMap.put(vertexValue, newvertexValue);
//            return newvertexValue;
//        } else {
//            return oldVertexValue;
//        }
//    }
    public static <E extends Enum<E>> VertexValue<E> getOldVertexValue(VertexValue<E> vertexValue) {
        VertexValue<E> oldVertexValue = MemoryManager.vertexValueMap.get(vertexValue);
        if(oldVertexValue == null) {
            MemoryManager.vertexValueMap.put(vertexValue, vertexValue);
            return vertexValue;
        } else {
            return oldVertexValue;
        }
    }
    public static <E extends Enum<E>> int indexOf(Object[] array, Object i) {
        int l = 0;
        for(Object j : array) {
            if((j == null && i == null) || j == i)
                return l;
            else if(j != null && j.equals(i))
                return l;
            else
                l++;
        }
        return -1;
    }
}
