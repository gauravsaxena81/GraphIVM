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

public class DeltaIvm1MQ1Aggregation {
    private enum ALIAS{
        ALIASUSERID;
    }
    private static Database database;
    private static Database viewDatabase;
    final static int MIXED_WORKLOAD_CONSTANT = 407869;//1k:407869:1m:12404312;
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static final String PASSWORD = "gaurav";
    private static final String USERNAME = "gaurav";
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
    private static void createView() {
        viewDatabase = new Database();
        RelationalTable relation = new RelationalTable();
        List<IndexedVertex<? extends Enum<?>>> indexedVertices = new ArrayList<IndexedVertex<? extends Enum<?>>>();
        IndexedVertex<ALIAS> indexedVertex = new IndexedVertex<>();
        indexedVertex.setAlias(ALIAS.ALIASUSERID);
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.addAll(0, new int[]{0});
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
                + ", count(tweet_id) FROM tweet where tweet_id < " + MIXED_WORKLOAD_CONSTANT + "group by user_id;")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(1);
        //projectedAttributeIndices.add(0);
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
    private Object[] createTweet(int user, int tweet) {
        Object[] tuple = new Object[2];
        tuple[0] = user;
        tuple[1] = 1;
        return tuple;
    }
    private Object[] createUser(int user) {
        Object[] tuple = new Object[1];
        tuple[0] = user;
        return tuple;
    }
    @Test
    public void insertMixedWorkLoadWithViewMaintenance() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select * from (select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX + ") A order by 2 asc;"); 
            DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
            int[] aggregatedAttributes = new int[]{0};
            ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0});
            for(int i = 0; i < 10; i++) {
                int t0 = 0, t1 = 0;
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        t0 = rs.getInt(1); 
                        t1 = rs.getInt(2);
                    }
                }
                long d = (System.nanoTime() - tm)  / 1000000;
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2));
                    List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                    for(Object[] tt : tuples)
                        viewMaintainer.applyTuple(tt, aggregatedAttributes, 1);
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2));
                    List<Object[]> tuples = deltaIvmManager.deleteTuple(database, "tweet", tuple);
                    for(Object[] tt : tuples)
                        viewMaintainer.applyTuple(tt, aggregatedAttributes, -1);
                }
                System.out.println(t0 + "," + t1);
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
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        t0 = rs.getInt(1); 
                        t1 = rs.getInt(2);
                    }
                }
                long d = (System.nanoTime() - tm)  / 1000000;
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2));
                    List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                    tuples = null;
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    Object[] tuple = createTweet(rs.getInt(1), rs.getInt(2));
                    deltaIvmManager.deleteTupleWithoutReturn(database, "tweet", tuple);
                }
                System.out.println(t0 + "," + t1);
            }
        }
    }
    public static void destruct() {
        database = null;        
    }
}
