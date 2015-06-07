package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class H21MQ3 {
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("~/bsmagen");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("~/bsmagen");
    }
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists view");
        other.executeUpdate("create temporary table view as select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id limit 0;");        
    }
    private void printViewCount(Statement other) throws SQLException {
        ResultSet count = other.executeQuery("select count(*) from view");
        count.next();
        System.out.println(count.getInt(1));        
    }
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.execute("drop table if exists tempRetweet");
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            other.executeUpdate("create temporary table tempRetweet as select * from retweet limit 0;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
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
                    prepareStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
                other.execute("delete from retweet where tweet_date = '2992-01-01'");
                other.execute("delete from tempRetweet where tweet_date = '2992-01-01'");
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
                prepareStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.execute("delete from retweet where tweet_date = '2992-01-01'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
//                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
//                    + ", C.tweet_date as r_tweet_date from users A, tweet B, retweet C "
//                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and A.user_id = ? "
//                    + " and C.tweet_id = ? and C.retweet_tweet_id = ? and C.tweet_date = '2992-02-21'");
//            Statement other = connection.createStatement() ){
//            int MAX = 5000;
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
//            System.out.println(TestUtil.getTestName() + (d) / 1000000 + "/" + MAX);
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
//            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
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
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
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
                    prepareStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
                prepareStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into temp values(?, ?, '2992-01-01', ?)");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into retweet(user_id, tweet_id, tweet_date, retweet_tweet_id) values(?, ?, '2992-01-01', ?)");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "users", x + i));
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x + i, x + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", x, x + i, x));
            }
            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                insertTempPs.setInt(1, x);
//                insertTempPs.setInt(2, x + i);
//                insertTempPs.setInt(3, x);
//                insertTempPs.execute();
//            }// time to insert without change in index
//            long d = System.nanoTime() - tm;
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                insertPs.setInt(1, x);
//                insertPs.setInt(2, x + i);
//                insertPs.setInt(3, x);
//                insertPs.execute();
//            }// time to insert with change in index
//            d = System.nanoTime() - tm - d;
            for(int k = 0; k < 10; k++) {
                tm = System.nanoTime();
                prepareStatement.execute();
                tm = (System.nanoTime() - tm + 0) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            tm = System.nanoTime();
            prepareStatement.execute();
            tm = (System.nanoTime() - tm + 0) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
            other.executeUpdate("Delete from retweet where tweet_date = '2992-01-01'");
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
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
                prepareStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain select A.user_id as f_user_id, B.user_id as t_user_id"
                + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            tm = System.nanoTime();
            prepareStatement.execute();
            tm = (System.nanoTime() - tm + d) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            printViewCount(other);
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0", "sa", ""); 
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
