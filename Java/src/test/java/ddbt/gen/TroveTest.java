package ddbt.gen;

import java.util.ArrayList;

import gnu.trove.list.array.TIntArrayList;
import net.sf.ehcache.pool.sizeof.AgentSizeOf;

import org.junit.Test;
import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.fastview.index.IntArrayList;

public class TroveTest {
    @Test
    public void testTrove() {
        int MAX = 100000;
        for(int j = 0; j < 20; j++) {
            System.out.println("Trove");
            TIntArrayList x = new TIntArrayList();
            long t = System.nanoTime();
            for(int i = 0; i < MAX; i++) {
                x.add(i);
            }
            System.out.println(System.nanoTime() - t);
            TestUtil.printSize(x);
            System.out.println("Me");
            t = System.nanoTime();
            IntArrayList y = new IntArrayList();
            for(int i = 0; i < MAX; i++) {
                y.add(i);
            }
            System.out.println(System.nanoTime() - t);
            TestUtil.printSize(y);
        }
    }
    @Test
    public void testArray() {
        int MAX = 100000;
        for(int j = 0; j < 20; j++) {
            System.out.println("Array");
            int[] x = new int[MAX];
            long t = System.nanoTime();
            for(int i = 0; i < MAX - 2; i++) {
                x[i] = x[i+1] + + x[i+2] + i;
            }
            System.out.println(System.nanoTime() - t);
            TestUtil.printSize(x);
            System.out.println("Me");
            t = System.nanoTime();
            IntArrayList y = new IntArrayList();
            for(int i = 0; i < MAX; i++) {
                y.add(i);
            }
            System.out.println(System.nanoTime() - t);
            TestUtil.printSize(y);
        }
    }
    @Test
    public void testArraySize() {
        int MAX = 100000;
        System.out.println("Array");
        ArrayList<int[]> a = new ArrayList<int[]>();
        for(int i = 0; i < MAX - 2; i++) {
            a.add(new int[2]);
        }
        TestUtil.printSize(a);
        System.out.println("Me");
        ArrayList<IntArrayList> b = new ArrayList<IntArrayList>();
        
        for(int i = 0; i < MAX; i++) {
            IntArrayList y = new IntArrayList();
            y.add(1);
            y.add(2);
            b.add(y);
        }
        TestUtil.printSize(b);
    }
    @Test
    public void testArraySize2() {
        TestUtil.printSize("Gaurav");
        TestUtil.printSize("Gaurav".getBytes());
        TestUtil.printSize("Gaurav".toCharArray());
    }
}
