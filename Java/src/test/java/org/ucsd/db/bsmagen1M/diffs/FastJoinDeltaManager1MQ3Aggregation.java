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
import org.ucsd.db.fastview.tableloader.AggregationSqlTableLoader;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.view.ViewMaintainer;

public class FastJoinDeltaManager1MQ3Aggregation {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID,
    }
    private static CntTable cntTable;
    private static final List<String> PROJECTED_ATTRIBUTES_LIST_RETWEET = Arrays.asList("r_tweet_id");
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        MemoryManager.init(3);
        AggregationSqlTableLoader aggregationSqlTableLoader = new AggregationSqlTableLoader();
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
        //tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("t_tweet_date", "t_user_id", "t_tweet_id"));
        
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
        retweetDefinition.setProjectedAttributeNamesList(PROJECTED_ATTRIBUTES_LIST_RETWEET);
        
        tableDefinition.getJoinDefinitionMap().put(retweetDefinition.getRelation(), retweetDefinition);
        offsetAttributes[3] = offsetAttributes[2] + retweetDefinition.getProjectedAttributeNamesList().size();
        
        HashMap<String, AggregatedAttribute> aggregatedAttributes = new HashMap<>();
        tableDefinition.setAggregatedAttributes(aggregatedAttributes);
        aggregatedAttributes.put("r_tweet_id", new AggregatedAttribute(Function.COUNT,0 , 1));
        
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");) {
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                    tableDefinition, cntTableAndIndices, 0);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id, tweet_id "
                    + "as t_tweet_id from tweet"), tweetDefinition, 
                    tableDefinition, cntTableAndIndices, 1);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select tweet_id as r_tweet_id"
                    + ", retweet_tweet_id from retweet"), retweetDefinition, 
                    tableDefinition, cntTableAndIndices, 2);
            cntTable = cntTableAndIndices.cntTable;
            aggregationSqlTableLoader.linkJoinTuples(cntTableAndIndices);
        }
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void insertProjectedAggregatedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        int X = 1000000000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
        int[] aggregateAttributes = new int[]{0};
        for(int k = 0; k < 20; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertReTweet(2, X + i, fastAggregationManager, viewMaintainer, aggregateAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(2, X + i, fastAggregationManager, viewMaintainer, aggregateAttributes, false);
            }
        }
    }
    @Test
    public void deleteProjectedAggregatedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        int X = 1000000000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
        int[] aggregateAttributes = new int[]{0};
        for(int k = 0; k < 20; k++) {
            System.gc();
            for(int i = 0; i < MAX; i++) {
                insertReTweet(2, X + i, fastAggregationManager, viewMaintainer, aggregateAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(2, X + i, fastAggregationManager, viewMaintainer, aggregateAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }
    @Test
    public void insertProjectedGroupedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastAggregationManager fastJoinDeltaManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
        int[] aggregateAttributes = new int[]{0};
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(2, 729, fastJoinDeltaManager, viewMaintainer, aggregateAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTweet(2, 729, fastJoinDeltaManager, viewMaintainer, aggregateAttributes, false);
            }
        }
    }
    @Test
    public void deleteProjectedGroupedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastAggregationManager fastJoinDeltaManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
        int[] aggregatedAttributes = new int[]{0};
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(2, 729, fastJoinDeltaManager, viewMaintainer, aggregatedAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(2, 729, fastJoinDeltaManager, viewMaintainer, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }
//    @Test
//    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 5000;
//        int X = 1000000000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        for(int k = 0, x = X; k < 15; k++, x += MAX) {
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet(2, x + i);
//                List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            }
//            for(int i = 0; i < MAX; i++) {
//                JoinTuple ins = createReTweet(2, x + i);
//                List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//            }
//        }
//        setUp();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet(2, X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            JoinTuple ins = createReTweet(2, X + i);
//            List<Object[]> t1 = fastJoinDeltaManager.deleteTuple(cntTable, ins);
//        }
//        System.out.println("deleteProjectedTupleTest:" + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
//    }

    @Test
    public void insertJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
        int[] aggregatedAttributes = new int[]{0};
        int X = 1000000000;
        insertUser(X, fastAggregationManager);
        for (int i = 0; i < MAX; i++) {
            insertTweet(X, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);            
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertReTweet(X + i, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(X + i, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);
            }
        }
    }
    @Test
    public void deleteJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 50000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
        int[] aggregatedAttributes = new int[]{0};
        int X = 2000000000;
        insertUser(X, fastAggregationManager);
        for (int i = 0; i < MAX; i++) {
            insertTweet(X, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);            
        }
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertReTweet(X + i, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteReTweet(X + i, X + i, fastAggregationManager, viewMaintainer, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }
//    @Test
//    public void insertJoinedTuple1ToMTest() throws ParseException, InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        int MAX = 50000;
//        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
//        int X = 1000000000;
//        for(int k = 0, x = X; k < 15; k++, x += MAX) {
//            JoinTuple ins = createUser(x);
//            List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                ins = createTweet(x, x + i);
//                List<Object[]> t2 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//            }
//        }
//        JoinTuple ins = createUser(X);
//        List<Object[]> t1 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        long tm = System.nanoTime();
//        for(int i = 0; i < MAX; i++) {
//            ins = createTweet(X, X + i);
//            List<Object[]> t2 = fastJoinDeltaManager.insertTuple(cntTable, ins);
//        }
//        System.out.println("insertJoinedTuple1ToMTest:" + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
//    }

    private void insertTweet(int user, int tweet, FastAggregationManager aggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregateAttributes, boolean doReturn) {
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
        if(doReturn) {
            List<Object[]> t = aggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), new ArrayList<String>());
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregateAttributes, 1);
            t = null;
        } else
            aggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.GROUPED_TUPLE, 1
                , "tweet", cntTable.getTableDefinition(), null);
    }
    private void deleteTweet(int user, int tweet, FastAggregationManager aggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregateAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add("1992-01-01");//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = aggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_RETWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregateAttributes, -1);
            t = null;
        } else
            aggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.GROUPED_TUPLE, 1);
    }
    private void insertReTweet(int retweetTweetId, int tweetId, FastAggregationManager fastAggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregateAttributes, boolean doReturn) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);//tweet_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>(1);
        projectedAttributeValues.add(tweetId);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 2, "retweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_RETWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregateAttributes, 1);
            t = null;
        } else {
            fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.AGGREGATED_TUPLE
                , 2, "retweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_RETWEET);
        }
    }
    private void deleteReTweet(int retweetTweetId, int tweetId, FastAggregationManager fastAggregationManager, ViewMaintainer viewMaintainer
        , int[] aggregateAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);//tweet_id - present
        vertexValues[0] = vertexValue;
        
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(tweetId);
        
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 2
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_RETWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregateAttributes, -1);
            t = null;
        } else {
            fastAggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.AGGREGATED_TUPLE, 2);
        }
    }
    private void insertUser(int user, FastAggregationManager fastAggregationManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.JOIN_TUPLE, 0, "users"
            , cntTable.getTableDefinition(), null);
    }
}
