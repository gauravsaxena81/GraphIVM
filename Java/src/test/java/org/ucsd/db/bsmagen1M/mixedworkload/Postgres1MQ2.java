package org.ucsd.db.bsmagen1M.mixedworkload;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ2 {
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen";
    private static final String USERNAME = "gaurav";
    private static final String PASSWORD = "gaurav";
    final static int MIXED_WORKLOAD_CONSTANT = 407869;//1k:407869:1m:12404312;
    private void printViewCount(Statement other) throws SQLException {
        ResultSet count = other.executeQuery("select count(*) from view");
        count.next();
        System.out.println(count.getInt(1));        
    }
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("create temporary table view as SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
            + ", tweet_id, tweet_date FROM follower A, tweet B WHERE A.user_id = B.user_id and B.tweet_id <" + MIXED_WORKLOAD_CONSTANT);
        other.executeUpdate("create index i_view on view(f_user_id, t_user_id, follower_id, tweet_id, tweet_date)");
    }
    @Test
    public void insertMixedWorkLoadWithoutViewMaintenance() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX);
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement prepareStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?");
            other.execute("analyze follower;");
            for(int k = 0; k < 5; k++) {
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setDate(3, rs.getDate(3));
                        updatePreparedStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                other.execute("analyze temp;");
                TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
                rs = ps.executeQuery();
                tm = System.nanoTime();
                TestUtil.initRowCount();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setDate(3, rs.getDate(3));
                        updatePreparedStatement.execute();
                        TestUtil.iterate(prepareStatement.executeQuery(), 5);
                    }
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                TestUtil.printRowCount();
            }
        }
    }
    @Test
    public void insertMixedWorkLoadWithoutViewMaintenanceThroughput() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement pStatement = 
                connection.prepareStatement("SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            other.execute("insert into temp select * from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            TestUtil.printPlan(other, "explain SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                            + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            for(int k = 0; k < 5; k++) {
                TestUtil.initRowCount();
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 5);
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                TestUtil.printRowCount();
            }
        }
    }
    @Test
    public void insertMixedWorkLoadWithViewMaintenance() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select user_id, tweet_id, tweet_date"
                    + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX);
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            printViewCount(other);
            PreparedStatement vInsertStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?");
            other.execute("analyze follower;");
            for(int k = 0; k < 10; k++) {
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setDate(3, rs.getDate(3));
                        updatePreparedStatement.execute();
                    }
                }
                other.execute("analyze temp;");
                other.execute("analyze view");
                TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                rs = ps.executeQuery();
                printViewCount(other);
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setDate(3, rs.getDate(3));
                        updatePreparedStatement.execute();
                        vInsertStatement.executeUpdate();
                    }
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where tweet_id >= " + MIXED_WORKLOAD_CONSTANT);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadWithViewMaintenanceThroughput() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement prepareStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            other.executeUpdate("insert into temp select * from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
            other.execute("analyze follower;");
            other.execute("analyze temp;");
            other.execute("analyze view");
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                            + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            for(int k = 0; k < 10; k++) {
                printViewCount(other);
                other.execute("analyze view");
                long tm = System.nanoTime();
                prepareStatement.execute();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where tweet_id >= " + MIXED_WORKLOAD_CONSTANT);
            }
        }
    }
}
