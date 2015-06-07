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
import org.ucsd.db.fastview.index.IntArrayList;
import org.ucsd.db.fastview.index.IntHashMap;
import org.ucsd.db.view.ViewMaintainer;

public class DeltaIvm1MQ2Aggregation {
    private enum ALIAS{
        ALIASUSERID;
    }
    private static Database database;
    @BeforeClass
    public static void setUp() throws SQLException, InstantiationException, IllegalAccessException, ClassNotFoundException {
        database = new Database();

        ViewDefinition viewDefinition = new ViewDefinition();
        database.setViewDefinition(viewDefinition);
        IntHashMap<String> projectedColumnOffsetMap = new IntHashMap<String>();
        projectedColumnOffsetMap.put("follower", 0);
        projectedColumnOffsetMap.put("tweet", 1);
        viewDefinition.setProjectedColumnOffsetMap(projectedColumnOffsetMap);
        
        DeltaIvmJoinGraph graph = new DeltaIvmJoinGraph();
        database.setJoinGraph(graph);
        graph.add("ALIASUSERID", "follower");
        graph.add("ALIASUSERID", "tweet");
        
        initDriver();
        try (Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");){
            Statement stat = connection.createStatement();
            
            createFollowerTable(database, stat);
            createTweetTable(database, stat);
        }
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
        RelationalTable table = new DeltaIvmSqlTableLoader().loadTable(database
                    , stat.executeQuery("SELECT user_id as t_user_id, count(tweet_id) FROM tweet group by user_id")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
        projectedAttributeIndices.add(1);
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
            , stat.executeQuery("SELECT user_id as u_user_id, follower_id from follower")
            , joinTupleDefinition);
        IntArrayList projectedAttributeIndices = new IntArrayList();
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
        int[] aggregatedAttributes = new int[]{0};
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0});
        int MAX = 50000;
        int x = 1000000000;
        for(int j = 0; j < 10; j++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(100, x + i);
                List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                for(Object[] k : tuples)
                    viewMaintainer.applyTuple(k, aggregatedAttributes, 1);
                tuple = null;
            }
            long t = (System.nanoTime() - tm) / 1000000;
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(100, x + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "tweet", tuple);
            }
            System.out.println(TestUtil.getTestName() + (t) + "/" + MAX);
        }
    }
    @Test
    public void deleteProjectedTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0});
        int[] aggregatedAttributes = new int[]{0};
        int MAX = 50000;
        int x = 1000000000;
        for(int j = 0; j < 10; j++) {
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(100, x + i);
                List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                for(Object[] k : tuples)
                    viewMaintainer.applyTuple(k, aggregatedAttributes, 1);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(100, x + i);
                List<Object[]> tuples = deltaIvmManager.deleteTuple(database, "tweet", tuple);
                for(Object[] k : tuples)
                    viewMaintainer.applyTuple(k, aggregatedAttributes, -1);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + (t) + "/" + MAX);
        }
    }
    private Object[] createTweet(int user, int tweet) throws ParseException {
        Object[] tuple = new Object[2];
        tuple[0] = user;
        tuple[1] = 1;
        return tuple;
    }
    @Test
    public void insertJoinTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0});
        int[] aggregatedAttributes = new int[]{0};
        int MAX = 50000;
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < 5; j++) {
                Object[] tuple = createFollower(X + i, j);
                deltaIvmManager.insertTupleWithoutReturn(database, "follower", tuple);
            }
        }
        for(int k = 0; k < 10; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(X + i, X + i);
                List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                for(Object[] l : tuples)
                    viewMaintainer.applyTuple(l, aggregatedAttributes, 1);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(X + i, X + i);
                deltaIvmManager.deleteTupleWithoutReturn(database, "tweet", tuple);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    @Test
    public void deleteJoinTuplesTest() throws ParseException, InstantiationException, IllegalAccessException
        , ClassNotFoundException, SQLException {
        DeltaIvmManager deltaIvmManager = new DeltaIvmManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0});
        int[] aggregatedAttributes = new int[]{0};
        int MAX = 50000;
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < 5; j++) {
                Object[] tuple = createFollower(X + i, j);
                deltaIvmManager.insertTupleWithoutReturn(database, "follower", tuple);
            }
        }
        for(int k = 0; k < 10; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(X + i, X + i);
                List<Object[]> tuples = deltaIvmManager.insertTuple(database, "tweet", tuple);
                for(Object[] l : tuples)
                    viewMaintainer.applyTuple(l, aggregatedAttributes, 1);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            for(int i = 0; i < MAX; i++) {
                Object[] tuple = createTweet(X + i, X + i);
                List<Object[]> tuples = deltaIvmManager.deleteTuple(database, "tweet", tuple);
                for(Object[] l : tuples)
                    viewMaintainer.applyTuple(l, aggregatedAttributes, -1);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
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
