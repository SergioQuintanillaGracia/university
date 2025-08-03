package libraries.dataStructures.graphs;

import libraries.dataStructures.models.Queue;
import libraries.dataStructures.models.ListPOI;
import libraries.dataStructures.linear.ArrayQueue;

// IN THE SECOND SESSION: include the following import statements:
import libraries.dataStructures.models.UFSet;
import libraries.dataStructures.hierarchical.ForestUFSet;
import libraries.dataStructures.models.PriorityQueue;
import libraries.dataStructures.hierarchical.BinaryHeap;

/** Abstract Graph class: basis for the Graph hierarchy, which
 *  defines a graph's behaviour.<br>
 *  It isn't an interface because it includes both the code for the operations
 *  of a graph that are independent both of its type as of their implementation.<br>
 *
 *  @version December 2018
 */

public abstract class Graph {

    protected boolean isDirected; // Indicates whether a graph is Directed or not
    protected int[] visited;      // Nodes visited in a traversal
    protected int visitOrder;     // Order in which nodes are visited in a traversal
    protected Queue<Integer> q;   // Queue for a BFS traversal

    /** Creates and empty graph, Directed if directed is true
     *  or Undirected otherwise.
     *
     * @param directed The type of graph, Directed or Undirected
     */
    public Graph(boolean directed) { isDirected = directed; }

    /** Checks whether a graph is directed or not.
     *
     * @return boolean true if the graph is Directed, false if it is Undirected
     */
    public boolean isDirected() { return isDirected; }

    /** Returns the number of vertices in a graph.
     *
     * @return int number of vertices
     */
    public abstract int numVertices();

    /** Returns the number of edges in a graph.
     *
     * @return int number of edges
     */
    public abstract int numEdges();

    /** Checks whether the edge (i, j) belongs to a graph.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     * @return boolean true if (i, j) appears in the graph, false otherwise.
     */
    public abstract boolean containsEdge(int i, int j);

    /** Returns the weight of the edge (i, j) in a graph, 0 if that edge
     *  is not contained in the graph.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     * @return double Weight of the edge (i, j), or 0 if it doesn't exist.
     */
    public abstract double edgeWeight(int i, int j);

    /** If it isn't in the graph, adds the edge (i, j) to an Unweighted graph.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     */
    public abstract void addEdge(int i, int j);

    /** If it isn't in the graph, adds the edge (i, j) with weight w to a Weighted graph.
     *
     * @param i    Source vertex
     * @param j    Target vertex
     * @param w    Weight of the edge (i, j)
     */
    public abstract void addEdge(int i, int j, double w);

    /** Returns a ListPOI that contains vertex i's adjacent vertices.
     *
     * @param i Vertex from which adjacent vertices are looked up.
     * @return ListPOI with the vertices adjacent to i
     */
    public abstract ListPOI<Adjacent> adjacentTo(int i);

    /** Returns a String with each of the vertices of a graph
     *  and their adjacent vertices, in insertion order.
     *
     * @return  String representing a Graph
     */
    public String toString() {
        StringBuilder res = new StringBuilder();
        for (int  i = 0; i < numVertices(); i++) {
            res.append("Vertex: ").append(i);
            ListPOI<Adjacent> l = adjacentTo(i);
            res.append(l.isEmpty() ? " without" : " with").append(" adjacents");
            for (l.begin(); !l.isEnd(); l.next()) {
                res.append(" ").append(l.get());
            }
            res.append("\n");
        }
        return res.toString();
    }

    /** Returns an array with each of the vertices of a graph and their
     *  adjacent vertices, in BFS order.
     *
     * @return  Vertex array visited in a BFS traversal
     */
    public int[] toArrayBFS() {
        int[] res = new int[numVertices()];
        visited = new int[numVertices()];
        visitOrder = 0;
        q = new ArrayQueue<>();
        for (int  i = 0; i < numVertices(); i++) {
            if (visited[i] == 0) {
                toArrayBFS(i, res);
            }
        }
        return res;
    }

    // BFS traversal from the source vertex, which stores its result in res
    protected void toArrayBFS(int origin, int[] res) {
        res[visitOrder++] = origin;
        visited[origin] = 1;
        q.enqueue(origin);
        while (!q.isEmpty()) {
            int u = q.dequeue();
            ListPOI<Adjacent> l = adjacentTo(u);
            for (l.begin(); !l.isEnd(); l.next()) {
                Adjacent a = l.get();
                if (visited[a.target] == 0) {
                    res[visitOrder++] = a.target;
                    visited[a.target] = 1;
                    q.enqueue(a.target);
                }
            }
        }
    }

