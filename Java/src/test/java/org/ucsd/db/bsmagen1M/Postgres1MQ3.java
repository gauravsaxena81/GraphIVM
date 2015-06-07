package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ3 {
    private static final String INSERT_USER_QUERY = "INSERT into %s values(%d)";
    private static final String INSERT_RETWEET_QUERY = "INSERT into temp values(2, %d, '2992-02-21', 2)";
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("create temporary table view as select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, retweet C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id;");
        other.execute("create index i_view on view(f_user_id, t_user_id, tweet_id, t_tweet_date, retweet_tweet_id, r_tweet_id, r_tweet_date);");
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
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            other.execute("analyze users;");
            other.execute("analyze tweet;");
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
                printViewCount(other);
                other.execute("analyze temp;");
                other.execute("analyze view;");
                TestUtil.printPlan(other, "explain insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    prepareStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where r_tweet_id >= " + X);
            }
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
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and V.f_user_id = A.user_id and V.t_user_id = B.user_id and V.tweet_id = B.tweet_id "
                    + " and V.t_tweet_date = B.tweet_date and V.retweet_tweet_id = C.retweet_tweet_id and V.r_tweet_id = C.tweet_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set retweet_tweet_id = ?, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            for(int k = 0; k < 10; k++) {
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    iStatement.execute();
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                printViewCount(other);
                other.execute("analyze temp;");
                other.execute("analyze view;");
                TestUtil.printPlan(other, "explain delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and V.f_user_id = A.user_id and V.t_user_id = B.user_id and V.tweet_id = B.tweet_id "
                    + " and V.t_tweet_date = B.tweet_date and V.retweet_tweet_id = C.retweet_tweet_id and V.r_tweet_id = C.tweet_id");
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    if(i % 100 == 0)
                        System.out.println(i);
                    updatePreparedStatement.setInt(1, 2);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.execute();
                    dStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
