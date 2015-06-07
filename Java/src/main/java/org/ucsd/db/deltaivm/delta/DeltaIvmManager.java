package org.ucsd.db.deltaivm.delta;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Stack;

import org.ucsd.db.deltaivm.datamodel.Database;
import org.ucsd.db.deltaivm.datamodel.IndexedVertex;
import org.ucsd.db.deltaivm.datamodel.RelationalTable;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.index.IIndex;
import org.ucsd.db.fastview.index.IndexLeaf;
import org.ucsd.db.fastview.index.IntArrayList;

public class DeltaIvmManager {
    public List<Object[]> insertTuple(Database database, String relationName, Object[] tuple) {
        insertTupleInternal(database, relationName, tuple);
        return getTuples(database, relationName, tuple);
    }
    private void insertTupleInternal(Database database, String relationName, Object[] tuple) {
        HashMap<Enum<? extends Enum<?>>, IIndex> indexes = database.getIndexes();
        RelationalTable relation = database.getRelation(relationName);
        
        for(IndexedVertex i : relation.getIndexedVertices()) {
            IIndex iIndex = indexes.get(i.getAlias());
            VertexValue vertexValue = new VertexValue();
            vertexValue.setAlias(i.getAlias());
            List<Object> list = new ArrayList<Object>();
            for(int j = 0; j < i.getJoinAttributeIndices().size(); j++)
                list.add(tuple[i.getJoinAttributeIndices().get(j)]);
            vertexValue.setAttributeValues(list);
            iIndex.add(vertexValue, relationName, relation.getTable().size());
        }
        IntArrayList list = relation.getProjectedTupleIndex().get(tuple);
        if(list == null) {
            list = new IntArrayList();
            relation.getProjectedTupleIndex().put(tuple, list);
        }
        list.add(relation.getTable().size());
        relation.getTable().add(tuple);        
    }
    public void insertTupleWithoutReturn(Database database, String relationName, Object[] tuple) {
        insertTupleInternal(database, relationName, tuple);
    }
    public List<Object[]> getTuples(Database database, String relationName, Object[] tuple) {
        HashMap<Enum<? extends Enum<?>>, IIndex> indexes = database.getIndexes();
        LinkedList<String> queue = new LinkedList<>();
        List<Object[]> table1 = new ArrayList<Object[]>();
        int columnCount = getColumnCount(database, relationName);
        Object[] joinedTuple = new Object[columnCount];
        int offset = database.getViewDefinition().getProjectedColumnOffsetMap().get(relationName);
        IntArrayList projectedAttributeIndices = database.getRelation(relationName).getProjectedAttributeIndices();
        for(int i = 0, k = offset; i < projectedAttributeIndices.size(); i++, k++)
            joinedTuple[k] = tuple[projectedAttributeIndices.get(i)];
        table1.add(joinedTuple);
        queue.push(relationName);
        HashSet<String> consideredTables = new HashSet<String>();
        List<IndexLeaf> leaves = new ArrayList<>();
        LinkedList<Object[]> fullTuples = new LinkedList<>();
        fullTuples.push(tuple);
        while(consideredTables.size() < database.getNumberOfRelations() && !queue.isEmpty()) {
            String currentRelationName = queue.pop();
            RelationalTable currentRelation = database.getRelation(currentRelationName);
            List<Object[]> tableOut = new ArrayList<Object[]>();
            int nextRelationAdded = 0;
            int neighbourSize = database.getJoinGraph().getJoinRelations(currentRelationName).size();
            A: for(Object[] j : table1) {
                leaves.clear();
                int currentNeighbourSize = neighbourSize;
                Object[] lastTuple = fullTuples.poll();
                for(IndexedVertex i : currentRelation.getIndexedVertices()) {
                    IIndex iIndex = indexes.get(i.getAlias());
                    IndexLeaf leaf = iIndex.getLeaf(getVertexValue(i, lastTuple//j
                        , 0/*database.getViewDefinition().getProjectedColumnOffsetMap().get(currentRelationName)*/));
                    //Can we join further?
                    if(leaf == null || leaf.getTupleIndexesOfOtherRelation(currentRelationName).isEmpty())
                        continue A;
                    if(consideredTables.contains(leaf.getOtherRelationName(currentRelationName))) {
                        currentNeighbourSize--;
                        continue;
                    }
                    leaves.add(leaf);
                }
                if(leaves.size() < currentNeighbourSize)
                    continue;
                else if(nextRelationAdded == 0) {
                    nextRelationAdded = 1;
                    for(String i : database.getJoinGraph().getJoinRelations(currentRelationName))
                        if(!consideredTables.contains(i))
                            queue.add(i);
                }
                consideredTables.add(currentRelationName);
                for(IndexLeaf i : leaves) {
                    String nextRelationName = i.getOtherRelationName(currentRelationName);
                    RelationalTable nextRelation = database.getRelation(nextRelationName);
                    IntArrayList nextTupleIndices = i.getTupleIndexes(nextRelationName);
                    for(int k = 0; k < nextTupleIndices.size(); k++) {
                        joinedTuple = new Object[columnCount];
                        System.arraycopy(j, 0, joinedTuple, 0, j.length);
                        Object[] attributeValues = nextRelation.getTable().get(nextTupleIndices.get(k));
                        fullTuples.offer(attributeValues);
                        copy(attributeValues, nextRelation.getProjectedAttributeIndices(), joinedTuple
                            , database.getViewDefinition().getProjectedColumnOffsetMap().get(nextRelationName), false);
                        tableOut.add(joinedTuple);
                    }
                }
            }
            if(!tableOut.isEmpty() || consideredTables.size() < database.getNumberOfRelations())
                table1 = tableOut;
        }
        return table1;
    }
    
