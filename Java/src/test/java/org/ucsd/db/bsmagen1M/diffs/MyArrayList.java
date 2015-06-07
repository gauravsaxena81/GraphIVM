package org.ucsd.db.bsmagen1M.diffs;

import java.util.ArrayList;

public class MyArrayList<T> extends ArrayList<Object> {
    private int hashCode = 0;
    public MyArrayList(int i) {
        super(i);
    }
    @Override
    public int hashCode() {
        int h = hashCode;
        if(h == 0) {
            h = super.hashCode();
        }
        hashCode = h;
        return h;
    }
}
