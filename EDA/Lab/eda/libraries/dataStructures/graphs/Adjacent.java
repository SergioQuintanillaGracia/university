package libraries.dataStructures.graphs;

/** Adjacent: represents a vertex adjacent to another one in a graph
 *  and the weight of the edge between them.<br>
 *
 *  @version May 2018
 */

public class Adjacent {

    protected int target;
    protected double weight;

    /** Creates the vertex v adjacent to another one in a graph
     *  and the weight of the edge between them.
     *
     * @param  t Vertex adjacent to another one
     * @param  w Weight of the edge between t and
     *           the vertex to which it is adjacent
     */
    public Adjacent(int t, double w) {
        target = t;
        weight = w;
    }

    /** Returns the vertex adjacent to another one in a graph,
     *  or target vertex of the edge between them.
     *
     * @return int  Vertex adjacent to another one
     */
    public int getTarget() { return target; }

    /** Returns the weight of an edge that connects a vertex
     *  in a graph to an adjacent one.
     *
     * @return double  Weight of the edge that connects a vertex
     *                 with an adjacent one, or 1 if the graph is unweighted.
     */
    public double getWeight() { return weight; }

    /** Returns a String that represent an adjacent vertex of
     *  a vertex in a graph and the weight of the edge between them.
     *
     * @return  String representation of an adjacent vertex
     */
    public String toString() { return target + "(" + weight + ") "; }
}
