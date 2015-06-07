package org.ucsd.db.bsmagen;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import net.sf.ehcache.pool.sizeof.AgentSizeOf;
import net.sf.ehcache.pool.sizeof.SizeOf;

import org.ucsd.db.fastview.tableloader.CntTableAndIndices;

import com.cedarsoftware.util.io.JsonReader;
import com.cedarsoftware.util.io.JsonWriter;

public class TestUtil {
    public static final int WARMUP_ITERAIONS = 20;

    public static void printPlan(Statement other, String sql) throws SQLException {
        ResultSet plan = other.executeQuery(sql);
        while(plan.next())
            System.out.println(plan.getString(1));        
    }
    public static CntTableAndIndices deserialize(String fileName) throws IOException, ClassNotFoundException {
        try (
            FileReader in = new FileReader(fileName);) {
            StringBuilder buf = new StringBuilder();
            char[] charArray = new char[1024];
            int len;
            while((len = in.read(charArray)) > -1)
                buf.append(charArray, 0, len);
            return (CntTableAndIndices) JsonReader.jsonToJava(buf.toString());
        } catch (FileNotFoundException e) {
            return null;
        }
    }

    public static void serialize(CntTableAndIndices cntTableAndIndices, String fileName) throws IOException {
        try (
            FileWriter out = new FileWriter(fileName)) {
            out.write(JsonWriter.objectToJson(cntTableAndIndices));
        }
    }
    public static void printSize(Object... o) {
        SizeOf s = new AgentSizeOf();
        System.out.println(s.deepSizeOf(1, false, o).getCalculated());
    }
    public static String getTestName() {
        StackTraceElement[] ste = Thread.currentThread().getStackTrace();
        return ste[2].getClassName() + "." + ste[2].getMethodName() + ":";
    }
    public static void loadH2Database(String database) throws SQLException {
        try(Connection connection = DriverManager.getConnection("jdbc:h2:" + database + ";QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0;", "sa", "");
            Connection connectionMem = DriverManager.getConnection("jdbc:h2:mem:" + database + ";QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0;", "sa", "");
            Statement stat = connectionMem.createStatement()) {
            
            ResultSet t = connection.createStatement().executeQuery("SCRIPT");
            int i = 0;
            while(t.next()) {
                String string = t.getString(1);
                if(i++ % 100000 == 0)
                    System.out.println(string);
                stat.execute(string);
            }
        }        
    }
    public static void checkH2Database(String database) throws SQLException {
        try(Connection connectionMem = DriverManager.getConnection("jdbc:h2:mem:" + database + ";QUERY_CACHE_SIZE=0;ANALYZE_AUTO=0;", "sa", "");
            Statement stat = connectionMem.createStatement()) {
            ResultSet t = stat.executeQuery("select count(*) from users;");
            t.next();
            System.out.println("Users:" + t.getInt(1));
            t = stat.executeQuery("select count(*) from follower;");
            t.next();
            System.out.println("Follower:" + t.getInt(1));
            t = stat.executeQuery("select count(*) from tweet;");
            t.next();
            System.out.println("Tweet:" + t.getInt(1));
            t = stat.executeQuery("select count(*) from Retweet;");
            t.next();
            System.out.println("Retweet:" + t.getInt(1));
        }
    }
    static int c = 0;
    public static void printRowCount() {
        System.out.println(c);
    }
    public static void initRowCount() {
        c = 0;
    }
    public static void iterate(ResultSet r, int x) throws SQLException {
        while(r.next()) {
            c++;
            for(int i = 1; i <= x; i++)
                r.getObject(i);
//                System.out.print(r.getObject(i)+",");
//            System.out.println();
        }
    }
}