package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.fastview.datamodel.AggregatedAttribute;
import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.Function;
import org.ucsd.db.fastview.datamodel.JoinTuple.TYPE;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.Vertex;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastAggregationManager;
import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.index.IntArrayList;
import org.ucsd.db.fastview.tableloader.AggregationSqlTableLoader;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.view.ViewMaintainer;

public class FastJoinDeltaManager1MQ1Aggregation {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID,
    }
    private static CntTable cntTable;
    private static ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0});
    private static final List<String> PROJECTED_ATTRIBUTES_LIST_TWEET = Arrays.asList("t_user_id");
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(2);
        AggregationSqlTableLoader sqlTableLoader = new AggregationSqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[3];
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        JoinTupleDefinition<ALIAS> userDefiniton = new JoinTupleDefinition<ALIAS>();
        userDefiniton.setRelation("users");
        List<Vertex<ALIAS>> vertices = new ArrayList<>();
        Vertex<ALIAS> vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("u_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        userDefiniton.setVertices(vertices);
        userDefiniton.setProjectedAttributeNamesList(Arrays.asList("u_user_id")); 
        
        tableDefinition.getJoinDefinitionMap().put(userDefiniton.getRelation(), userDefiniton);
        offsetAttributes[1] = offsetAttributes[0] + userDefiniton.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> tweetDefinition = new JoinTupleDefinition<ALIAS>();
        tweetDefinition.setRelation("tweet");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("t_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        tweetDefinition.setVertices(vertices);
        tweetDefinition.setProjectedAttributeNamesList(PROJECTED_ATTRIBUTES_LIST_TWEET);
        
        tableDefinition.getJoinDefinitionMap().put(userDefiniton.getRelation(), userDefiniton);
        offsetAttributes[2] = offsetAttributes[1] + userDefiniton.getProjectedAttributeNamesList().size();
        
        HashMap<String, AggregatedAttribute> aggregatedRelations = new HashMap<String, AggregatedAttribute>();
        tableDefinition.setAggregatedAttributes(aggregatedRelations);
        aggregatedRelations.put("t_user_id", new AggregatedAttribute(Function.COUNT, 0, 1));
        tableDefinition.setGroupByAttributes(Arrays.asList(0));
        
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav")) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                        tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id from tweet"), tweetDefinition, 
                        tableDefinition, cntTableAndIndices, 1);
            cntTable = cntTableAndIndices.cntTable;
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }

    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void insertProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        int x = 1000000000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        int[] aggregatedAttributes = new int[]{0};
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.addAll(0, new int[]{0});
        for(int k = 0; k < 15; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(2, x + i, fastAggregationManager, aggregatedAttributes, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTweet(2, x + i, fastAggregationManager, aggregatedAttributes, false);
            }
        }
    }
//    @Test
//    public void insertProjectedTupleThroughputTest() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 5000;
//        int x = 1000000000;
//        FastAggregationManager fastJoinDeltaManager = new FastAggregationManager();
//        for(int k = 0; k < 15; k++) {
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createTweet(2, x + i);
//                fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, ins);
//            }
//            List<Object[]> scan = fastJoinDeltaManager.scan(cntTable, createTweet(2, x));
//        }
//        setUp();
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createTweet(2, x + i);
//            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, ins);
//        }
//        List<Object[]> scan = fastJoinDeltaManager.scan(cntTable, createTweet(2, x));
//        System.out.println("insertTupleJoinsWithAnotherPerformanceTest:" + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
//    }
    @Test
    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        final int MAX = 50000;
        int x = 1000000000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        int[] aggregatedAttributes = new int[]{0};
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(2, x + i, fastAggregationManager, aggregatedAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(2, x + i, fastAggregationManager, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }

    private void insertTweet(int user, int tweet, FastAggregationManager fastAggregationManager
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>(1);
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                    , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregatedAttributes, 1);
            t = null;
        } else
            fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
    }
    private void deleteTweet(int user, int tweet, FastAggregationManager fastAggregationManager
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregatedAttributes, -1);
            t = null;
        } else
            fastAggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                        , TYPE.AGGREGATED_TUPLE, 1);
    }
    @Test
    public void insertJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastAggregationManager fastJoinDeltaManager = new FastAggregationManager();
        int x = 1000000000;
        int[] aggregatedAttributes = new int[]{0};
        for(int i = 0; i < MAX; i++) {
            insertUser(x + i, fastJoinDeltaManager);
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                for(int j = 1; j < 5; j++) {
                    insertTweet(x + i, x + i + j, fastJoinDeltaManager, aggregatedAttributes, true);
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                for(int j = 1; j < 5; j++) {
                    deleteTweet(x + i, x + i + j, fastJoinDeltaManager, aggregatedAttributes, false);
                }
            }
        }
    }
    
//    public void insertJoinedTupleThroughputTest() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 5000;
//        FastAggregationManager fastJoinDeltaManager = new FastAggregationManager();
//        int x = 1000000000;
//        for(int k = 0; k < 15; k++, x += MAX) {
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createUser(x + i);
//                List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                for(int j = 1; j < 5; j++) {
//                    JoinTuple ins = createTweet(x + i, x + i + j);
//                    fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, ins);
//                }
//                List<Object[]> t1 = fastJoinDeltaManager.scan(cntTable, createUser(x + i));
//            }
//        }
//        setUp();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createUser(x + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            for(int j = 1; j < 5; j++) {
//                JoinTuple ins = createTweet(x + i, x + i + j);
//                fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, ins);
//            }
//            List<Object[]> t1 = fastJoinDeltaManager.scan(cntTable, createUser(x + i));
//        }
//        System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
//    }
    
    @Test
    public void deleteJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        int[] aggregatedAttributes = new int[]{0};
        int x = 2000000000;
        for(int k = 0; k < 15; k++, x += MAX) {
            for(int i = 0; i < MAX; i++) {
                insertUser(x + i, fastAggregationManager);
            }
            for(int i = 0; i < MAX; i++) {
                insertTweet(x + i, x + i, fastAggregationManager, aggregatedAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(x + i, x + i, fastAggregationManager, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }

    private void insertUser(int user, FastAggregationManager fastAggregationManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.JOIN_TUPLE, 0, "users"
            , cntTable.getTableDefinition(), null);
    }
}
