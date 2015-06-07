package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

/**
 * Tuning: QUERY_CACHE_SIZE=0, CACHE_SIZE=0 because we do not want H2 to return cached result and actually run the query
 * ANALYZE_AUTO=0: because we do not want H2 to spend time updating analyze because we do not do it either
 * Output plans to make sure Index join is being used
 * Run in embedded mode to make sure TCP overhead is the least
 * Run SELECT 1 query to find overhead of Connecting and disconnecting
 * Used In-memory mode explicitly
 * @author gaurav
 *
 */
public class H2Q1 {
    public static void main(String[] args) throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        setUp();
        new H2Q1().insertProjectedTupleTest();
        destruct();
    }
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("~/bsmagen");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("~/bsmagen");
    }
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0;", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.execute("drop table if exists tempTweet");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            other.executeUpdate("create temporary table tempTweet as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                            "UPDATE temp set user_id = ?, tweet_id = ?");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into tempTweet values(2, ?, '2992-01-01')");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into tweet values(2, ?, '2992-01-01')");
            int MAX = 50000;
            int x = 1000000000;
            int count = 0;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    insertTempPs.setInt(1, x + i);
                    insertTempPs.execute();
                }
                long d = System.nanoTime() - tm;
                count = 0;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    insertPs.setInt(1, x + i);
                    insertPs.execute();
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    }
                }
                tm = (System.nanoTime() - tm - d);
                System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
                other.execute("delete from tempTweet where tweet_date = '2992-01-01'");
                other.execute("delete from tweet where tweet_date = '2992-01-01'");
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 2);
                updatePreparedStatement.setInt(2, x + i);
                updatePreparedStatement.execute();
                insertTempPs.setInt(1, x + i);
                insertTempPs.execute();
            }
            long d = System.nanoTime() - tm;
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 2);
                updatePreparedStatement.setInt(2, x + i);
                updatePreparedStatement.execute();
                insertPs.setInt(1, x + i);
                insertPs.execute();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            tm = (System.nanoTime() - tm - d);
            System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
            System.out.println(count);
            other.execute("delete from tweet where tweet_date = '2992-01-01'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
//                    + ", tweet_id, tweet_date FROM users A, tweet B WHERE A.user_id = B.user_id and B.user_id = 1 "
//                    + "and tweet_id = ? and tweet_date = '2992-02-21'");
//            Statement other = connection.createStatement() ) {
//            int MAX = 5000;
//            int x = 1000000000;
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "tweet"));
//            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
//                for(int i = 0; i < MAX; i++) {
//                    other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//                }
//                long tm = System.nanoTime();
//                count = 0;
//                for(int i = 0; i < MAX; i++);
//                for(int i = 0; i < MAX; i++) {
//                    prepareStatement.setInt(1, x + i);
//                    ResultSet t = prepareStatement.executeQuery(); 
//                    while(t.next()) {
//                        count++;
//                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                            Object object = t.getObject(j);
//                        }
//                    };
//                    deletePreparedStatement.setInt(1, x + i);
//                    deletePreparedStatement.executeUpdate();
//                }
//            }
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, x + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//            }
//            tm = System.nanoTime();
//            count = 0;
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, x + i);
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//                deletePreparedStatement.setInt(1, x + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            System.out.println("deleteProjectedTupleTest:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//            System.out.println(count);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_USER_QUERY = "INSERT into users values(%d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.execute("drop table if exists tempTweet");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            other.executeUpdate("create temporary table tempTweet as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into tempTweet values(2, ?, '2992-01-01')");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into tweet values(2, ?, '2992-01-01')");
            int MAX = 50000;
            int X = 1000000000;
            int count = 0;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USER_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    insertTempPs.setInt(1, X + i);
                    insertTempPs.execute();
                }
                long d = System.nanoTime() - tm;
                count = 0;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    insertPs.setInt(1, X + i);
                    insertPs.execute();
                    ResultSet t = prepareStatement.executeQuery();
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object obj = t.getObject(j);
                        }
                    };
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("delete from tempTweet where tweet_date = '2992-01-01'");
                other.execute("delete from tweet where tweet_date = '2992-01-01'");
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
                insertTempPs.setInt(1, X + i);
                insertTempPs.execute();
            }
            long d = System.nanoTime() - tm;
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
                insertPs.setInt(1, X + i);
                insertPs.execute();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            tm = (System.nanoTime() - tm - d) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from users where user_id >= " + X);
            other.execute("delete from tweet where tweet_date = '2992-01-01'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_USERS_QUERY = "INSERT into %s values(%d)";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=1048576;ANALYZE_AUTO=0", "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into temp values(?, ?, '2992-01-01')");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into tweet values(?, ?, '2992-01-01')");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USERS_QUERY, "users", x + i));
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTempPs.setInt(1, x + i);
                insertTempPs.setInt(2, x + i);
                insertTempPs.execute();
            }// time to insert without change in index
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertPs.setInt(1, x + i);
                insertPs.setInt(2, x + i);
                insertPs.execute();
            }// time to insert with change in index
            d = System.nanoTime() - tm - d;
            int count;
            for(int k = 0; k < 10; k++) {
                count = 0;
                tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            count = 0;
            tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            };
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from users where user_id >=" + x);
            other.execute("delete from tweet where tweet_date = '2992-01-01'");
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=1048576;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id"
                    + ", B.user_id, B.tweet_id, B.tweet_date as t_user_id, tweet_id, tweet_date FROM users A, temp B "
                    + "WHERE A.user_id = B.user_id");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into temp values(2, ?, '2992-01-01')");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into tweet values(2, ?, '2992-01-01')");
            int MAX = 50000;
            int x = 1000000000;
            int count = 0;
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTempPs.setInt(1, x + i);
                insertTempPs.execute();
            }// time to insert without change in index
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertPs.setInt(1, x + i);
                insertPs.execute();
            }// time to insert with change in index
            d = System.nanoTime() - tm - d;
            for(int k = 0; k < 10; k++) {
                count = 0;
                tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            count = 0;
            tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm + d) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
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
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
//            Statement other = connection.createStatement() ){
//            other.executeUpdate("drop table if exists temp");
//            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
//                                + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
//            int MAX = 5000;
//            int x = 1000000000;
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "temp", x + i));
//            }
//            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                long tm = System.nanoTime();
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//            }
//            long tm = System.nanoTime();
//            ResultSet t = prepareStatement.executeQuery(); 
//            while(t.next()) {
//                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                    Object object = t.getObject(j);
//                }
//            };
//            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
