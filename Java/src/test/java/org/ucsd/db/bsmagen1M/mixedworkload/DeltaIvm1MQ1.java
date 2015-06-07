package org.ucsd.db.bsmagen1M.mixedworkload;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.HashMap;
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
import org.ucsd.db.fastview.index.HashIndex;
import org.ucsd.db.fastview.index.IIndex;
import org.ucsd.db.fastview.index.IntArrayList;
import org.ucsd.db.fastview.index.IntHashMap;
import org.ucsd.db.view.ViewMaintainer;

public class DeltaIvm1MQ1 {
    private static Database database;
    private static Database viewDatabase;
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//1k:407869:1m:12404312;
    final static String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static final String PASSWORD = "gaurav";
    private static final String USERNAME = "gaurav";
    private enum ALIAS{
        ALIASUSERID;
    }
    @BeforeClass
    public static void setUp() throws SQLException, InstantiationException, IllegalAccessException, ClassNotFoundException {
        database = new Database();

        ViewDefinition viewDefinition = new ViewDefinition();
        database.setViewDefinition(viewDefinition);
        IntHashMap<String> projectedColumnOffsetMap = new IntHashMap<String>();
        projectedColumnOffsetMap.put("users", 0);
        projectedColumnOffsetMap.put("tweet", 1);
        viewDefinition.setProjectedColumnOffsetMap(projectedColumnOffsetMap);
        
        DeltaIvmJoinGraph graph = new DeltaIvmJoinGraph();
        database.setJoinGraph(graph);
        graph.add("ALIASUSERID", "users");
        graph.add("ALIASUSERID", "tweet");
        
        initDriver();
        try (Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD);){
            Statement stat = connection.createStatement();
            
            createUserTable(database, stat);
            createTweetTable(database, stat);
        }
        createView();
    }
    private static void createTweetTable(Database database, Statement stat) throws SQLException {
        DeltaIvmRelationDefinition joinTupleDefinition = new DeltaIvmRelationDefinition();
        joinTupleDefinition.setRelation("tweet");
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<>();
        joinTupleDefinition.setIndexedVertices(indexedVertices);
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<ALIAS>();
        indexedVertices.add(indexedVertex);
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.add(0);
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database, stat.executeQuery("SELECT user_id as t_user_id"
                + ", tweet_id, tweet_date FROM tweet where tweet_id < " + MIXED_WORKLOAD_CONSTANT)
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
    private static void createView() {
        viewDatabase = new Database();
        RelationalTable relation = new RelationalTable();
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<IndexedVertex<? extends Enum<?>>>();
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<>();
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.addAll(0, new int[]{0,1,2,3});
        indexedVertex.setJoinAttributeIndices(joinAttributeIndices);
        indexedVertices.add(indexedVertex);
        relation.setIndexedVertices(indexedVertices);
        relation.setProjectedAttributeIndices(joinAttributeIndices);
        viewDatabase.add("View", relation);
        
        DeltaIvmJoinGraph joinGraph = new DeltaIvmJoinGraph();
        joinGraph.add("View", "View");
        viewDatabase.setJoinGraph(joinGraph);
        
        ViewDefinition viewDefinition = new ViewDefinition();
        IntHashMap<String> projectedColumnOffsetMap = new IntHashMap<String>();
        projectedColumnOffsetMap.put("View", 0);
        viewDefinition.setProjectedColumnOffsetMap(projectedColumnOffsetMap);
        viewDatabase.setViewDefinition(viewDefinition);
        
        HashMap<Enum<? extends Enum<?>>, IIndex> aliasToIndexMap = viewDatabase.getIndexes();
        aliasToIndexMap.put(ALIAS.ALIASUSERID, new HashIndex());
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    
    private Object[] createTweet(int user, int tweet, String date) {
        Object[] tuple = new Object[3];
        tuple[0] = user;
        tuple[1] = tweet;
        tuple[2] = date;
        return tuple;
    }
    
    private Object[] createUser(int user) {
        Object[] tuple = new Object[1];
        tuple[0] = user;
        return tuple;
    }
    public static void destruct() {
        database = null;        
    }
    @Test
    public void insertMixedWorkLoadWithViewMaintenance() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select * from (select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX + ") A order by 2 asc;");
            DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
            ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3});
            for(int i = 0; i < 10; i++) {
                int t0 = 0, t1 = 0;
                String s = null;
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        t0 = rs.getInt(1); 
                        t1 = rs.getInt(2);
                        s = rs.getString(3);
                    }
                }
                long d = (System.nanoTime() - tm)  / 1000000;
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2), rs.getString(3));
                        List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                        for(Object[] t : tuples)
                            viewMaintainer.insertTuple(t);
                    }
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2), rs.getString(3));
                        List<Object[]> tuples = deltaIvmManager.deleteTuple(database, "tweet", tuple);
                        for(Object[] tt : tuples)
                            viewMaintainer.deleteTuple(tt);
                    }
                }
                System.out.println(t0 + "," + t1 + s);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadWithoutViewMaintenance() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select * from (select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX + ") A order by 2 asc;");
            DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
            for(int i = 0; i < 10; i++) {
                int t0 = 0, t1 = 0;
                String s = null;
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        t0 = rs.getInt(1); 
                        t1 = rs.getInt(2);
                        s = rs.getString(3);
                    }
                }
                long d = (System.nanoTime() - tm)  / 1000000;
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2), rs.getString(3));
                        List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                        tuples = null;
                    }
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2), rs.getString(3));
                        deltaIvmManager.deleteTupleWithoutReturn(database, "tweet", tuple);
                    }
                }
                System.out.println(t0 + "," + t1 + s);
            }
        }
    }
}