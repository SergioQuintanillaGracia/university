package libraries.dataStructures.models;

/** Represents the classes -disjoint subsets- that define an equivalence
 *  relationship in a cardinal set n
 *
 *  The elements of the set are represented through integers in the [0, n-1]
 *  interval, as its type does NOT intervene in the classes' definition.
 *
 *  The elements of a class are represented with just ONE of its members,
 *  any of them, as they are all equivalent. This element is called class
 *  identifier.
 *
 *  THe classes are defined dynamically based on N trivial classes,
 *  each formed by an element of the set.
 *
 *  The Union-Find Set model receives other names, such as
 *  Disjoint Set and Merge-Find Set.
 *
 *  @version December 2018
 */

public interface UFSet {

    /** Returns the identifier of the equivalence class -subset-
     * to which i belongs, after perfoming path compression.
     */
    int find(int i);

    /** PRECONDITION: classI != classJ AND classI = find(i) && classJ = find(j)
     *  Joins together the equivalence classes -subsets- identified by classI
     *  and classJ by joining their ranks.
     */
    void union(int classI, int classJ);
}
