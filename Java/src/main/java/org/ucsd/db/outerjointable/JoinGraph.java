package org.ucsd.db.outerjointable;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.ucsd.db.fastview.datamodel.Vertex;

public class JoinGraph {
    private class Node {
        private String relationName;
        private List<String> joinAttributeValues;
        private List<String> projectedAttributeValues;
        private HashMap<Vertex, Node> vertexToNodeMap = new HashMap<>();
    }
    private HashMap<String, Node> relationNameToNodeMap = new HashMap<String, Node>();
    public void add(String relationName, Map<Vertex, String> vertexToRelationNameMap
        , List<String> joinAttributeValues, List<String> projectedAttributeValues) {
        Node node = new Node();
        node.relationName = relationName;
        relationNameToNodeMap.put(node.relationName, node);
        node.joinAttributeValues = joinAttributeValues;
        node.projectedAttributeValues = projectedAttributeValues;
        for(Vertex i : vertexToRelationNameMap.keySet()) {
            Node joinNode = relationNameToNodeMap.get(vertexToRelationNameMap.get(i));
            if(joinNode == null) {
                joinNode = new Node();
                joinNode.relationName = vertexToRelationNameMap.get(i);
                relationNameToNodeMap.put(joinNode.relationName, joinNode);
            }
            node.vertexToNodeMap.put(i, joinNode);
            joinNode.vertexToNodeMap.put(i, node);
        }
    }
    public List<String> getJoinAttributeValues(String relationName) {
        Node node = relationNameToNodeMap.get(relationName);
        if(node == null)
            return Collections.EMPTY_LIST;
        else
            return node.joinAttributeValues;
    }
    public Collection<String> getProjectedAttributeValues(String relationName) {
        Node node = relationNameToNodeMap.get(relationName);
        if(node == null)
            return Collections.EMPTY_LIST;
        else
            return node.projectedAttributeValues;
    }
    public List<String> getJoinRelations(String relationName) {
        List<String> list = new ArrayList<>();
        Node node = relationNameToNodeMap.get(relationName);
        if(node != null)
            for(Node i : node.vertexToNodeMap.values())
                list.add(i.relationName);
        return list;
    }
    public Collection<Vertex> getVertices(String relationName) {
        Node node = relationNameToNodeMap.get(relationName);
        if(node != null) {
            return node.vertexToNodeMap.keySet();
        }
        return Collections.EMPTY_LIST;
    }
}
