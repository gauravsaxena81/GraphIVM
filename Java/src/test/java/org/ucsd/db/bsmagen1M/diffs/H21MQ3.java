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

public class H21MQ3 {
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("./bsmagen1m");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("./bsmagen1m");
    }
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.execute("drop table if exists tempRetweet");
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            other.executeUpdate("create temporary table tempRetweet as select * from retweet limit 0;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into tempRetweet values(2, ?, '2992-01-01', ?)");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into retweet values(2, ?, '2992-01-01', ?)");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    insertTempPs.setInt(1, 2);
                    insertTempPs.setInt(2, X + i);
                    insertTempPs.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    insertPs.setInt(1, 2);
                    insertPs.setInt(2, X + i);
                    insertPs.execute();
                    TestUtil.iterate(prepareStatement.executeQuery(), 7);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
                other.execute("delete from retweet where tweet_date = '2992-01-01'");
                other.execute("delete from tempRetweet where tweet_date = '2992-01-01'");
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            other.execute("delete from retweet where tweet_date = '2992-01-01'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into %s values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                "UPDATE temp set user_id = ?, tweet_id = ?, retweet_tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "users", X + i));
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", X + i, X + i));
            }
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3, X);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3, X);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(prepareStatement.executeQuery(), 7);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + X);
            other.execute("delete from retweet where tweet_date = '2992-01-01'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into %s values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21', %d)";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            
            prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "users", x + i));
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x + i, x + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", x, x + i, x));
            }
            long tm = System.nanoTime();
            for(int k = 0; k < 10; k++) {
                tm = System.nanoTime();
                TestUtil.iterate(prepareStatement.executeQuery(), 7);
                tm = (System.nanoTime() - tm + 0) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
            other.executeUpdate("Delete from retweet where tweet_date = '2992-01-01'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into temp values(2, ?, '2992-01-01', 2)");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into retweet values(2, ?, '2992-01-01', 2)");
            int MAX = 50000;
            int X = 1000000000;
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTempPs.setInt(1, X + i);
                insertTempPs.execute();
            }// time to insert without change in index
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertPs.setInt(1, X + i);
                insertPs.execute();
            }// time to insert with change in index
            d = System.nanoTime() - tm - d;
            for(int k = 0; k < 10; k++) {
                tm = System.nanoTime();
                TestUtil.iterate(prepareStatement.executeQuery(), 7);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            
            other.executeUpdate("Delete from retweet where tweet_date = '2992-01-01'");
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
    /*@Test
    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            other.executeUpdate("delete from temp");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, A.follower_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from follower A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 5000;
            int X = 1000000000;
            other.executeUpdate(String.format(INSERT_USER_QUERY, "follower", X));
            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", X, X));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", X, X + i, X));
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
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from follower where follower_id = -1");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }*/
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
