package org.ucsd.db.fastview.datamodel;

import java.util.List;

public class LeafTuple {
    private List<Object> projectedAttributeValues;//change to an array
    private int index;

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public LeafTuple(List<Object> projectedAttributeValues) {
        this.projectedAttributeValues = projectedAttributeValues;
    }


    public List<Object> getProjectedAttributeValues() {
        return projectedAttributeValues;
    }

    public void setProjectedAttributeValues(List<Object> projectedAttributeValues) {
        this.projectedAttributeValues = projectedAttributeValues;
    }
    @Override
    public int hashCode() {
        return projectedAttributeValues.hashCode();
    }
    @Override
    public boolean equals(Object o) {
        if(o instanceof LeafTuple)
            return o == this || ((LeafTuple)o).projectedAttributeValues.equals(this.projectedAttributeValues);
        else
            return false;
    }
}