package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ4Aggregation {
    private static final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(384827, %d)";//384827,17
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("create temporary table view as SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id limit 0;");
        other.execute("create index i_view on view(user_id, tweet_id)");
    }
    private void printViewCount(Statement other) throws SQLException {
        ResultSet count = other.executeQuery("select count(*) from view");
        count.next();
        System.out.println(count.getInt(1));        
    }
    @Test
    public void insertProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct "
                    + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
                + " WHERE A.user_id = B.user_id and A.tweet_id = B.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 384827, follower_id = ?");
            int MAX = 5000;
            int X = 1000000000;
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, X);
                updatePreparedStatement.execute();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct "
                    + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
                + " WHERE A.user_id = B.user_id and A.tweet_id = B.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
            
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using tweet A WHERE"
                + " V.user_id = A.user_id and V.tweet_id = A.tweet_id and count = 0 ");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 384827, follower_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            for(int k = 0; k < 5; k++) {
                uStatement.setInt(1, 1);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                other.execute("analyze temp;");
                other.execute("analyze view;");
                uStatement.setInt(1, -1);
                printViewCount(other);
                tm = System.nanoTime();
                TestUtil.printPlan(other, "explain delete from view V using tweet A WHERE"
                + " V.user_id = A.user_id and V.tweet_id = A.tweet_id and count = 0 ;");
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.execute();
                    uStatement.executeUpdate();
                    dStatement.executeUpdate();
                        
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String INSERT_TWEET_QUERY = "INSERT into tweet values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct "
                    + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
                + " WHERE A.user_id = B.user_id and A.tweet_id = B.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            connection.setAutoCommit(true);
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?");
            int MAX = 5000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(INSERT_TWEET_QUERY, 2, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, 3, 2 * X + i, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 2 * X + i));
            }
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
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
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
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
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_FOLLOWER_QUERY = "INSERT into temp values(%d, -1)";
        final String INSERT_TWEET_QUERY = "INSERT into tweet values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into retweet values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct "
                    + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
                + " WHERE A.user_id = B.user_id and A.tweet_id = B.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 5000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++) {
                //other.executeUpdate(String.format(INSERT_TWEET_QUERY, 2, X + i));
                //other.executeUpdate(String.format(INSERT_RETWEET_QUERY, 3, 2 * X + i, X + i));
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i, 4 * X + i, 1175996));
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            }
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 8; k++) {
                long tm = System.nanoTime();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, " explain insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                            + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                            + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
            long tm = System.nanoTime();
            if(uStatement.executeUpdate() == 0)
                iStatement.execute();
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct "
                    + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
                + " WHERE A.user_id = B.user_id and A.tweet_id = B.tweet_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 5000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, " explain insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id ");
            long tm = System.nanoTime();
            if(uStatement.executeUpdate() == 0)
                iStatement.execute();
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void deleteProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from follower limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) as ct "
                    + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id) B "
                + " WHERE A.user_id = B.user_id and A.tweet_id = B.tweet_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using tweet A, retweet B, retweet C, temp D "
                    + " WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id and count = 0 ");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as user_id, A.tweet_id, count(D.follower_id) "
                + " FROM tweet A, retweet B, retweet C, temp D WHERE A.tweet_id = B.retweet_tweet_id "
                + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id group by A.user_id, A.tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, X + i));
            other.execute("analyze tweet;");
            other.execute("analyze retweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                uStatement.setInt(1, 1);
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                other.execute("analyze view;");
                uStatement.setInt(1, -1);
                TestUtil.printPlan(other, " explain delete from view V using tweet A, retweet B, retweet C, temp D "
                    + " WHERE A.tweet_id = B.retweet_tweet_id "
                    + " and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id and count = 0; ");
                printViewCount(other);
                long tm = System.nanoTime();
                uStatement.executeUpdate();
                dStatement.executeUpdate();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
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
//            other.executeUpdate("create temporary table tempFollower as select * from follower limit 0;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.user_id as t_user_id, A.tweet_id, count(D.follower_id) "
//                + " FROM tweet A, retweet B, retweet C, temp D"
//                + " WHERE A.tweet_id = B.retweet_tweet_id and B.tweet_id = C.retweet_tweet_id and C.user_id = D.user_id");
//            int MAX = 50;
//            int X = 1000000000;
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_FOLLOWER_QUERY, "tempFollower", 3, X + i));
//            int count = 0;
//            for(int k = 0; k < 15; k++) {
//                count = 0;
//                long tm = System.nanoTime();
//                ResultSet t = prepareStatement.executeQuery();
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object obj = t.getObject(j);
//                    }
//                }
//                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
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
