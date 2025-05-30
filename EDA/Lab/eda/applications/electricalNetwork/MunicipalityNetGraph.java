package applications.electricalNetwork;

import libraries.dataStructures.graphs.UndirectedGraph;
import libraries.dataStructures.graphs.Graph;
import libraries.dataStructures.graphs.Adjacent;
import libraries.dataStructures.graphs.Edge;

import java.util.Scanner;
import libraries.dataStructures.models.ListPOI;
import libraries.dataStructures.linear.LinkedListPOI;
import libraries.dataStructures.models.Map;
import libraries.dataStructures.scattered.HashTable;
import java.io.File;
import java.io.FileNotFoundException;

/** MunicipalityNetGraph: labelled graph that represents an
 *  intercity electric grid with...
 *  A set of vertices labelled with the names of the
 *  municipalities in the network.
 *  A set of edges labelled with the cost in millions of euros
 *  required to refurbish the connection between each pair
 *  of municipalities of the network (vertices).
 *
 * @version (Curso 2022/23)
 */

public class MunicipalityNetGraph {

    public final static String FILES_DIR = "applications" + File.separator
                                              + "electricalNetwork" + File.separator;

    private static final int MAX_MUNICIPALITIES = 5000;

    private static final String NO_ACC_MSG = "The municipalities data file is not readable: has it been misplaced/misnamed?";
    private static final String NO_FOR_MSG = "Invalid format on line: ";
    private static final String NO_FDIS_MSG = "Electrical lines data file not found";

    // A MunicipalityNetGraph HAS A Graph to represent an electrical
    // network between multiple towns
    protected Graph graph;

    // A MunicipalityNetGraph HAS A Map that associates each vertex (int)
    // with its corresponding Municipality in a network.
    protected Map<Integer, Municipality> verticesToMunicipalities;

    // A MunicipalityNetGraph HAS A Map that associates each municipality
    // of a netework with its corresponding vertex (int).
    protected Map<Municipality, Integer> municipalitiesToVertices;

    // A MunicipalityNetGraph HAS A Map kruskalAdjacents that associates
    // each Municipality of a network with all its Adjacent ones in a
    // subnet with minimal cost, i.e. in the subnet that defines a Minimum
    // Spanning Tree of the graph
    /* TO BE COMPLETED: declare the map with Municipalities as keys and
          ListPOI<Municipalities> as values*/
    protected Map<Municipality, ListPOI<Municipality>> kruskalAdjacents;


    /** Builds the graph that represents an intercity electrical network
     *  as an UndirectedGgraph, from the data contained in two text files
     *  whose names start with the same given prefix (filePrefix): one contains
     *  the municipalities that make up the network, and another the electrical
     *  lines that connect them, including the cost to refurbish them.
     *
     *  At the same time that the graph is built, the two maps are populated,
     *  using efficiently HashTables.
     */
    public MunicipalityNetGraph(String filePrefix) {
        verticesToMunicipalities = new HashTable<Integer, Municipality>(MAX_MUNICIPALITIES);
        municipalitiesToVertices = new HashTable<Municipality, Integer>(MAX_MUNICIPALITIES);

        String townsFile = FILES_DIR + filePrefix + "_municipios.txt";
        try (Scanner in = new Scanner(new File(townsFile), "UTF-8")) {
            int vertex = 0;
            while (in.hasNext()) {
                String line = in.nextLine();
                String[] lA = line.split(";");
                String name = lA[0].toLowerCase();
                int population = Integer.parseInt(lA[1]);
                double area = Double.parseDouble(lA[2]);
                int posX = Integer.parseInt(lA[3]);
                int posY = Integer.parseInt(lA[4]);
                Municipality m = new Municipality(name, population, area, posX, posY);
                verticesToMunicipalities.put(vertex, m);
                municipalitiesToVertices.put(m, vertex);
                vertex++;
            }
            graph = new UndirectedGraph(verticesToMunicipalities.size());
            loadEdges(filePrefix);
        } catch (FileNotFoundException e) {
            System.out.println(NO_ACC_MSG);
        }
    }

