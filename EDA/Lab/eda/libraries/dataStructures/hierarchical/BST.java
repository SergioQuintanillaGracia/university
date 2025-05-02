package libraries.dataStructures.hierarchical;

import libraries.dataStructures.models.Queue;
import libraries.dataStructures.linear.ArrayQueue;
import libraries.util.Sorting;

import java.lang.reflect.Array;

/** BST<E> class that represents a Binary Search Tree through a
 *  link to its current root. Its features are:
 *  1.- The type of its elements is E (must implement Comparable<E>)
 *  2.- ATTRIBUTES (protected to be visible to child classes):
 *      HAS A BSTNode<E> root
 *
 * @version Marzo 2022
 **/
public class BST<E extends Comparable<E>> {
    // Attribute of BST
    protected NodeBST<E> root;

    /** Creates an empty Binary Search Tree */
    public BST() { root = null; }

    /**
     * Creates a new Binary Search Tree with the elements of the given array.
     * The resulting BST must be balanced.
     * @param a Array with the elements to add to the BST
     */
    public BST(E[] a) {
        root = buildBalanced(a, 0, a.length - 1);
    }

    /**
     * Builds a balanced BST with the elements in a range of an array.
     * @param a     Array with the elements to be added to the BST.
     * @param begin First index to be considered in the array (inclusive).
     * @param end   Last index to be considered in the array (inclusive).
     * @return Root of the subtree that contains the elements of the given array range.
     */
    protected NodeBST<E> buildBalanced(E[] a, int begin, int end) {
        if (end - begin < 0) {
            // Array contains no elements
            return null;
        }
        
        // Array contains more than 2 elements
        int middle = (end - begin) / 2 + begin;
        NodeBST<E> left = buildBalanced(a, begin, middle - 1);
        NodeBST<E> right = buildBalanced(a, middle + 1, end);
        return new NodeBST<>(a[middle], left, right);
    }

    /** Rebuilds the BST with the same data, such that it ends up balanced. */
    public void rebuildBalanced() {
        E[] arr = toArrayInOrder();
        root = buildBalanced(arr, 0, arr.length - 1);
    }

    /**
     * Returns the successor of an element in the BST
     * @param e Element whose successor we are looking for
     * @return  Successor of e, or null if e has no successor
     */
    public E successor(E e) {
        NodeBST<E> res = successor(e, root);
        if (res == null) { return null; }
        return res.data;
    }

    /**
     * IFF current != null: returns the node in 'current' that contains
     * "e"'s successor, or null if it does not exist.
     * @param e       Element whose successor we are looking for
     * @param current Current node in the search
     * @return Successor of "e" in the current node, or null if it does not exist.
     */
    protected NodeBST<E> successor(E e, NodeBST<E> current) {
        NodeBST<E> res = null;
        if (current != null) {
            int resC = current.data.compareTo(e);
            if (resC > 0) {
                res = successor(e, current.left);
                if (res == null) { res = current; }
            } else {
                res = successor(e, current.right);
            }
        }
        return res;
    }

    /** Searches for the given value in the BST
     * @param    e Element to look for
     * @return   Element in the BST that matches e, or null if none exist.
     */
    public E get(E e) {
        NodeBST<E> res = get(e, root);
        if (res == null) return null;
        return res.data;
    }

    /** Searches for the given value starting in the given node
     * @param    e       Element to look for
     * @param    current Current node in the search
     * @return   Node in the BST that contains e, or null if none exist.
     */
    protected NodeBST<E> get(E e, NodeBST<E> current) {
        if (current == null) { return null; }
        int cmp = e.compareTo(current.data);
        if (cmp < 0) { return get(e, current.left); }
        if (cmp > 0) { return get(e, current.right); }
        return current;
    }

    /** Update the element in the BST, if it is not present, adds it
     * @param    e Element to add/update.
     */
    public void add(E e) { root = add(e, root); }

    /** Update the element in the BST, if it is not present, adds it.
     *  This method starts the search from the given node.
     * @param    e Element to add/update.
     * @param    current Current node in the search
     * @return   The current node, or a new node containing "e" if current is null.
     */
    protected NodeBST<E> add(E e, NodeBST<E> current) {
        if (current == null) { return new NodeBST<>(e); }
        int cmp = e.compareTo(current.data);
        if (cmp < 0) { current.left = add(e, current.left); }
        else if (cmp > 0) { current.right = add(e, current.right); }
        else { current.data = e; }
        current.size = 1 + size(current.left) + size(current.right);
        return current;
    }

    /** Returns the number of elements of the Binary Search Tree */
    public int size() { return size(root); }

    /** Returns the size of the given node */
    protected int size(NodeBST<E> actual) {
        if (actual == null) { return 0; }
        return actual.size;
    }

    /** IFF !isEmpty(): returns the minimum value stored in the BST
      * @return Element with the lowest value
      */
    public E getMin() { return getMin(root).data; }

    /** Returns the element with the lowest value from the given node
     * @param    current  Current node in the search
     * @return   Node that contains the element with the lowest value
     */
    protected NodeBST<E> getMin(NodeBST<E> current) {
        if (current.left == null) { return current; }
        return getMin(current.left);
    }

    /** IFF !isEmpty(): removes the element with the lowest value from the BST
     * @return Minimum element of the BST.
     */
    public E removeMin() {
        E min = getMin();
        root = removeMin(root);
        return min;
    }

