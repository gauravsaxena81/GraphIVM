package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ3 {
    private static final String INSERT_USER_QUERY = "INSERT into %s values(%d)";
    private static final String INSERT_RETWEET_QUERY = "INSERT into temp values(2, %d, '2992-02-21', 2)";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(pStatement.executeQuery(), 7);
                    pStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
   
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into %s values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        final String UPDATE_RETWEET_QUERY = "UPDATE temp set user_id = %d, tweet_id = %d, retweet_tweet_id = %d";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "users", X + i));
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", X + i, X + i));
            }
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
                    TestUtil.iterate(pStatement.executeQuery(), 7);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            
            other.executeUpdate("delete from temp");
            PreparedStatement pStatement = connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int x = 1000000000;
            other.executeUpdate(String.format(INSERT_USER_QUERY, "users", x));
            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x, x));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", x, x + i, x));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 7);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i));
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 7);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }
}
