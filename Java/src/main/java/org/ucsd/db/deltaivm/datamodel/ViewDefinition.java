package org.ucsd.db.deltaivm.datamodel;

import org.ucsd.db.fastview.index.IntHashMap;

public class ViewDefinition {
    private IntHashMap<String> projectedColumnOffsetMap = new IntHashMap<String>();

    public IntHashMap<String> getProjectedColumnOffsetMap() {
        return projectedColumnOffsetMap;
    }

    public void setProjectedColumnOffsetMap(IntHashMap<String> projectedColumnOffsetMap) {
        this.projectedColumnOffsetMap = projectedColumnOffsetMap;
    }
}