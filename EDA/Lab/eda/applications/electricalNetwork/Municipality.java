package applications.electricalNetwork;

/** Municipality class: represents a town in the labelled graph of the app.
 *  Because it is used as a key in two of the Maps in said graph,
 *  it must override the equals and hashCode methods inherited from Object
 *
 * @version (Curso 2022/23)
 */

public class Municipality {

    private final String name;    // Municipality's name
    private final int population; // Number of inhabitants
    private final double area;    // In square km
    private final int posX, posY; // Coordinates in the map

    /** Creates a municipality with the given name, population, area and map location.
     *  @param  n  Name (String)
     *  @param  p  Number (int) of inhabitants
     *  @param  a  Area (double) in Km2
     *  @param pX X coordinate (int) in the map
     *  @param pY Y coordinate (int) in the map
     */
    public Municipality(String n, int p, double a, int pX, int pY) {
        name = n;
        population = p;
        area = a;
        posX = pX; posY = pY;
    }

    /** Creates a municipality with the given name, with 0 inhabitants,
     *  0km2 area, and position at (0, 0) in the map
     *  @param  n   Name (String)
     */
    public Municipality(String n) {
        this(n, 0, 0.0, 0, 0);
    }

    /** Returns a municipality's name
      * @return String Name
     */
    public String getName() { return name; }

    /** Returns a municipality's population
      * @return int Population
     */
    public int getPopulation() { return population; }

    /** Returns a municipality's area
      * @return  double area in sq. km
     */
    public double getArea() { return area; }

    /** Returns the horizontal coordinate of a municipality in the map
      * @return int Pixels as horizontal coordinate
     */
    public int getPosX() { return posX; }

    /** Returns the vertical coordinate of a municipality in the map
      * @return int Pixels as vertical coordinate
     */
    public int getPosY() { return posY; }

    /** Returns a String with the components of a municipality in a
     *  given format. DO NOT MODIFY the code, as the format is the same
     *  used in the data files from which the application's graph is created.
     *  @return String representing the municipality in text format
     */
    public String toString() {
        return String.format("%s;%d;%s;%d;%d", name, population, area, posX, posY);
    }

    /** Checks whether a municipality (this) is equal to another, i.e., if they share the name
      * @param  other   Municipality to be compared against this
      * @return boolean true if this and other are equal, false otherwise
     */
    public boolean equals(Object other) {
        return other instanceof Municipality
               && this.name.equals(((Municipality) other).name);
    }

    /** Returns the hash value of a municipality, using just its name
      * @return int Hash value
     */
    public int hashCode() { return name.hashCode(); }

}
