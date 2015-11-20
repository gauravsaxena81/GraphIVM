# GraphIVM

## Prototype implemetation of graph based representation of materiliazed views for low latency updates

### Abstract
Incremental View Maintenance (IVM) is the process of incrementally maintaining the view when the underlying data change. Given the high fre- quency of data modifications in many practical scenarios, it is imperative that an IVM approach is as efficient as possible. One technique commonly used to ac- celerate IVM is the materialization of a set of additional auxiliary views, which can be leveraged to speedup the maintenance of the original view. However, existing approaches assume that these auxiliary views are relational tables. We argue that this assumption creates both space and time inefficien- cies by introducing redundancies that would have been avoided if the auxiliary views were stored in a non-relational format. Based on this observation, we propose a novel non-relational auxiliary view, referred to as the join graph, and a corresponding GraphIVM system, which leverages the join graph to acceler- ate incremental view maintenance. The join graph, which intuitively represents how tuples of the underlying database join with each other, is shown to be com- pact and non-redundant, leading to an efficient IVM approach. This approach also benefits from two additional optimizations, described in the paper, that allow it to further speedup the IVM process. Experiments of the GraphIVM system against state of the art IVM approaches verify that in all cases, but ex- tremely simple views, GraphIVM significantly outperforms state of the art IVM approaches. More importantly, its speedup over other approaches increases as the views become more complex (measured in terms of fanout and number of
joins).

### Detailed description and experiments
http://www.slideshare.net/gsaxena81/graphivm-accelerating-ivmthrough-nonrelational-caching-49866935
