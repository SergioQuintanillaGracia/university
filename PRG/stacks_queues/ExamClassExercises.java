import java.util.NoSuchElementException;

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

    public static QueueIntLinked removeOdd(StackIntLinked p) {
        StackIntLinked reversed = new StackIntLinked();

        // Move the elements to another stack, reversing them
        while (!p.isEmpty()) {
            reversed.push(p.pop());
        }

        QueueIntLinked q = new QueueIntLinked();

        // Move the even elements from the reversed stack to the original
        // one, and therefore keeping them in the original order
        // The odd elements are moved from the reversed stack to q, which
        // means the ones close to the top in the original stack will be
        // at the last positions in q
        while (!reversed.isEmpty()) {
            if (reversed.top() % 2 == 0) {
                p.push(reversed.pop());
            } else {
                q.enqueue(reversed.pop());
            }
        }

        return q;
    }

    public static int minimum(QueueIntLinked q) throws NoSuchElementException {
        int n = q.size();
        
        if (n == 0) {
            throw new NoSuchElementException("Empty queue");
        }

        int min = q.first();

        while (n-- > 0) {
            if (q.first() < min) {
                min = q.first();
            }

            q.enqueue(q.dequeue());
        }

        return min;
    }

    public static StackIntLinked removeGreater(StackIntLinked p, int x) {
        StackIntLinked reversed = new StackIntLinked();

        while (!p.isEmpty()) {
            reversed.push(p.pop());
        }

        StackIntLinked greater = new StackIntLinked();

        while (!reversed.isEmpty()) {
            if (reversed.top() > x) {
                greater.push(reversed.pop());
            } else {
                p.push(reversed.pop());
            }
        }
        return greater;
    }

    public static void moveBack2(QueueIntLinked q, int x) {
        int n = q.size();
        boolean found = false;

        while (n-- > 0) {
            if (!found && q.first() == x) {
                found = true;
                q.dequeue();
            } else {
                q.enqueue(q.dequeue());
            }
        }

        if (found) {
            q.enqueue(x);
        }
    }

    public static QueueIntLinked merge_probably_not_right(QueueIntLinked q1, QueueIntLinked q2) {
        QueueIntLinked merged = new QueueIntLinked();
        int n1 = q1.size();
        int n2 = q2.size();
        int size = Math.min(n1, n2);

        for (int i = 0; i < size; i++) {
            merged.enqueue(q1.first());
            merged.enqueue(q2.first());
            q1.enqueue(q1.dequeue());
            q2.enqueue(q2.dequeue());
        }

        if (size == n1) {
            for (int i = size; i < n2; i++) {
                merged.enqueue(q2.first());
                q2.enqueue(q2.dequeue());
            }
        }

        else if (size == n2) {
            for (int i = size; i < n1; i++) {
                merged.enqueue(q1.first());
                q1.enqueue(q1.dequeue());
            }
        }

        return merged;
    }
}
