package org.ucsd.db.fastview.datamodel;

public class AggregatedAttribute {
    public final Function function;
    public final int index;
    public final int viewIndex;
    public AggregatedAttribute(Function function, int index, int viewIndex) {
        super();
        this.function = function;
        this.index = index;
        this.viewIndex = viewIndex;
    }
}
