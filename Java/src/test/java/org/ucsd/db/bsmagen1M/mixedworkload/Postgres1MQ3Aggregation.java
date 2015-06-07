package org.ucsd.db.bsmagen1M.mixedworkload;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ3Aggregation {
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static final String USERNAME = "gaurav";
    private static final String PASSWORD = "gaurav";
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//1k:407869:1m:12404312;
    private void createSource(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists source");
        other.execute("create temporary table source as select user_id, tweet_id, tweet_date, -1 as retweet_tweet_id "
                        + " from tweet T union select * from retweet order by 2 desc limit 50000;");
    }
    private void createView(Statement other) throws SQLException {
        other.execute("drop table if exists view");
        other.executeUpdate("create temporary table view as select A.user_id, count(C.tweet_id) from users A, tweet B, retweet C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id limit 10;");
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
            createTempTweet(other);
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id, count(C.tweet_id) from users A, tempTweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            PreparedStatement tweetUStatement = connection.prepareStatement("insert into tempTweet values(?, ?, ?);");
            PreparedStatement retweetUStatement = 
                connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?, retweet_tweet_id = ?");
            other.execute("analyze users;");
            for(int k = 0; k < 5; k++) {
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                other.execute("delete from tempTweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                while(rs.next()) {
                    if(rs.getInt(4) > -1) {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setDate(3, rs.getDate(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                tm = System.nanoTime();
                rs = ps.executeQuery();
                other.execute("analyze tempTweet;");
                other.execute("analyze temp;");
                TestUtil.printPlan(other, "explain select A.user_id, count(C.tweet_id) from users A, tempTweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        tweetUStatement.setInt(1, rs.getInt(1));
                        tweetUStatement.setInt(2, rs.getInt(2));
                        tweetUStatement.setDate(3, rs.getDate(3));
                        tweetUStatement.execute();
                    } else {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setDate(3, rs.getDate(3));
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
    private void createTempTweet(Statement other) throws SQLException {
        other.execute("create temp table tempTweet as select * from tweet where tweet_id <" + MIXED_WORKLOAD_CONSTANT);
        other.executeUpdate("create index i_tempTweet_userId on tempTweet(user_id)");
        other.executeUpdate("create index i_tempTweet_tweetId on tempTweet(tweet_id)");        
    }
    @Test
    public void insertMixedWorkLoadWithoutViewMaintenanceThroughput() throws SQLException {
        try(Connection connection = DriverManager.getConnection(URL, USERNAME, PASSWORD)) {
            int MAX = 50000;
            Statement other = connection.createStatement();
            createSource(other);
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            createTempTweet(other);
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id, count(C.tweet_id) from users A, tempTweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
            other.executeUpdate("insert into temp select * from source where retweet_tweet_id > -1");
            PreparedStatement iStatement = connection.prepareStatement("insert into tempTweet select user_id, tweet_id"
                            + " , tweet_date from source where retweet_tweet_id = -1;");
            other.execute("analyze users;");
            other.execute("analyze temp;");
            for(int k = 0; k < 5; k++) {
                other.executeUpdate("delete from tempTweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                other.execute("analyze tempTweet;");
                TestUtil.printPlan(other, "explain select A.user_id, count(C.tweet_id) from users A, tempTweet B, temp C "
                        + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id;");
                long tm = System.nanoTime();
                iStatement.executeUpdate();
                TestUtil.iterate(pStatement.executeQuery(), 2);
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
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
            createTempTweet(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tempTweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tempTweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id group by A.user_id");
            PreparedStatement tweetUStatement = connection.prepareStatement("insert into tempTweet values(?, ?, ?);");
            PreparedStatement retweetUStatement = 
                connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?, retweet_tweet_id = ?");
            other.execute("analyze users;");
            for(int k = 0; k < 5; k++) {
                ResultSet rs = ps.executeQuery();
                long tm = System.nanoTime();
                other.execute("delete from tempTweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                while(rs.next()) {
                    if(rs.getInt(4) > -1) {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setDate(3, rs.getDate(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
                    }
                }
                long d = System.nanoTime() - tm;
                rs = ps.executeQuery();
                other.execute("analyze tempTweet;");
                other.execute("analyze temp;");
                TestUtil.printPlan(other, "explain update view A set count = count + ct "
                    + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tempTweet B, temp C where A.user_id = B.user_id "
                        + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                    + " WHERE A.user_id = B.user_id;");
                printViewCount(other);
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        tweetUStatement.setInt(1, rs.getInt(1));
                        tweetUStatement.setInt(2, rs.getInt(2));
                        tweetUStatement.setDate(3, rs.getDate(3));
                        tweetUStatement.execute();
                    } else {
                        retweetUStatement.setInt(1, rs.getInt(1));
                        retweetUStatement.setInt(2, rs.getInt(2));
                        retweetUStatement.setDate(3, rs.getDate(3));
                        retweetUStatement.setInt(4, rs.getInt(4));
                        retweetUStatement.execute();
                        if(uStatement.executeUpdate() == 0)
                            iStatement.executeUpdate();
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
            createTempTweet(other);
            PreparedStatement vUStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select A.user_id, count(C.tweet_id) as ct from users A, tempTweet B, temp C where A.user_id = B.user_id "
                    + " and B.tweet_id = C.retweet_tweet_id group by A.user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement vIStatement = 
                connection.prepareStatement("insert into view select A.user_id, count(C.tweet_id) from users A, tempTweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and not exists (select 1 from view V where V.user_id = A.user_id) group by A.user_id");
            other.executeUpdate("insert into temp select * from source where retweet_tweet_id > -1");
            PreparedStatement iStatement = connection.prepareStatement("insert into tweet select user_id, tweet_id"
                            + " , tweet_date from source where retweet_tweet_id = -1;");
            for(int k = 0; k < 5; k++) {
                other.executeUpdate("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                printViewCount(other);
                other.execute("analyze tempTweet;");
                other.execute("analyze temp;");
                long tm = System.nanoTime();
                iStatement.executeUpdate();
                vUStatement.executeUpdate();
                vIStatement.executeUpdate();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                createView(other);
            }
        }
    }
}
