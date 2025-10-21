public class EmployeeMain {
    public static void main(String[] args) {
        Employee e = new Employee("Juan", 5);
        System.out.println(e);

        Executive boss = new Executive("Francisco", 1500);
        boss.assignBudget(2000);
        System.out.println(boss);
    }
}
