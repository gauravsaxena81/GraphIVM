package org.ucsd.db.deltaivm.tableloader;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import org.ucsd.db.deltaivm.datamodel.Database;
import org.ucsd.db.deltaivm.datamodel.DeltaIvmRelationDefinition;
import org.ucsd.db.deltaivm.datamodel.IndexedVertex;
import org.ucsd.db.deltaivm.datamodel.RelationalTable;
import org.ucsd.db.fastview.datamodel.VertexValue;
import org.ucsd.db.fastview.delta.FastViewUtil;
import org.ucsd.db.fastview.index.HashIndex;
import org.ucsd.db.fastview.index.IIndex;
import org.ucsd.db.fastview.index.IntArrayList;

public class DeltaIvmSqlTableLoader {
    public <E extends Enum<E>> RelationalTable loadTable(Database database, ResultSet t, DeltaIvmRelationDefinition joinTupleDefinition) 
        throws SQLException {
        HashMap<Enum<? extends Enum<?>>, IIndex> aliasToIndexMap = database.getIndexes();
        RelationalTable relationalTable = new RelationalTable();
        database.add(joinTupleDefinition.getRelation(), relationalTable);
        relationalTable.setIndexedVertices(joinTupleDefinition.getIndexedVertices());
        int rowNum = -1;
        while(t.next()) {
            rowNum++;
            Object[] row = new Object[t.getMetaData().getColumnCount()];
            for(int i = 0; i < t.getMetaData().getColumnCount(); i++) {
                row[i] = t.getObject(i + 1);
                if(t.wasNull())
                    row[i] = null;
            }
            IntArrayList list = relationalTable.getProjectedTupleIndex().get(row);
            if(list == null) {
                list = new IntArrayList();
                relationalTable.getProjectedTupleIndex().put(row, list);
            }
            list.add(relationalTable.getTable().size());
            
            relationalTable.getTable().add(row);
            
            for(IndexedVertex v : joinTupleDefinition.getIndexedVertices()) {
                IIndex index = aliasToIndexMap.get(v.getAlias());
                if(index == null) {
                    index = new HashIndex();
                    aliasToIndexMap.put(v.getAlias(), index);
                }
                VertexValue vertexValue = new VertexValue();
                List<Object> joinAttributeValues = new ArrayList<>();
                vertexValue.setAttributeValues(joinAttributeValues);
                vertexValue.setAlias(v.getAlias());
                for(int i = 0; i < v.getJoinAttributeIndices().size(); i++) {
                    joinAttributeValues.add(t.getObject(v.getJoinAttributeIndices().get(i) + 1));
                }
                index.add(vertexValue, joinTupleDefinition.getRelation(), rowNum);
            }
        }
        return relationalTable;
    }
}
