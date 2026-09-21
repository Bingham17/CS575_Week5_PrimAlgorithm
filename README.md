# CS575_Week5_PrimAlgorithm

Input for this program will be the number of vertices and the number of edges, followed by lines that have three numbers.  This will be the vertex numbers for two vertices, and then the length of the edge.  All vertices will be numbered from 0 to (num_vertices - 1), and the graphs will be connected.

Your program should print out the total length of the minimum spanning tree, followed by the edges that are in the tree.  For example:

4 5
0 1 10
1 2 5
2 3 3
3 0 4
0 3 55
There are four vertices, and five edges. The best tree use the 1:2 edge, the 2:3 edge, and the 3:0 edge, for a total length of 12. Your program should print out...

12
1 2 5
2 3 3
3 0 4
The order that you print the edges is not important, and some input files might have different trees of equal length.
