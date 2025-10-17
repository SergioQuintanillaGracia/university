public class IncidentsOperatorMorning {
    IDAL dal;

    public ICollection<Incident> GetAllIncidentsWithAtLeastMorningOperator() {
        // Only one line of code
        return dal.GetWhere<Incident>(incident => incident.WorkOrder.Operators.Any(oper => oper.Shift == Shift.morning)).toList();
    }
}