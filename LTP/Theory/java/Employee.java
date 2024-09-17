public class Employee {
    protected String name;
    protected int nEmployee, salary;
    static private int counter = 0;

    public Employee(String name, int salary) {
        this.name = name;
        this.salary = salary;
        nEmployee = ++counter;
    }

    @Override
    public String toString() {
        return "Employee: " +
                "name='" + name + '\'' +
                ", nEmployee=" + nEmployee +
                ", salary=" + salary;
    }
}
