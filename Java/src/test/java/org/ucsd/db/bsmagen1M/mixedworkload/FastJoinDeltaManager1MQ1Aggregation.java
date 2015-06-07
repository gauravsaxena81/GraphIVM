package org.ucsd.db.bsmagen1M.mixedworkload;

import java.io.File;
import java.io.FileNotFoundException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

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
import org.ucsd.db.fastview.delta.FastJoinDeltaManager;
import org.ucsd.db.fastview.tableloader.AggregationSqlTableLoader;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;

public class FastJoinDeltaManager1MQ1Aggregation {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID,
    }
    private static CntTable cntTable;
    private static final List<String> PROJECTED_ATTRIBUTES_LIST_TWEET = Arrays.asList("t_user_id");
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//407869:12404312;
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static final String PASSWORD = "gaurav";
    private static final String USERNAME = "gaurav";
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(2);
        AggregationSqlTableLoader sqlTableLoader = new AggregationSqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[3];
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
        tweetDefinition.setVertices(vertices);
        tweetDefinition.setProjectedAttributeNamesList(PROJECTED_ATTRIBUTES_LIST_TWEET);
        
        tableDefinition.getJoinDefinitionMap().put(userDefiniton.getRelation(), userDefiniton);
        offsetAttributes[2] = offsetAttributes[1] + userDefiniton.getProjectedAttributeNamesList().size();
        
        HashMap<String, AggregatedAttribute> aggregatedRelations = new HashMap<String, AggregatedAttribute>();
        tableDefinition.setAggregatedAttributes(aggregatedRelations);
        aggregatedRelations.put("t_user_id", new AggregatedAttribute(Function.COUNT, 0, 1));
        tableDefinition.setGroupByAttributes(Arrays.asList(0));
        
        initDriver();
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                        tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id from tweet where tweet_id < " 
                        + MIXED_WORKLOAD_CONSTANT), tweetDefinition, tableDefinition, cntTableAndIndices, 1);
            cntTable = cntTableAndIndices.cntTable;
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }

    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }

    private void insertTweet(int user, int tweet, FastAggregationManager fastAggregationManager
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<>(1);
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                    , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
            t = null;
        } else
            fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
    }
    private void deleteTweet(int user, int tweet, FastAggregationManager fastAggregationManager
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = fastAggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
            t = null;
        } else
            fastAggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues
                        , TYPE.AGGREGATED_TUPLE, 1);
    }

    private void insertUser(int user, FastAggregationManager fastAggregationManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.JOIN_TUPLE, 0, "users"
            , cntTable.getTableDefinition(), null);
    }
    @Test
    public void insertMixedWorkLoadPdPv() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
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
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
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
    @Test
    public void insertMixedWorkLoadPdPvFile() throws SQLException, FileNotFoundException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            FastAggregationManager fastAggregationManager = new FastAggregationManager();
            int[] aggregatedAttributes = new int[]{0};
            long av = 0;
            for(int i = 0; i < 20; i++) {
                Scanner scanner = new Scanner(new File("/home/ucsd/gaurav/dbtoaster/examples/data/fastview/tweetInput.csv"));
                System.gc();
                long tm = System.nanoTime();
                while(scanner.hasNextLine()) {
                    String nextLine = scanner.nextLine();
                    String[] split = nextLine.split(",");
                    insertTweet(Integer.parseInt(split[0]), Integer.parseInt(split[1]), fastAggregationManager
                                    , aggregatedAttributes, true);
                }
                long t = (System.nanoTime() - tm)  / 1000000;
                scanner.close();
                scanner = new Scanner(new File("/home/ucsd/gaurav/dbtoaster/examples/data/fastview/tweetInput.csv"));
                av += t;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                while(scanner.hasNextLine()) {
                    String nextLine = scanner.nextLine();
                    String[] split = nextLine.split(",");
                    deleteTweet(Integer.parseInt(split[0]), Integer.parseInt(split[1]), fastAggregationManager
                                    , aggregatedAttributes, false);
                }
                scanner.close();
            }
            System.out.println("Average:" + av / 20);
        }
    }
    public void destruct() {
        cntTable = null;
    }
}
