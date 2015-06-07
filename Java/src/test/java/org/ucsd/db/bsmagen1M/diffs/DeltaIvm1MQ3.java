package org.ucsd.db.bsmagen1M.diffs;

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
import org.ucsd.db.fastview.index.IntArrayList;
import org.ucsd.db.fastview.index.IntHashMap;
import org.ucsd.db.view.ViewMaintainer;

public class DeltaIvm1MQ3 {
    private enum ALIAS{
        ALIASUSERID, ALIASTWEETID;
    }
    private static Database database;
    @BeforeClass
    public static void setUp() throws SQLException, InstantiationException, IllegalAccessException, ClassNotFoundException {
        database = new Database();

        ViewDefinition viewDefinition = new ViewDefinition();
        database.setViewDefinition(viewDefinition);
        IntHashMap<String> projectedColumnOffsetMap = new IntHashMap<String>();
        projectedColumnOffsetMap.put("users", 0);
        projectedColumnOffsetMap.put("tweet", 1);
        projectedColumnOffsetMap.put("retweet", 4);
        viewDefinition.setProjectedColumnOffsetMap(projectedColumnOffsetMap);
        
        DeltaIvmJoinGraph graph = new DeltaIvmJoinGraph();
        database.setJoinGraph(graph);
        graph.add("ALIASUSERID", "users");
        graph.add("ALIASUSERID", "tweet");
        graph.add("ALIASTWEETID", "tweet");
        graph.add("ALIASTWEETID", "retweet");
        
        initDriver();
        try (Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");){
            Statement stat = connection.createStatement();
            
            createUserTable(database, stat);
            createTweetTable(database, stat);
            createRetweetTable(database, stat);
        }
    }
    private static void createRetweetTable(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("retweet");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASTWEETID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
                    , stat.executeQuery("SELECT retweet_tweet_id, tweet_id as r_tweet_id, tweet_date as r_tweet_date from retweet;")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        projectedAttributeIndices.add(1);
        projectedAttributeIndices.add(2);
        table.setProjectedAttributeIndices(projectedAttributeIndices);        
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
        
        indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
                , stat.executeQuery("SELECT user_id as t_user_id, tweet_id, tweet_date FROM tweet;")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        projectedAttributeIndices.add(1);
        projectedAttributeIndices.add(2);
        table.setProjectedAttributeIndices(projectedAttributeIndices);        
    }
    private static void createUserTable(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("users");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database, stat.executeQuery("SELECT user_id as u_user_id from users")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(0);
        table.setProjectedAttributeIndices(projectedAttributeIndices);        
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void insertProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        //ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        int MAX = 50000;
        int x = 1000000000;
        for(int j = 0; j < 10; j++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createReTweet(2, x + i);
                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "retweet", tuple);
//                for(Object[] k : t1)
//                    viewMaintainer.insertTuple(k);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createReTweet(2, x + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "retweet", tuple);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    @Test
    public void deleteProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        //ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        int MAX = 50000;
        int x = 1000000000;
        for(int j = 0; j < 10; j++) {
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createReTweet(2, x + i);
//                List<Object[]> t1 = 
                  deltaIvmManager.insertTupleWithoutReturn(database, "retweet", tuple);
//                for(Object[] k : t1)
//                    viewMaintainer.insertTuple(k);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createReTweet(2, x + i);
                List<Object[]> t1 = deltaIvmManager.deleteTuple(database, "retweet", tuple);
//                for(Object[] k : t1)
//                    viewMaintainer.deleteTuple(k);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    private Object[] createReTweet(int retweetTweetId, int tweetId) {
        Object[] tuple = new Object[3];
        tuple[0] = retweetTweetId;
        tuple[1] = tweetId;
        tuple[2] = "2992-01-01";
        return tuple;
    }
    private Object[] createTweet(int user, int tweet) throws ParseException {
        Object[] tuple = new Object[3];
        tuple[0] = user;
        tuple[1] = tweet;
        tuple[2] = "2992-01-01";
        return tuple;
    }
    @Test
    public void insertJoinTuples1To1Test() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int MAX = 50000;
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            Object[] ins = createUser(X + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "users", ins);
            ins = createTweet(X + i, X + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "tweet", ins);
        }
        //ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        for(int k = 0; k < 10; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createReTweet(X + i, X + i);
                List<Object[]> t1 = deltaIvmManager.insertTuple(database, "retweet", tuple);
//                for(Object[] l : t1)
//                    viewMaintainer.insertTuple(l);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                Object[] ins = createTweet(X + i, X + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "retweet", ins);
            }
        }
    }
    @Test
    public void deleteJoinTuples1To1Test() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int MAX = 50000;
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            Object[] ins = createUser(X + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "users", ins);
            ins = createTweet(X + i, X + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "tweet", ins);
        }
        //ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4,5,6});
        for(int k = 0; k < 10; k++) {
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createReTweet(X + i, X + i);
                //List<Object[]> t1 = 
                                deltaIvmManager.insertTupleWithoutReturn(database, "retweet", tuple);
//                for(Object[] l : t1)
//                    viewMaintainer.insertTuple(l);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] ins = createTweet(X + i, X + i);
                List<Object[]> t1 = deltaIvmManager.deleteTuple(database, "retweet", ins);
//                for(Object[] l : t1)
//                    viewMaintainer.deleteTuple(l);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
        }
    }
    private Object[] createUser(int user) {
        Object[] tuple = new Object[1];
        tuple[0] = user;
        return tuple;
    }
    public static void destruct() {
        database = null;        
    }
}