    /**
     * Adds to the graph the weighted edges found in the data file
     * that contains the electrical lines and their refurbishing costs.
     * To do so, it uses the municipalitiesToVertices Map.
     */
    private void loadEdges(String filePrefix) {
        String costsFile = FILES_DIR + filePrefix + "_costes.txt";
        try (Scanner in = new Scanner(new File(costsFile), "UTF-8")) {
            while (in.hasNext()) {
                String line = in.nextLine();
                String[] costData = line.split(";");
                if (costData.length != 3) {
                    System.out.println(NO_FOR_MSG);
                    break;
                }
                String source = costData[0].trim().toLowerCase();
                String target = costData[1].trim().toLowerCase();
                double cost = Double.parseDouble(costData[2]);
                int sourceVertex = getVertex(new Municipality(source));
                int targetVertex = getVertex(new Municipality(target));
                graph.addEdge(sourceVertex, targetVertex, cost);
            }
        } catch (FileNotFoundException e) {
            System.err.println(NO_FDIS_MSG);
        }
    }

    /** Returns the number of municipalities in an electrical network,
     *  or the number of vertices in the graph representing it.
     */
    public int numVertices() { return graph.numVertices(); }

    /** Returns the number of electrical lines of a network, or the
     *  number of edges in the graph representing it.
     */
    public int numEdges() { return graph.numEdges(); }

    /** Returns the vertex associated to the municipalities of
     *  an electrical network, -1 if m does not appear in it.
     *
     *  A getter method of the municipalitiesToVertices Map:
     * @param m A municipality in the network
     */
    public int getVertex(Municipality m) {
        Integer vertex = municipalitiesToVertices.get(m);
        if (vertex == null) { return -1; }
        return vertex;
    }

    /** Returns the municipality of an electrical network associated
     *  to vertex v in the graph representing it, or null if v is
     *  outside the interval [0, numMunicipalities() - 1].
     *
     * A getter method of the verticesToMunicipalities Map:
     * @param v A vertex in the network's graph
     */
    public Municipality getMunicipality(int v) {
        return verticesToMunicipalities.get(v);
    }

    /** Returns the list of Adjacents to a vertex v, or null if
     *  v is not in the interval [0, numMunicipality() - 1].
     *  @param v Vertex in the graph
     */
    public ListPOI<Adjacent> adjacentTo(int v) {
        return graph.adjacentTo(v);
    }

    /** If it exists, computes the electrical lines of a minimal
     *  cost subnet for an intercity electrical grid, i.e., the
     *  edges that define a Minimum Spanning Tree of the graph
     *  that represents the network, and returns its cost.
     *  Additionally, it creates and populates the kruskalAdjacents
     *  Map, which links each Municipality in the network to all
     *  its Adjacents in said Tree.
     *
     * If no minimal cost subnet exists, it returns -1.0;
     */
    public double createKruskalAdjacents() {
        kruskalAdjacents = new HashTable<>(numVertices());
        
        // STEP 1. Using Graph#kruskal(), obtain the set of edges
        // that define a Minimum Spanning Tree of the graph that
        // represents the intercity electrical network.
        /* TO BE COMPLETED */
        Edge[] edges = graph.kruskal();

        // STEP 2. If kruskal returns null, return -1.0
        if (edges == null) return -1;

        // STEP 3. If kruskal returns a set of edges, traverse it
        // to compute its cost (optimum) and, at the same time, it
        // builds the kruskalAdjacents Map with the help of the
        // verticesToMunicipalities Map
        double weightSum = 0;
        
        for (Edge e : edges) {
            weightSum += e.getWeight();
            
            int source = e.getSource();
            int target = e.getTarget();
            Municipality sourceMun = verticesToMunicipalities.get(source);
            Municipality targetMun = verticesToMunicipalities.get(target);
            
            // Create ListPOIs if they don't exist
            if (kruskalAdjacents.get(sourceMun) == null) {
                 kruskalAdjacents.put(sourceMun, new LinkedListPOI<Municipality>());
            }
            if (kruskalAdjacents.get(targetMun) == null) {
                 kruskalAdjacents.put(targetMun, new LinkedListPOI<Municipality>());
            }
            
            // Add to the linked lists information about this connection
            kruskalAdjacents.get(sourceMun).add(targetMun);
            kruskalAdjacents.get(targetMun).add(sourceMun);
        }

        // STEP 4. Return the total cost of the refurbishment, i.e.,
        // the sum of the edges that make up the MST
        return weightSum;
    }
}
