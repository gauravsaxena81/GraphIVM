package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.fastview.datamodel.CntTable;
import org.ucsd.db.fastview.datamodel.MemoryManager;
import org.ucsd.db.fastview.datamodel.TableDefinition;
import org.ucsd.db.fastview.datamodel.Vertex;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastJoinDeltaManager;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.fastview.tableloader.SqlTableLoader;

public class FastViewFanout {
    private enum ALIAS{
        ALIASUSERID1, ALIASUSERID2
    }
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen100k";
    private static CntTable cntTable;
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        MemoryManager.init(3);
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[4];
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        JoinTupleDefinition<ALIAS> followerDefiniton1 = new JoinTupleDefinition<ALIAS>();
        followerDefiniton1.setRelation("follower1");
        List<Vertex<ALIAS>> vertices = new ArrayList<>();
        Vertex<ALIAS> vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f1_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID1);
        vertices.add(vertex);
        followerDefiniton1.setVertices(vertices);
        followerDefiniton1.setProjectedAttributeNamesList(Arrays.asList("f1_follower_id"));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton1.getRelation(), followerDefiniton1);
        offsetAttributes[1] = offsetAttributes[0] + followerDefiniton1.getProjectedAttributeNamesList().size();
        
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        JoinTupleDefinition<ALIAS> followerDefiniton2 = new JoinTupleDefinition<ALIAS>();
        followerDefiniton2.setRelation("follower2");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f2_follower_id"));
        vertex.setAlias(ALIAS.ALIASUSERID1);
        vertices.add(vertex);
        
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f2_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID2);
        vertices.add(vertex);
        followerDefiniton2.setVertices(vertices);
        //followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("follower_id", "f_user_id"));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton2.getRelation(), followerDefiniton2);
        offsetAttributes[2] = offsetAttributes[1] + followerDefiniton2.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> tweetDefinition = new JoinTupleDefinition<ALIAS>();
        tweetDefinition.setRelation("tweet");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID2);
        vertices.add(vertex);
        tweetDefinition.setVertices(vertices);
        tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[3] = offsetAttributes[2] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(Connection connection = DriverManager.getConnection(URL, "gaurav", "gaurav");) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as f1_user_id"
                    + ", follower_id as f1_follower_id from follower"), followerDefiniton1, tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as f2_user_id"
                    + ", follower_id as f2_follower_id from follower"), followerDefiniton2, tableDefinition, cntTableAndIndices, 1);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as r_user_id, tweet_id"
                    + ", tweet_date from tweet"), tweetDefinition, tableDefinition, cntTableAndIndices, 2);
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);
        }
        cntTable = cntTableAndIndices.cntTable;
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void testFanoutSpace() throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        System.out.println("----------------testFanout1-----------------");
        gIvmFanoutSpaceTest(1);
        System.out.println("----------------testFanout10-----------------");
        gIvmFanoutSpaceTest(10);
        System.out.println("----------------testFanout30-----------------");
        gIvmFanoutSpaceTest(30);
        System.out.println("----------------testFanout50-----------------");
        gIvmFanoutSpaceTest(50);
        System.out.println("----------------testFanout70-----------------");
        gIvmFanoutSpaceTest(70);
        System.out.println("----------------testFanout100-----------------");
        gIvmFanoutSpaceTest(100);
    }
    @Test
    public void testFanout1() throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        System.out.println("----------------testFanout1-----------------");
//        gIvmFanoutTest(1);
        System.out.println("----------------testFanout10-----------------");
//        gIvmFanoutTest(10);
        System.out.println("----------------testFanout30-----------------");
        gIvmFanoutTest(30);
        System.out.println("----------------testFanout50-----------------");
//        gIvmFanoutTest(50);
        System.out.println("----------------testFanout70-----------------");
//        gIvmFanoutTest(70);
        System.out.println("----------------testFanout100-----------------");
