package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;

public class PostgresQ1Aggregation {
    private static final String INSERT_QUERY = "INSERT into temp values(2, %d, '2992-02-21')";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id, count(B.user_id) "
                                + " FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            int MAX = 5000;
            int x = 1000000000;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                int count = 0;
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    for(int j = 0; j < 5; j++)
                        other.executeUpdate(String.format(INSERT_QUERY, x + i + j));
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    }
                    other.executeUpdate("DELETE from temp");
                }
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_QUERY, x + i));
                other.executeUpdate("DELETE from temp");
            }
            long d = System.nanoTime() - tm;
            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);   
            int count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                for(int j = 0; j < 5; j++)
                    other.executeUpdate(String.format(INSERT_QUERY, x + i + j));
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
                other.executeUpdate("DELETE from temp");
            }
            System.out.println("insertProjectedTupleTest:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            System.out.println(count);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
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
//            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
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
        final String INSERT_TWEET_QUERY = "INSERT into temp values(%d, %d, '2992-02-21')";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, count(B.user_id) "
                                + " FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            int MAX = 5000;
            int X = 1000000000;
            int count = 0;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USER_QUERY, X + i));
            for(int k = 0, x = X; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
                count = 0;
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(INSERT_TWEET_QUERY, X + i, x + i));
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    }
                    other.executeUpdate("DELETE from temp");
                }
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, X + i, X + i));
                other.executeUpdate("DELETE from temp");
            }
            long d = System.nanoTime() - tm;
            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, X + i, X + i));
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
                other.executeUpdate("DELETE from temp");
            }
            System.out.println("insertJoinTuples:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from users where user_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_USERS_QUERY = "INSERT into users values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into tempTweet values(%d, %d, '2992-02-21')";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table tempTweet as select * from tweet limit 0;");
            prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, count(B.user_id) "
                                + " FROM users A, tempTweet B WHERE A.user_id = B.user_id group by A.user_id");
            int MAX = 5000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_USERS_QUERY, x + i));
                for(int j = 0; j < 5; j++)
                    other.executeUpdate(String.format(INSERT_TWEET_QUERY, x + i, x + i + j));
            }
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
            System.out.println("insertJoinTuplesThroughput:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from users where user_id >=" + x);
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id"
                    + ", count(B.user_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            int MAX = 5000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, x + i));
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                long tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            int count = 0;
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            }
            System.out.println("insertProjectedTupleThroughputTest:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
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
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
//            Statement other = connection.createStatement() ){
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
//            System.out.println("testH2Sql:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
