package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;

public class PostgresQ2 {
    
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            int MAX = 5000;
            int X = 1000000000;
            int count = 0;
            for(int k = 0; k < 8; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 100);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                count = 0;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 100);
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
            MAX = 50000;
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 100);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 100);
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
            System.out.println(count);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
//                + ", tweet_id, tweet_date FROM follower A, tweet B WHERE A.user_id = B.user_id "
//                + "and tweet_id = ? and tweet_date = '2992-02-21' and A.user_id = 2");
//            Statement other = connection.createStatement() ){
//            int MAX = 50;
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
//            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
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
//            System.out.println("testPostgres:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//            System.out.println(count);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into follower values(%d, %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            int MAX = 50000;
            final int x = 1000000000;
            int count = 0;
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 42; j++)
                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, x + i, j)); 
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                count = 0;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    ResultSet t = prepareStatement.executeQuery();
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object obj = t.getObject(j);
                        }
                    }
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, x + i);
                updatePreparedStatement.setInt(2, x + i);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, x + i);
                updatePreparedStatement.setInt(2, x + i);
                updatePreparedStatement.execute();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            tm = (System.nanoTime() - tm - d) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            System.out.println(count);
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
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id");
            int MAX = 50000;
            int x = 1000000000;
            int count = 0;
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 42; j++)
                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, x + i, j)); 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, x + i, x + i));
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            for(int k = 0; k < 2; k++) {
                count = 0;
                long tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object object = t.getObject(j);
                    }
                };
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            count = 0;
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            }
            tm =  (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            System.out.println("Count:" + count);
            other.executeUpdate("Delete from follower where user_id >= " + x);
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
        String INSERT_QUERY = "INSERT into temp values(100, %d, '2992-02-21')";
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id"
                    + ", B.user_id, B.tweet_id, B.tweet_date as t_user_id, tweet_id, tweet_date FROM follower A, temp B "
                    + "WHERE A.user_id = B.user_id");
            int MAX = 50000;
            int X = 1000000000; 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, X + i));
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                int count = 0;
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id; ");
            int count = 0;
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            }
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
//    @Test
//    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
//            Statement other = connection.createStatement() ){
//            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
//                                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id");
//            int MAX = 50;
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
//            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
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
//            System.out.println("testPostgres:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();   
    }
}
