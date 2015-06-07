package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.deltaivm.datamodel.Database;
import org.ucsd.db.deltaivm.datamodel.DeltaIvmJoinGraph;
import org.ucsd.db.deltaivm.datamodel.DeltaIvmRelationDefinition;
import org.ucsd.db.deltaivm.datamodel.IndexedVertex;
import org.ucsd.db.deltaivm.datamodel.RelationalTable;
import org.ucsd.db.deltaivm.datamodel.ViewDefinition;
import org.ucsd.db.deltaivm.delta.DeltaIvmManager;
import org.ucsd.db.deltaivm.tableloader.DeltaIvmSqlTableLoader;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.index.IntArrayList;
import org.ucsd.db.fastview.index.IntHashMap;
import org.ucsd.db.view.ViewMaintainer;

public class DeltaIvm1MQ4 {
    private enum ALIAS{
        ALIASUSERID, ALIASTWEETID, ALIASRETWEETID;
    }
    private static Database database;
    @BeforeClass
    public static void setUp() throws SQLException, InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(0);
        database = new Database();

        ViewDefinition viewDefinition = new ViewDefinition();
        database.setViewDefinition(viewDefinition);
        IntHashMap<String> projectedColumnOffsetMap = new IntHashMap<String>();
        projectedColumnOffsetMap.put("tweet", 0);
        projectedColumnOffsetMap.put("retweet1", 2);
        projectedColumnOffsetMap.put("retweet2", 5);
        projectedColumnOffsetMap.put("follower", 8);
        viewDefinition.setProjectedColumnOffsetMap(projectedColumnOffsetMap);
        
        DeltaIvmJoinGraph graph = new DeltaIvmJoinGraph();
        database.setJoinGraph(graph);
        graph.add("ALIASTWEETID", "tweet");
        graph.add("ALIASTWEETID", "retweet1");
        graph.add("ALIASRETWEETID", "retweet1");
        graph.add("ALIASRETWEETID", "retweet2");
        graph.add("ALIASUSERID", "retweet2");
        graph.add("ALIASUSERID", "follower");
        
        initDriver();
        try (Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");){
            Statement stat = connection.createStatement();
            
            createTweetTable(database, stat);
            createRetweetTable1(database, stat);
            createRetweetTable2(database, stat);
            createFollowerTable(database, stat);
        }
    }
    private static void createTweetTable(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("tweet");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASTWEETID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(1);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
                    , stat.executeQuery("SELECT user_id as t_user_id, tweet_id FROM tweet;")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        projectedAttributeIndices.add(1);
        table.setProjectedAttributeIndices(projectedAttributeIndices);        
    }
    private static void createRetweetTable1(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("retweet1");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASTWEETID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        
        indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASRETWEETID);
        joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(1);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
                    , stat.executeQuery("SELECT  retweet_tweet_id as r1_retweet_tweet_id, tweet_id as r1_tweet_id"
                                    + ", tweet_date as r1_tweet_date from retweet")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        projectedAttributeIndices.add(1);
        projectedAttributeIndices.add(2);
        table.setProjectedAttributeIndices(projectedAttributeIndices);
    }
    private static void createRetweetTable2(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("retweet2");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASRETWEETID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        
        indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(1);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
                    , stat.executeQuery("SELECT retweet_tweet_id as r2_retweet_tweet_id, user_id as r2_user_id"
                                    + ", tweet_id as r2_tweet_id from retweet")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        projectedAttributeIndices.add(1);
        projectedAttributeIndices.add(2);
        table.setProjectedAttributeIndices(projectedAttributeIndices);        
    }
    private static void createFollowerTable(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("follower");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
            , stat.executeQuery("SELECT user_id as u_user_id, follower_id from follower where user_id = 384827")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        projectedAttributeIndices.add(1);
        table.setProjectedAttributeIndices(projectedAttributeIndices);        
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void insertProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int MAX = 5000;
        int x = 1000000000;
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
        for(int j = 0; j < 10; j++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createFollower(384827, x + i);
                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "follower", tuple);
                for(Object[] k : t1)
                    viewMaintainer.insertTuple(k);
                t1 = null;
            }
            long t = (System.nanoTime() - tm) / 1000000;
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createFollower(384827, x + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "follower", tuple);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    @Test
    public void deleteProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int MAX = 5000;
        int x = 1000000000;
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
        for(int j = 0; j < 10; j++) {
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createFollower(384827, x + i);
                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "follower", tuple);
                for(Object[] k : t1)
                    viewMaintainer.insertTuple(k);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createFollower(384827, x + i);
                List<Object[]> t1 = deltaIvmManager.deleteTuple(database, "follower", tuple);
                for(Object[] k : t1)
                    viewMaintainer.deleteTuple(k);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
