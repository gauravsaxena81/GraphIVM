package org.ucsd.db.bsmagen1M.diffs;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

/**
 * 
 * Turned auto analyze off
 * Analyzed temp tables before executing queries
 * used prepared statements
 * increased buffer pool size to 2G
 * increased cache size to 1G
 * Made sure plans used index join for eager case
 * Made sure there were no disk reads
 * @author gaurav
 *
 */
public class Postgres1MQ1 {
    private static final String INSERT_QUERY = "INSERT into temp values(2, %d, '2992-02-21');";
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, IOException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                            "UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int x = 1000000000;
            for(int k = 0; k < 7; k++) {
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
                    TestUtil.iterate(prepareStatement.executeQuery(), 4); 
                }
                tm = (System.nanoTime() - tm - d);
                System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
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

    @Test
    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, IOException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                            "UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int x = 1000000000;
            for(int k = 0; k < 10; k++) {
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
                    TestUtil.iterate(prepareStatement.executeQuery(), 4); 
                }
                tm = (System.nanoTime() - tm - d);
                System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_USER_QUERY = "INSERT into users values(%d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                            "UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USER_QUERY, X + i));
            other.execute("analyze users;");
            other.execute("analyze temp;");
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
                    TestUtil.iterate(prepareStatement.executeQuery(), 4); 
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            
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
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
            PreparedStatement prepareStatement = connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            other.execute("analyze users;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USERS_QUERY, "users", x + i));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "temp", x + i, x + i));
            other.execute("analyze users;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(prepareStatement.executeQuery(), 4);
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            other.executeUpdate("Delete from users where user_id >=" + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, x + i));
            other.execute("analyze users;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(prepareStatement.executeQuery(), 4);
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            
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
    
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }
}
