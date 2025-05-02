package libraries.dataStructures.hierarchical;



/** Friendly class BSTNode<E>: represents a Node of a BST, so it
 *  HAS A:
 *  1.- E data, represents the Element that occupies a Node in a BST
 *  2.- BSTNode<E> left, a reference to the left child of a Node in a BST
 *  3.- BSTNode<E> right, a reference to the right child of a Node in a BST
 *  4.- size, an integer that represents the number of child nodes that hang from a Node
 *      An BST with Nodes that store their size is called a Ranked BST
 *
 *  @param <E>, the type of data stored in the BST
 *  @version Marzo 2017
 */
class NodeBST<E> {
     // attributes
    protected E data;
    protected int size;
    protected NodeBST<E> left;
    protected NodeBST<E> right;

    /** Constructor for a node without children
      * @param  data Value to be stored in this node
      */
    public NodeBST(E data) {
        this(data, null, null);
    }

    /**  Constructor for a node with a left child and a right child.
      * @param  data Value to be stored in this node
      * @param  left Left child of this node
      * @param  right Right child of this node
      */
    public NodeBST(E data, NodeBST<E> left, NodeBST<E> right) {
        this.data = data;
        this.left = left;
        this.right = right;
        size = 1;
        if (left != null) { size += left.size; }
        if (right != null) { size += right.size; }
    }
}
