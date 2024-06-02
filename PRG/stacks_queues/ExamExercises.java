public class ExamExercises {
    public static void main(String[] args) {
        StackIntLinked s = new StackIntLinked();
        s.push(1);
        s.push(2);
        s.push(3);
        s.push(4);
        s.push(5);

        System.out.println(removeElementsGreaterThanX(s, 0));
    }

    public static void moveBack(QueueIntLinked q, int x) {
        boolean found = false;
        int n = q.size();

        while (--n >= 0) {
            int v = q.dequeue();
            
            if (v == x && !found) {
                found = true;
            } else {
                q.enqueue(v);
            }
        }

        if (found) {
            q.enqueue(x);
        }
    }

    public static QueueIntLinked removeElementsInOddPositions(QueueIntLinked q) {
        QueueIntLinked newQ = new QueueIntLinked();

        for (int i = 0; i < q.size(); i++) {
            if (i % 2 == 0) {
                newQ.enqueue(q.first());
            }

            q.enqueue(q.dequeue());
        }

        return newQ;
    }

    public static StackIntLinked removeElementsGreaterThanX(StackIntLinked s, int x) {
        StackIntLinked temp = new StackIntLinked();
        StackIntLinked newS = new StackIntLinked();
        int size = s.size();

        for (int i = 0; i < size; i++) {
            temp.push(s.pop());
        }

        for (int i = 0; i < size; i++) {
            if (temp.top() <= x) {
                newS.push(temp.top());
            }

            s.push(temp.pop());
        }

        return newS;
    }

    public static StackIntLinked removeNegative(QueueIntLinked q) {
        StackIntLinked s = new StackIntLinked();
        int n = q.size();
        
        for (int i = 0; i < n; i++) {
            if (q.first() < 0) {
                s.push(q.dequeue());
            } else {
                q.enqueue(q.dequeue());
            }
        }

        return s;
    }

    public static void prioritize(QueueIntLinked q, int d) {
        QueueIntLinked aux = new QueueIntLinked();
        int n = q.size();

        while (n-- > 0) {
            int val = q.dequeue();
            
            if (val == d) {
                q.enqueue(val);
            } else {
                aux.enqueue(val);
            }
        }

        while (!aux.isEmpty()) {
            q.enqueue(aux.dequeue());
        }
    }
}
