package org.ucsd.db.deltaivm.datamodel;

import java.util.HashMap;

import org.ucsd.db.fastview.index.IIndex;

public class Database {
    private HashMap<Enum<? extends Enum<?>>, IIndex> indexes = new HashMap<>();
    private HashMap<String, RelationalTable> relationalTables = new HashMap<>();
    private DeltaIvmJoinGraph joinGraph;
    private ViewDefinition viewDefinition = new ViewDefinition();
    
    public DeltaIvmJoinGraph getJoinGraph() {
        return joinGraph;
    }

    public void setJoinGraph(DeltaIvmJoinGraph joinGraph) {
        this.joinGraph = joinGraph;
    }

    public HashMap<Enum<? extends Enum<?>>, IIndex> getIndexes() {
        return indexes;
    }

    public void add(String relation, RelationalTable relationalTable) {
        relationalTables.put(relation, relationalTable);
    }
    
    public RelationalTable getRelation(String relationName) {
        return relationalTables.get(relationName);
    }

    public ViewDefinition getViewDefinition() {
        return viewDefinition;
    }

    public void setViewDefinition(ViewDefinition viewDefinition) {
        this.viewDefinition = viewDefinition;
    }

    public int getNumberOfRelations() {
        return relationalTables.size();
    }
}
