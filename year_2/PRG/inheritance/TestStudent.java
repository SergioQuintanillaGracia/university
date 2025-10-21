public class TestStudent {
    public static void main(String[] args) {
        Student s = new Student("John", 19);
        Person s2 = new Student("John", 19);
        Person s3 = new Person("John", 19);

        System.out.printf("Student: %s\n", s);
        System.out.printf("Perstudent: %s\n", s2);
        System.out.printf("Person: %s\n", s3);
    }
}
