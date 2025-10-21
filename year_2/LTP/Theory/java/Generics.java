public class Generics {
    public static void main(String[] args) {
        System.out.println(length(new Long[]{1L, 2L, 3L}));

        GenericEntry<Integer, String> e = new GenericEntry<>(5, "Hello");
        System.out.printf("{%d, %s}\n", e.getKey(), e.getValue());
    }

    public static <T> int length(T[] arr) {
        return arr.length;
    }
}
