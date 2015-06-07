package org.ucsd.db.outerjointable;

import java.util.HashMap;

import org.ucsd.db.fastview.index.IIndex;

public class OuterJoinTableLoaderResult {
    public OuterJoinTable table;
    public HashMap<String, IIndex> aliasToIndexMap;
}
