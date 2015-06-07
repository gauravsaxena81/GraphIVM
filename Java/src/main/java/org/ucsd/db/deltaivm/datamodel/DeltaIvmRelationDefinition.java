package org.ucsd.db.deltaivm.datamodel;

import java.util.List;

public class DeltaIvmRelationDefinition {

    private String relation;
    private List<IndexedVertex<? extends Enum<?>>> indexedVertices;

    public void setRelation(String relation) {
        this.relation = relation;
    }

    public String getRelation() {
        return relation;
    }

    public List<IndexedVertex<? extends Enum<?>>> getIndexedVertices() {
        return indexedVertices;
    }

    public void setIndexedVertices(List<IndexedVertex<? extends Enum<?>>> indexedVertices) {
        this.indexedVertices = indexedVertices;
    }
}