    private void copy(Object[] j, IntArrayList projectedAttributeIndices
        , Object[] joinedTuple, int offset, boolean b) {
        int offset1 = b? offset : 0;
        for(int i = 0, k = offset; i < projectedAttributeIndices.size(); i++, k++)
            joinedTuple[k] = j[projectedAttributeIndices.get(i) + offset1];
    }

    private VertexValue getVertexValue(IndexedVertex indexedVertex, Object[] tuple, int offset) {
        VertexValue vertexValue = new VertexValue();
        vertexValue.setAlias(indexedVertex.getAlias());
        List<Object> list = new ArrayList<Object>();
        for(int i = 0; i < indexedVertex.getJoinAttributeIndices().size(); i++)
            list.add(tuple[offset + indexedVertex.getJoinAttributeIndices().get(i)]);
        vertexValue.setAttributeValues(list);
        return vertexValue;
    }
    private int getColumnCount(Database database, String relationName) {
        int count = 0;
        Stack<String> stack = new Stack<>();
        stack.push(relationName);
        HashSet<String> consideredRelatons = new HashSet<>();
        while(!stack.isEmpty()) {
            relationName = stack.pop();
            consideredRelatons.add(relationName);
            count += database.getRelation(relationName).getProjectedAttributeIndices().size();
            for(String i : database.getJoinGraph().getJoinRelations(relationName))
                if(!consideredRelatons.contains(i))
                    stack.push(i);
        }
        return count;
    }
    public void deleteTupleWithoutReturn(Database database, String relationName, Object[] tuple) {
        deleteTupleInternal(database, relationName, tuple);
    }
    private void deleteTupleInternal(Database database, String relationName, Object[] tuple) {
        HashMap<Enum<? extends Enum<?>>, IIndex> indexes = database.getIndexes();
        RelationalTable relation = database.getRelation(relationName);
        IntArrayList tupleList = relation.getProjectedTupleIndex().get(tuple);
        int index = tupleList.remove(tupleList.size() - 1);
        if(tupleList.isEmpty()) {
            relation.getProjectedTupleIndex().remove(tuple);
        }
        for(IndexedVertex i : relation.getIndexedVertices()) {
            IIndex iIndex = indexes.get(i.getAlias());
            VertexValue vertexValue = new VertexValue();
            vertexValue.setAlias(i.getAlias());
            List<Object> list = new ArrayList<Object>();
            for(int j = 0; j < i.getJoinAttributeIndices().size(); j++)
                list.add(tuple[i.getJoinAttributeIndices().get(j)]);
            vertexValue.setAttributeValues(list);
            iIndex.delete(vertexValue, relationName, index);
        }
        relation.getTable().set(index, null);        
    }
    public List<Object[]> deleteTuple(Database database, String relationName, Object[] tuple) {
        List<Object[]> tuples = getTuples(database, relationName, tuple);
        deleteTupleInternal(database, relationName, tuple);
        return tuples;
    }
}