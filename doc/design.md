# Key Design/Implementation Details


PAPAS uses directed Acyclic graph structures togther with unique identifiers to keep track of the links between particles and structures.

## Identifiers

In PAPAS each cluster, track, particle etc is given a unique Identifier.  

An Identifier is a 64bit unsigned integer encoded using bit shifts:-

    - a unique id (counter 1, 2, 3 etc) and
    - the item type: ecalCluster, hcalCluster, track, particle...

Simple and can be extended or modified
￼
## Directed Acyclic Graph

In a Directed Acyclic Graph (DAG)

    •  A Node represents an item
    •  Links between Nodes are directed, no cycles
    •  Each Node can have multiple children/multiple parents

￼￼￼￼￼￼￼￼￼￼￼￼￼￼￼PAPAS includes a  DirectedAcyclicTool library that provides (templated) classes/ algorithms & allows graph to be traversed, subgraphs to be identified

## PAPAS approach

•  Each item of interest has an Id eg Cluster, Particle, Track

•  Store items in collections (dict /unordered_map), indexed on Id

•  Separate collections for different types (EcalClusters, Tracks etc)

•  For each item we also create a Node, templated on Id

•  Each Node is stored in a Nodes collection, indexed on Id

•  The history and/or links between items are stored in the Nodes eg nodeParticle.addChild(nodeCluster) !

Given the collections and the history we can find out how a reconstructed particle came about.

##Other resources

Powerpoint talk decribing Directed Acyclic Tool (DAG):

https://indico.cern.ch/event/490466/contributions/1168040/attachments/1218006/1779467/DAGtool.pdf

Powerpoint talk pdf for PAPAS implementation:

https://indico.cern.ch/event/547855/contributions/2221423/attachments/1300412/1941158/Papas_cpp3.pdf

 