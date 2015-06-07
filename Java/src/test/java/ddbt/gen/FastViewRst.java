package ddbt.gen;

import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.JoinTuple;
import org.ucsd.db.fastview.datamodel.JoinTuple.TYPE;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastJoinDeltaManager;
import org.ucsd.db.fastview.delta.FastViewUtil;

public class FastViewRst {
    private enum ALIAS{
        RS, ST
    }
    @Test
    public void insertProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        CntTable cntTable = new CntTable();
        int[] offsetAttributes = new int[] {0, 1, 3, 4};
        TableDefinition tableDefinition = new TableDefinition();
        cntTable.setTableDefinition(tableDefinition);
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        List<String> projectedAttributeNamesListR = new ArrayList<String>();
        projectedAttributeNamesListR.add("A");
        projectedAttributeNamesListR.add("B");
        
        List<String> projectedAttributeNamesListS = new ArrayList<String>();
        projectedAttributeNamesListS.add("B");
        projectedAttributeNamesListS.add("C");
        
        List<String> projectedAttributeNamesListT = new ArrayList<String>();
        projectedAttributeNamesListT.add("C");
        projectedAttributeNamesListT.add("D");
        
        int MAX = 100000;
        int X = 1;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        
        for(int i = 0; i < MAX; i++) {
            insertTupleR(1, X + i, fastJoinDeltaManager, cntTable, tableDefinition, projectedAttributeNamesListR);
        }
        
        for(int i = 0; i < MAX; i++) {
            insertTupleS(X + i, MAX + X + i, fastJoinDeltaManager, cntTable, tableDefinition, projectedAttributeNamesListS);
        }
        
        for(int k = 0; k < 20; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTupleT(MAX + X + i, 1, fastJoinDeltaManager, cntTable, tableDefinition, projectedAttributeNamesListT);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTupleT(MAX + X + i, 1, fastJoinDeltaManager, cntTable, tableDefinition, projectedAttributeNamesListT);
            }
        }
    }
    
    private void insertTupleR(int i, int j, FastJoinDeltaManager fastJoinDeltaManager, CntTable cntTable
        , TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.RS);
        List<Object> attributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(attributeValues);
        attributeValues.add(j);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(i);
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 0, "R"
            , tableDefinition, projectedAttributeNamesList);
    }
    private void insertTupleS(int i, int j, FastJoinDeltaManager fastJoinDeltaManager, CntTable cntTable
        , TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.RS);
        List<Object> attributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(attributeValues);
        attributeValues.add(i);
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ST);
        attributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(attributeValues);
        attributeValues.add(j);
        vertexValues[1] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(i);
        projectedAttributeValues.add(j);
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1, "S"
            , tableDefinition, projectedAttributeNamesList);
    }
    private void insertTupleT(int i, int j, FastJoinDeltaManager fastJoinDeltaManager, CntTable cntTable
        , TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        JoinTuple t = new JoinTuple(TYPE.JOIN_TUPLE, 2);
        t.setTableDefinition(tableDefinition);
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ST);
        List<Object> attributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(attributeValues);
        attributeValues.add(i);
        vertexValues[0] = vertexValue;
        
        t.setVertexValues(vertexValues);
        
        t.setProjectedAttributeNames(projectedAttributeNamesList);
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(j);
        t.addLeafTuple(projectedAttributeValues);
        fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 2, "T"
            , tableDefinition, projectedAttributeNamesList);
    }
    private void deleteTupleT(int i, int j, FastJoinDeltaManager fastJoinDeltaManager, CntTable cntTable
        , TableDefinition tableDefinition, List<String> projectedAttributeNamesList) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ST);
        List<Object> attributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(attributeValues);
        attributeValues.add(i);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(j);
        fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2);
    }
}
