package examExample;

/**
 * Clase TimeInstant: permite representar instantes o marcas de tiempo
 * (Timestamp) con detalle de horas y minutos. Así, esta clase representa 
 * el momento que define un instante de tiempo, en este caso, las horas y 
 * los minutos de un día cualquiera.
 * 
 *  @author IIP 
 *  @version Curso 2023-24
 */
public class TimeInstant {
    /** 
     *  Variable entera para almacenar el atributo horas. 
     *  Debe pertenecer al rango [0..23].
     */
    private int hours;
    /** 
     *  Variable entera para almacenar el atributo minutos. 
     *  Debe pertenecer al rango [0..59].
     */
    private int minutes;
    
    /**
     *  Crea un TimeInstant con el valor de las horas y 
     *  los minutos que recibe como argumentos, iniHours y 
     *  iniMinutes, respectivamente.
     *  Precondición: 0 <= iniHours < 24, 0 <= iniMinutes < 60.
     */
    public TimeInstant(int iniHours, int iniMinutes) {
        hours = iniHours;
        minutes = iniMinutes;
    }
    
    /** Devuelve el TimeInstant this en el formato "hh:mm". */
    public String toString() {
        String hh = "0" + hours;
        hh = hh.substring(hh.length() - 2);
        String mm = "0" + minutes;
        mm = mm.substring(mm.length() - 2);
        return hh + ":" + mm;
    }
   
    /** Devuelve el numero de minutos transcurridos desde las 00:00 
     *  hasta el instante representado por el TimeInstant this.
     */
    public int toMinutes() {
        return hours * 60 + minutes;
    }
}
