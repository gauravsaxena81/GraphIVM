package org.ucsd.db.bsmagen1M;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;

public class Postgres1MQ2Aggregation {
    private void createView(Statement other) throws SQLException {
        other.executeUpdate("create temporary table view as SELECT follower_id, count(C.user_id) as count FROM "
            + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id limit 0;");  
        other.execute("create index i_view on view(follower_id)");
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
            Statement other = connection.createStatement() ) {
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
                + " WHERE A.follower_id = B.follower_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 100, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                other.execute("analyze temp;");
                other.execute("analyze view;");
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
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void deleteProjectedTupleTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ) {
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            other.execute("insert into view SELECT follower_id, count(C.user_id) FROM "
                        + " follower A, tweet C WHERE A.user_id = C.user_id  group by follower_id ");
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
                + " WHERE A.follower_id = B.follower_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using follower B, temp C where B.user_id = C.user_id "
                    + " and A.follower_id = B.follower_id and count = 1");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = 100, tweet_id = ?");
            int MAX = 50000;
            int X = 1000000000;
            for(int k = 0; k < 10; k++) {
                uStatement.setInt(1, 1);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, X + i);
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
                    updatePreparedStatement.setInt(1, X + i);
                    updatePreparedStatement.execute();
                    if(dStatement.executeUpdate() == 0)
                        uStatement.execute();
                }
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            TestUtil.printPlan(other, "explain delete from view A using follower B, temp C where B.user_id = C.user_id "
                    + " and A.follower_id = B.follower_id and count = 1;");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
                + " WHERE A.follower_id = B.follower_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                                + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            other.execute("analyze follower;");
            int MAX = 50000;
            final int x = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into follower values");
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 3; j++)
                    s.append(String.format("(%d, %d)", x + i, 2 * x + j)).append(","); 
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze follower;");
            for(int k = 0; k < 10; k++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                other.execute("analyze temp;");
                other.execute("analyze view;");
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    if(uStatement.executeUpdate() == 0)
                        iStatement.execute();
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
                other.execute("delete from view;");
            }
            TestUtil.printPlan(other, "explain insert into view SELECT A.user_id as f_user_id, follower_id, B.user_id as t_user_id"
                    + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id;");
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, x + i);
                updatePreparedStatement.setInt(2, x + i);
                updatePreparedStatement.execute();
            }
            long d = System.nanoTime() - tm;
            tm = System.nanoTime();
            for(int i = 0; i < MAX; i++);
            for(int i = 0; i < MAX; i++) {
                updatePreparedStatement.setInt(1, x + i);
                updatePreparedStatement.setInt(2, x + i);
                updatePreparedStatement.execute();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
            }
            tm = (System.nanoTime() - tm - d) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            printViewCount(other);
            other.executeUpdate("Delete from follower where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void deleteJoinTuples() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
            createView(other);
            other.execute("insert into view SELECT follower_id, count(C.user_id) FROM "
                            + " follower A, tweet C WHERE A.user_id = C.user_id  group by follower_id ");
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct * ?"
                + " FROM (SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
                + " WHERE A.follower_id = B.follower_id");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using follower B, temp C where B.user_id = C.user_id "
                    + " and A.follower_id = B.follower_id and count = 1");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            PreparedStatement updatePreparedStatement = connection.prepareStatement("UPDATE temp set user_id = ?, tweet_id = ?");
            int MAX = 50000;
            final int x = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into follower values");
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 3; j++)
                    s.append(String.format("(%d, %d)", x + i, 2 * x + j)).append(","); 
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze follower;");
            for(int k = 0; k < 10; k++) {
                uStatement.setInt(1, 1);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    int executeUpdate = uStatement.executeUpdate() ;
                    if(executeUpdate == 0) {
                        iStatement.execute();
                    }
                }
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                }
                long d = System.nanoTime() - tm;
                other.execute("analyze temp;");
                other.execute("analyze view;");
                printViewCount(other);
                uStatement.setInt(1, -1);
                tm = System.nanoTime();
                for(int i = 0; i < MAX; i++);
                for(int i = 0; i < MAX; i++) {
                    updatePreparedStatement.setInt(1, x + i);
                    updatePreparedStatement.setInt(2, x + i);
                    updatePreparedStatement.execute();
                    if(dStatement.executeUpdate() == 0)
                        uStatement.execute();
                }
                tm = (System.nanoTime() - tm - d) / 1000000;
                printViewCount(other);
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
            TestUtil.printPlan(other, "explain delete from view A using follower B, temp C where B.user_id = C.user_id "
                    + " and A.follower_id = B.follower_id and count = 1;");
            other.executeUpdate("Delete from follower where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    
    @Test
    public void insertJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", "");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
                + " WHERE A.follower_id = B.follower_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 2;
            int x = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into follower values");
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 3; j++)
                    s.append(String.format("(%d, %d)", x + i, 2 * x + j)).append(","); 
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze follower;");
            s = new StringBuilder("INSERT into temp values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21')", x + i, x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            
            for(int k = 0; k < 10; k++) {
                other.execute("analyze view;");
                printViewCount(other);
                long tm = System.nanoTime();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
                other.execute("delete from view where follower_id >= " + x);
            }
            other.executeUpdate("Delete from follower where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void deleteJoinTuplesThroughput() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaura");
            Statement other = connection.createStatement()){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            other.execute("insert into view SELECT follower_id, count(C.user_id) FROM "
                            + " follower A, tweet C WHERE A.user_id = C.user_id  group by follower_id ");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using follower B, temp C where B.user_id = C.user_id "
                    + " and A.follower_id = B.follower_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int x = 1000000000;
            StringBuilder s = new StringBuilder("INSERT into follower values");
            for(int i = 0; i < MAX; i++)
                for(int j = 0; j < 3; j++)
                    s.append(String.format("(%d, %d)", x + i, j)).append(","); 
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze follower;");
            s = new StringBuilder("INSERT into temp values");
            for(int i = 0; i < MAX; i++)
                s.append(String.format("(%d, %d, '2992-02-21')", x + i, x + i)).append(",");
            other.executeUpdate(s.deleteCharAt(s.length() - 1).toString());
            other.execute("analyze temp;");
            for(int k = 0; k < 10; k++) {
                iStatement.executeUpdate();
                other.execute("analyze view;");
                printViewCount(other);
                long tm = System.nanoTime();
                dStatement.executeUpdate();                     
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                printViewCount(other);
            }
            other.executeUpdate("Delete from follower where user_id >= " + x);
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    @Test
    public void insertProjectedTupleThroughputTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        initDriver();
        String INSERT_QUERY = "INSERT into temp values(100, %d, '2992-02-21')";
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            PreparedStatement uStatement = 
                connection.prepareStatement("update view A set count = count + ct "
                + " FROM (SELECT follower_id, count(C.user_id) as ct FROM "
                        + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id) B "
                + " WHERE A.follower_id = B.follower_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int X = 1000000000; 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                other.execute("analyze temp;");
                other.execute("analyze view;");
                long tm = System.nanoTime();
                if(uStatement.executeUpdate() == 0)
                    iStatement.execute();
                System.out.println(TestUtil.getTestName() + (System.nanoTime() - tm) / 1000000 + "/" + MAX);
                other.execute("delete from view;");
            }
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
        String INSERT_QUERY = "INSERT into temp values(100, %d, '2992-02-21')";
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen1m", "gaurav", "gaurav"); 
            Statement other = connection.createStatement() ){
            other.executeUpdate("create temporary table temp as select * from tweet limit 0;");
            createView(other);
            other.execute("insert into view SELECT follower_id, count(C.user_id) FROM "
                            + " follower A, tweet C WHERE A.user_id = C.user_id  group by follower_id ");
            PreparedStatement dStatement = 
                connection.prepareStatement("delete from view A using follower B, temp C where B.user_id = C.user_id "
                    + " and A.follower_id = B.follower_id");
            PreparedStatement iStatement = 
                connection.prepareStatement("insert into view SELECT follower_id, count(C.user_id) as ct FROM "
                    + " follower A, temp C WHERE A.user_id = C.user_id  group by follower_id");
            int MAX = 50000;
            int X = 1000000000; 
            for(int i = 0; i < MAX; i++)
                other.executeUpdate(String.format(INSERT_QUERY, X + i));
            for(int k = 0; k < 10; k++) {
                iStatement.execute();
                other.execute("analyze temp;");
                other.execute("analyze view;");
                printViewCount(other);
                long tm = System.nanoTime();
                dStatement.execute();
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
//            other.executeUpdate("create temporary table temp as select * from tweet limit 1;");
//            PreparedStatement prepareStatement = 
//                connection.prepareStatement("SELECT A.follower_id, follower_id, B.user_id as t_user_id"
//                                + ", tweet_id, tweet_date FROM follower A, temp B WHERE A.user_id = B.user_id");
//            int MAX = 5000;
//            for(int k = 0, x = 1000000; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                for(int i = 0; i < MAX; i++) {
//                    other.executeUpdate(String.format(INSERT_QUERY, "tweet", x + i));
//                }
//            }
//            PreparedStatement deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "temp"));
//            for(int k = 0, x = 1000000; k < TestUtil.WARMUP_ITERAIONS; k++, x += MAX) {
//                for(int i = 0; i < MAX; i++) {
//                    prepareStatement.setInt(1, 2);
//                    prepareStatement.setInt(2, x + i);
//                    prepareStatement.setString(2, "2992-02-21");
//                    ResultSet t = prepareStatement.executeQuery(); 
//                    while(t.next()) {
//                        for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                            Object object = t.getObject(j);
//                        }
//                    };
//                    deletePreparedStatement.setInt(1, 2);
//                    deletePreparedStatement.setInt(2, x + i);
//                    deletePreparedStatement.setString(3, "2992-02-21");
//                    deletePreparedStatement.executeUpdate();
//                }
//            }
//            deletePreparedStatement = connection.prepareStatement(String.format(DELETE_QUERY, "tweet"));            
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "temp", 1000000 + i));
//            }
//            long tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++) {
//                deletePreparedStatement.setInt(1, 2);
//                deletePreparedStatement.setInt(2, 1000000 + i);
//                deletePreparedStatement.setString(3, "2992-02-21");
//                deletePreparedStatement.executeUpdate();
//            }
//            long d = System.nanoTime() - tm;
//            System.out.println("testH2Sql:" + (d) / 1000000 + "/" + MAX);
//            for(int i = 0; i < MAX; i++) {
//                other.executeUpdate(String.format(INSERT_QUERY, "temp", 1000000 + i));
//            }
//            tm = System.nanoTime();
//            for(int i = 0; i < MAX; i++);
//            for(int i = 0; i < MAX; i++) {
//                prepareStatement.setInt(1, 2);
//                prepareStatement.setInt(2, 1000000 + i);
//                prepareStatement.setString(2, "2992-02-21");
//                ResultSet t = prepareStatement.executeQuery(); 
//                while(t.next()) {
//                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
//                        Object object = t.getObject(j);
//                    }
//                };
//                deletePreparedStatement.setInt(1, 2);
//                deletePreparedStatement.setInt(2, 1000000 + i);
//                deletePreparedStatement.setString(3, "2992-02-21");
//                deletePreparedStatement.executeUpdate();
//            }
//            System.out.println("testH2Sql:" + (System.nanoTime() - tm - d) / 1000000 + "/" + MAX);
//        } catch(SQLException e) {
//            e.printStackTrace();
//        }
//    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
