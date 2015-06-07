package org.ucsd.db.fastview.datamodel;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.ucsd.db.fastview.datamodel.JoinTuple.Wrapper;
import org.ucsd.db.fastview.index.IntHashMap;

public final class MemoryManager {
    /*
     * This is very fragile. It assumes that the indexes of the relation which could share the vertexvalues
     * always differ by 1, however the process of assigning ids is arbitrary!!
     */
    public static ArrayHashMap<VertexValue[], int[]> map = new ArrayHashMap<>();
    //public static ArrayHashMap<Object[], VertexValue> vertexValueMap;
    public static HashMap<VertexValue, VertexValue> vertexValueMap;
    public static IntHashMap<Wrapper> mapOfProjectedTupleToIndex;
    public static List<Object>[] tables;
    private static HashMap<Integer, Integer> intMap;
    public static int[] ensureArraySize(int[] array, int index) {
        if(array == null) 
            array = new int[16];
        while(index >= array.length)
            array = Arrays.copyOf(array, array.length * 2);    
        return array;
    }
    public static int addToTable(int index, List<Object> projectedAttributeValues) {
        int start = tables[index].size();
        tables[index].addAll(projectedAttributeValues);
        return start;
    }
    public static void deleteFromTable(int index, int start, int size) {
        if(start + size == tables[index].size()) {
            for(int i = start, m = start + size - 1; m >= i; m--)
                tables[index].remove(m);
        } else {
            for(int i = start, m = start + size - 1; m >= i; m--)
                tables[index].set(i, null);
        }
    }
    public static void init(int numberoOfTables) {
        map = new ArrayHashMap<VertexValue[], int[]>();
        vertexValueMap = new HashMap<>(2 << 10);
        mapOfProjectedTupleToIndex = new IntHashMap<>();
        intMap = new HashMap<Integer, Integer>();
        tables = new List[numberoOfTables];
        for(int i = 0; i < numberoOfTables; i++)
            tables[i] = new ArrayList<Object>();
    }
    public static Integer getInteger(Integer i) {
        Integer j = intMap.get(i);
        if(j == null) {
            intMap.put(i, i);
            return i;
        } else
            return j;
    }
    public static void destruct() {
        vertexValueMap = null;
        mapOfProjectedTupleToIndex = null;
        tables = null;
        intMap = null;
        map = null;
    }
    public static Object getMemoryManagerDs() {
        return new Object[]{vertexValueMap, mapOfProjectedTupleToIndex, tables, intMap, map};
    }
}