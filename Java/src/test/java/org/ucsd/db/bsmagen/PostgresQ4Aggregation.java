package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;

public class PostgresQ4Aggregation {
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("drop table if exists temp");
            other.execute("drop table if exists tempFollower");
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            other.executeUpdate("create temporary table tempFollower as select * from follower limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
                    + "FROM tweet A, retweet B, retweet C, temp D"
                    + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                    + " and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 17, follower_id = ?");
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            int MAX = 5000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                int count = 0;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                    ResultSet t = prepareStatement.executeQuery();
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object obj = t.getObject(j);
                        }
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from follower where follower_id >= " + X);
                other.execute("delete from tempFollower where follower_id >= " + X);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
                    + "FROM tweet A, retweet B, retweet C, temp D"
                    + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                    + " and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            int count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X);
                updatePreparedStatement.execute();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.execute("delete from follower where follower_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
//                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id, B.tweet_date as r1_tweet_date "
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
//            System.out.println(TestUtil.getTestName() + (d) / 1000000 + "/" + MAX);
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
//            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
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
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.execute("drop table if exists tempFollower");
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            other.executeUpdate("create temporary table tempFollower as select * from follower limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id, B.tweet_date as r1_tweet_date "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into tempFollower values(?, -1)");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into follower values(?, -1)");
            int MAX = 50000;
            int X = 1000000000;
            int count = 0;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, 2, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, 3, 2 * X + i, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 2 * X + i));
            }
            for(int k = 0; k < 15; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
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
                    updatePreparedStatement.execute();
                    insertPs.setInt(1, X + i);
                    insertPs.execute();
                    ResultSet t = prepareStatement.executeQuery(); 
                    while(t.next()) {
                        count++;
                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                            Object object = t.getObject(j);
                        }
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from follower where follower_id = -1");
                other.execute("delete from tempFollower where follower_id = -1");
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            count = 0;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X + i);
                updatePreparedStatement.execute();
                ResultSet t = prepareStatement.executeQuery(); 
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object object = t.getObject(j);
                    }
                }
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
            other.execute("delete from follower where follower_id = -1");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_TWEET_QUERY = "INSERT into tweet values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id, B.tweet_date as r1_tweet_date "
                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                + " and C.user_id = D.user_id");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into temp values(?, -1)");
            PreparedStatement insertPs = connection.prepareStatement("INSERT into follower values(?, -1)");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, 2, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, 3, 2 * X + i, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 2 * X + i));
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertTempPs.setInt(1, X + i);
                insertTempPs.execute();
            }// time to insert without change in index
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                insertPs.setInt(1, X + i);
                insertPs.execute();
            }// time to insert with change in index
            d = System.nanoTime() - tm - d;
            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
                int count = 0;
                tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery(); 
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object object = t.getObject(j);
                    }
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
                            + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id, B.tweet_date as r1_tweet_date "
                            + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
                            + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, temp D"
                            + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                            + " and C.user_id = D.user_id");
            int count = 0;
            tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery(); 
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object object = t.getObject(j);
                }
            }
            tm = (System.nanoTime() - tm + d) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            System.out.println(count);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
            other.execute("delete from follower where follower_id = -1");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
                    + "FROM tweet A, retweet B, retweet C, temp D"
                    + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                    + " and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement insertTempPs = connection.prepareStatement("INSERT into temp values(17, ?)");
            int MAX = 5000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                insertTempPs.setInt(1, X + i);
                insertTempPs.execute();
            }// time to insert without change in index
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            TestUtil.printPlan(other, "explain SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
                            + "FROM tweet A, retweet B, retweet C, temp D"
                            + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                            + " and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            for(int k = 0; k < 10; k++) {
                int count = 0;
                long tm = System.nanoTime();
                ResultSet t = prepareStatement.executeQuery();
                while(t.next()) {
                    count++;
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
                            + "FROM tweet A, retweet B, retweet C, temp D"
                            + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
                            + " and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int count = 0;
            long tm = System.nanoTime();
            ResultSet t = prepareStatement.executeQuery();
            while(t.next()) {
                count++;
                for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                    Object obj = t.getObject(j);
                }
            };
            tm = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
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
//        try(Connection connection = DriverManager.getConnection("jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
//            Statement other = connection.createStatement() ){
//            other.executeUpdate("create temporary table tempFollower as select * from follower limit 0;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, B.tweet_id as r1_tweet_id"
//                + ", B.retweet_tweet_id as r1_retweet_tweet_id, B.tweet_id as r1_tweet_id, B.tweet_date as r1_tweet_date "
//                + ", C.retweet_tweet_id as r2_retweet_tweet_id, C.user_id as r2_user_id, C.tweet_id as r2_tweet_id"
//                + ", D.user_id as f_user_id, D.follower_id FROM tweet A, retweet B, retweet C, tempFollower D"
//                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id "
//                + " and C.user_id = D.user_id");
//            int MAX = 5000;
//            int X = 1000000000;
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "tempFollower", 3, X + i));
//            int count = 0;
//            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
//                ResultSet t = prepareStatement.executeQuery();
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object obj = t.getObject(j);
//                    }
//                };
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
