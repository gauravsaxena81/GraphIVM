package org.ucsd.db.outerjointable;

import java.util.List;

import org.ucsd.db.fastview.datamodel.VertexValue;

public class Tuple {
    private List<VertexValue> vertexValues;
    private List<Object> projectedAttributes;
    private String relationName;
    
    public List<Object> getProjectedAttributes() {
        return projectedAttributes;
    }
    public void setProjectedAttributes(List<Object> projectedAttributes) {
        this.projectedAttributes = projectedAttributes;
    }
    public String getRelationName() {
        return relationName;
    }
    public void setRelationName(String relationName) {
        this.relationName = relationName;
    }
    public List<VertexValue> getVertexValues() {
        return vertexValues;
    }
    public void setVertexValues(List<VertexValue> vertexValues) {
        this.vertexValues = vertexValues;
    }
}
