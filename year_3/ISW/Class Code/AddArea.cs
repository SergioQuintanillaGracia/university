public class AddArea() {
    IDAL dal;

    public void AddArea(Area a) {
        bool alreadyExists = dal.GetWhere<Area>(area => area.Name == a.Name).Any();

        if (alreadyExists) {
            // Throw exception
        } else {
            dal.Insert<Area>(a);
            // Now the changes are in the DBContext, but we want to store them
            // in the actual database:
            dal.Commit();
        }
    }
}