public class Executive extends Employee {
    protected int budget;

    public Executive(String name, int salary) {
        super(name, salary);
    }

    public void assignBudget(int b) {
        budget = b;
    }

    @Override
    public String toString() {
        String s = super.toString();
        s = s + ", budget=" + budget;
        return s;
    }
}
