package org.ucsd.db.view;

import java.util.HashMap;

public class ViewMaintainer {
    private HashMap<Wrapper, int[]> projectedTupleIndex = new HashMap<>();
    //private static final int MAP_SIZE = 2 << 10;
    //private ArrayList<Wrapper>[] map = new ArrayList[MAP_SIZE];
    private int[] attributeIndices;
    
    public ViewMaintainer(int[] attributeIndices) {
        this.attributeIndices = attributeIndices;
        //for(int i = 0; i < map.length; i++)
            //map[i] = new ArrayList<ViewMaintainer.Wrapper>();
    }
    
    public void insertTuple(Object[] tuple) {
        projectedTupleIndex.put(new Wrapper(tuple), new int[]{1});
    }
    public void deleteTuple(Object[] tuple) {
        projectedTupleIndex.remove(new Wrapper(tuple));
    }
    private class Wrapper {
        Object[] tuple;
        private int h;
        public Wrapper(){}
        
        public Wrapper(Object[] tuple){
            init(tuple);
        }
        void init(Object[] tuple) {
            this.tuple = tuple;
            int hh = 1;
            for(int i : attributeIndices)
                if(tuple[i] != null)
                    hh += hh * 31 + tuple[i].hashCode();
                else
                    System.out.println();
            h = hh;
        }
        public int hashCode() {
            return h;
        }
        public boolean equals(Object o) {
            Wrapper other = (Wrapper)o;
            for(int i : attributeIndices) {
                if(!tuple[i].equals(other.tuple[i]))
                    return false;
            }
            return true;
        }
    }
    Wrapper wrapper = new Wrapper();
    public void applyTuple(Object[] tuple, int[] aggregatedAttributes, int posNeg) {
        wrapper.init(tuple);
        int[] value = projectedTupleIndex.get(wrapper);
        if(value == null) {
            value = new int[aggregatedAttributes.length + 1];
            Wrapper saveTuple = new Wrapper();
            saveTuple.init(tuple);
            projectedTupleIndex.put(saveTuple, value);
        } else {
            for(int i : aggregatedAttributes) {
                value[i] = value[i] + posNeg * (int)tuple[i];
            }
            value[value.length - 1] += posNeg;
            if(value[value.length - 1] == 0)
                projectedTupleIndex.remove(wrapper);
        }
    }
//    public void applyTuple1(Object[] tuple, int[] aggregatedAttributes, int posNeg) {
//        wrapper.init(tuple);
//        ArrayList<Wrapper> list = map[wrapper.h & (MAP_SIZE)];
//        int[] value = null;
//        if(list.isEmpty()){
//            Wrapper saveTuple = new Wrapper();
//            saveTuple.init(tuple);
//            saveTuple.value = value = new int[aggregatedAttributes.length + 1];
//            list.add(saveTuple);
//        } else {
//            for(Wrapper i : list)
//                if(i.equals(wrapper))
//                    value = i.value;
//        }
//        for(int i : aggregatedAttributes) {
//            value[i] = value[i] + posNeg * (int)tuple[i];
//        }
//            
//    }
}