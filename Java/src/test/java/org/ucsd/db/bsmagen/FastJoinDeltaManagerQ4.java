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

public class FastJoinDeltaManagerQ4 {
    private static CntTable cntTable;
    private enum ALIAS {
        ALIASTWEETID, ALIASRETWEETID, ALIASUSERID,
    }
   @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(4);
        
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[5];
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        JoinTupleDefinition<ALIAS> tweetDefiniton = new JoinTupleDefinition<ALIAS>();
        tweetDefiniton.setRelation("tweet");
        List<Vertex<ALIAS>> vertices = new ArrayList<>();
        Vertex<ALIAS> vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("tweet_id"));
        vertex.setAlias(ALIAS.ALIASTWEETID);
        vertices.add(vertex);
        tweetDefiniton.setVertices(vertices);
        tweetDefiniton.setProjectedAttributeNamesList(Arrays.asList("t_user_id", "tweet_id"));

        tableDefinition.getJoinDefinitionMap().put(tweetDefiniton.getRelation(), tweetDefiniton);
        offsetAttributes[1] = offsetAttributes[0] + tweetDefiniton.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> retweet1Definiton = new JoinTupleDefinition<ALIAS>();
        retweet1Definiton.setRelation("retweet1");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r1_retweet_tweet_id"));
        vertex.setAlias(ALIAS.ALIASTWEETID);
        vertices.add(vertex);
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r1_tweet_id"));
        vertex.setAlias(ALIAS.ALIASRETWEETID);
        vertices.add(vertex);
        retweet1Definiton.setVertices(vertices);
        retweet1Definiton.setProjectedAttributeNamesList(Arrays.asList("r1_tweet_date", "r1_retweet_tweet_id", "r1_tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(retweet1Definiton.getRelation(), retweet1Definiton);
        offsetAttributes[2] = offsetAttributes[1] + retweet1Definiton.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> retweet2Definiton = new JoinTupleDefinition<ALIAS>();
        retweet2Definiton.setRelation("retweet2");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r2_retweet_tweet_id"));
        vertex.setAlias(ALIAS.ALIASRETWEETID);
        vertices.add(vertex);
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r2_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        retweet2Definiton.setVertices(vertices);
        retweet2Definiton.setProjectedAttributeNamesList(Arrays.asList("r2_tweet_id", "r2_retweet_tweet_id", "r2_user_id"));
        
        tableDefinition.getJoinDefinitionMap().put(retweet2Definiton.getRelation(), retweet2Definiton);
        offsetAttributes[3] = offsetAttributes[2] + retweet2Definiton.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> followerDefiniton = new JoinTupleDefinition<ALIAS>();
        followerDefiniton.setRelation("users");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        vertex = new Vertex<ALIAS>();
        followerDefiniton.setVertices(vertices);
        followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("f_user_id"));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
        offsetAttributes[4] = offsetAttributes[3] + followerDefiniton.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "")){
            connection.setAutoCommit(false);
            
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select A.user_id as t_user_id, A.tweet_id from tweet A")
                , tweetDefiniton, tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.tweet_date as r1_tweet_date from retweet B"), retweet1Definiton, tableDefinition, cntTableAndIndices, 1);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select C.retweet_tweet_id as r2_retweet_tweet_id"
                + ", C.user_id as r2_user_id, C.tweet_id as r2_tweet_id from retweet C"), retweet2Definiton
                , tableDefinition, cntTableAndIndices, 2);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select D.user_id as f_user_id"
                + " from users D"), followerDefiniton, tableDefinition, cntTableAndIndices, 3);
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);

            cntTable = cntTableAndIndices.cntTable;
            //s.sizeOf(cntTable.size());
            //s.deepSizeOf(1, false, cntTable.getTuples().get(0).getProjectedTupleList()).getCalculated();
            //TestUtil.printSize(cntTable, MemoryManager.getMemoryManagerDs());
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
        int MAX = 5000;
        int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
        for(int k = 0; k < 11; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertFollower(17, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteFollower(17, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
        }
    }

    @Test
    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
        for(int k = 0; k < 10; k++) {
            for(int i = 0; i < MAX; i++) {
                insertFollower(17, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteFollower(17, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
//    @Test
//    public void insertJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 50000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            insertTweet(X + i, fastJoinDeltaManager);
//            insertReTweet1(X + i, 2 * X + i, fastJoinDeltaManager);
//        }
//        for(int k = 0; k < 10; k++) {
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                insertReTweet2(2 * X + i, X + i, fastJoinDeltaManager);
//                insertFollower(X + i, 2 * X + i, fastJoinDeltaManager, false);
//            }
//            long t = (System.nanoTime() - tm)  / 1000000;
//            for(int i = 0; i < MAX; i++) {
//                deleteReTweet2(2 * X + i, X + i, fastJoinDeltaManager);
//                deleteFollower(X + i, 2 * X + i, fastJoinDeltaManager, false);
//            }
//            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
//        }
//    }
//    @Test
//    public void deleteJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 5000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createTweet(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createReTweet1(X + i, 2 * X + i);
//            t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet2(2 * X + i, X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//                ins = createFollower(X + i, 2 * X + i);
//                t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            }
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createFollower(X + i, 2 * X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//            }
//        }
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createTweet(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createReTweet1(X + i, 2 * X + i);
//            t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet2(2 * X + i, X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createFollower(X + i, 2 * X + i);
//            t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createFollower(X + i, 2 * X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//        }
//        System.out.println("deleteJoinedTuple1To1Test:" + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
//    }
    @Test
    public void insertJoinedTuple1To1Test() throws InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1});
        int X = 1000000000;
        int fanout = 294;
        for(int j = 0; j < fanout; j++)
            insertReTweet1(729, X + j, fastJoinDeltaManager);
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < fanout; j++)
                insertReTweet2(X + j, X + i, fastJoinDeltaManager, viewMaintainer, false);
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertFollower(X + i, 2 * X + i, fastJoinDeltaManager, viewMaintainer, true);                
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteFollower(X + i, 2 * X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
        }
    }
    @Test
    public void deleteJoinedTuple1To1Test() throws InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1});
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            insertReTweet2(4431, X + i, fastJoinDeltaManager, viewMaintainer, false);
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertFollower(X + i, 2 * X + i, fastJoinDeltaManager, viewMaintainer, true);                
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteFollower(X + i, 2 * X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
        }
    }

    private void insertReTweet1(int retweetTweetId, int tweetId, FastJoinDeltaManager fastJoinDeltaManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASRETWEETID);
        joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(tweetId);
        vertexValues[1] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add("2992-01-01");//tweet date
        projectedAttributeValues.add(retweetTweetId);
        projectedAttributeValues.add(tweetId);
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
            , 1, "retweet1", cntTable.getTableDefinition(), null);
    }
    private void insertReTweet2(int retweetTweetId, int userId, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASRETWEETID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(userId);
        vertexValues[1] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(-1);
        projectedAttributeValues.add(retweetTweetId);
        projectedAttributeValues.add(userId);
        
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
            , 2, "retweet2", cntTable.getTableDefinition(), null);
            for(Object[] i : t)
                viewMaintainer.insertTuple(i);
            t = null;
        } else {
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , 2, "retweet2", cntTable.getTableDefinition(), null);
        }
    }
    private void deleteReTweet2(int retweetTweetId, int userId, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASRETWEETID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(userId);
        vertexValues[1] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(-1);
        projectedAttributeValues.add(retweetTweetId);
        projectedAttributeValues.add(userId);
        
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 2);
            for(Object[] i : t)
                viewMaintainer.insertTuple(i);
            t = null;
        } else {
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2);
        }
    }
