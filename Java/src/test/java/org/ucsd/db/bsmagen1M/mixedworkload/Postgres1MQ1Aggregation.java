package org.ucsd.db.bsmagen1M.mixedworkload;

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
public class Postgres1MQ1Aggregation {
    private static final String URL = "jdbc:postgresql://localhost:5432/bsmagen1m";
    private static final String USERNAME = "gaurav";
    private static final String PASSWORD = "gaurav";
    final static int MIXED_WORKLOAD_CONSTANT = 12404312;//1k:407869:1m:12404312;
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("drop table if exists view;");
        other.executeUpdate("create temporary table view as SELECT A.user_id, count(B.tweet_id) as count FROM users A, tweet B "
            + " WHERE A.user_id = B.user_id group by A.user_id;");
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
            other.execute("analyze users;");
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
                rs = ps.executeQuery();
                other.execute("analyze temp;");
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setDate(3, rs.getDate(3));
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
            other.execute("analyze users;");
            other.execute("analyze temp;");
            while(rs.next()) {
                if(rs.getInt(4) == -1) {
                    iStatement.setInt(1, rs.getInt(1));
                    iStatement.setInt(2, rs.getInt(2));
                    iStatement.setDate(3, rs.getDate(3));
                    iStatement.execute();
                }
            }
            for(int k = 0; k < 5; k++) {
                long tm = System.nanoTime();
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
            PreparedStatement ps = connection.prepareStatement("select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX);
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            printViewCount(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select user_id, count(tweet_id) as ct from temp group by user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id, count(B.tweet_id) FROM users A, temp B "
                        + " WHERE A.user_id = B.user_id group by A.user_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?, tweet_date = ?");
            other.execute("analyze users;");
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
                rs = ps.executeQuery();
                other.execute("analyze temp;");
                other.execute("analyze view;");
                TestUtil.printPlan(other, "explain update view A set count = count + ct "
                    + " FROM (select user_id, count(tweet_id) as ct from temp group by user_id) B "
                    + " WHERE A.user_id = B.user_id");
                tm = System.nanoTime();
                while(rs.next()) {
                    if(rs.getInt(4) == -1) {
                        updatePreparedStatement.setInt(1, rs.getInt(1));
                        updatePreparedStatement.setInt(2, rs.getInt(2));
                        updatePreparedStatement.setDate(3, rs.getDate(3));
                        updatePreparedStatement.execute();
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
            PreparedStatement ps = connection.prepareStatement("select user_id, tweet_id, tweet_date"
                + ", -1 as retweet_tweet_id from tweet T union select * from retweet order by 2 desc limit " + MAX);
            Statement other = connection.createStatement();
            other.executeUpdate("drop table if exists temp");
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            printViewCount(other);
            PreparedStatement vUStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (select user_id, count(tweet_id) as ct from temp group by user_id) B "
                + " WHERE A.user_id = B.user_id");
            PreparedStatement vIStatement = 
                connection.prepareStatement("insert into view SELECT A.user_id, count(B.tweet_id) FROM users A, temp B "
                        + " WHERE A.user_id = B.user_id and not exists (select 1 from view V where V.user_id = A.user_id)"
                        + " group by A.user_id ");
            
            PreparedStatement iStatement = connection.prepareStatement("insert into temp values(?, ?, ?);");
            ResultSet rs = ps.executeQuery();
            while(rs.next()) {
                if(rs.getInt(4) == -1) {
                    iStatement.setInt(1, rs.getInt(1));
                    iStatement.setInt(2, rs.getInt(2));
                    iStatement.setDate(3, rs.getDate(3));
                    iStatement.execute();
                }
            }
            for(int k = 0; k < 5; k++) {
                System.gc();
                printViewCount(other);
                long tm = System.nanoTime();
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
