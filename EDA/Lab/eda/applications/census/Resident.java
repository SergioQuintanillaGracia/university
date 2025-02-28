package applications.census;

import java.util.Random;

/**
 * Resident: represents a person registered in the census of a municipality
 *
 * @author  EDA Professors
 * @version September 2023 (Translation Feb. 24)
 */

public class Resident implements Comparable<Resident> {

    private String dni, name, surname1, surname2;
    private int pc; /** pc: postal code */

    /** 100 most frequent surnames, Spain, 2022, according to INE */
    private static String[] surnames = {"GARCIA", "RODRIGUEZ", "GONZALEZ", "FERNANDEZ", "LOPEZ",
        "MARTINEZ", "SANCHEZ", "PEREZ", "GOMEZ", "MARTIN", "JIMENEZ", "HERNANDEZ", "RUIZ", "DIAZ",
        "MORENO", "MUÃ‘OZ", "ALVAREZ", "ROMERO", "GUTIERREZ", "ALONSO", "NAVARRO", "TORRES", "DOMINGUEZ",
        "RAMOS", "VAZQUEZ", "RAMIREZ", "GIL", "SERRANO", "MORALES", "MOLINA", "BLANCO", "SUAREZ",
        "CASTRO", "ORTEGA", "DELGADO", "ORTIZ", "MARIN", "RUBIO", "NUÃ‘EZ", "MEDINA", "SANZ", "CASTILLO",
        "IGLESIAS", "CORTES", "GARRIDO", "SANTOS", "GUERRERO", "LOZANO", "CANO", "CRUZ", "MENDEZ",
        "FLORES", "PRIETO", "HERRERA", "PEÃ‘A", "LEON", "MARQUEZ", "CABRERA", "GALLEGO", "CALVO", "VIDAL",
        "CAMPOS", "REYES", "VEGA", "FUENTES", "CARRASCO", "DIEZ", "AGUILAR", "CABALLERO", "NIETO",
        "SANTANA", "VARGAS", "PASCUAL", "GIMENEZ", "HERRERO", "HIDALGO", "MONTERO", "LORENZO", "SANTIAGO",
        "BENITEZ", "DURAN", "IBAÃ‘EZ", "ARIAS", "MORA", "FERRER", "CARMONA", "VICENTE", "ROJAS", "SOTO",
        "CRESPO", "ROMAN", "PASTOR", "VELASCO", "PARRA", "SAEZ", "MOYA", "BRAVO", "RIVERA", "GALLARDO",
        "SOLER"};

    /** 50 most frequent male names, Spain, 2022, according to INE */
    private static String[] maleNames = {"ANTONIO", "MANUEL", "JOSE", "FRANCISCO", "DAVID",
        "JUAN", "JAVIER", "DANIEL", "JOSE ANTONIO", "FRANCISCO JAVIER", "JOSE LUIS", "CARLOS",
        "ALEJANDRO", "JESUS", "MIGUEL", "JOSE MANUEL", "MIGUEL ANGEL", "RAFAEL", "PABLO", "PEDRO",
        "ANGEL", "SERGIO", "FERNANDO", "JOSE MARIA", "JORGE", "LUIS", "ALBERTO", "ALVARO",
        "JUAN CARLOS", "ADRIAN", "DIEGO", "JUAN JOSE", "RAUL", "IVAN", "RUBEN", "JUAN ANTONIO",
        "OSCAR", "ENRIQUE", "RAMON", "ANDRES", "JUAN MANUEL", "SANTIAGO", "VICENTE", "MARIO",
        "VICTOR", "JOAQUIN", "EDUARDO", "ROBERTO", "MARCOS", "JAIME"};

