package libraries.dataStructures.graphs;

/** UndirectedGraph class: implementation of an Undirected graph
 *  (weighted or unweighted) through Adjacency Lists:<br>
 *  an Undirected graph IS A Directed Graph such that if the Edge (i, j)
 *  is present in i's Adjacency List, then the Edge (j, i) should also
 *  be present in j's List.
 *
 * @version December 2019
 */
public class UndirectedGraph extends DirectedGraph {

    /** Creates an empty Undirected graph with nV vertices.
     *  @param numV  Number of vertices in the graph
     */
    public UndirectedGraph(int numV) {
        super(numV);
        isDirected = false;
    }

    /** If it isn't in the graph, adds the edge (i, j) to an Undirected
     *  Unweighted graph, at the end of the list of adjacent vertices of i.
     *  It also adds edge (j, i).
     *
     * @param i    Source vertex
     * @param j    Target vertex
     */
    public void addEdge(int i, int j) {
        if (!containsEdge(i, j)) {
            theArray[i].add(new Adjacent(j, 1));
            theArray[j].add(new Adjacent(i, 1));
            numE++;
        }
    }

    /** If it isn't in the graph, adds the edge (i, j) with weight w to an
     *  Undirected Weighted graph, at the end of the list of adjacent vertices of i.
     *  It also adds edge (j, i) with the same weight.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     * @param w    Weight of the edge (i, j)
     */
    public void addEdge(int i, int j, double w) {
        if (!containsEdge(i, j)) {
            theArray[i].add(new Adjacent(j, w));
            theArray[j].add(new Adjacent(i, w));
            numE++;
        }
    }
}
