public class hex_variables {
    public static void main(String[] args) {
        int i = 0x07fffffff;
        System.out.println("%32d".formatted(i));
        /* In %32d, the number 32 means the minimum number of digits
        the value will take when printed, so it will print something
        like this:
                              2147483647
        with spaces before.
        */
        
    }
}
