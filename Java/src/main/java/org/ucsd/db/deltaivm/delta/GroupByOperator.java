package org.ucsd.db.deltaivm.delta;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;

import org.ucsd.db.fastview.datamodel.AggregatedAttribute;
import org.ucsd.db.fastview.datamodel.TableDefinition;

public class GroupByOperator {
    public List<Object[]> groupBy(List<Object[]> table, Collection<AggregatedAttribute> aggregatedAttributes, List<Integer> groupByAttributes) {
        if(table.size() == 1)
            return table;
        HashMap<List<Object>, int[]> valueMap = new HashMap<>();
        for(int i = 0; i < table.size(); i++) {
            List<Object> list = new ArrayList<Object>();
            for(int j : groupByAttributes) {
                list.add(table.get(i)[j]);
            }
            int[] values = valueMap.get(list);
            if(values == null) {
                values = new int[aggregatedAttributes.size()];
                valueMap.put(list, values);
            }
            int j = 0;
            for(AggregatedAttribute aggregatedAttribute : aggregatedAttributes) {
//                if(aggregatedAttribute.function == Function.COUNT)
//                    values[j]++;
//                else if(aggregatedAttribute.function == Function.SUM)
                    values[j] += (int)table.get(i)[aggregatedAttribute.viewIndex];
            }
        }
        List<Object[]> aggregatedTable = new ArrayList<>();
        for(List<Object> i : valueMap.keySet()) {
            for(int j : valueMap.get(i))
                i.add(j);
            aggregatedTable.add(i.toArray());
        }
        return aggregatedTable;
    }
    public void convertToGroupedTuple(List<Object> projectedAttributeValues, TableDefinition tableDefinition
        , List<String> projectedAttributeNamesList) {
        for(int i = 0; i < projectedAttributeValues.size(); i++) {
            AggregatedAttribute aggregatedAttribute = tableDefinition.getAggregatedAttribute(projectedAttributeNamesList.get(i));
            if(aggregatedAttribute != null) {
                switch(aggregatedAttribute.function) {
                    case SUM:
                        break;
                    case COUNT:
                        projectedAttributeValues.set(i, 1);
                        break;
                }
            }
        }
    }
}
