package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
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
import org.ucsd.db.fastview.datamodel.JoinTuple;
import org.ucsd.db.fastview.datamodel.JoinTuple.TYPE;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.Vertex;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastAggregationManager;
import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.tableloader.AggregationSqlTableLoader;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.view.ViewMaintainer;

public class FastJoinDeltaManager1MQ4Aggregation {
    private enum ALIAS {
        ALIASTWEETID, ALIASRETWEETID, ALIASUSERID,
    }
    private static final List<String> PROJECTED_ATTRIBUTES_LIST_FOLLOWER = Arrays.asList("follower_id");
    private static CntTable cntTable;
   @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(4);
        AggregationSqlTableLoader aggregationSqlTableLoader = new AggregationSqlTableLoader();
        
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
        //retweet1Definiton.setProjectedAttributeNamesList(Arrays.asList("r1_tweet_date", "r1_retweet_tweet_id", "r1_tweet_id"));
        
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
        //retweet2Definiton.setProjectedAttributeNamesList(Arrays.asList("r2_tweet_id", "r2_retweet_tweet_id", "r2_user_id"));
        
        tableDefinition.getJoinDefinitionMap().put(retweet2Definiton.getRelation(), retweet2Definiton);
        offsetAttributes[3] = offsetAttributes[2] + retweet2Definiton.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> followerDefiniton = new JoinTupleDefinition<ALIAS>();
        followerDefiniton.setRelation("follower");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        vertex = new Vertex<ALIAS>();
        followerDefiniton.setVertices(vertices);
        //followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("follower_id", "f_user_id"));
        followerDefiniton.setProjectedAttributeNamesList(PROJECTED_ATTRIBUTES_LIST_FOLLOWER);
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
        offsetAttributes[4] = offsetAttributes[3] + followerDefiniton.getProjectedAttributeNamesList().size();
        
        HashMap<String, AggregatedAttribute> aggregatedAttributes = new HashMap<>();
        tableDefinition.setAggregatedAttributes(aggregatedAttributes);
        aggregatedAttributes.put("follower_id", new AggregatedAttribute(Function.COUNT, 0, 2));
        
