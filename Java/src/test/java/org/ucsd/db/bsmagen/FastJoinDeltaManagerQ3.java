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

public class FastJoinDeltaManagerQ3 {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID,
    }
    private static CntTable cntTable;
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(3);
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[4];
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
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("t_tweet_id"));
        vertex.setAlias(ALIAS.ALIASTWEETID);
        vertices.add(vertex);
        tweetDefinition.setVertices(vertices);
        tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("t_tweet_date", "t_user_id", "t_tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[2] = offsetAttributes[1] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> retweetDefinition = new JoinTupleDefinition<ALIAS>();
        retweetDefinition.setRelation("retweet");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("retweet_tweet_id"));
        vertex.setAlias(ALIAS.ALIASTWEETID);
        vertices.add(vertex);
        retweetDefinition.setVertices(vertices);
        retweetDefinition.setProjectedAttributeNamesList(Arrays.asList("r_tweet_id", "r_tweet_date", "retweet_tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(retweetDefinition.getRelation(), retweetDefinition);
        offsetAttributes[3] = offsetAttributes[2] + retweetDefinition.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(//Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", "");
            Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "gaurav");) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                    tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id, tweet_id "
                    + "as t_tweet_id, tweet_date as t_tweet_date from tweet"), tweetDefinition, 
                    tableDefinition, cntTableAndIndices, 1);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select tweet_id as r_tweet_id"
                    + ", tweet_date as r_tweet_date, retweet_tweet_id from retweet"), retweetDefinition, 
                    tableDefinition, cntTableAndIndices, 2);
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
        int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        for(int k = 0, x = X; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertReTweet(2, x + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(2, x + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

    @Test
    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        for(int k = 0; k < 10; k++) {
            for(int i = 0; i < MAX; i++) {
                insertReTweet(2, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(2, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

    private void insertReTweet(int retweetTweetId, int tweetId, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);//tweet_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(tweetId);
        projectedAttributeValues.add("2992-01-01");
        projectedAttributeValues.add(retweetTweetId);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 2, "retweet"
                , cntTable.getTableDefinition(), null);
//            for(Object[] i : t)
//                viewMaintainer.insertTuple(i);
            t = null;
        } else {
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2, "retweet"
                            , cntTable.getTableDefinition(), null);
        }
    }
    private void deleteReTweet(int retweetTweetId, int tweetId, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintenaner
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);//tweet_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(tweetId);
        projectedAttributeValues.add("2992-01-01");
        projectedAttributeValues.add(retweetTweetId);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 2);
            for(Object[] i : t)
                viewMaintenaner.deleteTuple(i);
            t = null;
        } else {
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2);
        }
    }
//    @Test
//    public void insertJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 50000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createUser(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createTweet(X + i, X + i);
//            List<Object[]> t2 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet(X + i, 2 * X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            }
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet(X + i, 2 * X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//            }
//        }
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createUser(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createTweet(X + i, X + i);
//            List<Object[]> t2 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet(X + i, 2 * X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
//    }
//    @Test
//    public void deleteJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 50000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createUser(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createTweet(X + i, X + i);
//            List<Object[]> t2 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        for(int k = 0; k < 10; k++) {
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet(X + i, 2 * X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet(X + i, 2 * X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//            }
//            tm = (System.nanoTime() - tm)  / 1000000;
//            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
//        }
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet(X + i, 2 * X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet(X + i, 2 * X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//        }
//        tm = (System.nanoTime() - tm)  / 1000000;
//        System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
//    }
    @Test
    public void insertJoinedTuple1ToMTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        int X = 1000000000;
        insertUser(X, fastJoinDeltaManager);
        for(int i = 0; i < MAX; i++) {
            insertTweet(X, X + i, fastJoinDeltaManager, viewMaintainer, false);
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertReTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
        }
    }
    @Test
    public void deleteJoinedTuple1ToMTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        int X = 1000000000;
        insertUser(X, fastJoinDeltaManager);
        for(int i = 0; i < MAX; i++) {
            insertTweet(X, X + i, fastJoinDeltaManager, viewMaintainer, false);
        }
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertReTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    
//    private void deleteTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
//        , boolean doReturn) {
//        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
//        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
//        vertexValue.setAlias(ALIAS.ALIASUSERID);
//        List<Object> joinAttributeValues = new ArrayList<>(1);
//        vertexValue.setAttributeValues(joinAttributeValues);
//        joinAttributeValues.add(user);//t_user_id - present
//        vertexValues[0] = vertexValue;
//        
//        List<Object> projectedAttributeValues = new ArrayList<Object>();
//        projectedAttributeValues.add("2992-01-01");//tweet date;
//        projectedAttributeValues.add(tweet);//tweet_id - not present
//        projectedAttributeValues.add(user);
//        if(doReturn) {
//            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1);
//            t = null;
//        } else
//            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1);
//    }

    private void insertTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(tweet);//t_user_id - present
        vertexValues[1] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add("2992-01-01");//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 1
            , "tweet", cntTable.getTableDefinition(), null);
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1
                , "tweet", cntTable.getTableDefinition(), null);
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
