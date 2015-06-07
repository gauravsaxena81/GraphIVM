package org.ucsd.db.bsmagen1M.mixedworkload;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
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
import org.ucsd.db.fastview.tableloader.AggregationSqlTableLoader;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;

public class FastJoinDeltaManager1MQ2Aggregation {
    private enum ALIAS {
        ALIASUSERID
    }
    private static CntTable cntTable;
    private static final List<String> PROJECTED_ATTRIBUTES_LIST_TWEET = Arrays.asList("tweet_id");
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//1k:367022,1m:12404312;
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        MemoryManager.init(2);
        AggregationSqlTableLoader aggregationSqlTableLoader = new AggregationSqlTableLoader();
        
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
        followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("follower_id"));
        //followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("f_user_id"));
        
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
        tweetDefinition.setProjectedAttributeNamesList(PROJECTED_ATTRIBUTES_LIST_TWEET);
        
        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[2] = offsetAttributes[1] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        HashMap<String, AggregatedAttribute> aggregatedAttributes = new HashMap<>();
        tableDefinition.setAggregatedAttributes(aggregatedAttributes);
        aggregatedAttributes.put("tweet_id", new AggregatedAttribute(Function.COUNT, 0, 1));
        tableDefinition.setGroupByAttributes(Arrays.asList(0));
        
        initDriver();
        try(Connection connection = DriverManager.getConnection(URL, "gaurav", "gaurav")){
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as f_user_id"
                    + ", follower_id from follower"), followerDefiniton, tableDefinition, cntTableAndIndices, 0);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as r_user_id, tweet_id"
                    + " from tweet where tweet_id > " + MIXED_WORKLOAD_CONSTANT), tweetDefinition, tableDefinition, cntTableAndIndices, 1);
        }
        cntTable = cntTableAndIndices.cntTable;
        aggregationSqlTableLoader.linkJoinTuples(cntTableAndIndices);
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }

    private void insertTweet(int user, int tweet, FastAggregationManager aggregationManager
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<Object>(1);
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = aggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
            t = null;
        } else
            aggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.AGGREGATED_TUPLE, 1
                , "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
    }
    private void deleteTweet(int user, int tweet, FastAggregationManager aggregationManager, int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(tweet);//tweet_id - not present
        if(doReturn) {
            List<Object[]> t = aggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
            t = null;
        } else
            aggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.AGGREGATED_TUPLE, 1);
    }
    
    private void insertFollower(int user, int follower, FastAggregationManager fastAggregationManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.JOIN_TUPLE, 0, "follower"
            , cntTable.getTableDefinition(), null);
    }
    @Test
    public void insertMixedWorkLoadPdPv() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, "gaurav", "gaurav")) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select * from (select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX + ") A order by 2 asc;");
            FastAggregationManager fastAggregationManager = new FastAggregationManager();
            int[] aggregatedAttributes = new int[]{0};
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
                    if(rs.getInt(4) == -1)
                        insertTweet(rs.getInt(1), rs.getInt(2), fastAggregationManager, aggregatedAttributes, true);
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        deleteTweet(rs.getInt(1), rs.getInt(2), fastAggregationManager, aggregatedAttributes, false);
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
            FastAggregationManager fastAggregationManager = new FastAggregationManager();
            int[] aggregatedAttributes = new int[]{0};
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
                    if(rs.getInt(4) == -1)
                        insertTweet(rs.getInt(1), rs.getInt(2), fastAggregationManager, aggregatedAttributes, false);
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        deleteTweet(rs.getInt(1), rs.getInt(2), fastAggregationManager, aggregatedAttributes, false);
                }
                System.out.println(t0 + "," + t1);
            }
        }
    }
    public void destruct() {
        cntTable = null;
    }
}