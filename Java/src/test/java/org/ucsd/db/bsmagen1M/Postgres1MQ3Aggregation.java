package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ3Aggregation {
    private static final String INSERT_USER_QUERY = "INSERT into %s values(%d)";
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("create temporary table view as select A.user_id, count(C.tweet_id) from users A, tweet B, retweet C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
        other.execute("create index i_view on view(user_id)");
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
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 2);
                updatePreparedStatement.setInt(2, X + i);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, 2);
                updatePreparedStatement.setInt(2, X + i);
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
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 5; k++) {
                uStatement.setInt(1, 1);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                uStatement.setInt(1, -1);
                other.execute("analyze temp;");
                other.execute("analyze view;");
                printViewCount(other);
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    uStatement.executeUpdate();
                    dStatement.executeUpdate();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            TestUtil.printPlan(other, "explain update view A set count = count + ct * -1"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            TestUtil.printPlan(other, "explain delete from view V using users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String UPDATE_RETWEET_QUERY = "UPDATE temp set user_id = %d, tweet_id = %d, retweet_tweet_id = %d";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            int MAX = 50000;
            int X = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into users values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d)", X + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            s = new StringBuilder("INSERT into tweet values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21')", X + i, X + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
            }
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X));
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
            }
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void deleteJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, retweet_tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into users values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d)", X + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            s = new StringBuilder("INSERT into tweet values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21')", X + i, X + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            for(int k = 0; k < 5; k++) {
                uStatement.setInt(1, 1);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3, X + i);
                    updatePreparedStatement.executeUpdate();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3,  X + i);
                    updatePreparedStatement.executeUpdate();
                }
                long d = System.nanoTime() - tm;
                other.execute("analyze temp;");
                other.execute("analyze view;");
                printViewCount(other);
                uStatement.setInt(1, -1);
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3,  X + i);
                    updatePreparedStatement.executeUpdate();
                    uStatement.executeUpdate();
                    dStatement.executeUpdate();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            TestUtil.printPlan(other, "explain update view A set count = count + ct * -1"
                    + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            TestUtil.printPlan(other, "explain delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id;");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21', %d)";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            other.executeUpdate("delete from temp");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            int MAX = 50000;
            int x = 1000000000;
            other.executeUpdate(String.format(INSERT_USER_QUERY, "users", x));
            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x, x));
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", x, x + i, x));
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            }
            long tm = System.nanoTime();
            if(uStatement.executeUpdate() == 0)
                iStatement.execute();
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void deleteJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            int MAX = 50000;
            int x = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into users values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d)", x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            s = new StringBuilder("INSERT into tweet values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21')", x + i, x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            s = new StringBuilder("INSERT into temp values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21', %d)", x, x + i, x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
                printViewCount(other);
                uStatement.setInt(1, 1);
                System.out.println("uStatement.setInt(1, 1);");
                if(uStatement.executeUpdate() == 0)
                    iStatement.executeUpdate();
                other.execute("analyze view;");
                printViewCount(other);
                uStatement.setInt(1, -1);
                long tm = System.nanoTime();
                uStatement.executeUpdate();
                dStatement.executeUpdate();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            TestUtil.printPlan(other, "explain update view A set count = count + ct * -1"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            TestUtil.printPlan(other, "explain delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            int MAX = 50000;
            int X = 1000000000;
            String INSERT_RETWEET_QUERY = "INSERT into temp values(2, %d, '2992-02-21', 2)";
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i));
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
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
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and not exists (select 1 from view D where D.user_id = A.user_id) group by A.user_id");
            int MAX = 50000;
            int X = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into temp values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(2, %d, '2992-02-21', 2)", X + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
                uStatement.setInt(1, 1);
                int executeUpdate = uStatement.executeUpdate();
                if(executeUpdate == 0)
                    System.out.println(iStatement.executeUpdate());
                other.execute("analyze view;");
                printViewCount(other);
                long tm = System.nanoTime();
                uStatement.setInt(1, -1);
                System.out.println(uStatement.executeUpdate());
                System.out.println(dStatement.executeUpdate());
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            TestUtil.printPlan(other, "explain update view A set count = count + ct * -1"
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            TestUtil.printPlan(other, "explain delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and V.user_id = A.user_id and count = 0;");
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
//            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
//            other.executeUpdate("delete from temp");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
//                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
//            int MAX = 50;
//            int X = 1000000000;
//            other.executeUpdate(String.format(INSERT_USER_QUERY, "follower", X));
//            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", X, X));
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", X, X + i, X));
//            int count = 0;
//            for(int k = 0; k < TestUtil.WARMUP_ITERAIONS; k++) {
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
