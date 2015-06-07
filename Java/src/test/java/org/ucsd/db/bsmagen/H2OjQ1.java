package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Types;

import org.junit.Test;

public class H2OjQ1 {
    private static final String INSERT_QUERY = "INSERT into %s values(?, ?, ?, '2992-02-21')";
    private static final String DELETE_QUERY = "DELETE FROM %s WHERE f_user_id = 1 and tweet_id = ? and tweet_date = '2992-02-21'";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", ""); 
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT f_user_id, follower_id, tweet_id, tweet_date FROM follower_tweet_oj "
                    + "WHERE f_user_id = 1 and follower_id = 0 and tweet_id = ? and tweet_date = '2992-02-21'");
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table tempOj as select * from follower_tweet_oj limit 0;");
            other.executeUpdate("create temporary table tempTweet as select * from tweet limit 0");
            PreparedStatement insertPreparedStatement = connection.prepareStatement(String.format(INSERT_QUERY, "tempOj"));
            int MAX = 5000;
            int x = 1000000000;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                for(int i = 0; i < MAX; i++) {
                    insertPreparedStatement.setInt(1, x + i);
                    insertPreparedStatement.executeUpdate();
                    prepareStatement.setInt(1, x + i);
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    };
                }
            }
            insertPreparedStatement = connection.prepareStatement(String.format(INSERT_QUERY, "follower_tweet_oj"));
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertPreparedStatement.setInt(1, x + i);
                insertPreparedStatement.executeUpdate();
                prepareStatement.setInt(1, x + i);
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            System.out.println("testH2Sql:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            other.executeUpdate("Delete from follower_tweet_oj where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertProjectedTupleTest1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", ""); 
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT f_user_id, follower_id, tweet_id, tweet_date FROM follower_tweet_oj "
                    + "WHERE f_user_id = 1 and follower_id = 0 and tweet_id = ? and tweet_date = '2992-02-21'");
            PreparedStatement insertStatement = 
                connection.prepareStatement("INSERT into follower_tweet_oj values(?, ?, ?, '2992-02-21')");
            Statement other = connection.createStatement()) {
            final int X = 1000000000;
            final int MAX = 5000;
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                ResultSet r = other.executeQuery("SELECT distinct f_user_id, follower_id from follower_tweet_oj where f_user_id = 2");
                boolean isEmpty = true;
                while(r.next()) {
                    isEmpty = false;
                    insertStatement.setNull(1, r.getInt(1));
                    insertStatement.setNull(2, r.getInt(2));
                    insertStatement.setInt(3, X);
                    insertStatement.execute();
                }
                if(isEmpty) {
                    insertStatement.setNull(1, Types.INTEGER);
                    insertStatement.setNull(2, Types.INTEGER);
                    insertStatement.setInt(3, X);
                    insertStatement.execute();
                }
            }
            System.out.println("insertProjectedTupleTest1:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            other.executeUpdate("delete from follower_tweet_oj where tweet_date = '2992-02-21'");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", ""); 
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT f_user_id, follower_id, tweet_id, tweet_date FROM follower_tweet_oj "
                + "WHERE f_user_id = 1 and follower_id = 0 and tweet_id = ? and tweet_date = '2992-02-21'");
            Statement other = connection.createStatement() ) {
            other.executeUpdate("create temporary table temp_follower_tweet_oj as select * from follower_tweet_oj limit 1;");
            int MAX = 5000;
            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "temp_follower_tweet_oj"));
            PreparedStatement insertPreparedStatement = connection.prepareStatement(String.format(INSERT_QUERY, "temp_follower_tweet_oj"));
            int X = 1000000000;
            int count = 0;
            for(int k = 0, x = X; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
                for(int i = 0; i < MAX; i++) {
                    insertPreparedStatement.setInt(1, X + i);
                    insertPreparedStatement.execute();
                }
                long tm = System.nanoTime();
                count = 0;
                for(int i = 0; i < MAX; i++) {
                    count++;
                    prepareStatement.setInt(1, X + i);
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    };
                    deletePreparedStatement.setInt(1, X + i);
                    deletePreparedStatement.executeUpdate();
                }
            }
            deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "follower_tweet_oj"));
            insertPreparedStatement = connection.prepareStatement(String.format(INSERT_QUERY, "follower_tweet_oj"));
            for(int i = 0; i < MAX; i++) {
                insertPreparedStatement.setInt(1, X + i);
                insertPreparedStatement.execute();
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                count++;
                prepareStatement.setInt(1, X + i);
                ResultSet t = prepareStatement.executeQuery(); 
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object object = t.getObject(j);
                    }
                };
                deletePreparedStatement.setInt(1, X + i);
                deletePreparedStatement.executeUpdate();
            }
            System.out.println("testH2Sql:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into %s values(?, -1, null, null)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", ""); 
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT f_user_id, follower_id, tweet_id, tweet_date FROM follower_tweet_oj "
                    + "WHERE f_user_id = ? and follower_id = -1");
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower_tweet_oj limit 1;");
            int MAX = 5000;
            int x = 1000000000;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                for(int i = 0; i < MAX; i++)
                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "temp", x + i));
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(INSERT_TWEET_QUERY, "temp", x + i, x + i));
                    prepareStatement.setInt(1, 100000000 + i);
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    };
                }
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x + i, x + i));
            }
            long d = System.nanoTime() - tm;
            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
            other.execute("Delete from tweet where tweet_date = '2992-02-21'");
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "follower", x + i));
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                prepareStatement.setInt(1, x + i);
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x + i, x + i));
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            System.out.println("testH2Sql:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from follower where follower_id = -1");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into %s values(%d, -1)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table tempTweet as select * from tweet limit 1;");
            prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, tempTweet B WHERE A.user_id = B.user_id");
            int MAX = 5000;
            int x = 1000000000;
            int count = 0;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                ResultSet t = prepareStatement.executeQuery();
                count = 0;
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                };
            }
            other.executeUpdate("DELETE from tempTweet");
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "follower", x + i));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tempTweet", x + i, x + i));
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            count = 0;
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            };
            System.out.println("testH2Sql:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from follower where follower_id = -1");
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id"
                    + ", B.user_id, B.tweet_id, B.tweet_date as t_user_id, tweet_id, tweet_date FROM follower A, temp B "
                    + "WHERE A.user_id = B.user_id");
            int MAX = 5000;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                for(int i = 0; i < MAX; i++)
                    other.executeUpdate(String.format(INSERT_QUERY, "temp", 100000000 + i));
                long tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            other.execute("Delete from temp;");
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, "temp", 100000000 + i));
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            int count = 0;
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            }
            System.out.println("testH2Sql:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
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
    @Test
    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id");
            int MAX = 5000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_QUERY, "temp", x + i));
            }
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
                long tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery(); 
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object object = t.getObject(j);
                    }
                };
            }
            other.executeUpdate("Delete from temp");
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_QUERY, "temp", 1000000 + i));
            }
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery(); 
            while(t.next()) {
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object object = t.getObject(j);
                }
            };
            System.out.println("testH2Sql:" + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
