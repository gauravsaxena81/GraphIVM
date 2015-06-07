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
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists view");
        other.executeUpdate("create temporary table view as SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as count"
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id limit 0;");
        other.execute("create index i_view on view(user_id, tweet_id)");
    }
    private void printViewCount(Statement other) throws SQLException {
        ResultSet count = other.executeQuery("select count(*) from view");
        count.next();
        System.out.println(count.getInt(1));        
    }
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            createView(other);
//            PreparedStatement uStatement = connection.prepareStatement("update view A set count = "
//                            + "(select A.count + B.ct from (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct"
//                            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
//                            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
//                            + "where A.user_id = B.user_id and A.tweet_id = B.tweet_id);");
            PreparedStatement uStatement = connection.prepareStatement("update view set count = count + ?"
                + " where user_id = ? and tweet_id = ?;");
            PreparedStatement pStatement = 
                            connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 384827, follower_id = ?");
            int MAX = 50000;
            int X = 1000000000;
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
                    ResultSet q = pStatement.executeQuery();
                    while(q.next()) {
                        uStatement.setInt(1, q.getInt(3));
                        uStatement.setInt(2, q.getInt(1));
                        uStatement.setInt(3, q.getInt(2));
                        if(uStatement.executeUpdate() == 0)
                            iStatement.execute();
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set follower_id = follower_id + 1 limit 10");
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
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
                ResultSet q = pStatement.executeQuery();
                while(q.next()) {
                    uStatement.setInt(1, q.getInt(3));
                    uStatement.setInt(2, q.getInt(1));
                    uStatement.setInt(2, q.getInt(2));
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
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
        final String INSERT_TWEET_QUERY = "INSERT into tweet values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            createView(other);
//          PreparedStatement uStatement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct"
//          + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
//          + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
//          + "where A.user_id = B.user_id and A.tweet_id = B.tweet_id);");
            PreparedStatement uStatement = connection.prepareStatement("update view set count = "
            + "where A.user_id = ? and A.tweet_id = ?;");
            PreparedStatement pStatement = 
                      connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                      + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                      + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement iStatement = 
            connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            connection.setAutoCommit(true);
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?");
            int MAX = 5000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                //other.executeUpdate(String.format(INSERT_TWEET_QUERY, 2, X + i));
                //other.executeUpdate(String.format(INSERT_RETWEET_QUERY, 3, 2 * X + i, X + i));
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
                    ResultSet q = pStatement.executeQuery();
                    while(q.next()) {
                        uStatement.setInt(1, q.getInt(2));
                        uStatement.setInt(2, q.getInt(1));
                        if(uStatement.executeUpdate() == 0)
                            iStatement.execute();
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
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
                ResultSet q = pStatement.executeQuery();
                while(q.next()) {
                    uStatement.setInt(1, q.getInt(2));
                    uStatement.setInt(2, q.getInt(1));
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(%d, -1)";
        final String INSERT_TWEET_QUERY = "INSERT into tweet values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            createView(other);
//          PreparedStatement uStatement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct"
//          + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
//          + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
//          + "where A.user_id = B.user_id and A.tweet_id = B.tweet_id);");
            PreparedStatement uStatement = connection.prepareStatement("update view set count = "
            + "where user_id = ? and tweet_id = ?;");
            PreparedStatement pStatement = 
                      connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                      + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                      + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement iStatement = 
            connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 5000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                //other.executeUpdate(String.format(INSERT_TWEET_QUERY, 2, X + i));
                //other.executeUpdate(String.format(INSERT_RETWEET_QUERY, 3, 2 * X + i, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 1175996));
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            }
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                ResultSet q = pStatement.executeQuery();
                while(q.next()) {
                    uStatement.setInt(1, q.getInt(2));
                    uStatement.setInt(2, q.getInt(1));
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set follower_id = follower_id + 1 limit 10");
                other.execute("delete from view;");
            }
//            TestUtil.printPlan(other, " explain insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
//                            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
//                            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
            printViewCount(other);
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
            createView(other);
//          PreparedStatement uStatement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct"
//          + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
//          + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
//          + "where A.user_id = B.user_id and A.tweet_id = B.tweet_id);");
            PreparedStatement uStatement = connection.prepareStatement("update view set count = count + ? "
            + " where user_id = ? and tweet_id = ?;");
            PreparedStatement pStatement = 
                  connection.prepareStatement("SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                  + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                  + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                ResultSet q = pStatement.executeQuery();
                while(q.next()) {
                    uStatement.setInt(1, q.getInt(3));
                    uStatement.setInt(2, q.getInt(1));
                    uStatement.setInt(3, q.getInt(2));
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set follower_id = follower_id + 1 limit 10");
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, " explain insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
            long tm = System.nanoTime();
            ResultSet q = pStatement.executeQuery();
            while(q.next()) {
                uStatement.setInt(1, q.getInt(3));
                uStatement.setInt(2, q.getInt(1));
                uStatement.setInt(3, q.getInt(2));
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
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
//    @Test
//    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
//            Statement other = connection.createStatement() ){
//            other.executeUpdate("create temporary table tempFollower as select * from follower limit 0;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
//                + " FROM tweet A, retweet B, retweet C, temp D"
//                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id");
//            int MAX = 50;
//            int X = 1000000000;
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "tempFollower", 3, X + i));
//            int count = 0;
//            for(int k = 0; k < 15; k++) {
//                count = 0;
//                long tm = System.nanoTime();
//                ResultSet t = prepareStatement.executeQuery();
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object obj = t.getObject(j);
//                    }
//                }
//                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
//            }
//            count = 0;
//            long tm = System.nanoTime();
//            ResultSet t = prepareStatement.executeQuery();
//            while(t.next()) {
//                count++;
//                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                    Object obj = t.getObject(j);
//                }
//            };
//            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
//            System.out.println(count);
//            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
//            other.executeUpdate("Delete from follower where follower_id = -1");
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance(); 
    }
}
