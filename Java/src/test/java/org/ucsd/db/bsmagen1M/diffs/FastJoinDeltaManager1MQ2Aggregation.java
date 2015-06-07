package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.text.ParseException;
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
import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.index.IntArrayList;
import org.ucsd.db.fastview.tableloader.AggregationSqlTableLoader;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.view.ViewMaintainer;

public class FastJoinDeltaManager1MQ2Aggregation {
    private enum ALIAS {
        ALIASUSERID
    }
    private static CntTable cntTable;
    ViewMaintainer viewMaintainer = new ViewMaintainer(new int[0]);
    private static final List<String> PROJECTED_ATTRIBUTES_LIST_TWEET = Arrays.asList("tweet_id");
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
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav")){
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as f_user_id"
                    + ", follower_id from follower"), followerDefiniton, tableDefinition, cntTableAndIndices, 0);
            aggregationSqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as r_user_id, tweet_id"
                    + " from tweet"), tweetDefinition, tableDefinition, cntTableAndIndices, 1);
        }
        cntTable = cntTableAndIndices.cntTable;
        aggregationSqlTableLoader.linkJoinTuples(cntTableAndIndices);
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }
    @Test
    public void insertProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        final int MAX = 50000;
        final int X = 1000000000;
        FastAggregationManager fastJoinDeltaManager = new FastAggregationManager();
        int[] aggregatedAttributes = new int[]{0};
        IntArrayList joinAttributeIndices = new IntArrayList();
        joinAttributeIndices.addAll(0, new int[]{0});
        for(int k = 0; k < 15; k++) {
            System.gc();
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(100, X + i, fastJoinDeltaManager, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTweet(100, X + i, fastJoinDeltaManager, aggregatedAttributes, true);
            }
        }
    }
    @Test
    public void deleteProjectedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        final int MAX = 50000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        int[] aggregatedAttributes = new int[]{0};
        int X = 1000000000;
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(100, X + i, fastAggregationManager, aggregatedAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(100, X + i, fastAggregationManager, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }

    private void insertTweet(int user, int tweet, FastAggregationManager aggregationManager
        , int[] aggregatedAttributes, boolean doReturn) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>(1);
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>(1);
        projectedAttributeValues.add(user);
        if(doReturn) {
            List<Object[]> t = aggregationManager.insertTuple(cntTable, vertexValues, projectedAttributeValues
                , TYPE.AGGREGATED_TUPLE, 1, "tweet", cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregatedAttributes, 1);
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
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<Object>();
        projectedAttributeValues.add(tweet);//tweet_id - not present
        if(doReturn) {
            List<Object[]> t = aggregationManager.deleteTuple(cntTable, vertexValues, projectedAttributeValues, 1
                            , cntTable.getTableDefinition(), PROJECTED_ATTRIBUTES_LIST_TWEET);
//            for(Object[] i : t)
//                viewMaintainer.applyTuple(i, aggregatedAttributes, -1);
            t = null;
        } else
            aggregationManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.AGGREGATED_TUPLE, 1);
    }
    @Test
    public void insertJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        final int MAX = 50000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        int[] aggregatedAttributes = new int[]{0};
        int X = 1000000000;
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < 5; j++) {
                insertFollower(X + i, i, fastAggregationManager);
            }
        }
        for(int k = 0; k < 15; k++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTweet(X + i, X + i, fastAggregationManager, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
            for(int i = 0; i < MAX; i++) {
                deleteTweet(X + i, X + i, fastAggregationManager, aggregatedAttributes, false);
            }
        }
    }
    
    @Test
    public void deleteJoinedTupleTest() throws ParseException, InstantiationException, IllegalAccessException
    , ClassNotFoundException {
        final int MAX = 50000;
        FastAggregationManager fastAggregationManager = new FastAggregationManager();
        int X = 2000000000;
        for(int i = 0; i < MAX; i++) {
            for(int j = 0; j < 44; j++) {
                insertFollower(X + i, i, fastAggregationManager);
            }
        }
        int[] aggregatedAttributes = new int[]{0};
        for(int k = 0; k < 15; k++) {
            for(int i = 0; i < MAX; i++) {
                insertTweet(X + i, X + i, fastAggregationManager, aggregatedAttributes, false);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                deleteTweet(X + i, X + i, fastAggregationManager, aggregatedAttributes, true);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm)  / 1000000 + "/" + MAX);
        }
    }

    private void insertFollower(int user, int follower, FastAggregationManager fastAggregationManager) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.ALIASUSERID);
        List<Object> joinAttributeValues = new ArrayList<>();
        joinAttributeValues.add(user);//t_user_id - present
        vertexValue.setAttributeValues(joinAttributeValues);
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user);
        fastAggregationManager.insertTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, TYPE.JOIN_TUPLE, 0, "follower"
            , cntTable.getTableDefinition(), null);
    }
}