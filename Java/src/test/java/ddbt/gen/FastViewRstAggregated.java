package ddbt.gen;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.fastview.datamodel.AggregatedAttribute;
import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.Function;
import org.ucsd.db.fastview.datamodel.JoinTuple.TYPE;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastAggregationManager;
import org.ucsd.db.fastview.delta.FastViewUtil;

public class FastViewRstAggregated {
    private enum ALIAS{
        RS, ST
    }
    public static void main(String... args) throws InstantiationException, IllegalAccessException, ClassNotFoundException, ParseException {
        new FastViewRstAggregated().insertTupleAggregateTest();
    }
    @Test
    public void insertTupleAggregateTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        MemoryManager.init(3);
        CntTable cntTable = new CntTable();
        int[] offsetAttributes = new int[] {0, 1, 1, 2};
        TableDefinition tableDefinition = new TableDefinition();
        cntTable.setTableDefinition(tableDefinition);
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        List<String> projectedAttributeNamesListR = new ArrayList<String>();
        projectedAttributeNamesListR.add("A");
        
        List<String> projectedAttributeNamesListS = new ArrayList<String>();
        
        List<String> projectedAttributeNamesListT = new ArrayList<String>();
        projectedAttributeNamesListT.add("D");
        
        Map<String, AggregatedAttribute> aggregatedAttributes = new HashMap<String, AggregatedAttribute>();
        tableDefinition.setAggregatedAttributes(aggregatedAttributes);
        aggregatedAttributes.put("A", new AggregatedAttribute(Function.SUM, 0, 0));
        aggregatedAttributes.put("D", new AggregatedAttribute(Function.SUM, 0, 1));
        
        tableDefinition.setGroupByAttributes(new ArrayList<Integer>());
        
        int MAX = 100000;
        int X = 1;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        
        for(int i = 0; i < MAX; i++) {
            insertTupleR(1, X + i, tableDefinition, projectedAttributeNamesListR, fastAggregationManager, cntTable);
        }
        for(int i = 0; i < MAX; i++) {
            insertTupleS(X + i, MAX + X + i, tableDefinition, projectedAttributeNamesListR, fastAggregationManager, cntTable);
        }
        
        for(int k = 0; k < 30; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTupleT(MAX + X + i, 1, tableDefinition, projectedAttributeNamesListT, fastAggregationManager, cntTable);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTupleT(MAX + X + i, 1, tableDefinition, projectedAttributeNamesListT, fastAggregationManager, cntTable);
            }
        }
    }
    private void insertTupleR(int i, int j, TableDefinition tableDefinition, List<String> projectedAttributeNamesList
        , FastAggregationManager fastAggregationManager, CntTable cntTable) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.RS);
        List<Object> attributeValues = new ArrayList<>();
        attributeValues.add(j);
        vertexValue.setAttributeValues(attributeValues);
        vertexValues[0] = vertexValue;
                        
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(i);
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
            , TYPE.AGGREGATED_TUPLE, 0, "R", tableDefinition, projectedAttributeNamesList);
    }
    private void insertTupleS(int i, int j, TableDefinition tableDefinition, List<String> projectedAttributeNamesList
        , FastAggregationManager fastAggregationManager, CntTable cntTable) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.RS);
        List<Object> attributeValues = new ArrayList<>();
        attributeValues.add(i);
        vertexValue.setAttributeValues(attributeValues);
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ST);
        attributeValues = new ArrayList<>();
        attributeValues.add(j);
        vertexValue.setAttributeValues(attributeValues);
        vertexValues[1] = vertexValue;
        
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, null
                , TYPE.GROUPED_TUPLE, 1, "S", tableDefinition, projectedAttributeNamesList);
                        
    }
    private void insertTupleT(int i, int j, TableDefinition tableDefinition, List<String> projectedAttributeNamesList
        , FastAggregationManager fastAggregationManager, CntTable cntTable) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ST);
        List<Object> attributeValues = new ArrayList<>();
        attributeValues.add(i);
        vertexValue.setAttributeValues(attributeValues);
        vertexValues[0] = vertexValue;
                        
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(j);
        List<Object[]> t = fastAggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                        , TYPE.AGGREGATED_TUPLE, 2, "T", tableDefinition, projectedAttributeNamesList);
        t = null;
    }
    private void deleteTupleT(int i, int j, TableDefinition tableDefinition, List<String> projectedAttributeNamesList
        , FastAggregationManager fastAggregationManager, CntTable cntTable) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ST);
        List<Object> attributeValues = new ArrayList<>();
        attributeValues.add(i);
        vertexValue.setAttributeValues(attributeValues);
        vertexValues[0] = vertexValue;
                        
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(j);
        fastAggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                        , TYPE.AGGREGATED_TUPLE, 2);
    }
}
