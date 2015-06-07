package org.ucsd.db.bsmagen1M;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
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

public class FastJoinDeltaManager1MQ2 {
    private enum ALIAS {
        ALIASUSERID
    }
    private static CntTable cntTable;
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException
    , IOException, SQLException {
        MemoryManager.init(2);
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[3];
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        JoinTupleDefinition<ALIAS> followerDefiniton = new JoinTupleDefinition<ALIAS>();
        followerDefiniton.setRelation("follower");
        List<Vertex<ALIAS>> vertices = new ArrayList<>();
        Vertex<ALIAS> vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        followerDefiniton.setVertices(vertices);
        followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("follower_id", "f_user_id"));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
        offsetAttributes[1] = offsetAttributes[0] + followerDefiniton.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> tweetDefinition = new JoinTupleDefinition<ALIAS>();
        tweetDefinition.setRelation("tweet");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID);
        vertices.add(vertex);
        tweetDefinition.setVertices(vertices);
        tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("tweet_id", "tweet_date", "r_user_id"));
        
        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[2] = offsetAttributes[1] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav")) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as f_user_id"
                    + ", follower_id from follower"), followerDefiniton, tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as r_user_id, tweet_id"
                    + ", tweet_date from tweet"), tweetDefinition, tableDefinition, cntTableAndIndices, 1);
        }
        cntTable = cntTableAndIndices.cntTable;
        sqlTableLoader.linkJoinTuples(cntTableAndIndices);
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }
    @Test
    public void insertProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException, IOException, SQLException {
        final int MAX = 50000;
        final int X = 1000000000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4});
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(100, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteTweet(100, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    @Test
    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException, IOException, SQLException {
        final int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4});
        for(int k = 0; k < 25; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(100, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(100, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
        for(int i = 0; i < MAX; i++) {
            insertTweet(100, X + i, fastJoinDeltaManager, viewMaintainer, false);
        }
        long tm = System.nanoTime();
        for(int i = 0; i < MAX; i++) {
            deleteTweet(100, X + i, fastJoinDeltaManager, viewMaintainer, true);
        }
        System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
    }

    private void insertTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add("1992-01-01");//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 1
            , "tweet", cntTable.getTableDefinition(), null);
            for(Object[] i : t)
                viewMaintainer.insertTuple(i);
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1
                , "tweet", cntTable.getTableDefinition(), null);
    }
    private void deleteTweet(int user, int tweet, FastJoinDeltaManager fastJoinDeltaManager, ViewMaintainer viewMaintainer
        , boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add("1992-01-01");//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1);
            for(Object[] i : t)
                viewMaintainer.insertTuple(i);
            t = null;
        } else
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1);
    }
    @Test
    public void insertJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException, IOException, SQLException {
        final int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < 5; j++) {
                insertFollower(X + i, j, fastJoinDeltaManager);
            }
        }
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4});
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            long t = (System.nanoTime() - tm)  / 1000000;
            for(int i = 0; i < MAX; i++) {
                deleteTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
        }
    }
    
    @Test
    public void deleteJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException, IOException, SQLException {
        final int MAX = 50000;
        FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
        int X = 1000000000;
        ViewMaintainer viewMaintainer = new ViewMaintainer(new int[]{0,1,2,3,4});
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < 5; j++) {
                insertFollower(X + i, j, fastJoinDeltaManager);
            }
        }
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(X + i, X + i, fastJoinDeltaManager, viewMaintainer, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }

    private void insertFollower(int user, int follower, FastJoinDeltaManager fastJoinDeltaManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(follower);//FOLLOWER
        projectedAttributeValues.add(user);
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 0, "follower"
            , cntTable.getTableDefinition(), null);
    }

    public static void destruct() {
        cntTable = null;
        MemoryManager.destruct();
    }
}
