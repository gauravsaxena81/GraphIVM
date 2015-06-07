package org.ucsd.db.fastview.datamodel;

import java.util.List;

public class Vertex<E extends Enum> {
    private List<String> joinAttributes;
    private E alias;
    private int hashCode;

    public List<String> getJoinAttributes() {
        return joinAttributes;
    }

    public void setJoinAttributes(List<String> joinAttributes) {
        this.joinAttributes = joinAttributes;
    }
    @Override
    public int hashCode() {
        if(hashCode == 0) 
            for(String i : joinAttributes)
                hashCode += hashCode * 31 + i.hashCode();
        return hashCode;
    }
    @Override
    public boolean equals(Object o) {
        if(o != null && o instanceof Vertex)
            return joinAttributes.equals(((Vertex)o).joinAttributes);
        else
            return false;
    }
    @Override
    public String toString() {
        return joinAttributes.toString();
    }

    public E getAlias() {
        return alias;
    }

    public void setAlias(E alias) {
        this.alias = alias;
    }
}
