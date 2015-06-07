package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ4 {
    private static final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(384827, %d)";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 384827, follower_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
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
                    TestUtil.iterate(pStatement.executeQuery(), 9);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
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
                TestUtil.iterate(pStatement.executeQuery(), 9);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
//                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id "
//                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
//                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, follower D"
//                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
//                + " and C.user_id = D.user_id and D.user_id = 3 and D.follower_id = ?");
//            Statement other = connection.createStatement() ){
//            int MAX = 50;
//            int X = 1000000000;
//            PreparedStatement deletePreparedStatement 
//                = connection.prepareStatement("Delete from follower where user_id = 3 and follower_id = ?");
//            for(int k = 0, x = X; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                System.out.println(k);
//                for(int i = 0; i < MAX; i++)
//                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "follower", 3, X + i));
//                int count = 0;
//                long tm = System.nanoTime();
//                for(int i = 0; i < MAX; i++);
//                for(int i = 0; i < MAX; i++) {
//                    prepareStatement.setInt(1, X + i);
//                    ResultSet t = prepareStatement.executeQuery();
//                    while(t.next()) {
//                        count++;
//                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                            Object obj = t.getObject(j);
//                        }
//                    }
//                    deletePreparedStatement.setInt(1, X + i);
//                }
//            }
//            //--
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "follower", 3, X + i));
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, X);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "follower", 3, X + i));
//            int count = 0;
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, X + i);
//                ResultSet t = prepareStatement.executeQuery();
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object obj = t.getObject(j);
//                    }
//                }
//                deletePreparedStatement.setInt(1, X + i);
//            }
//            System.out.println("testPostgres:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//            System.out.println(count);
//            other.executeUpdate("Delete from follower where follower_id >= " + X);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_RETWEET2_QUERY = "INSERT into retweet values";
        final String INSERT_RETWEET2_VALUE = "(%d, %d, '2992-02-21', %d)";
        final String INSERT_RETWEET1_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            connection.setAutoCommit(false);
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?");
            int MAX = 50000;
            int fanout = 277;
            int X = 1000000000;
            for(int j = 0; j < fanout; j++)
                other.executeUpdate(String.format(INSERT_RETWEET1_QUERY, -1, X + j, 336759));
            for(int i = 0; i < MAX; i++) {
                StringBuilder b = new StringBuilder(INSERT_RETWEET2_QUERY);
                for(int j = 0; j < fanout; j++) {
                    b.append(String.format(INSERT_RETWEET2_VALUE, X + i, 4 * X + i + j, X + j)).append(",");
                }
                b.deleteCharAt(b.length() - 1);
                other.executeUpdate(b.toString());
                System.out.println("Completed:" + i);
            }
            connection.commit();
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
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
                    TestUtil.iterate(pStatement.executeQuery(), 9);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id ");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(%d, -1)";
        final String INSERT_RETWEET2_QUERY = "INSERT into retweet values";
        final String INSERT_RETWEET2_VALUE = "(%d, %d, '2992-02-21', %d)";
        final String INSERT_RETWEET1_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            
            connection.setAutoCommit(false);
            int MAX = 50000;
            int fanout = 277;
            int X = 1000000000;
            for(int j = 0; j < fanout; j++)
                other.executeUpdate(String.format(INSERT_RETWEET1_QUERY, -1, X + j, 336759));
            for(int i = 0; i < MAX; i++) {
                StringBuilder b = new StringBuilder(INSERT_RETWEET2_QUERY);
                for(int j = 0; j < fanout; j++) {
                    b.append(String.format(INSERT_RETWEET2_VALUE, X + i, 4 * X + i + j, X + j)).append(",");
                }
                b.deleteCharAt(b.length() - 1);
                other.executeUpdate(b.toString());
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
                System.out.println("Completed:" + i);
            }
            connection.commit();
            
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 9);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 9);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id ");
            
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance(); 
    }
}
