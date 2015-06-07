package org.ucsd.db.bsmagen1M;

import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
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
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
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
                    prepareStatement.execute(); 
                }
                tm = (System.nanoTime() - tm - d);
                System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
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
                prepareStatement.execute();
            }
            tm = (System.nanoTime() - tm - d);
            System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
            printViewCount(other);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, IOException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using temp B where f_user_id = B.user_id and t_user_id = B.user_id "
                                + " and A.tweet_id = B.tweet_id "
                                + " and  A.tweet_date = B.tweet_date");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                            "UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int x = 1000000000;
            for(int k = 0; k < 7; k++) {
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    prepareStatement.execute();
                }
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
                    int executeUpdate = dStatement.executeUpdate();
                }
                tm = (System.nanoTime() - tm - d);
                System.out.println(TestUtil.getTestName() + tm / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            
            printViewCount(other);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("create temporary table view as SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id limit 0;");
        other.executeUpdate("create index i_view on view(f_user_id, t_user_id, tweet_id, tweet_date)");
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

    private void printViewCount(Statement other) throws SQLException {
        ResultSet count = other.executeQuery("select count(*) from view");
        count.next();
        System.out.println(count.getInt(1));        
    }

    
    @Test
    public void deleteJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_USER_QUERY = "INSERT into users values(%d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using temp B where f_user_id = B.user_id and t_user_id = B.user_id "
                                + " and A.tweet_id = B.tweet_id "
                                + " and  A.tweet_date = B.tweet_date");
            PreparedStatement updatePreparedStatement = connection.prepareStatement(
                            "UPDATE temp set user_id = ?, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_USER_QUERY, X + i));
            other.execute("analyze users;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    prepareStatement.execute(); 
                }
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
                    dStatement.executeUpdate();                    
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
            }
            /*TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");*/
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
                prepareStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from users where user_id >= " + X);
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
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
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
                    prepareStatement.execute(); 
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            printViewCount(other);
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
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
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
                prepareStatement.execute();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            long tm = System.nanoTime();
            prepareStatement.execute();
            tm = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from users where user_id >=" + x);
        } catch(SQLException e) {
            e.printStackTrace();
        } finally {
            if(prepareStatement != null)
                prepareStatement.close();
        }
    }
    
    @Test
    public void deleteJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_USERS_QUERY = "INSERT into %s values(%d)";
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using temp B where f_user_id = B.user_id and t_user_id = B.user_id "
                                + " and A.tweet_id = B.tweet_id "
                                + " and  A.tweet_date = B.tweet_date");
            prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
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
                prepareStatement.execute();
                long tm = System.nanoTime();
                dStatement.executeUpdate();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            long tm = System.nanoTime();
            prepareStatement.execute();
            tm = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            printViewCount(other);
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
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, x + i));
            other.execute("analyze users;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                prepareStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            long tm = System.nanoTime();
            prepareStatement.execute();
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using temp B where f_user_id = B.user_id and t_user_id = B.user_id "
                        + " and A.tweet_id = B.tweet_id "
                        + " and  A.tweet_date = B.tweet_date");
            int MAX = 50000;
            int x = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, x + i));
            other.execute("analyze users;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                prepareStatement.execute();
                long tm = System.nanoTime();
                dStatement.executeUpdate();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, " explain SELECT A.user_id as f_user_id, B.user_id as t_user_id"
                        + ", tweet_id, tweet_date FROM users A, temp B WHERE A.user_id = B.user_id ");
            long tm = System.nanoTime();
            prepareStatement.execute();
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
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
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
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
        Class.forName("org.postgresql.Driver").newInstance();        
    }
}
