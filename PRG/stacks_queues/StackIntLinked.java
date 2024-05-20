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
}