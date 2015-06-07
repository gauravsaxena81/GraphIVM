package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.junit.Test;

public class H2Overhead {
    @Test
    public void networkOverheadTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            ){
            connection.createStatement().execute("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement other = connection.prepareStatement("SELECT 1") ;
            int MAX = 50000;
            for(int j = 0; j < 10; j++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++)
                    other.execute();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++)
                other.execute();
            tm = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
        }
    }
    @Test
    public void enumerationAndNetwoekOverheadTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:h2:~/bsmagen;QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0", "sa", ""); 
            ){
            connection.createStatement().execute("create temporary table temp as select * from tweet limit 1;");
            PreparedStatement other = connection.prepareStatement("SELECT 1, 2, 3, 4") ;
            int MAX = 50000;
            for(int j = 0; j < 10; j++) {
                long tm = System.nanoTime();
                for(int i = 0; i < MAX; i++) {
                    ResultSet t = other.executeQuery();
                    while(t.next()) {
                        for(int k = 1, max = t.getMetaData().getColumnCount(); k <= max; k++) {
                            Object obj = t.getObject(k);
                        }
                    }
                }
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
            long tm = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                ResultSet t = other.executeQuery();
                while(t.next()) {
                    for(int j = 1, max = t.getMetaData().getColumnCount(); j <= max; j++) {
                        Object obj = t.getObject(j);
                    }
                }
            }
            tm = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
        }
    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.h2.Driver").newInstance();        
    }
}
