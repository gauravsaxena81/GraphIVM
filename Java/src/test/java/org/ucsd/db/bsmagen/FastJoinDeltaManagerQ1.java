package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.Vertex;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastJoinDeltaManager;
import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.fastview.tableloader.SqlTableLoader;
import org.ucsd.db.view.ViewMaintainer;

public class FastJoinDeltaManagerQ1 {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID,
    }
    ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3});
    private static CntTable cntTable;
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(2);
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
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
        tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("tweet_id", "tweet_date", "t_user_id"));

        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[2] = offsetAttributes[1] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "gaurav")) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                        tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id, tweet_id, tweet_date from tweet"), tweetDefinition, 
                        tableDefinition, cntTableAndIndices, 1);
            cntTable = cntTableAndIndices.cntTable;
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);
            //TestUtil.printSize(cntTable);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }

    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void scanTest() {
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        fastJoinDeltaManager.scan(cntTable);
    }
    @Test
    public void insertProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        System.gc();
        int MAX = 50000;
        int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        for(int k = 0; k < 15; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(2, X + i, fastJoinDeltaManager, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteTweet(2, X + i, fastJoinDeltaManager, false);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

//    @Test
//    public void insertProjectedTupleThroughputTest() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 500000;
//        int x = 1000000000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        for(int k = 0; k < 10; k++) {
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createTweet(2, x + i);
//                fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, ins);
//            }
//        }
//        fastJoinDeltaManager.scan(cntTable, createTweet(2, x));
//        MAX = 500000;
//        setUp();
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createTweet(2, x + i);
//            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, ins);
//        }
//        fastJoinDeltaManager.scan(cntTable, createTweet(2, x));
//        long t = (System.nanoTime() - tm)  / 1000000;
//        System.out.println(TestUtil.getTestName() + t + "/" + MAX);
//    }
    @Test
    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        final int MAX = 50000;
        int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(2, X + i, fastJoinDeltaManager, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(2, X + i, fastJoinDeltaManager, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

    private void insertTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet), 1
            , "tweet", cntTable.getTableDefinition(), null);
//            for(Object[] i : t)
//                viewMaintainer.insertTuple(i);
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet), 1
                , "tweet", cntTable.getTableDefinition(), null);
    }
    private void deleteTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet), 1);
            for(Object[] i : t)
                viewMaintainer.insertTuple(i);
        } else
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet), 1);
    }

    private VertexValue<ALIAS>[] getvertexValues(int user) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        return vertexValues;
    }

    private List<Object> getprojectedAttributeValues(int user, int tweet) {
        ArrayList<Object> projectedAttributeValues = new ArrayList<Object>(3);
        projectedAttributeValues.add("2992-01-01");//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        return projectedAttributeValues;
    }

    @Test
    public void insertJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int x = 1000000000;
        for(int i = 0; i < MAX; i++) {
            insertUser(x + i, fastJoinDeltaManager);
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(x + i, x + i, fastJoinDeltaManager, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteTweet(x + i, x + i, fastJoinDeltaManager, false);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    
    @Test
    public void deleteJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int x = 1000000000;
        for(int i = 0; i < MAX; i++) {
            insertUser(x + i, fastJoinDeltaManager);
        }
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(x + i, x + i, fastJoinDeltaManager, true);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(x + i, x + i, fastJoinDeltaManager, false);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

    private void insertUser(int user, FastJoinDeltaManager fastJoinDeltaManager) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(user);
        
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 0, "users"
            , cntTable.getTableDefinition(), null);
    }

    public static void destruct() {
        cntTable = null;
        MemoryManager.destruct();
    }
}
