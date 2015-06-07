package org.ucsd.db.bsmagen1M.mixedworkload;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class H21MQ1Aggregation {
    private static final String URL = "jdbc:h2:mem:./bsmagen1m;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;";
    private static final String USERNAME = "sa";
    private static final String PASSWORD = "";
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//1k:407869:1m:12404312;
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("./bsmagen1m");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("./bsmagen1m");
    }
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists view");
        other.executeUpdate("create temporary table view as SELECT A.user_id"
            + ", count(B.tweet_id) as count FROM users A, tweet B WHERE A.user_id = B.user_id and B.tweet_id < " 
                        + MIXED_WORKLOAD_CONSTANT + "group by A.user_id");
        other.execute("create index i_view on view(user_id)");
    }
    private void printViewCount(Statement other) throws SQLException {
        ResultSet count = other.executeQuery("select count(*) from view");
        count.next();
        System.out.println(count.getInt(1));        
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
            PreparedStatement pStatement = connection.prepareStatement("SELECT A.user_id"
                + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?");
            for(int k = 0; k < 10; k++) {
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setString(3, rs.getString(3));
                        updatePreparedStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setString(3, rs.getString(3));
                        updatePreparedStatement.execute();
                        TestUtil.iterate(pStatement.executeQuery(), 2);
                    }
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadWithoutViewMaintenanceThroughput() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX);
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            PreparedStatement pStatement = connection.prepareStatement("SELECT A.user_id"
                            + ", count(B.tweet_id) FROM users A, temp B WHERE A.user_id = B.user_id group by A.user_id");
            PreparedStatement iStatement = connection.prepareStatement("insert into temp values(?, ?, ?);");
            ResultSet rs = ps.executeQuery();
            while(rs.next()) {
                if(rs.getInt(4) == -1) {
                    iStatement.setInt(1, rs.getInt(1));
                    iStatement.setInt(2, rs.getInt(2));
                    iStatement.setString(3, rs.getString(3));
                    iStatement.execute();
                }
            }
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
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
            PreparedStatement mStatement = connection.prepareStatement("merge into view KEY(user_id) SELECT A.user_id"
                + ", V.count + count(B.tweet_id) FROM users A, temp B, view V WHERE A.user_id = B.user_id "
                + " and V.user_id = A.user_id group by A.user_id; ");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?");
            for(int k = 0; k < 10; k++) {
                System.gc();
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setString(3, rs.getString(3));
                        updatePreparedStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setString(3, rs.getString(3));
                        updatePreparedStatement.execute();
                        mStatement.executeUpdate();
                    }
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                createView(other);
            }
        }
    }
    @Test
    public void insertMixedWorkLoadWithViewMaintenanceThroughput() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            PreparedStatement ps = connection.prepareStatement("select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX);
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            printViewCount(other);
            PreparedStatement mStatement = connection.prepareStatement("merge into view KEY(user_id) SELECT A.user_id"
                + ", V.count + count(B.tweet_id) FROM users A, temp B, view V WHERE A.user_id = B.user_id "
                + " and V.user_id = A.user_id group by A.user_id; ");
            
            PreparedStatement iStatement = connection.prepareStatement("insert into temp values(?, ?, ?);");
            ResultSet rs = ps.executeQuery();
            while(rs.next()) {
                if(rs.getInt(4) == -1) {
                    iStatement.setInt(1, rs.getInt(1));
                    iStatement.setInt(2, rs.getInt(2));
                    iStatement.setString(3, rs.getString(3));
                    iStatement.execute();
                }
            }
            for(int k = 0; k < 10; k++) {
                System.gc();
                printViewCount(other);
                long tm = System.nanoTime();
                mStatement.executeUpdate();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                printViewCount(other);
                createView(other);
            }
        }
    }
}