//    private Object[] createTweet(int user, int tweet) throws ParseException {
//        Object[] tuple = new Object[3];
//        tuple[0] = user;
//        tuple[1] = tweet;
//        tuple[2] = "2992-01-01";
//        return tuple;
//    }
//    @Test
//    public void insertJoinTuples1To1Test() throws ParseException, InstantiationException, IllegalAccessException
//        , ClassNotFoundException, SQLException {
//        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
//        int MAX = 50000;
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            Object[] ins = createTweet(1, X + i);
//            List<Object[]> t1 = deltaIvmManager.insertTuple(database, "tweet", ins);
//            ins = createReTweet1(X + i, 2 * X + i);
//            t1 = deltaIvmManager.insertTuple(database, "retweet1", ins);
//        }
//        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
//        for(int k = 0; k < 15; k++) {
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                Object[] ins = createReTweet2(2 * X + i, X + i);
//                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "retweet2", ins);
//                ins = createFollower(X + i, 2 * X + i);
//                t1 = deltaIvmManager.insertTuple(database, "follower", ins);
//                for(Object[] l : t1)
//                    viewMaintainer.insertTuple(l);
//            }
//            long t = (System.nanoTime() - tm) / 1000000;
//            for(int i = 0; i < MAX; i++) {
//                Object[] ins = createReTweet2(2 * X + i, X + i);
//                deltaIvmManager.deleteTupleWithoutReturn(database, "retweet2", ins);
//                ins = createFollower(X + i, 2 * X + i);
//                deltaIvmManager.deleteTupleWithoutReturn(database, "follower", ins);
//            }
//            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
//        }
//    }
//    @Test
//    public void deleteJoinTuples1To1Test() throws ParseException, InstantiationException, IllegalAccessException
//        , ClassNotFoundException, SQLException {
//        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
//        int MAX = 5000;
//        int X = 1000000000;
//        for(int i = 0; i < MAX; i++) {
//            Object[] ins = createTweet(1, X + i);
//            List<Object[]> t1 = deltaIvmManager.insertTuple(database, "tweet", ins);
//            ins = createReTweet1(X + i, 2 * X + i);
//            t1 = deltaIvmManager.insertTuple(database, "retweet1", ins);
//        }
//        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
//        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
//            for(int i = 0; i < MAX; i++) {
//                Object[] ins = createReTweet2(2 * X + i, X + i);
//                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "retweet2", ins);
//                ins = createFollower(X + i, 2 * X + i);
//                t1 = deltaIvmManager.insertTuple(database, "follower", ins);
//                for(Object[] l : t1)
//                    viewMaintainer.insertTuple(l);
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                Object[] ins = createReTweet2(2 * X + i, X + i);
//                List<Object[]> t1 = deltaIvmManager.deleteTuple(database, "retweet2", ins);
//                ins = createFollower(X + i, 2 * X + i);
//                t1 = deltaIvmManager.deleteTuple(database, "follower", ins);
//                for(Object[] l : t1)
//                    viewMaintainer.deleteTuple(l);
//            }
//            long t = (System.nanoTime() - tm) / 1000000;
//        }
//    }
    @Test
    public void insertJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            Object[] ins = createReTweet2(1175996, X + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "retweet2", ins);
        }
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] ins = createFollower(X + i, 2 * X + i);
                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "follower", ins);
                for(Object[] l : t1)
                    viewMaintainer.insertTuple(l);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                Object[] ins = createFollower(X + i, 2 * X + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "follower", ins);
            }
        }
    }
    @Test
    public void deleteJoinedTuple1To1Test() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        int MAX = 5000;
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            Object[] ins = createReTweet2(1175996, X + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "retweet2", ins);
        }
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6,7,8});
        for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] ins = createFollower(X + i, 2 * X + i);
                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "follower", ins);
                for(Object[] l : t1)
                    viewMaintainer.insertTuple(l);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                Object[] ins = createFollower(X + i, 2 * X + i);
                List<Object[]> t1 = deltaIvmManager.deleteTuple(database, "follower", ins);
                for(Object[] l : t1)
                    viewMaintainer.deleteTuple(l);
            }
        }
    }
    private Object[] createReTweet2(int retweetTweetId, int userId) {
        Object[] tuple = new Object[3];
        tuple[0] = retweetTweetId;
        tuple[1] = userId;
        tuple[2] = 1;
        return tuple;
    }
//    private Object[] createReTweet1(int retweetTweetId, int tweetId) {
//        Object[] tuple = new Object[3];
//        tuple[0] = tweetId;
//        tuple[1] = retweetTweetId;
//        tuple[2] = "2992-01-01";
//        return tuple;
//    }
    private Object[] createFollower(int user, int follower) {
        Object[] tuple = new Object[2];
        tuple[0] = user;
        tuple[1] = follower;
        return tuple;
    }
    public static void destruct() {
        database = null;        
    }
}
