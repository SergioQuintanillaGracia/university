public class leap_years {
    public static void main(String[] args) {
        for (int year = 0; year <= 5000000; year++) {
            if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
                System.out.println("%d: TRUE".formatted(year));
            
            } else {
                System.out.println("%d: FALSE".formatted(year));
            }
        }
    }
}