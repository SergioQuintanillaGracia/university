public class product {
    public static void main(String[] args) {
        int res = product(7, -8);

        System.out.println(res);
    }


    public static int product(int a, int b) {
        int s = 0;

        int absA = Math.abs(a);
        int absB = Math.abs(b);

        while (--absB >= 0) {
            s += absA;
        }
        
        if (a >= 0 && b >= 0 || a < 0 && b < 0) return s;
        else return -s;
    }
}