//        gIvmFanoutTest(100);
    }
    @Test
    public void testFanoutPv() throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        System.out.println("----------------testFanout1-----------------");
        gIvmFanoutPvTest(1);
        System.out.println("----------------testFanout10-----------------");
        gIvmFanoutPvTest(10);
        System.out.println("----------------testFanout30-----------------");
        gIvmFanoutPvTest(30);
        System.out.println("----------------testFanout50-----------------");
        gIvmFanoutPvTest(50);
        System.out.println("----------------testFanout70-----------------");
        gIvmFanoutPvTest(70);
        System.out.println("----------------testFanout100-----------------");
        gIvmFanoutPvTest(100);
    }
    @Test
    public void testFanoutScan() throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        System.out.println("----------------testFanout1-----------------");
        gIvmFanoutScanTest(1);
        System.out.println("----------------testFanout10-----------------");
        gIvmFanoutScanTest(10);
        System.out.println("----------------testFanout30-----------------");
        gIvmFanoutScanTest(30);
        System.out.println("----------------testFanout50-----------------");
        gIvmFanoutScanTest(50);
        System.out.println("----------------testFanout70-----------------");
        gIvmFanoutScanTest(70);
        System.out.println("----------------testFanout100-----------------");
        gIvmFanoutScanTest(100);
    }
    private void gIvmFanoutTest(int fanout) throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        setUp();
        it = 0;
        final int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        for(int j = 0; j < fanout; j++) {
            insertFollower1(2 * X + j, -1, fastJoinDeltaManager);
        }
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < fanout; j++) {
                insertFollower2(X + i, 2 * X + j, fastJoinDeltaManager);
            }
        }
        for(int k = 0; k < 5; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(X + i, X + i, fastJoinDeltaManager, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTweet(X + i, X + i, fastJoinDeltaManager, false);
            }
        }
    }
    private void gIvmFanoutSpaceTest(int fanout) throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        destruct();
        System.gc();
        setUp();
        final int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        for(int j = 0; j < fanout; j++) {
            insertFollower1(2 * X + j, -1, fastJoinDeltaManager);
        }
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < fanout; j++) {
                insertFollower2(X + i, 2 * X + j, fastJoinDeltaManager);
            }
        }
        for(int i = 0; i < MAX; i++) {
            insertTweet(X + i, X + i, fastJoinDeltaManager, true);
        }
        System.out.println(TestUtil.getTestName());
        TestUtil.printSize(cntTable, MemoryManager.getMemoryManagerDs());
    }
    private void gIvmFanoutPvTest(int fanout) throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        setUp();
        final int MAX = 5000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        for(int j = 0; j < fanout; j++) {
            insertFollower1(2 * X + j, -1, fastJoinDeltaManager);
        }
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < fanout; j++) {
                insertFollower2(X + i, 2 * X + j, fastJoinDeltaManager);
            }
        }
        for(int k = 0; k < 10; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(X + i, X + i, fastJoinDeltaManager, false);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTweet(X + i, X + i, fastJoinDeltaManager, false);
            }
        }
    }
    private void gIvmFanoutScanTest(int fanout) throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        setUp();
        final int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        for(int j = 0; j < fanout; j++) {
            insertFollower1(2 * X + j, -1, fastJoinDeltaManager);
        }
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < fanout; j++) {
                insertFollower2(X + i, 2 * X + j, fastJoinDeltaManager);
            }
        }
        for(int i = 0; i < MAX; i++) {
            insertTweet(X + i, X + i, fastJoinDeltaManager, false);
        }
        for(int k = 0; k < 10; k++) {
            System.gc();
            long tm = System.nanoTime();
            fastJoinDeltaManager.scan(cntTable);
            long t = (System.nanoTime() - tm)  / 1000000;
            System.out.println(TestUtil.getTestName() + t);
        }
    }
    private void insertTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID2);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>(1);
        projectedAttributeValues.add(tweet);//tweet_id - not present
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 2, "tweet"
                , cntTable.getTableDefinition(), null);
            if(it++ == 0)
                System.out.println(t.size());
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2, "tweet"
                , cntTable.getTableDefinition(), null);
    }
    int it = 0;
    private void deleteTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID2);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(tweet);//tweet_id - not present
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 2);
            t = null;
        } else
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2);
    }
    private void insertFollower1(int user, int follower, FastJoinDeltaManager fastJoinDeltaManager) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID1);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(follower);//FOLLOWER
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 0, "follower"
            , cntTable.getTableDefinition(), null);
    }
    private void deleteFollower1(int user, int follower, FastJoinDeltaManager fastJoinDeltaManager) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID1);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(follower);//FOLLOWER
        fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 0);
    }
    private void insertFollower2(int user, int follower, FastJoinDeltaManager fastJoinDeltaManager) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID1);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(follower);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID2);
        joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);
        vertexValues[1] = vertexValue;
        
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1, "follower"
            , cntTable.getTableDefinition(), null);
    }
    private void deleteFollower2(int user, int follower, FastJoinDeltaManager fastJoinDeltaManager) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID1);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(follower);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID2);
        joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);
        vertexValues[1] = vertexValue;
        
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1);
    }
    public void destruct() {
        cntTable = null;
        MemoryManager.destruct();
    }
}