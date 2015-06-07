package org.ucsd.db.bsmagen1M.mixedworkload;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
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
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.fastview.tableloader.SqlTableLoader;

/**
 * Not complete.
 *
 */
public class FastJoinDeltaManager1MQ5 {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID, ALIASUSERID_1
    }
    final static int MIXED_WORKLOAD_CONSTANT = 12354312;//1k:407869:1m:12404312;
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static CntTable cntTable;
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(3);
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
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
        tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("t_tweet_date", "t_user_id", "t_tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[2] = offsetAttributes[1] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        JoinTupleDefinition<ALIAS> retweetDefinition = new JoinTupleDefinition<ALIAS>();
        retweetDefinition.setRelation("retweet");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("retweet_tweet_id"));
        vertex.setAlias(ALIAS.ALIASTWEETID);
        vertices.add(vertex);
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r_user_id"));
        vertex.setAlias(ALIAS.ALIASUSERID_1);
        vertices.add(vertex);
        retweetDefinition.setVertices(vertices);
        retweetDefinition.setProjectedAttributeNamesList(Arrays.asList("r_tweet_id", "r_tweet_date", "retweet_tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(retweetDefinition.getRelation(), retweetDefinition);
        offsetAttributes[3] = offsetAttributes[2] + retweetDefinition.getProjectedAttributeNamesList().size();
        
        
        JoinTupleDefinition<ALIAS> users1Definition = new JoinTupleDefinition<ALIAS>();
        retweetDefinition.setRelation("users1");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("retweet_tweet_id"));
        vertex.setAlias(ALIAS.ALIASUSERID_1);
        vertices.add(vertex);
        retweetDefinition.setVertices(vertices);
        retweetDefinition.setProjectedAttributeNamesList(Arrays.asList("r_tweet_id", "r_tweet_date", "retweet_tweet_id"));
        
        tableDefinition.getJoinDefinitionMap().put(retweetDefinition.getRelation(), retweetDefinition);
        offsetAttributes[3] = offsetAttributes[2] + retweetDefinition.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(Connection connection = DriverManager.getConnection(URL, "gaurav", "gaurav");) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                    tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id, tweet_id "
                    + "as t_tweet_id, tweet_date as t_tweet_date from tweet where tweet_id < " + MIXED_WORKLOAD_CONSTANT)
                    , tweetDefinition, tableDefinition, cntTableAndIndices, 1);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as r_user_id, tweet_id as r_tweet_id"
                    + ", tweet_date as r_tweet_date, retweet_tweet_id from retweet where tweet_id < " + MIXED_WORKLOAD_CONSTANT)
                    , retweetDefinition, tableDefinition, cntTableAndIndices, 2);
            cntTable = cntTableAndIndices.cntTable;
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }

    private void insertReTweet(int retweetTweetId, int tweetId, String date, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);//tweet_id - present
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(tweetId);
        projectedAttributeValues.add(date);
        projectedAttributeValues.add(retweetTweetId);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 2, "retweet"
                , cntTable.getTableDefinition(), null);
            t = null;
        } else {
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2, "retweet"
                            , cntTable.getTableDefinition(), null);
        }
    }
    private void deleteReTweet(int retweetTweetId, int tweetId, String date, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(retweetTweetId);//tweet_id - present
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(tweetId);
        projectedAttributeValues.add(date);
        projectedAttributeValues.add(retweetTweetId);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 2);
            t = null;
        } else {
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 2);
        }
    }

    private void insertTweet(int user, int tweet, String date, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(tweet);//t_user_id - present
        vertexValues[1] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(date);//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, 1
            , "tweet", cntTable.getTableDefinition(), null);
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1
                , "tweet", cntTable.getTableDefinition(), null);
    }
    private void deleteTweet(int user, int tweet, String date, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[2];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASTWEETID);
        joinAttributeValues = new ArrayList<>(1);
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(tweet);//t_user_id - present
        vertexValues[1] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(date);//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1);
            t = null;
        } else
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 1);
    }
    private void insertUser(int user, FastJoinDeltaManager fastJoinDeltaManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(user);
        
        fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, 0, "users"
            , cntTable.getTableDefinition(), null);
    }
    
    @Test
    public void insertMixedWorkLoadPdPv() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, "gaurav", "gaurav")) {
            int MAX = 100000;
            PreparedStatement ps = connection.prepareStatement("select * from (select user_id, tweet_id, tweet_date"
                            + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX + ") A order by 2 asc;");
            FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
            for(int i = 0; i < 10; i++) {
                int t0 = 0, t1 = 0;
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        t0 = rs.getInt(1); 
                        t1 = rs.getInt(2);
                    } else {
                        t0 = rs.getInt(1); 
                        t1 = rs.getInt(4);
                    }
                }
                long d = (System.nanoTime() - tm)  / 1000000;
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        insertTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, true);
                    else
                        insertReTweet(rs.getInt(4), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, true);
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        deleteTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                    else
                        deleteReTweet(rs.getInt(4), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                }
                System.out.println(t0 + "," + t1);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadPv() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, "gaurav", "gaurav")) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select * from (select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX + ") A order by 2 asc;");
            FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
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
                    if(rs.getInt(4) == -1)
                        insertTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                    else
                        insertReTweet(rs.getInt(4), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        deleteTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                    else
                        deleteReTweet(rs.getInt(4), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                }
                System.out.println(t0 + "," + t1 + s);
            }
        }
    }

    public static void destruct() {
        cntTable = null;
        MemoryManager.destruct();
    }
}