    /** Removes the minimum element starting from the given node
     * @param    current  Current node in the search
     * @return Root node of the subtree whose current root is the current node.
     */
    protected NodeBST<E> removeMin(NodeBST<E> current) {
        if (current.left == null) { return current.right; }
        current.left = removeMin(current.left);
        current.size--;
        return current;
    }

    /** Removes the node that contains "e"
     * @param  e The object to search for and remove
     */
    public void remove(E e) { root = remove(e, root); }

    /** Removes the node that contains "e", starting from the given node
     * @param  e       Object to search for and remove
     * @param  current Current node in the search
     * @return Root node of the subtree whose current root is the current node.
     */
    protected NodeBST<E> remove(E e, NodeBST<E> current) {
        if (current == null) { return current; }
        int cmp = e.compareTo(current.data);
        if (cmp < 0) { current.left = remove(e, current.left); }
        else if (cmp > 0) { current.right = remove(e, current.right); }
        else {
            if (current.right == null) { return current.left; }
            if (current.left == null) { return current.right; }
            current.data = getMin(current.right).data;
            current.right = removeMin(current.right);
        }
        current.size = 1 + size(current.left) + size(current.right);
        return current;
    }

    /** Whether the Binary Search Tree is empty. */
    public boolean isEmpty() { return root == null; }

    /**
     * inOrder traversal of the BST
     * @return String with the elements according to the inOrder traversal
     */
    public String toStringInOrder() {
        StringBuilder sb = new StringBuilder().append("[");
        if (root != null) { toStringInOrder(sb, root); }
        return sb.append("]").toString();
    }

    /**
     * inOrder traversal of the BST starting in the given node
     * @param sb      StringBuilder to accumulate the text string
     * @param current Current node in the traversal
     */
    protected void toStringInOrder(StringBuilder sb, NodeBST<E> current) {
        if (current.left != null) {
            toStringInOrder(sb, current.left);
            sb.append(",");
        }
        sb.append(current.data.toString());
        if (current.right != null) {
            sb.append(",");
            toStringInOrder(sb, current.right);
        }
    }

    /**
     * preOrder traversal of the BST
     * @return String with the elements according to the preOrder traversal
     */
    public String toStringPreOrder() {
        StringBuilder sb = new StringBuilder().append("[");
        if (root != null) { toStringPreOrder(sb, root); }
        return sb.append("]").toString();
    }

    /**
     * preOrder traversal of the BST starting in the given node
     * @param sb      StringBuilder to accumulate the text string
     * @param current Current node in the traversal
     */
    protected void toStringPreOrder(StringBuilder sb, NodeBST<E> current) {
        sb.append(current.data.toString());
        if (current.left != null) {
            sb.append(",");
            toStringPreOrder(sb, current.left);
        }
        if (current.right != null) {
            sb.append(",");
            toStringPreOrder(sb, current.right);
        }
    }

    /**
     * postOrder traversal of the BST
     * @return String with the elements according to the postOrder traversal
     */
    public String toStringPostOrden() {
        StringBuilder sb = new StringBuilder().append("[");
        if (root != null) { toStringPostOrden(sb, root); }
        return sb.append("]").toString();
    }

    /**
     * postOrder traversal of the BST starting in the given node
     * @param sb      StringBuilder to accumulate the text string
     * @param current Current node in the traversal
     */
    protected void toStringPostOrden(StringBuilder sb, NodeBST<E> current) {
        if (current.left != null) {
            toStringPostOrden(sb, current.left);
            sb.append(",");
        }
        if (current.right != null) {
            toStringPostOrden(sb, current.right);
            sb.append(",");
        }
        sb.append(current.data.toString());
    }

    /**
     * Level-based traversal of the BST
     * @return String with the elements according to the level-based traversal
     */
    public String toStringLevels() {
        if (this.root == null) { return "[]"; }
        StringBuilder res = new StringBuilder().append("[");
        Queue<NodeBST<E>> q = new ArrayQueue<>();
        q.enqueue(this.root);
        while (!q.isEmpty()) {
            NodeBST<E> actual = q.dequeue();
            res.append(actual.data.toString());
            res.append(", ");
            if (actual.left != null) { q.enqueue(actual.left); }
            if (actual.right != null) { q.enqueue(actual.right); }
        }
        // For efficiency, to delete the last ", "
        // from res, we subtract 2 to its current length
        res.setLength(res.length() - 2);
        return res.append("]").toString();
    }

    /**
     * Builds an array sorted in ascending order with all the elements
     * of the BST, which would be the result of an InOrder traversal of it.
     * @return Array with the values of the BST according to the InOrder traversal
     */
    @SuppressWarnings("unchecked")
    public E[] toArrayInOrder() {
        E[] v = (E[]) new Comparable[size()];
        toArrayInOrder(v, root, 0);
        return v;
    }

    /**
     * Builds an array sorted in ascending order with all the elements
     * of the BST, which would be the result of an InOrder traversal of it.
     * @param v       Array with the elements according to an InOrder traversal
     * @param current Current node in the traversal
     * @param pos     Position/Index in the array
     */
    protected void toArrayInOrder(E[] v, NodeBST<E> current, int pos) {
        if (current != null) {
            toArrayInOrder(v, current.left, pos);
            int auxPos = pos + size(current.left);
            v[auxPos++] = current.data;
            toArrayInOrder(v, current.right, auxPos);
        }
    }

}
