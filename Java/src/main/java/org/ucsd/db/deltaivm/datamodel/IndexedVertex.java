package org.ucsd.db.deltaivm.datamodel;

import org.ucsd.db.fastview.index.IntArrayList;

public class IndexedVertex<E extends Enum<E>> {
    private IntArrayList joinAttributeIndices;
    private E alias;
    private int hashCode;

   
    public IntArrayList getJoinAttributeIndices() {
        return joinAttributeIndices;
    }
    public void setJoinAttributeIndices(IntArrayList joinAttributeIndices) {
        this.joinAttributeIndices = joinAttributeIndices;
    }
    @Override
    public int hashCode() {
        if(hashCode == 0) 
            for(int i = 0; i < joinAttributeIndices.size(); i++)
                hashCode += hashCode * 31 + joinAttributeIndices.get(i);
        return hashCode;
    }
    @Override
    public boolean equals(Object o) {
        return (o != null && o instanceof IndexedVertex && alias.equals(((IndexedVertex)o).alias)
            && joinAttributeIndices.equals(((IndexedVertex)o).joinAttributeIndices));
    }
    @Override
    public String toString() {
        return alias + ":" + joinAttributeIndices.toString();
    }

    public E getAlias() {
        return alias;
    }

    public void setAlias(E alias) {
        this.alias = alias;
    }
}