        tableDefinition.setGroupByAttributes(Arrays.asList(0,1));
        
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav")){
            connection.setAutoCommit(false);
            
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select A.user_id as t_user_id, A.tweet_id from tweet A-- where user_id = 2 and tweet_id = 729")
                , tweetDefiniton, tableDefinition, cntTableAndIndices, 0);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id from retweet B-- where retweet_tweet_id = 4374")
                , retweet1Definiton, tableDefinition, cntTableAndIndices, 1);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select C.retweet_tweet_id as r2_retweet_tweet_id"
                + ", C.user_id as r2_user_id from retweet C-- where retweet_tweet_id in(29884, 68319)"), retweet2Definiton
                , tableDefinition, cntTableAndIndices, 2);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select D.user_id as f_user_id"
                + ", D.follower_id from follower D "), followerDefiniton, tableDefinition, cntTableAndIndices, 3);

            cntTable = cntTableAndIndices.cntTable;
            aggregationSqlTableLoader.linkJoinTuples(cntTableAndIndices);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void insertProjectedTupleTest() {
        int MAX = 50000;
        int X = 1000000000;
        FastAggregationManager FastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1});
        int[] aggregatedAttributes = new int[]{0};
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertFollower(384827, X + i, FastAggregationManager, viewMaintainer, aggregatedAttributes, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteFollower(384827, X + i, FastAggregationManager, viewMaintainer, aggregatedAttributes, false);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

    @Test
    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        int X = 1000000000;
        FastAggregationManager FastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1});
        int[] aggregatedAttributes = new int[0];
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertFollower(384827, X + i, FastAggregationManager, viewMaintainer, aggregatedAttributes, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteFollower(384827, X + i, FastAggregationManager, viewMaintainer, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
//    @Test
//    public void insertJoinedTuple1To1Test() {
//        int MAX = 50000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createTweet(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createReTweet1(X + i, 2 * X + i);
//            t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        for(int k = 0; k < 10; k++) {
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins1 = createReTweet2(2 * X + i, X + i);
//                List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins1);
//                JoinTuple ins2 = createFollower(X + i, 2 * X + i);
//                t1 = fastJoinDeltaManager.insertTuple(cntTable, ins2);
//            }
//            long t = (System.nanoTime() - tm)  / 1000000;
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins2 = createFollower(X + i, 2 * X + i);
//                JoinTuple ins1 = createReTweet2(2 * X + i, X + i);
//                fastJoinDeltaManager.deleteTuple(cntTable, ins2);
//                fastJoinDeltaManager.deleteTuple(cntTable, ins1);
//            }
//            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
//        }
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet2(2 * X + i, X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            ins = createFollower(X + i, 2 * X + i);
//            t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        long t = (System.nanoTime() - tm)  / 1000000;
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins2 = createFollower(X + i, 2 * X + i);
//            JoinTuple ins1 = createReTweet2(2 * X + i, X + i);
//            fastJoinDeltaManager.deleteTuple(cntTable, ins2);
//            fastJoinDeltaManager.deleteTuple(cntTable, ins1);
//        }
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createTweet(X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//            ins = createReTweet1(X + i, 2 * X + i);
//            t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//        }
//        System.out.println(TestUtil.getTestName() + t + "/" + MAX);
//    }
//    @Test
//    public void deleteJoinedTuple1To1Test() throws InstantiationException, IllegalAccessException
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
        int MAX = 5000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1});
        int[] aggregatedAttributes = new int[]{0};
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {//change fanout: 294
            insertReTweet2(1175996, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);
        }
        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertFollower(X + i, 2 * X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, true);                
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteFollower(X + i, 2 * X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);
            }
        }
    }
    @Test
    public void deleteJoinedTuple1To1Test() throws InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1});
        int[] aggregatedAttributes = new int[]{0};
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            insertReTweet2(1175996, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);
        }
        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
            for(int i = 0; i < MAX; i++) {
                insertFollower(X + i, 2 * X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);                
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteFollower(X + i, 2 * X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }

//    private JoinTuple createReTweet1(int retweetTweetId, int tweetId) throws ParseException {
//        JoinTuple tuple = new JoinTuple(TYPE.GROUPED_TUPLE, 1);
//        tuple.setTableDefinition(cntTable.getTableDefinition());
//        
//        VertexValue[] vertexValues = new VertexValue[1];
//        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
//        vertexValue.setAlias(ALIAS.ALIASTWEETID);
//        List<Object> joinAttributeValues = new ArrayList<>();
//        vertexValue.setAttributeValues(joinAttributeValues);
//        joinAttributeValues.add(retweetTweetId);
//        vertexValues[0] = vertexValue;
//        
//        vertexValue = new VertexValue<ALIAS>();
//        vertexValue.setAlias(ALIAS.ALIASRETWEETID);
//        joinAttributeValues = new ArrayList<>();
//        vertexValue.setAttributeValues(joinAttributeValues);
//        joinAttributeValues.add(tweetId);
//        vertexValues[1] = vertexValue;
//        
//        tuple.setVertexValues(vertexValues);
//        
//        List<Object> projectedAttributeValues = new ArrayList<>();
//        projectedAttributeValues.add("2992-01-01");//tweet date
//        projectedAttributeValues.add(retweetTweetId);
//        projectedAttributeValues.add(tweetId);
//        tuple.addLeafTuple(projectedAttributeValues); 
//        return tuple;
//    }
    private void insertReTweet2(int retweetTweetId, int userId, FastAggregationManager aggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregatedAttributes, boolean doReturn) {
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
            List<Object[]> t = aggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
            , TYPE.GROUPED_TUPLE, 2, "retweet2", cntTable.getTableDefinition(), null);
            for(Object[] i : t)
                viewMaintainer.applyTuple(i, aggregatedAttributes, 1);
            t = null;
        } else {
            aggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , TYPE.GROUPED_TUPLE, 2, "retweet2", cntTable.getTableDefinition(), null);
        }
    }
    private void deleteReTweet2(int retweetTweetId, int userId, FastAggregationManager aggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregatedAttributes, boolean doReturn) {
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
            List<Object[]> t = aggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 2
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_FOLLOWER);
            for(Object[] i : t)
                viewMaintainer.applyTuple(i, aggregatedAttributes, -1);
            t = null;
        } else {
            aggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , TYPE.GROUPED_TUPLE, 2);
        }
    }
    private JoinTuple createTweet(int tweet) {
        JoinTuple tuple = new JoinTuple(TYPE.JOIN_TUPLE, 0);
        tuple.setTableDefinition(cntTable.getTableDefinition());
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(tweet);//tweet id
        vertexValues[0] = vertexValue;
        
        tuple.setVertexValues(vertexValues);
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(1);//user-id present
        projectedAttributeValues.add(tweet);
        tuple.addLeafTuple(projectedAttributeValues); 
        return tuple;
    }
    private void insertFollower(int user, int foll, FastAggregationManager fastAggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(foll);
        //projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 3, "follower", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_FOLLOWER);
            for(Object[] i : t)
                viewMaintainer.applyTuple(i, aggregatedAttributes, 1);
            t = null;
        } else
            fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 3, "follower", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_FOLLOWER);
    }
    private void deleteFollower(int user, int foll, FastAggregationManager fastAggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(foll);
//        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 3
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_FOLLOWER);
            for(Object[] i : t)
                viewMaintainer.applyTuple(i, aggregatedAttributes, -1);
            t = null;
        } else
            fastAggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 3);
    }

    public static void destruct() {
        cntTable = null;
        MemoryManager.destruct();
    }
}
