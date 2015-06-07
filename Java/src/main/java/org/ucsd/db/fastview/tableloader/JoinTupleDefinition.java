package org.ucsd.db.fastview.tableloader;

import java.util.Collections;
import java.util.List;
import java.util.Map;

import org.ucsd.db.fastview.datamodel.AggregatedAttribute;
import org.ucsd.db.fastview.datamodel.Vertex;

public class JoinTupleDefinition<E extends Enum<E>> {
    private Map<String, AggregatedAttribute> aggregatedAttributes;
    private String relation;
    private List<Vertex<E>> vertices;
    private List<String> projectedAttributeNamesList;
    public AggregatedAttribute getAggregatedAttribute(String aggregatedAttribute) {
        return aggregatedAttributes.get(aggregatedAttribute);
    }
	public String getRelation() {
		return relation;
	}
	public void setRelation(String relation) {
		this.relation = relation;
	}
	public List<Vertex<E>> getVertices() {
        return vertices;
    }
    public void setVertices(List<Vertex<E>> vertices2) {
        this.vertices = vertices2;
    }
    public List<String> getProjectedAttributeNamesList() {
	    if(projectedAttributeNamesList == null)
	        return Collections.EMPTY_LIST;
	    else
	        return projectedAttributeNamesList;
	}
	public void setProjectedAttributeNamesList(
			List<String> projectedAttributeNamesList) {
		this.projectedAttributeNamesList = projectedAttributeNamesList;
	}
}