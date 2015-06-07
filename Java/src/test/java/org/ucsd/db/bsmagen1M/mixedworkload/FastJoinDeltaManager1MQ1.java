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
import java.util.List;
import java.util.Scanner;

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

public class FastJoinDeltaManager1MQ1 {
    private enum ALIAS {
        ALIASTWEETID, ALIASUSERID,
    }
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//1k:407869:1m:12404312;
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static final String PASSWORD = "gaurav";
    private static final String USERNAME = "gaurav";
    private static CntTable cntTable;
    @BeforeClass
    public static void setUp() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        MemoryManager.init(2);
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
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
        tweetDefinition.setProjectedAttributeNamesList(Arrays.asList("tweet_id", "tweet_date", "t_user_id"));

        tableDefinition.getJoinDefinitionMap().put(tweetDefinition.getRelation(), tweetDefinition);
        offsetAttributes[2] = offsetAttributes[1] + tweetDefinition.getProjectedAttributeNamesList().size();
        
        initDriver();
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as u_user_id from users"), userDefiniton, 
                        tableDefinition, cntTableAndIndices, 0);
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as t_user_id, tweet_id"
                            + ", tweet_date from tweet where tweet_id <" + MIXED_WORKLOAD_CONSTANT + ""), tweetDefinition, 
                        tableDefinition, cntTableAndIndices, 1);
            cntTable = cntTableAndIndices.cntTable;
            sqlTableLoader.linkJoinTuples(cntTableAndIndices);
            //TestUtil.printSize(cntTable);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }

    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }

    private void insertTweet(int user, int tweet, String date, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.insertTuple(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet, date), 1
            , "tweet", cntTable.getTableDefinition(), null);
            t = null;
        } else
            fastJoinDeltaManager.insertTupleWithoutReturn(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet, date), 1
                , "tweet", cntTable.getTableDefinition(), null);
    }
    private void deleteTweet(int user, int tweet, String date, FastJoinDeltaManager fastJoinDeltaManager, boolean doReturn) {
        if(doReturn) {
            List<Object[]> t = fastJoinDeltaManager.deleteTuple(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet, date), 1);
        } else
            fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, getvertexValues(user), getprojectedAttributeValues(user, tweet, date), 1);
    }

    private VertexValue<ALIAS>[] getvertexValues(int user) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;//FastViewUtil.getOldVertexValue(vertexValue);
        
        return vertexValues;
    }

    private List<Object> getprojectedAttributeValues(int user, int tweet, String date) {
        ArrayList<Object> projectedAttributeValues = new ArrayList<Object>(3);
        projectedAttributeValues.add(date);//tweet date;
        projectedAttributeValues.add(tweet);//tweet_id - not present
        projectedAttributeValues.add(user);
        return projectedAttributeValues;
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
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
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
                        insertTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, true);
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        deleteTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                }
                System.out.println(t0 + "," + t1 + s);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadPv() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
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
                }
                long t = (System.nanoTime() - tm - d)  / 1000000;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                rs = ps.executeQuery();
                while(rs.next()) {
                    if(rs.getInt(4) == -1)
                        deleteTweet(rs.getInt(1), rs.getInt(2), rs.getString(3), fastJoinDeltaManager, false);
                }
                System.out.println(t0 + "," + t1 + "," + s);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadPdPvFile() throws SQLException, FileNotFoundException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            FastJoinDeltaManager fastJoinDeltaManager = new FastJoinDeltaManager();
            long av = 0;
            for(int i = 0; i < 20; i++) {
                Scanner scanner = new Scanner(new File("/home/ucsd/gaurav/dbtoaster/examples/data/fastview/tweetInput.csv"));
                System.gc();
                long tm = System.nanoTime();
                while(scanner.hasNextLine()) {
                    String nextLine = scanner.nextLine();
                    String[] split = nextLine.split(",");
                    insertTweet(Integer.parseInt(split[0]), Integer.parseInt(split[1]), split[2], fastJoinDeltaManager, true);
                }
                long t = (System.nanoTime() - tm)  / 1000000;
                scanner.close();
                scanner = new Scanner(new File("/home/ucsd/gaurav/dbtoaster/examples/data/fastview/tweetInput.csv"));
                av += t;
                System.out.println(TestUtil.getTestName() + t + "/" + MAX);
                while(scanner.hasNextLine()) {
                    String nextLine = scanner.nextLine();
                    String[] split = nextLine.split(",");
                    deleteTweet(Integer.parseInt(split[0]), Integer.parseInt(split[1]), split[2], fastJoinDeltaManager, false);
                }
                scanner.close();
            }
            System.out.println("Average:" + av / 20);
        }
    }

    public static void destruct() {
        cntTable = null;
        MemoryManager.destruct();
    }
}
