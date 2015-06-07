package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class H21MQ4Aggregation {
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("./bsmagen1m");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("./bsmagen1m");
    }
    private static final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(384827, %d)";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            
            PreparedStatement pStatement = 
                            connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 384827, follower_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(pStatement.executeQuery(), 3);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set follower_id = follower_id + 1 limit 10");
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            
            PreparedStatement pStatement = 
                      connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                      + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                      + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            connection.setAutoCommit(true);
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 1175996));
            }
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(pStatement.executeQuery(), 3);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            }
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(%d, -1)";
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            
            PreparedStatement pStatement = 
                  connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                  + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                  + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 1175996));
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            }
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 3);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set follower_id = follower_id + 1 limit 10");
                
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            
            PreparedStatement pStatement = 
                      connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                      + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                      + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 3);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set follower_id = follower_id + 1 limit 10");
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance(); 
    }
}
