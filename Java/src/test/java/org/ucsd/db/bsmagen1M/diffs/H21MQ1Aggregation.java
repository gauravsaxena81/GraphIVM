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

public class H21MQ1Aggregation {
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("./bsmagen1m");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("./bsmagen1m");
    }
    private static final String INSERT_QUERY = "INSERT into temp values(2, %d, '2992-02-21');";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement()) {
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement pStatement = connection.prepareStatement("SELECT A.user_id"
                + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                "UPDATE temp set user_id = ?, tweet_id = ?");
            int MAX = 50000;
            int x = 1000000000;
            for(int k = 0; k < 15; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(pStatement.executeQuery(), 2);
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm  + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id, count(B.tweet_id) FROM users A, temp B "
                        + " WHERE A.user_id = B.user_id group by A.user_id");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    /* Postgres Function ~ 5933 - 752 ~= 5200ms
     --create temporary table temp as select * from tweet limit 0;      
CREATE OR REPLACE FUNCTION insertProjectedTuples() RETURNS TABLE(tms TIMESTAMP, tme TIMESTAMP) AS $$
    BEGIN
      for i in 100000000..100005000 LOOP
        INSERT into temp values(1, i, '2992-02-21');
        PERFORM A.user_id as f_user_id, B.user_id as t_user_id
               , tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id;
        delete from temp;
      end LOOP;
      return query SELECT tms, tme;
    END;
    $$ LANGUAGE plpgsql;
    
    select insertProjectedTuples();
    --select count(*) from temp;
     */

    //    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;", "sa", ""); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
//                    + ", tweet_id, tweet_date FROM users A, tweet B WHERE A.user_id = B.user_id and B.user_id = 1 "
//                    + "and tweet_id = ? and tweet_date = '2992-02-21'");
//            Statement other = connection.createStatement() ) {
//            int MAX = 50;
//            int x = 1000000000;
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "tweet"));
//            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
//                for(int i = 0; i < MAX; i++) {
//                    other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//                }
//                long tm = System.nanoTime();
//                int count = 0;
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
//            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//            }
//            tm = System.nanoTime();
//            int count = 0;
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
//          PreparedStatement uStatement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (select user_id, count(tweet_id) as ct from temp group by user_id) B "
//          + "where A.user_id = B.user_id);");
          PreparedStatement pStatement = connection.prepareStatement("SELECT A.user_id"
              + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
          PreparedStatement updatePreparedStatement = connection.prepareStatement(
              "UPDATE temp set user_id = ?, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USER_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(pStatement.executeQuery(), 2);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
            }
            other.executeUpdate("Delete from users where user_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_USERS_QUERY = "INSERT into %s values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;", "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
          PreparedStatement pStatement = connection.prepareStatement("SELECT A.user_id"
              + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USERS_QUERY, "users", x + i));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "temp", x + i, x + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                connection.createStatement().execute("update temp set tweet_id = tweet_id + 1 limit 10");
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id, count(B.tweet_id) FROM users A, temp B "
                        + " WHERE A.user_id = B.user_id group by A.user_id");
            other.executeUpdate("Delete from users where user_id >=" + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
          PreparedStatement pStatement = connection.prepareStatement("SELECT A.user_id"
              + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, x + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set tweet_id = tweet_id + 1 limit 10");
                
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id"
              + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id ");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    /* POSTGRES function takes ~55ms/5000tuples
    CREATE OR REPLACE FUNCTION insertProjectedTuplesThroughput() RETURNS TABLE(tms TIMESTAMP, tme TIMESTAMP) AS $$
   BEGIN
     tms := localtimestamp;
     IF NOT EXISTS (SELECT 1 FROM pg_class WHERE relname = 'temp')
     THEN
           create temporary table temp as select * from tweet limit 0;      
     END IF;
     
     for i in 100000000..100005000 LOOP
       INSERT into temp values(1, i, '2992-02-21');
     end LOOP;
     PERFORM A.user_id as f_user_id, B.user_id as t_user_id
              , tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id;
     delete from temp;
     tme := localtimestamp;
     return query SELECT tms, tme;
   END;
   $$ LANGUAGE plpgsql;
    */
    
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
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;", "sa", ""); 
//            Statement other = connection.createStatement() ){
//            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
//                                + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
//            int MAX = 50;
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
//                }
//                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
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