//    private void insertTweet(int tweet, FastJoinDeltaManager fastJoinDeltaManager) {
//        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
//        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
//        vertexValue.setAlias(ALIAS.ALIASTWEETID);
//        List<Object> joinAttributeValues = new ArrayList<>();
//        vertexValue.setAttributeValues(joinAttributeValues);
//        joinAttributeValues.add(tweet);//tweet id
//        vertexValues[0] = vertexValue;
//        
//        List<Object> projectedAttributeValues = new ArrayList<>();
//        projectedAttributeValues.add(1);//user-id present
//        projectedAttributeValues.add(tweet);
//        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
//            , 0, "tweet", cntTable.getTableDefinition(), null);
//    }
    private void insertFollower(int user, int foll, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        //projectedAttributeValues.add(foll);
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 3, "users"
            , cntTable.getTableDefinition(), null);
//            for(Object[] i : t)
//                viewMaintainer.insertTuple(i);
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 3, "users"
                , cntTable.getTableDefinition(), null);
    }
    private void deleteFollower(int user, int foll, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        //projectedAttributeValues.add(foll);
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 3);
            for(Object[] i : t)
                viewMaintainer.insertTuple(i);
            t = null;
        } else
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 3);
    }

    public static void destruct() {
        cntTable = null;
    }
}
