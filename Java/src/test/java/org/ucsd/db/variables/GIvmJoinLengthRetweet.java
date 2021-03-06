package org.ucsd.db.variables;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
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
import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.tableloader.CntTableAndIndices;
import org.ucsd.db.fastview.tableloader.JoinTupleDefinition;
import org.ucsd.db.fastview.tableloader.SqlTableLoader;

public class GIvmJoinLengthRetweet {
    private enum ALIAS {
        ALIASUSERID_0, ALIASUSERID_1, ALIASUSERID_2, ALIASUSERID_3, ALIASUSERID_4, ALIASUSERID_5, ALIASUSERID_6, ALIASUSERID_7,
        ALIASUSERID_8, ALIASUSERID_9, ALIASUSERID_10
    }
    public static CntTableAndIndices setUp(int joinLength) throws InstantiationException, IllegalAccessException, ClassNotFoundException
    , IOException, SQLException {
        SqlTableLoader sqlTableLoader = new SqlTableLoader();
        
        CntTableAndIndices cntTableAndIndices = new CntTableAndIndices();
        TableDefinition tableDefinition = new TableDefinition();
        cntTableAndIndices.cntTable.setTableDefinition(tableDefinition);
        
        int[] offsetAttributes = new int[joinLength + 2];
        tableDefinition.setOffsetAttributes(offsetAttributes);
        
        initDriver();
        
        JoinTupleDefinition<ALIAS> followerDefiniton = new JoinTupleDefinition<ALIAS>();
        followerDefiniton.setRelation("retweet_" + 0);
        List<Vertex<ALIAS>> vertices = new ArrayList<>();
        Vertex<ALIAS> vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("tweet_id_" + 0));
        vertex.setAlias(ALIAS.valueOf("ALIASUSERID_" + 0));
        vertices.add(vertex);
        followerDefiniton.setVertices(vertices);
        followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("tweet_id_" + 0, "retweet_tweet_id_" + 0));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
        offsetAttributes[1] = offsetAttributes[0] + followerDefiniton.getProjectedAttributeNamesList().size();
        
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "")) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select tweet_id as tweet_id_" + 0
                + ", retweet_tweet_id as retweet_tweet_id_" + 0 + " from retweet"), followerDefiniton, tableDefinition, cntTableAndIndices, 0);
        }
        
        for(int i = 1; i < joinLength - 1; i++) {
            followerDefiniton = new JoinTupleDefinition<ALIAS>();
            followerDefiniton.setRelation("retweet_" + i);
            vertices = new ArrayList<>();
            vertex = new Vertex<ALIAS>();
            vertex.setJoinAttributes(Arrays.asList("retweet_tweet_id_" + i));
            vertex.setAlias(ALIAS.valueOf("ALIASUSERID_" + (i - 1)));
            vertices.add(vertex);
            vertex = new Vertex<ALIAS>();
            vertex.setJoinAttributes(Arrays.asList("tweet_id_" + i));
            vertex.setAlias(ALIAS.valueOf("ALIASUSERID_" + i));
            vertices.add(vertex);
            followerDefiniton.setVertices(vertices);
            //followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("retweet_tweet_id_" + i, "tweet_id_" + i));
            
            tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
            offsetAttributes[i + 1] = offsetAttributes[i] + followerDefiniton.getProjectedAttributeNamesList().size();
            try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "")) {
                sqlTableLoader.loadTable(connection.createStatement().executeQuery("select tweet_id as tweet_id_" + i
                    + ", retweet_tweet_id as retweet_tweet_id_" + i + " from retweet"), followerDefiniton, tableDefinition, cntTableAndIndices, i);
            }
        }
        int i = joinLength - 1;
        followerDefiniton = new JoinTupleDefinition<ALIAS>();
        followerDefiniton.setRelation("retweet_" + i);
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("retweet_tweet_id_" + i));
        vertex.setAlias(ALIAS.valueOf("ALIASUSERID_" + (i - 1)));
        vertices.add(vertex);
        followerDefiniton.setVertices(vertices);
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("r_user_id"));
        vertex.setAlias(ALIAS.valueOf("ALIASUSERID_" + i));
        vertices.add(vertex);
        followerDefiniton.setVertices(vertices);
        //followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("retweet_tweet_id_" + i, "tweet_id_" + i));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
        offsetAttributes[i + 1] = offsetAttributes[i] + followerDefiniton.getProjectedAttributeNamesList().size();
        
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "")) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as r_user_id, tweet_id as tweet_id_" + i
                + ", retweet_tweet_id as retweet_tweet_id_" + i + " from retweet"), followerDefiniton, tableDefinition, cntTableAndIndices, i);
        }
        i = joinLength;
        followerDefiniton = new JoinTupleDefinition<ALIAS>();
        followerDefiniton.setRelation("follower");
        vertices = new ArrayList<>();
        vertex = new Vertex<ALIAS>();
        vertex.setJoinAttributes(Arrays.asList("f_user_id"));
        vertex.setAlias(ALIAS.valueOf("ALIASUSERID_" + (i - 1)));
        vertices.add(vertex);
        vertex = new Vertex<ALIAS>();
        followerDefiniton.setVertices(vertices);
        followerDefiniton.setProjectedAttributeNamesList(Arrays.asList("f_user_id", "follower_id"));
        
        tableDefinition.getJoinDefinitionMap().put(followerDefiniton.getRelation(), followerDefiniton);
        offsetAttributes[i + 1] = offsetAttributes[i] + followerDefiniton.getProjectedAttributeNamesList().size();
        
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "")) {
            sqlTableLoader.loadTable(connection.createStatement().executeQuery("select user_id as f_user_id, follower_id from follower where user_id = 13")
                , followerDefiniton, tableDefinition, cntTableAndIndices, i);
        }
        
        sqlTableLoader.linkJoinTuples(cntTableAndIndices);
        return cntTableAndIndices;
    }
    private static void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();
    }
    @Test
    public void gIvmJoinLengthTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, IOException, SQLException {
        System.out.println("-------------------------Join Length = 3-------------------------");
        testJoinLength(2);
        System.out.println("-------------------------Join Length = 4-------------------------");
        testJoinLength(3);
        System.out.println("-------------------------Join Length = 5-------------------------");
        testJoinLength(4);
        System.out.println("-------------------------Join Length = 6-------------------------");
        testJoinLength(5);
    }
    private void testJoinLength(int length) throws InstantiationException, IllegalAccessException, ClassNotFoundException, IOException, SQLException {
        MemoryManager.destruct();
        MemoryManager.init(length + 1);
        CntTableAndIndices cntTableAndIndices = setUp(length);
        CntTable cntTable = cntTableAndIndices.cntTable;
        FastJoinDeltaManager deltaManager = new FastJoinDeltaManager();
        int MAX = 500;
        int X = 1000000000;
        for(int j = 0; j < 10; j++) {
            long tm = System.nanoTime();
            for(int i = 0; i < MAX ; i++) {
                insertFollower(13, X + i, cntTable.getTableDefinition(), length, deltaManager, cntTable);
            }
            long t = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + t + "/" + MAX);
            for(int i = 0; i < MAX ; i++) {
                deleteFollower(13, X + i, cntTable.getTableDefinition(), length, deltaManager, cntTable);
            }
        }
    }
    private void insertFollower(int user_id, int follower_id, TableDefinition tableDefinition, int length
        , FastJoinDeltaManager fastJoinDeltaManager, CntTable cntTable) {
        
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.valueOf("ALIASUSERID_" + (length - 1)));
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user_id);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user_id);//FOLLOWER
        projectedAttributeValues.add(follower_id);//FOLLOWER
        
        List<Object[]> insertTuple = fastJoinDeltaManager.insertTuple(cntTable, vertexValues, projectedAttributeValues, length, "follower"
            , tableDefinition, null);
        insertTuple = null;
    }
    private void deleteFollower(int user_id, int follower_id, TableDefinition tableDefinition, int length
        , FastJoinDeltaManager fastJoinDeltaManager, CntTable cntTable) {
        VertexValue<ALIAS>[] vertexValues = new VertexValue[1];
        VertexValue<ALIAS> vertexValue = new VertexValue<ALIAS>();
        vertexValue.setAlias(ALIAS.valueOf("ALIASUSERID_" + (length - 1)));
        List<Object> joinAttributeValues = new ArrayList<>();
        vertexValue.setAttributeValues(joinAttributeValues);
        joinAttributeValues.add(user_id);//t_user_id - present
        vertexValues[0] = vertexValue;
        
        List<Object> projectedAttributeValues = new ArrayList<>();
        projectedAttributeValues.add(user_id);//FOLLOWER
        projectedAttributeValues.add(follower_id);//FOLLOWER
        
        fastJoinDeltaManager.deleteTupleWithoutReturn(cntTable, vertexValues, projectedAttributeValues, length);
    }
}