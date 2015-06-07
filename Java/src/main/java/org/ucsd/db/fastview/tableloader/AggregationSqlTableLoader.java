package org.ucsd.db.fastview.tableloader;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.ucsd.db.fastview.datamodel.JoinTuple;
import org.ucsd.db.fastview.datamodel.JoinTuple.TYPE;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.Vertex;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastViewUtil;

public final class AggregationSqlTableLoader {
    
    public <E extends Enum<E>>void loadTable(ResultSet t, JoinTupleDefinition<E> joinTupleDefiniton, TableDefinition tableDefinition
                    , CntTableAndIndices ret, int tableIndex) throws SQLException {
        int count = 0;
        while(t.next()) {
            if(count++ % 100000 == 0)
            System.out.println(count);
            VertexValue<E>[] vertexValues = new VertexValue[joinTupleDefiniton.getVertices().size()];
            int l = 0;
            for(Vertex<E> j : joinTupleDefiniton.getVertices()) {
                VertexValue<E> vertexValue = new VertexValue<E>();
                vertexValue.setAlias(j.getAlias());
                List<Object> attributeValues = new ArrayList<>();
                //attributeValues.add(j.getAlias());
                for(String k : j.getJoinAttributes()) {
                    Object object = t.getObject(k);
                    if(!t.wasNull()) {
                        attributeValues.add(object);
                    } else
                        attributeValues.add(null);
                }
                vertexValue.setAttributeValues(attributeValues);
                //vertexValues[l++] = FastViewUtil.getOldVertexValue(attributeValues.toArray());
                vertexValues[l++] = FastViewUtil.getOldVertexValue(vertexValue);
            }
            //int joinTupleIndex = FastViewUtil.getJoinTupleIndex(vertexValues, joinTupleDefiniton.getRelation()
              //          , ret.vertexAliasToIndexMap);
            JoinTuple joinTuple;
            int[] joinTupleIndex = MemoryManager.map.get(vertexValues);
            if(joinTupleIndex != null && joinTupleIndex[tableIndex % 2] > -1) {
                joinTuple = ret.cntTable.get(joinTupleIndex[tableIndex % 2]);
            } else {
                if(joinTupleDefiniton.getProjectedAttributeNamesList().isEmpty())
                    joinTuple = new JoinTuple(TYPE.JOIN_TUPLE, tableIndex);
                else if(FastViewUtil.isAggregated(tableDefinition, joinTupleDefiniton.getProjectedAttributeNamesList())) {
                    joinTuple = new JoinTuple(TYPE.AGGREGATED_TUPLE, tableIndex);
                    joinTuple.setTableDefinition(tableDefinition);
                    joinTuple.setProjectedAttributeNames(joinTupleDefiniton.getProjectedAttributeNamesList());
                } else {
                    joinTuple = new JoinTuple(tableIndex);
                    joinTuple.setProjectedAttributeNames(joinTupleDefiniton.getProjectedAttributeNamesList());
                }
                joinTuple.setTableDefinition(tableDefinition);
                joinTuple.setVertexValues(vertexValues);
                if(joinTupleIndex == null) {
                    joinTupleIndex = new int[]{-1, -1};
                    MemoryManager.map.put(vertexValues, joinTupleIndex);
                }
                joinTupleIndex[tableIndex % 2] = ret.cntTable.size();
                ret.cntTable.add(joinTuple);
            }
            if(joinTupleDefiniton.getProjectedAttributeNamesList().isEmpty())
                joinTuple.addLeafTuple(Collections.EMPTY_LIST);
            else {
                List<Object> projectedAttributeValues = new ArrayList<>();
                for(String j : joinTupleDefiniton.getProjectedAttributeNamesList()) {
                    Object object = t.getObject(j);
                    if(!t.wasNull()) {
                        projectedAttributeValues.add(object);
                    } else
                        projectedAttributeValues.add(null);
                }
                joinTuple.addLeafTuple(projectedAttributeValues);
            }
        }
    }
    
//    public CntTableAndIndices loadTable(ResultSet t, List<JoinTupleDefinition> joinTupleDefinitions
//        , Map<String, AggregatedAttribute> aggregatedAttributes) throws SQLException {
//        int[] offsetAttributes = new int[joinTupleDefinitions.size() + 1];
//        
//        for(int i = 1; i < offsetAttributes.length; i++) {
//            offsetAttributes[i] = offsetAttributes[i - 1] 
//                + joinTupleDefinitions.get(i - 1).getProjectedAttributeNamesList().size();
//        }
//        int count = 0;
//        CntTableAndIndices ret = new CntTableAndIndices();
//        TableDefinition tableDefinition = new TableDefinition();
//        ret.cntTable.setTableDefinition(tableDefinition);
//        tableDefinition.setAggregatedAttributes(aggregatedAttributes);
//        tableDefinition.setOffsetAttributes(offsetAttributes);
//        for(JoinTupleDefinition i : joinTupleDefinitions) {
//            tableDefinition.getJoinDefinitionMap().put(i.getRelation(), i);
//        }
//        while(t.next()) {
//            if(count++ % 100 == 0)
//                System.out.println(count);
//            int tableIndex = 0;
//            for(JoinTupleDefinition i : joinTupleDefinitions) {
//                List<VertexValue<E>> vertexValues = new ArrayList<VertexValue<E>>();
//                for(Vertex j : i.getVertices()) {
//                    VertexValue<E> vertexValue = new VertexValue<E>();
//                    vertexValues.add(vertexValue);
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
//                int joinTupleIndex = FastViewUtil.getJoinTupleIndex(vertexValues, i.getRelation()
//                            , ret.vertexAliasToIndexMap);
//                JoinTuple joinTuple;
//                if(joinTupleIndex > -1) {
//                    joinTuple = ret.cntTable.get(joinTupleIndex);
//                } else {
//                    if(i.getProjectedAttributeNamesList().isEmpty())
//                        joinTuple = new JoinTuple(TYPE.GROUPED_TUPLE, tableIndex);
//                    else if(FastViewUtil.isAggregated(tableDefinition, i.getProjectedAttributeNamesList())) {
//                        joinTuple = new JoinTuple(TYPE.AGGREGATED_TUPLE, tableIndex);
//                        joinTuple.setTableDefinition(tableDefinition);
//                        joinTuple.setProjectedAttributeNames(i.getProjectedAttributeNamesList());
//                    } else {
//                        joinTuple = new JoinTuple(tableIndex);;
//                        joinTuple.setProjectedAttributeNames(i.getProjectedAttributeNamesList());
//                    }
//                    joinTuple.setRelationName(i.getRelation());
//                    joinTuple.setVertexValues(vertexValues);
//                    ret.cntTable.add(joinTuple);
//                    for(VertexValue<E> j : vertexValues) {
//                        IIndex index = ret.vertexAliasToIndexMap.get(j.getAlias());
//                        if(index == null) {
//                            index = new HashIndex();
//                            ret.vertexAliasToIndexMap.put(j.getAlias(), index);
//                        }
//                        index.add(j, i.getRelation(), ret.cntTable.size() - 1);
//                    }
//                }
//                if(i.getProjectedAttributeNamesList().isEmpty())
//                    joinTuple.addLeafTuple(null);
//                else {
//                    List<Object> projectedAttributeValues = new ArrayList<>();
//                    for(String j : i.getProjectedAttributeNamesList()) {
//                        projectedAttributeValues.add(t.getObject(j));
//                    }
//                    joinTuple.addLeafTuple(projectedAttributeValues);
//                }
//                tableIndex++;
//            }
//        }
//        linkJoinTuples(ret);
//        return ret;
//    }

    public void linkJoinTuples(CntTableAndIndices ret) {
        for(JoinTuple i : ret.cntTable.getTuples()) {
            for(VertexValue j : i.getVertexValues()) {
                j.addJoinTuple(i.getIndex(), i);
//                IIndex iIndex = ret.vertexAliasToIndexMap.get(j.getAlias());
//                IndexLeaf leaf = iIndex.getLeaf(j);
//                if(leaf != null) {
//                    IntArrayList tupleIndexesOfOtherRelation = leaf.getTupleIndexesOfOtherRelation(i.getRelationName());
//                    for(int k = 0; k < tupleIndexesOfOtherRelation.size(); k++) {
//                        i.addJoinedTuple(j, ret.cntTable.get(tupleIndexesOfOtherRelation.get(k)));
//                    }
//                }
            }
        }
    }
}