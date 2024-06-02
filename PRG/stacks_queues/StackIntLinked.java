class StackIntLinked {
    private NodeInt top;
    private int size;

    public StackIntLinked() {
        this.top = null;
        this.size = 0;
    }

    public void push(int x) {
        this.top = new NodeInt(x, top);
        this.size++;
    }

    public int pop() {
        int x = this.top.data;
        this.top = this.top.next;
        this.size--;
        return x;
    }

    public int top() {
        return this.top.data;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int size() {
        return this.size;
    }

    public boolean pushMRedundancy20212022(int x, int max) {
        int counter = 0;
        NodeInt n = top;

        while (n != null && counter < max) {
            if (n.data == x) {
                counter++;
            }

            n = n.next;
        }

        if (counter < max) {
            top = new NodeInt(x, top); // I think this is not wrong, the stack goes in reverse to the queue
            size++;
            return true;
        }

        return false;
    }

    public int pushR(int x) {
        int count = 0;
        NodeInt curr = top;
        boolean added = false;

        while (curr != null) {
            if (curr.data == x) {
                count++;

                if (!added) {
                    curr.next = new NodeInt(x, curr.next);
                    added = true;
                }
            }

            curr = curr.next;
        }
        
        if (!added) {
            top = new NodeInt(x, top);
        }

        return count;
    }
}