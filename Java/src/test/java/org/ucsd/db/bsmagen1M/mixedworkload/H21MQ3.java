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

public class H21MQ3 {
    private static final String URL = "jdbc:h2:mem:~/bsmagen;QUERY_CACHE_SIZE=0;CACHE_SIZE=14680064;ANALYZE_AUTO=0;";
    private static final String USERNAME = "sa";
    private static final String PASSWORD = "";
    final static int MIXED_WORKLOAD_CONSTANT = 407869;//1k:407869:1m:12404312;
    @BeforeClass
    public static void setUp() throws SQLException {
        TestUtil.loadH2Database("~/bsmagen");
    }
    @AfterClass
    public static void destruct() throws SQLException {
        TestUtil.checkH2Database("~/bsmagen");
    }
    private void createSource(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists source");
        other.execute("create temporary table source as select user_id, tweet_id, tweet_date, -1 as retweet_tweet_id "
                        + " from tweet T union select * from retweet order by 2 desc limit 50000;");
    }
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists view");
        other.executeUpdate("create temporary table view as select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, retweet C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id "
                    + " and B.tweet_id < " + MIXED_WORKLOAD_CONSTANT + " and C.tweet_id < " + MIXED_WORKLOAD_CONSTANT);        
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
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id;");
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
                rs = ps.executeQuery();
                tm = System.nanoTime();
                TestUtil.initRowCount();
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
                        TestUtil.iterate(pStatement.executeQuery(), 7);
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
            createSource(other);
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from retweet limit 0;");
            PreparedStatement pStatement = 
                connection.prepareStatement("select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id;");
            other.executeUpdate("insert into temp select * from source where retweet_tweet_id > -1");
            PreparedStatement iStatement = connection.prepareStatement("insert into tweet select user_id, tweet_id"
                            + " , tweet_date from source where retweet_tweet_id = -1;");
            for(int k = 0; k < 10; k++) {
                other.executeUpdate("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                TestUtil.initRowCount();
                long tm = System.nanoTime();
                iStatement.executeUpdate();
                TestUtil.iterate(pStatement.executeQuery(), 7);
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                TestUtil.initRowCount();
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
            PreparedStatement vIStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id;");
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
//                        TestUtil.printPlan(other, "explain insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
//                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
//                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
//                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id;");
                        vIStatement.executeUpdate();
                    }
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where tweet_id >= " + MIXED_WORKLOAD_CONSTANT);
                other.execute("delete from view where r_tweet_id >= " + MIXED_WORKLOAD_CONSTANT);
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
            PreparedStatement pStatement = 
                connection.prepareStatement("insert into view select A.user_id as f_user_id, B.user_id as t_user_id"
                    + ", B.tweet_id, B.tweet_date as t_tweet_date, C.retweet_tweet_id, C.tweet_id as r_tweet_id"
                    + ", C.tweet_date as r_tweet_date from users A, tweet B, temp C "
                    + " where A.user_id = B.user_id and B.tweet_id = C.retweet_tweet_id;");
            other.executeUpdate("insert into temp select * from source where retweet_tweet_id > -1");
            PreparedStatement iStatement = connection.prepareStatement("insert into tweet select user_id, tweet_id"
                            + " , tweet_date from source where retweet_tweet_id = -1;");
            for(int k = 0; k < 10; k++) {
                other.executeUpdate("delete from tweet where tweet_id >=" + MIXED_WORKLOAD_CONSTANT);
                printViewCount(other);
                long tm = System.nanoTime();
                iStatement.executeUpdate();
                pStatement.executeUpdate();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                printViewCount(other);
                other.execute("update temp set tweet_id = tweet_id + 1 limit 10");
                other.execute("delete from view where tweet_id >= " + MIXED_WORKLOAD_CONSTANT);
                other.execute("delete from view where r_tweet_id >= " + MIXED_WORKLOAD_CONSTANT);
            }
        }
    }
}