    /** PRECONDITION: !this.isDirected()
     *  Returns a subset of edges that connect all vertices in an Undirected
     *  and Connected graph, or null if the graph is Unconnected.
     *
     * @return Edge[], array with the numV - 1 edges that connect the numV
     *         vertices of the graph, or null if the graph is Unconnected.
     */
    public Edge[] bfsSpanningTree() {
        // A spanning tree has as many edges as the number of vertices - 1
        Edge[] res = new Edge[numVertices() - 1];
        // Array that stores a 0 for vertices that haven't been visited, and a 1 for
        // the ones visited
        visited = new int[numVertices()];
        visitOrder = 0;
        // Queue that stores the vertices discovered that need to be explored
        q = new ArrayQueue<>();
        
        if (!bfsSpanningTree(0, res)) {
            // If the check returns false, the graph is not complete, return null
            return null;
        }
        
        // The graph is complete, return the edges that form a spanning tree
        return res;
    }
    
    protected boolean bfsSpanningTree(int origin, Edge[] res) {
        // Set the first node as visited and enqueue it to start exploring from it
        visited[origin] = 1;
        q.enqueue(origin);
        
        while (!q.isEmpty()) {  // While there are vertices pending to be explored
            // Get the next vertex to be explored and get the list of adjacent vertices to it
            int u = q.dequeue();
            ListPOI<Adjacent> l = adjacentTo(u);
            for (l.begin(); !l.isEnd(); l.next()) {  // For every adjacent vertex
                Adjacent a = l.get();
                // If the vertex hasn't been visited yet, add the edge between u and this
                // vertex to res, mark the vertex as visited, and enqueue it so it is explored
                // in the future
                if (visited[a.target] == 0) {
                    res[visitOrder++] = new Edge(u, a.getTarget(), a.getWeight());
                    visited[a.target] = 1;
                    q.enqueue(a.target);
                }
            }
        }
        
        // Check if every vertex has been visited
        // If so, return true (the graph is connected), else, return false (the graph is unconnected)
        for (int i = 0; i < numVertices(); i++) {
            if (visited[i] == 0) {
                return false;
            }
        }
        
        return true;
    }

    /** PRECONDITION: !this.isDirected()
     * Returns a subset of edges that, with minimal cost, connect all the
     * vertices of an Undirected and Connected graph, or null if the graph
     * is Unconnected.
     *
     * @return Edge[], array with the numV - 1 edges that connect the numV
     *         vertices with minimum cost, or null if the graph is Unconnected
     */
    public Edge[] kruskal() {
        PriorityQueue<Edge> feasibleEdges = new BinaryHeap<>();
        Edge[] res = new Edge[numVertices() - 1];
        int index = 0;  // Cardinality
        
        // Fill feasible edges with every edge
        for (int v = 0; v < numVertices(); v++) {
            ListPOI<Adjacent> adj = adjacentTo(v);
            
            for (adj.begin(); !adj.isEnd(); adj.next()) {
                Adjacent a = adj.get();
                feasibleEdges.add(new Edge(v, a.getTarget(), a.getWeight()));
            }
        }
        
        // It generates some sort of "hashmap" from int to int. It links
        // 0 (node 0) to 0 (class 0), 1 (node 1) to 1 (class 1)...
        UFSet set = new ForestUFSet(numVertices());
        
        while (index < numVertices() - 1 && !feasibleEdges.isEmpty()) {
            Edge e = feasibleEdges.removeMin();
            
            // Add the edge only if it doesn't form a cycle
            if (set.find(e.getSource()) != set.find(e.getTarget())) {
                set.union(set.find(e.getSource()), set.find(e.getTarget()));
                res[index++] = e;
            }
        }
        
        if (index == numVertices() - 1) {
            return res;
        }
        
        return null;
    }
    
    public boolean graphMethod(int n, int p) {
        UFSet set = new ForestUFSet(numVertices());
        // Initially, as many connected components as vertices
        int componentCount = numVertices();
        
        for (int v = 0; v < numVertices(); v++) {
            // For every vertex, get its adjacents
            ListPOI<Adjacent> adj = adjacentTo(v);
            
            for (adj.begin(); !adj.isEnd(); adj.next()) {
                int u = adj.get().getTarget();
                double w = adj.get().getWeight();
                
                // For every vertex u adjacent to v, only take into account the
                // edge [v, u] if its weight is >= p, and if creating the union
                // doesn't form a cycle
                if (w >= p && set.find(v) != set.find(u)) {
                    // Do the union of v and u in the set, and decrease componentCount
                    set.union(set.find(v), set.find(u));
                    componentCount--;
                    
                    // Stop the method if there are already less than n components
                    if (componentCount < n) return false;
                }
            }
        }
        
        return componentCount == n;
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}
