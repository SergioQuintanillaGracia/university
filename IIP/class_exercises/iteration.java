public class iteration {
    public static void main(String[] args) {
        int n = 20;

        System.out.println("The result for n = %d is %.15f".formatted(n, tailor(1, n)));
    }


    public static int ex1(int n) {
        int res = 0;

        for (int i = 1; i <= n; i++) {
            res += i;
        }

        return res;
    }
    

    public static int ex2(int n) {
        int res = 0;

        for (int i = 1; i <= n; i++) {
            res += pow(i, 2);
        }

        return res;
    }


    public static double ex3(int n) {
        double res = 0;

        for (int i = 1; i <= n; i++) {
            res += 1.0/i;
        }

        return res;
    }


    public static double ex4(int n) {
        double res = 0;

        for (int i = 1; i <= n; i++) {
            res += 1.0 / pow(i, 2);
        }

        return res;
    }


    public static int ex5(int n) {
        int res = 0;
        int p = 1;

        for (int i = 0; i <= n; i++) {
            res += p;
            p *= 2;
        }

        return res;
    }


    public static double ex6(int n) {
        double res = 0;
        int p = 1;

        for (int i = 0; i <= n; i++) {
            res += 1 / p;
            p *= 2;
        }

        return res;
    }


    public static int factorio(int n) {
        if (n > 1) {
            return n * factorio(n - 1);
        
        } else {
            return 1;
        }
    }


    public static double pow(double x, int n) {
        double res = 1;

        for (int i = 0; i < n; i++) {
            res *= x;
        }

        return res;
    }


    public static double tailor(double x, int n) {
        double sum = 1;
        double term = 1;

        if (x < 0) return 1 / tailor(-x, n);

        for (int i = 1; i <= n; i++) {
            term *= x / i;
            sum += term;
        }

        return sum;
    }
}
