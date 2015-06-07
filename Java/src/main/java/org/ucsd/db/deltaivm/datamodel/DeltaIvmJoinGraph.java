package org.ucsd.db.deltaivm.datamodel;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class DeltaIvmJoinGraph {
    private class Node {
        private String relationName;
        private HashMap<String, Node> vertexAliasToNodeMap = new HashMap<>();
    }
    private HashMap<String, Node> relationNameToNodeMap = new HashMap<String, Node>();
    private HashMap<String, Node> aliasToNodeMap = new HashMap<String, Node>();
    
    public void add(String alias, String relationName) {
        Node node = relationNameToNodeMap.get(relationName);
        if(node == null) {
            node = new Node();
            node.relationName = relationName;
            relationNameToNodeMap.put(node.relationName, node);
        }
        Node joinNode = aliasToNodeMap.get(alias);
        aliasToNodeMap.put(alias, node);
        if(joinNode != null) {
            node.vertexAliasToNodeMap.put(alias, joinNode);
            joinNode.vertexAliasToNodeMap.put(alias, node);
        }
            
    }
    public List<String> getJoinRelations(String relationName) {
        List<String> list = new ArrayList<>();
        Node node = relationNameToNodeMap.get(relationName);
        if(node != null)
            for(Node i : node.vertexAliasToNodeMap.values())
                list.add(i.relationName);
        return list;
    }
}
