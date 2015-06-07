package org.ucsd.db.fastview.index;

import org.ucsd.db.fastview.datamodel.VertexValue;

public interface IIndex {
	IndexLeaf getLeaf(VertexValue vertexValue);
	IndexLeaf add(VertexValue vertexValue, String relationName, int joinTupleIndex);
	IndexLeaf delete(VertexValue vertexValue, String relationName, int joinTupleIndex);
}
