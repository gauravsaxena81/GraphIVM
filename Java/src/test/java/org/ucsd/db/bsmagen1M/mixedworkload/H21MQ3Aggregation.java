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

public class H21MQ3Aggregation {
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
    private void createSource(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists source");
        other.execute("create temporary table source as select user_id, tweet_id, tweet_date, -1 as retweet_tweet_id "
                        + " from tweet T union select * from retweet order by 2 desc limit 50000;");
    }
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists view");
        other.executeUpdate("create temporary table view as select A.user_id, count(C.tweet_id) as count from users A, tweet B, retweet C "
            + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
            + " and B.tweet_id < " + MIXED_WORKLOAD_CONSTANT + " and C.tweet_id < " + MIXED_WORKLOAD_CONSTANT + "group by A.user_id;");
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
            Statement other = connection.createStatement();
            createSource(other);
            PreparedStatement ps = connection.prepareStatement("select * from source order by tweet_id asc");
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            PreparedStatement tweetUStatement = connection.prepareStatement("insert into tweet values(?, ?, ?);");
            PreparedStatement retweetUStatement = 
                connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?, retweet_tweet_id = ?");
            for(int k = 0; k < 10; k++) {
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                other.execute("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                while(rs.next()) {
                    if(rs.getInt(4) > -1) {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setString(3, rs.getString(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                rs = ps.executeQuery();
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        tweetUStatement.setInt(1, rs.getInt(1));
                        tweetUStatement.setInt(2, rs.getInt(2));
                        tweetUStatement.setString(3, rs.getString(3));
                        tweetUStatement.execute();
                    } else {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setString(3, rs.getString(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
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
            Statement other = connection.createStatement();
            createSource(other);
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
            other.executeUpdate("insert into temp select * from source where retweet_tweet_id > -1");
            PreparedStatement iStatement = connection.prepareStatement("insert into tweet select user_id, tweet_id"
                            + " , tweet_date from source where retweet_tweet_id = -1;");
            for(int k = 0; k < 10; k++) {
                other.executeUpdate("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                long tm = System.nanoTime();
                iStatement.executeUpdate();
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
            Statement other = connection.createStatement();
            createSource(other);
            createView(other);
            PreparedStatement ps = connection.prepareStatement("select * from source order by tweet_id asc");
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 1;");
            PreparedStatement mStatement = 
                connection.prepareStatement("merge into view KEY(user_id) select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
            PreparedStatement tweetUStatement = connection.prepareStatement("insert into tweet values(?, ?, ?);");
            PreparedStatement retweetUStatement = 
                connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?, retweet_tweet_id = ?");
            for(int k = 0; k < 10; k++) {
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                other.execute("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                while(rs.next()) {
                    if(rs.getInt(4) > -1) {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setString(3, rs.getString(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                rs = ps.executeQuery();
                printViewCount(other);
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        tweetUStatement.setInt(1, rs.getInt(1));
                        tweetUStatement.setInt(2, rs.getInt(2));
                        tweetUStatement.setString(3, rs.getString(3));
                        tweetUStatement.execute();
                    } else {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setString(3, rs.getString(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
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
            Statement other = connection.createStatement();
            createSource(other);
            createView(other);
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            PreparedStatement mStatement = 
                connection.prepareStatement("merge into view KEY(user_id) select A.user_id, count(C.tweet_id) from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
            other.executeUpdate("insert into temp select * from source where retweet_tweet_id > -1");
            PreparedStatement iStatement = connection.prepareStatement("insert into tweet select user_id, tweet_id"
                            + " , tweet_date from source where retweet_tweet_id = -1;");
            for(int k = 0; k < 10; k++) {
                other.executeUpdate("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                printViewCount(other);
                long tm = System.nanoTime();
                iStatement.executeUpdate();
                mStatement.executeUpdate();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                createView(other);
            }
        }
    }
}
