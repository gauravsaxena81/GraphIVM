package org.ucsd.db.bsmagen;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

import org.junit.Test;

public class PostgresOverhead {
    @Test
    public void netwrokOverheadTest() throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException {
        initDriver();
        try(Connection connection = DriverManager.getConnection("jdbc:postgresql://localhost:5432/bsmagen", "gaurav", ""); 
            PreparedStatement other = connection.prepareStatement("SELECT 1") ){
            int MAX = 50000;
            for(int j = 0; j < 10; j++) {
                long tm = System.nanoTime();;
                for(int i = 0; i < MAX; i++)
                    other.executeQuery();
                tm = (System.nanoTime() - tm) / 1000000;
                System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
            }
            long tm = System.nanoTime();;
            for(int i = 0; i < MAX; i++)
                other.execute("SELECT 1");
            tm = (System.nanoTime() - tm) / 1000000;
            System.out.println(TestUtil.getTestName() + tm + "/" + MAX);
        }
    }
    private void initDriver() throws InstantiationException, IllegalAccessException, ClassNotFoundException {
        Class.forName("org.postgresql.Driver").newInstance();        
    }
}
