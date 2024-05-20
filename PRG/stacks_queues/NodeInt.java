class NodeInt {
    int data;
    NodeInt next;

    NodeInt(int d) {
        data = d;
        next = null;
    }

    NodeInt(int d, NodeInt s) {
        this(d);
        next = s;
    }
}
