// CSD feb 2013 Juansa Sendra

public class Pool4 extends Pool { //kids cannot enter if there are instructors waiting to exit
    public void init(int ki, int cap)           {}
    public void kidSwims()      {log.swimming();}
    public void kidRests()      {log.resting(); }
    public void instructorSwims()   {log.swimming();}
    public void instructorRests()   {log.resting(); }
}
