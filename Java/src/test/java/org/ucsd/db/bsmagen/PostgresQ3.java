package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;

public class PostgresQ3 {
    private static final String INSERT_USER_QUERY = "INSERT into %s values(%d)";
    private static final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
    private static final String INSERT_RETWEET_QUERY = "INSERT into temp values(2, %d, '2992-02-21', 2)";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            int MAX = 500;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                int count = 0;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    ResultSet t = prepareStatement.executeQuery();
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object obj = t.getObject(j);
                        }
                    };
                }
                System.out.println("testH2Sql:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            }
            MAX = 5000;
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 2);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            int count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 2);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            System.out.println(count);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
//                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
//                    + ", C.tweet_date as r_tweet_date from users A, tweet B, retweet C "
//                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and A.user_id = ? "
//                    + " and C.tweet_id = ? and C.retweet_tweet_id = ? and C.tweet_date = '2992-02-21'");
//            Statement other = connection.createStatement() ){
//            int MAX = 50;
//            int X = 1000000000;
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "retweet"));
//            for(int k = 0, x = X; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                System.out.println(k);
//                other.executeUpdate(String.format(INSERT_USER_QUERY, "users", x));
//                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x, x));
//                for(int i = 0; i < MAX; i++)
//                    other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "retweet", x, x + i, x));
//                int count = 0;
//                long tm = System.nanoTime();
//                for(int i = 0; i < MAX; i++);
//                for(int i = 0; i < MAX; i++) {
//                    prepareStatement.setInt(1, x);
//                    prepareStatement.setInt(2, x + i);
//                    prepareStatement.setInt(3, x);
//                    ResultSet t = prepareStatement.executeQuery();
//                    while(t.next()) {
//                        count++;
//                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                            Object obj = t.getObject(j);
//                        }
//                    }
//                    deletePreparedStatement.setInt(1, X);
//                    deletePreparedStatement.setInt(2, X + i);
//                    deletePreparedStatement.executeUpdate();
//                }
//            }
//            //--
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "retweet", X, X + i, X));
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, X);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "retweet", X, X + i, X));
//            int count = 0;
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, X);
//                prepareStatement.setInt(2, X + i);
//                prepareStatement.setInt(3, X);
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//                deletePreparedStatement.setInt(1, X);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            System.out.println("testPostgres:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//            System.out.println(count);
//            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
//            other.executeUpdate("Delete from users where user_id >= " + X);
//            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into %s values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into temp values(%d, %d, '2992-02-21', %d)";
        final String UPDATE_RETWEET_QUERY = "UPDATE temp set user_id = %d, tweet_id = %d, retweet_tweet_id = %d";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            int count = 0;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "users", X + i));
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", X + i, X + i));
            }
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0, x = 0; k < 2; k++, x += MAX) {
                count = 0;
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, x, x + i, x));
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    }
                }
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            long d = System.nanoTime() - tm;
            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
                ResultSet t = prepareStatement.executeQuery(); 
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object object = t.getObject(j);
                    }
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            System.out.println("Count:" + count);
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
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table tempReTweet as select * from retweet limit 1;");
            other.executeUpdate("delete from tempReTweet");
            prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, tempRetweet C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 5000;
            int x = 1000000000;
            other.executeUpdate(String.format(INSERT_USER_QUERY, "users", x));
            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x, x));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "tempRetweet", x, x + i, x));
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                ResultSet t = prepareStatement.executeQuery();
                int count = 0;
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            int count = 0;
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            };
            System.out.println("testPostgres:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        } finally {
            if(prepareStatement != null)
                prepareStatement.close();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            PreparedStatement prepareStatement = 
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
            int count = 0;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            count = 0;
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            };
            System.out.println("testPostgres:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println("Count:" + count);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    /**
     * This method has to do exactly the same amount of work as insert*throughput method. Therefore, this method is not
     * correctly implemented and shouldn't be used 
     * @throws InstantiationException
     * @throws IllegalAccessException
     * @throws ClassNotFoundException
     */
    @Test
    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table tempRetweet as select * from retweet limit 1;");
            other.executeUpdate("delete from tempRetweet");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, A.follower_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from follower A, tweet B, tempRetweet C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50;
            int X = 1000000000;
            other.executeUpdate(String.format(INSERT_USER_QUERY, "follower", X));
            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", X, X));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "tempRetweet", X, X + i, X));
            int count = 0;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            count = 0;
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            };
            System.out.println("testPostgres:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from follower where follower_id = -1");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }
}