//    @Test
//    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException
//    , ClassNotFoundException {
//        initDriver();
//        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
//                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
//                    + ", C.tweet_date as r_tweet_date from users A, tweet B, retweet C "
//                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id and A.user_id = ? "
//                    + " and C.tweet_id = ? and C.retweet_tweet_id = ? and C.tweet_date = '2992-02-21'");
//            Statement other = connection.createStatement() ){
//            int MAX = 50;
//            int X = 1000000000;
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "retweet"));
//            for(int k = 0, x = X; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                System.out.println(k);
//                other.executeUpdate(String.format(INSERT_USER_QUERY, "users", x));
//                other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x, x));
//                for(int i = 0; i < MAX; i++)
//                    other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "retweet", x, x + i, x));
//                int count = 0;
//                long tm = System.nanoTime();
//                for(int i = 0; i < MAX; i++);
//                for(int i = 0; i < MAX; i++) {
//                    prepareStatement.setInt(1, x);
//                    prepareStatement.setInt(2, x + i);
//                    prepareStatement.setInt(3, x);
//                    ResultSet t = prepareStatement.executeQuery();
//                    while(t.next()) {
//                        count++;
//                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                            Object obj = t.getObject(j);
//                        }
//                    }
//                    deletePreparedStatement.setInt(1, X);
//                    deletePreparedStatement.setInt(2, X + i);
//                    deletePreparedStatement.executeUpdate();
//                }
//            }
//            //--
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "retweet", X, X + i, X));
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, X);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testPostgres:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++)
//                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "retweet", X, X + i, X));
//            int count = 0;
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, X);
//                prepareStatement.setInt(2, X + i);
//                prepareStatement.setInt(3, X);
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    count++;
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//                deletePreparedStatement.setInt(1, X);
//                deletePreparedStatement.setInt(2, X + i);
//                deletePreparedStatement.executeUpdate();
//            }
//            System.out.println("testPostgres:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//            System.out.println(count);
//            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
//            other.executeUpdate("Delete from users where user_id >= " + X);
//            other.executeUpdate("Delete from retweet where tweet_date = '2992-02-21'");
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    @Test
    public void insertJoinTuples1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        final String UPDATE_RETWEET_QUERY = "UPDATE temp set user_id = %d, tweet_id = %d, retweet_tweet_id = %d";
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X + i));
                }
                other.execute("analyze temp;");
                long d = System.nanoTime() - tm;
                printViewCount(other);
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    other.executeUpdate(String.format(UPDATE_RETWEET_QUERY, X, X + i, X + i));
                    prepareStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where tweet_id >= " + X);
            }
            TestUtil.printPlan(other, "explain insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and V.f_user_id = A.user_id and V.t_user_id = B.user_id and V.tweet_id = B.tweet_id "
                    + " and V.t_tweet_date = B.tweet_date and V.retweet_tweet_id = C.retweet_tweet_id and V.r_tweet_id = C.tweet_id");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
            for(int k = 0; k < 10; k++) {
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3, X + i);
                    updatePreparedStatement.executeUpdate();
                    prepareStatement.execute();
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3, X + i);
                    updatePreparedStatement.executeUpdate();
                }
                long d = System.nanoTime() - tm;
                printViewCount(other);
                other.execute("analyze temp;");
                other.execute("analyze view;");
                TestUtil.printPlan(other, "explain delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and V.f_user_id = A.user_id and V.t_user_id = B.user_id and V.tweet_id = B.tweet_id "
                    + " and V.t_tweet_date = B.tweet_date and V.retweet_tweet_id = C.retweet_tweet_id and V.r_tweet_id = C.tweet_id");
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    if(i % 100 == 0)
                        System.out.println(i);
                    updatePreparedStatement.setInt(1, X);
                    updatePreparedStatement.setInt(2, X + i);
                    updatePreparedStatement.setInt(3, X + i);
                    updatePreparedStatement.executeUpdate();
                    dStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + X);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_RETWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21', %d)";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 2;
            int x = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into users values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d)", x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            s = new StringBuilder("INSERT into tweet values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21')", x + i, x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", x, x + i, x + i));
            for(int k = 0; k < 10; k++) {
                printViewCount(other);
                long tm = System.nanoTime();
                prepareStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where r_tweet_id >= " + x);
            }
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        } finally {
            if(prepareStatement != null)
                prepareStatement.close();
        }
    }
    @Test
    public void deleteJoinTuplesThroughput1To1() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        final String INSERT_TWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21')";
        final String INSERT_RETWEET_QUERY = "INSERT into %s values(%d, %d, '2992-02-21', %d)";
        initDriver();
        PreparedStatement prepareStatement = null;
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and V.f_user_id = A.user_id and V.t_user_id = B.user_id and V.tweet_id = B.tweet_id "
                    + " and V.t_tweet_date = B.tweet_date and V.retweet_tweet_id = C.retweet_tweet_id and V.r_tweet_id = C.tweet_id");
            prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int x = 1000000000;
            other.executeUpdate(String.format(INSERT_USER_QUERY, "users", x));
            other.executeUpdate(String.format(INSERT_TWEET_QUERY, "tweet", x, x));
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, "temp", x, x + i, x + i));
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                prepareStatement.execute();
                other.execute("analyze view;");
                printViewCount(other);
                long tm = System.nanoTime();
                dStatement.executeUpdate();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            long tm = System.nanoTime();
            prepareStatement.execute();
            System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from tweet where tweet_date = '2992-02-21'");
            other.executeUpdate("Delete from users where user_id >= " + x);
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
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i));
            other.execute("analyze users;");
            other.execute("analyze tweet;");
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                prepareStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            long tm = System.nanoTime();
            prepareStatement.execute();
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
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view V using users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and V.f_user_id = A.user_id and V.t_user_id = B.user_id and V.tweet_id = B.tweet_id "
                    + " and V.t_tweet_date = B.tweet_date and V.retweet_tweet_id = C.retweet_tweet_id and V.r_tweet_id = C.tweet_id");
            int MAX = 50000;
            int X = 1000000000;
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_RETWEET_QUERY, X + i));
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                prepareStatement.execute();
                other.execute("analyze view;");
                printViewCount(other);
                long tm = System.nanoTime();
                dStatement.executeUpdate();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            TestUtil.printPlan(other, "explain insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                            + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                            + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
//                connection.prepareStatement("select A.user_id as f_user_id, A.follower_id, B.user_id as t_user_id"
//                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
//                    + ", C.tweet_date as r_tweet_date from follower A, tweet B, temp C "
//                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id");
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
