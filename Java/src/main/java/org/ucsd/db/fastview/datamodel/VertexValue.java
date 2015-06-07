package org.ucsd.db.fastview.datamodel;

import java.util.Arrays;
import java.util.List;

import org.ucsd.db.fastview.delta.FastViewUtil;

public class VertexValue<E extends Enum<E>> {
    private E alias;//move to join tuple defintion
    //TODO rename to join attribute values
    private List<Object> attributeValues;
    private int hashCode = 0;
    private JoinTuple[] firstList = new JoinTuple[1];
    private JoinTuple[] secondList = new JoinTuple[1];
    
    public JoinTuple[] getJoinTuples(int index) {
        /*
         * This is very fragile. It assumes that the indexes of the relation which could share the vertexvalues
         * always differ by 1, however the process of assigning ids is arbitrary!!
         */
        /*if((index & 1) == 0)
            return trim(firstList);
        else
            return trim(secondList);*/
        if((index & 1) == 0)
            return (firstList);
        else
            return (secondList);
    }
//    private JoinTuple[] trim(JoinTuple[] list) {
//        if(list[list.length - 1] != null)
//            return list;
//        else
//            return Arrays.copyOf(list, FastViewUtil.indexOf(list, null));
//    }
    public void addJoinTuple(int index, JoinTuple joinTuple) {
        JoinTuple[] list1;
        JoinTuple[] list2;
        if((index & 1) == 0) {
            list1 = firstList;
            list2 = secondList;
        } else {
            list1 = secondList;
            list2 = firstList;
        }
        if(list1[0] == null) {
            for(JoinTuple j : list2) {
                if(j != null)
                    j.addJoinedTuple(this);
                else
                    break;
            }
        }
        int i = FastViewUtil.indexOf(list1, null);
        if(i == -1) {
            int len = list1.length;
            list1 = ensureArraySize(list1, len);
            list1[len] = joinTuple;
            if((index & 1) == 0)
                firstList = list1;
            else
                secondList = list1;
        } else
            list1[i] = joinTuple; 
        if(list2[0] != null)
            joinTuple.addJoinedTuple(this);
        //update local knowledge
    }
    public static JoinTuple[] ensureArraySize(JoinTuple[] array, int index) {
        if(index > array.length - 1)
            return Arrays.copyOf(array, array.length * 2);
        else
            return array;
    }
    public void removeJoinTuple(int index, JoinTuple joinTuple) {
        JoinTuple[] list1;
        JoinTuple[] list2;
        if((index & 1) == 0) {
            list1 = firstList;
            list2 = secondList;
        } else {
            list1 = secondList;
            list2 = firstList;
        }
        //list1.remove(joinTuple);
        int i = FastViewUtil.indexOf(list1, joinTuple);
        if(i < list1.length - 1 && list1[i + 1] != null)
            System.arraycopy(list1, i + 1, list1, i, list1.length - i - 1);
        else
            list1[i] = null;
        //update local knowledge
        boolean empty = list1[0] == null;
        for(JoinTuple j : list2) {
            if(j != null)
                j.removeJoinedTuple(this, empty);
            else
                break;
        }
    }
//    public List<Object> getAttributeValues() {
//        return attributeValues;
//    }
    public void setAttributeValues(List<Object> attributeValues) {
        this.attributeValues = attributeValues;
    }
    
    public E getAlias() {
        return alias;
    }
    public void setAlias(E alias) {
        this.alias = alias;
    }
    public int hashCode() {
        int h = hashCode;
        if(h == 0) {
            h = 1;
            for(Object i : attributeValues)
                h = h * 31 + i.toString().hashCode();
            h += h * 31 + alias.hashCode();
            hashCode = h;
        }
        return h;
    }
    
    public boolean equals(Object o) {
        if(o instanceof VertexValue) {
            return  o == this || (alias.equals(((VertexValue)o).alias) 
                && attributeValues.equals(((VertexValue)o).attributeValues));
        } else
            return false;
    }
    @Override
    public String toString() {
        return String.format("%s:%s", alias, attributeValues.toString());
    }
}