    /** 50 most frequent female names, Spain, 2022, according to INE */
    private static String[] femaleNames = {"MARIA CARMEN", "MARIA", "CARMEN", "ANA MARIA",
        "LAURA", "MARIA PILAR", "MARIA DOLORES", "ISABEL", "JOSEFA", "MARIA TERESA", "ANA", "MARTA",
        "CRISTINA", "MARIA ANGELES", "LUCIA", "MARIA ISABEL", "MARIA JOSE", "FRANCISCA", "ANTONIA",
        "DOLORES", "PAULA", "SARA", "ELENA", "MARIA LUISA", "RAQUEL", "ROSA MARIA", "MANUELA",
        "MARIA JESUS", "PILAR", "CONCEPCION", "MERCEDES", "JULIA", "BEATRIZ", "ALBA", "SILVIA",
        "NURIA", "IRENE", "PATRICIA", "ROSARIO", "JUANA", "ROCIO", "ANDREA", "TERESA", "ENCARNACION",
        "MONTSERRAT", "MONICA", "ALICIA", "MARIA MAR", "SANDRA", "SONIA"};

    /** Array to compute the DNI's checknum letter, according to url:
     *  https://www.interior.gob.es/opencms/ca/servicios-al-ciudadano/tramites-y-gestiones/dni/calculo-del-digito-de-control-del-nif-nie/
     */
    private static char[] dniLetter = {'T', 'R', 'W', 'A', 'G', 'M', 'Y', 'F', 'P', 'D', 'X', 'B',
        'N', 'J', 'Z', 'S', 'Q', 'V', 'H', 'L', 'C', 'K', 'E'};

    /**
     * Creates a new Resident (with random values)
     */
    public Resident() {
        Random r = new Random();
        /* dni in range [10000000 .. 99999999], random value */
        int n = r.nextInt(99999999) + 10000000;
        /* the dni's letter is computed as a checksum, as seen in this url:
        https://www.interior.gob.es/opencms/ca/servicios-al-ciudadano/tramites-y-gestiones/dni/calculo-del-digito-de-control-del-nif-nie/
         */
        dni = n + "" + dniLetter[n % 23];
        /* first surname, random value */
        n = r.nextInt(surnames.length);
        surname1 = surnames[n];
        /* second surname, random value */
        n = r.nextInt(surnames.length);
        surname2 = surnames[n];
        /* name, random value, with a 50-50 chance of male-female */
        n = r.nextInt(100);
        if (n % 2 == 0) {
            n = r.nextInt(femaleNames.length);
            name = femaleNames[n];
        } else {
            n = r.nextInt(maleNames.length);
            name = maleNames[n];
        }
        /* postal code in range [46000, 46999] (province of Valencia), random value */
        pc = 46000 + r.nextInt(999);
    }

    /**
     * Creates a Resident (using values from the parameters)
     */
    public Resident(String dni, String name, String sn1, String sn2, int pc) {
        this.dni = dni;
        this.name = name;
        this.surname1 = sn1;
        this.surname2 = sn2;
        this.pc = pc;
    }

    /**
     * Field checker methods
     */
    public String getDni() { return dni; }
    public String getName() { return name; }
    public String getSurname1() { return surname1; }
    public String getSurname2() { return surname2; }
    public int getPC() { return pc; }

    /**
     * Returns a String that represents a (this) Resident as text
     */
    public String toString() {
        return surname1 + " " + surname2 + ", " + name
               + " (dni: " + dni + ". pc: " + pc + ")";
    }

    /**
     * Checks if a (this) Resident is equal to another,
     * i.e. if both have the same value
     * (comparing values of the dni field)
     *
     * @param other the other Resident.
     * @return true  if this and other have the same dni
     *         false otherwise
     */
    @Override
    public boolean equals(Object other) {
        if (!(other instanceof Resident)) return false;
        Resident res = (Resident) other;
        return dni.equals(res.dni);
    }

    /**
     * Compares a (this) Resident with another.
     *
     * @param other the Resident to compare to this.
     * @return -1 if this is lower than the other
     *         1  if this is higher than the other
     *         0  if this and other are equal
     */
    public int compareTo(Resident res) {
        int comp = surname1.compareTo(res.surname1);
        if (comp != 0) return comp;
        
        comp = surname2.compareTo(res.surname2);
        if (comp != 0) return comp;
        
        comp = name.compareTo(res.name);
        if (comp != 0) return comp;
        
        return dni.compareTo(res.dni);
    }

}
