package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.ParseException;
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
        try (Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");){
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
                + ", tweet_id, tweet_date FROM tweet;")
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
    @Test
    public void insertProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        int MAX = 50000;
        int x = 1000000000;
        //ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3});
        for(int j = 0; j < 15; j++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(2, x + i);
                List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
//                for(Object[] k : tuples)
//                    viewMaintainer.insertTuple(k);
                tuple = null;
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(2, x + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "tweet", tuple);
                tuple = null;
            }
        }
    }
    @Test
    public void deleteProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
//        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3});
        int MAX = 50000;
        int x = 1000000000;
        for(int j = 0; j < 15; j++) {
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(2, x + i);
                //List<Object[]> tuples = 
                                deltaIvmManager.insertTupleWithoutReturn(database, "tweet", tuple);
//                for(Object[] k : tuples)
//                    viewMaintainer.insertTuple(k);
                tuple = null;
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(2, x + i);
                List<Object[]> tuples = deltaIvmManager.deleteTuple(database, "tweet", tuple);
//                for(Object[] k : tuples)
//                    viewMaintainer.deleteTuple(k);
                tuple = null;
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    private Object[] createTweet(int user, int tweet) throws ParseException {
        Object[] tuple = new Object[3];
        tuple[0] = user;
        tuple[1] = tweet;
        tuple[2] = "2992-01-01";
        return tuple;
    }
    @Test
    public void insertJoinTuples() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
//        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3});
        int MAX = 50000;
        int x = 1000000000;
        for(int i = 0; i < MAX; i++) {
            Object[] tuple = createUser(x + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "users", tuple);
        }
        for(int j = 0; j < 15; j++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(x + i, x + i);
                List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
//                for(Object[] k : tuples)
//                    viewMaintainer.insertTuple(k);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(x + i, x + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "tweet", tuple);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    @Test
    public void deleteJoinTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
//        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3});
        int MAX = 50000;
        int x = 1000000000;
        for(int i = 0; i < MAX; i++) {
            Object[] tuple = createUser(x + i);
            deltaIvmManager.insertTupleWithoutReturn(database, "users", tuple);
        }
        for(int j = 0; j < 15; j++) {
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(x + i, x + i);
                //List<Object[]> tuples = 
                                deltaIvmManager.insertTupleWithoutReturn(database, "tweet", tuple);
//                for(Object[] k : tuples)
//                    viewMaintainer.insertTuple(k);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(x + i, x + i);
                List<Object[]> tuples = deltaIvmManager.deleteTuple(database, "tweet", tuple);
                //for(Object[] k : tuples)
                  //  viewMaintainer.insertTuple(k);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
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
