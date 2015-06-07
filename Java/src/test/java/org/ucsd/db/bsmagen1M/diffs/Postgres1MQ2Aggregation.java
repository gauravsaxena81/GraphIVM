package org.ucsd.db.bsmagen1M.diffs;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ2Aggregation {
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 100, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
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
                    TestUtil.iterate(pStatement.executeQuery(), 2);
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                TestUtil.printPlan(other, "explain SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id;");
            }
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into follower values(%d, %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            int MAX = 50000;
            final int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 3; j++)
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
            TestUtil.printPlan(other, "explain SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id;");
            
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
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 3; j++)
                    other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, x + i, j)); 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, x + i, x + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                TestUtil.printPlan(other, "explain SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id;");
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
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int X = 1000000000; 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                TestUtil.printPlan(other, "explain SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id;");
            }
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();           
    }
}
