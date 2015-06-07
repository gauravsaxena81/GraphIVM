package org.ucsd.db.fastview.tableloader;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

import org.ucsd.db.fastview.datamodel.JoinTuple;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.Vertex;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastViewUtil;

public final class SqlTableLoader {
    public <E extends Enum<E>> void loadTable(ResultSet t, JoinTupleDefinition<E> JoinTupleDefinition, TableDefinition tableDefinition
        , CntTableAndIndices ret, int tableIndex) throws SQLException {
        int count = 0;
        while(t.next()) {
            if(count++ % 100000 == 0)
                System.out.println(count);
            
                VertexValue<E>[] vertexValues = new VertexValue[JoinTupleDefinition.getVertices().size()];
                int l = 0;
                for(Vertex<E> j : JoinTupleDefinition.getVertices()) {
                    VertexValue<E> vertexValue = new VertexValue<E>();
                    vertexValue.setAlias(j.getAlias());
                    List<Object> attributeValues = new ArrayList<>();
                    for(String k : j.getJoinAttributes()) {
                        Object object = t.getObject(k);
                        if(!t.wasNull()) {
                            if(object instanceof Integer)
                                attributeValues.add(MemoryManager.getInteger((Integer) object));
                            else
                                attributeValues.add(object);
                        }
                    }
                    vertexValue.setAttributeValues(attributeValues);
                    vertexValues[l++] = FastViewUtil.getOldVertexValue(vertexValue);
                }
//                int joinTupleIndex = FastViewUtil.getJoinTupleIndex(vertexValues, JoinTupleDefinition.getRelation()
//                                , ret.vertexAliasToIndexMap);
                JoinTuple joinTuple = null;
                int[] joinTupleIndex = MemoryManager.map.get(vertexValues);
                if(joinTupleIndex != null && joinTupleIndex[tableIndex % 2] > -1) {
                    joinTuple = ret.cntTable.get(joinTupleIndex[tableIndex % 2]);
                } else {
                    joinTuple = new JoinTuple(tableIndex);
                    joinTuple.setVertexValues(vertexValues);
                    joinTuple.setTableDefinition(tableDefinition);
                    joinTuple.setProjectedAttributeNames(JoinTupleDefinition.getProjectedAttributeNamesList());
                    if(joinTupleIndex == null) {
                        joinTupleIndex = new int[]{-1, -1};
                        MemoryManager.map.put(vertexValues, joinTupleIndex);
                    }
                    joinTupleIndex[tableIndex % 2] = ret.cntTable.size();
                    ret.cntTable.add(joinTuple);
//                    for(VertexValue j : joinTuple.getVertexValues())
//                        j.addJoinTuple(tableIndex, joinTuple);
                }
                List<Object> projectedAttributeValues = new ArrayList<>(JoinTupleDefinition.getProjectedAttributeNamesList().size());
                for(String j : JoinTupleDefinition.getProjectedAttributeNamesList()) {
                    Object object = t.getObject(j);
                    if(!t.wasNull()) {
                        if(object instanceof Integer)
                            projectedAttributeValues.add(MemoryManager.getInteger((Integer) object));
                        else
                            projectedAttributeValues.add(object.toString());
                    } else
                        projectedAttributeValues.add(null);
                }
                joinTuple.addLeafTuple(projectedAttributeValues);
        }
    }
    
//    public CntTableAndIndices loadTable(ResultSet t, List<JoinTupleDefinition> JoinTupleDefinitions) throws SQLException {
//        int[] offsetAttributes = new int[JoinTupleDefinitions.size() + 1];
//        
//        for(int i = 1; i < offsetAttributes.length; i++) {
//            offsetAttributes[i] = offsetAttributes[i - 1] 
//                + JoinTupleDefinitions.get(i - 1).getProjectedAttributeNamesList().size();
//        }
//        int count = 0;
//        CntTableAndIndices ret = new CntTableAndIndices();
//        TableDefinition tableDefinition = new TableDefinition();
//        ret.cntTable.setTableDefinition(tableDefinition);
//        tableDefinition.setOffsetAttributes(offsetAttributes);
//        while(t.next()) {
//            if(count++ % 10000 == 0)
//                System.out.println(count);
//            int tableIndex = 0;
//            for(JoinTupleDefinition i : JoinTupleDefinitions) {
//                VertexValue[] vertexValues = new VertexValue[i.getVertices().size()];
//                int l = 0;
//                for(Vertex j : i.getVertices()) {
//                    VertexValue vertexValue = new VertexValue();
//                    vertexValues[l++] = vertexValue;
//                    vertexValue.setAlias(j.getAlias());
//                    List<Object> attributeValues = new ArrayList<>();
//                    vertexValue.setAttributeValues(attributeValues);
//                    for(String k : j.getJoinAttributes()) {
//                        Object object = t.getObject(k);
//                        if(!t.wasNull()) {
//                            attributeValues.add(object);
//                        } else
//                            attributeValues.add(null);
//                    }
//                }
//                //int joinTupleIndex = FastViewUtil.getJoinTupleIndex(vertexValues, i.getRelation(), ret.vertexAliasToIndexMap);
//                JoinTuple joinTuple;
//                int joinTupleIndex = MemoryManager.map.get(vertexValues);
//                if(joinTupleIndex > -1) {
//                    joinTuple = ret.cntTable.get(joinTupleIndex);
//                } else {
//                    JoinTuple tuple = new JoinTuple(tableIndex);
//                    tuple.setRelationName(i.getRelation());
//                    tuple.setVertexValues(vertexValues);
//                    tuple.setTableDefinition(tableDefinition);
//                    MemoryManager.map.put(vertexValues, ret.cntTable.size());
//                    ret.cntTable.add(tuple);
//                    joinTuple = tuple;
////                    for(VertexValue j : vertexValues) {
////                        IIndex index = ret.vertexAliasToIndexMap.get(j.getAlias());
////                        if(index == null) {
////                            index = new HashIndex();
////                            ret.vertexAliasToIndexMap.put(j.getAlias(), index);
////                        }
////                        index.add(j, i.getRelation(), ret.cntTable.size() - 1);
////                    }
//                }
//                List<Object> projectedAttributeValues = new MyArrayList<>(i.getProjectedAttributeNamesList().size());
//                for(String j : i.getProjectedAttributeNamesList()) {
//                    Object object = t.getObject(j);
//                    if(!t.wasNull()) {
//                        projectedAttributeValues.add(object);
//                    } else
//                        projectedAttributeValues.add(null);
//                }
//                joinTuple.addLeafTuple(projectedAttributeValues);
//                tableIndex++;
//            }
//        }
//        linkJoinTuples(ret);
//        return ret;
//    }
    public void linkJoinTuples(CntTableAndIndices ret) {
        int c = 0;
        for(JoinTuple i : ret.cntTable.getTuples()) {
            if(c++ % 100000 == 0)
                System.out.println(c);
            for(VertexValue j : i.getVertexValues()) {
                j.addJoinTuple(i.getIndex(), i);
//                IIndex iIndex = ret.vertexAliasToIndexMap.get(j.getAlias());
//                IndexLeaf leaf = iIndex.getLeaf(j);
//                if(leaf != null) {
//                    IntArrayList tupleIndexesOfOtherRelation = leaf.getTupleIndexesOfOtherRelation(i.getRelationName());
//                    for(int k = 0; k < tupleIndexesOfOtherRelation.size(); k++) {
//                        ret.cntTable.get(tupleIndexesOfOtherRelation.get(k)).addJoinedTuple(j, null);
//                    }
//                }
            }
        }
    }
}