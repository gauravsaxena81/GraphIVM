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

public class H21MQ2Aggregation {
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                , "sa", ""); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            
            PreparedStatement pStatement =  connection.prepareStatement("SELECT follower_id, count(C.user_id) FROM "
                            + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 100, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 15; k++) {
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
                    TestUtil.iterate(pStatement.executeQuery(), 2);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT follower_id, count(C.user_id) FROM "
                            + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
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
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.execute();
                TestUtil.iterate(pStatement.executeQuery(), 2);
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.follower_id, follower_id, B.user_id as t_user_id"
//                + ", tweet_id, tweet_date FROM follower A, tweet B WHERE A.user_id = B.user_id "
//                + "and tweet_id = ? and tweet_date = '2992-02-21' and A.user_id = 2");
//            Statement other = connection.createStatement() ){
//            int MAX = 5000;
//            final int X = 1000000000;
//            int count = 0;
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(DELETE_QUERY);
//            for(int k = 0, x = 1000000; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                for(int i = 0; i < MAX; i++) 
//                    other.executeUpdate(String.format(INSERT_QUERY, x + i));
//                count = 0;
//                long tm = System.nanoTime();
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
//                    deletePreparedStatement.setInt(1, 2);
//                    deletePreparedStatement.setInt(2, x + i);
//                    deletePreparedStatement.setString(3, "2992-02-21");
//                    deletePreparedStatement.executeUpdate();
//                }
//            }
//            deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "tweet"));            
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, X + i));
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, 2);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.setString(3, "2992-02-21");
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++) 
//                other.executeUpdate(String.format(INSERT_QUERY, X + i));
//            count = 0;
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, X + i);
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//                deletePreparedStatement.setInt(1, 2);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.setString(3, "2992-02-21");
//                deletePreparedStatement.executeUpdate();
//            }
//            System.out.println("testH2Sql:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//            System.out.println(count);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into follower values(%d, %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            
//          PreparedStatement u1Statement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (SELECT follower_id, count(C.user_id) as ct FROM "
//              + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
//          + "where A.follower_id = B.follower_id);");
              PreparedStatement pStatement =  connection.prepareStatement("SELECT follower_id, count(C.user_id) FROM "
                              + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            int MAX = 50000;
            final int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 5; j++)
                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, x + i, j)); 
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    TestUtil.iterate(pStatement.executeQuery(), 2);
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT follower_id, count(C.user_id) FROM "
                      + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            other.executeUpdate("Delete from follower where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_TWEET_QUERY = "INSERT into temp values(%d, %d, '2992-02-21')";
        final String INSERT_FOLLOWER_QUERY = "INSERT into follower values(%d, %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
//          PreparedStatement u1Statement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (SELECT follower_id, count(C.user_id) as ct FROM "
//              + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
//          + "where A.follower_id = B.follower_id);");
          PreparedStatement pStatement =  connection.prepareStatement("SELECT follower_id, count(C.user_id) FROM "
                          + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 5; j++)
                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, x + i, j)); 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, x + i, x + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set tweet_id = tweet_id + 1 limit 10");
                
            }
            other.executeUpdate("Delete from follower where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        String INSERT_QUERY = "INSERT into temp values(100, %d, '2992-02-21')";
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;"
                        , "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
//          PreparedStatement u1Statement = connection.prepareStatement("update view A set count = "
//          + "(select A.count + B.ct from (SELECT follower_id, count(C.user_id) as ct FROM "
//              + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
//          + "where A.follower_id = B.follower_id);");
      PreparedStatement pStatement =  connection.prepareStatement("SELECT follower_id, count(C.user_id) FROM "
                      + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int X = 1000000000; 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, X + i));
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                connection.createStatement().execute("update temp set tweet_id = tweet_id + 1 limit 10");
                
            }
            
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
//            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.follower_id, follower_id, B.user_id as t_user_id"
//                                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id");
//            int MAX = 5000;
//            for(int k = 0, x = 1000000; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                for(int i = 0; i < MAX; i++) {
//                    other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//                }
//            }
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "temp"));
//            for(int k = 0, x = 1000000; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                for(int i = 0; i < MAX; i++) {
//                    prepareStatement.setInt(1, 2);
//                    prepareStatement.setInt(2, x + i);
//                    prepareStatement.setString(2, "2992-02-21");
//                    ResultSet t = prepareStatement.executeQuery(); 
//                    while(t.next()) {
//                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                            Object object = t.getObject(j);
//                        }
//                    };
//                    deletePreparedStatement.setInt(1, 2);
//                    deletePreparedStatement.setInt(2, x + i);
//                    deletePreparedStatement.setString(3, "2992-02-21");
//                    deletePreparedStatement.executeUpdate();
//                }
//            }
//            deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "tweet"));            
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "temp", 1000000 + i));
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, 2);
//                deletePreparedStatement.setInt(2, 1000000 + i);
//                deletePreparedStatement.setString(3, "2992-02-21");
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "temp", 1000000 + i));
//            }
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, 2);
//                prepareStatement.setInt(2, 1000000 + i);
//                prepareStatement.setString(2, "2992-02-21");
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//                deletePreparedStatement.setInt(1, 2);
//                deletePreparedStatement.setInt(2, 1000000 + i);
//                deletePreparedStatement.setString(3, "2992-02-21");
//                deletePreparedStatement.executeUpdate();
//            }
//            System.out.println("testH2Sql:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
