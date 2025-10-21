package libraries.dataStructures.graphs;

import libraries.dataStructures.models.ListPOI;
import libraries.dataStructures.linear.LinkedListPOI;

/** Implementation of a Directed graph (weighted or unweighted)
 *  through Adjacency Lists.<br>
 *
 * @version December 2019
 */
public class DirectedGraph extends Graph {

    protected int numV, numE;
    protected ListPOI<Adjacent>[] theArray;

    /** Creates an empty Directed graph with nV vertices.
     *  @param nV  Number of vertices in the graph
     */
    @SuppressWarnings("unchecked")
    public DirectedGraph(int nV) {
        super(true);
        numV = nV;
        numE = 0;
        theArray = new ListPOI[numV];
        for (int i = 0; i < numV; i++)
            theArray[i] = new LinkedListPOI<>();
    }

    /** Returns the number of vertices in a graph.
     *  @return int number of vertices
     */
    public int numVertices() { return numV; }

    /** Returns the number of edges in a graph.
     *  @return int number of edges
     */
    public int numEdges() { return numE; }

    /** Checks whether the edge (i, j) belongs to a graph.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     * @return boolean true if (i, j) appears in the graph, false otherwise.
     */
    public boolean containsEdge(int i, int j) {
        ListPOI<Adjacent> l = theArray[i];
        boolean found = false;
        for (l.begin(); !l.isEnd() && !found; l.next()) {
            if (l.get().target == j) { found = true; }
        }
        return found;
    }

    /** Returns the weight of the edge (i, j) in a graph, 0 if that edge
     *  is not contained in the graph.
     *  @return double Weight of the edge (i, j), or 0 if it doesn't exist.
     */
    public double edgeWeight(int i, int j) {
        ListPOI<Adjacent> l = theArray[i];
        for (l.begin(); !l.isEnd(); l.next()) {
            if (l.get().target == j) {
                return l.get().weight;
            }
        }
        return 0.0;
    }

    /** If it isn't in the graph, adds the edge (i, j) to an Unweighted graph,
     *  at the end of the list of adjacent vertices of i.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     */
    public void addEdge(int i, int j) {
        if (!containsEdge(i, j)) {
            theArray[i].add(new Adjacent(j, 1));
            numE++;
        }
    }

    /** If it isn't in the graph, adds the edge (i, j) with weight w to a Weighted graph,
     *  at the end of the list of adjacent vertices of i.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     * @param w    Weight of the edge (i, j)
     */
    public void addEdge(int i, int j, double w) {
        if (!containsEdge(i, j)) {
            theArray[i].add(new Adjacent(j, w));
            numE++;
        }
    }

    /** Returns a ListPOI that contains vertex i's adjacent vertices.
     *
     * @param i Vertex from which adjacent vertices are looked up.
     * @return ListPOI with the vertices adjacent to i
     */
    public ListPOI<Adjacent> adjacentTo(int i) {
        return theArray[i];
    }
}
