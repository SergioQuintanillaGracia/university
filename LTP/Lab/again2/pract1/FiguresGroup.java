
/**
 * class FiguresGroup.
 * 
 * @author LTP 
 * @version 2020-21
 */

public class FiguresGroup {
    private static final int NUM_FIGURES = 10;
    private Figure[] figuresList = new Figure[NUM_FIGURES];
    private int numF = 0;
    
    public void add(Figure f) {
        figuresList[numF++] = f;
    }
    
    public String toString() {
        String s = "";
        for (int i = 0; i < numF; i++) {
            s += "\n(%.2fm^2) - %s".formatted(figuresList[i].area(), figuresList[i]);
        }
        return s;
    }

    private boolean found(Figure f) {
        for (int i = 0; i < numF; i++) {
        	if (figuresList[i].equals(f)) return true;
        }
        return false;
    }

    private boolean included(FiguresGroup g) {
		for (int i = 0; i < g.numF; i++) {
        	if (!found(g.figuresList[i])) return false;
        }
		return true;
    }

    public boolean equals(Object g) {
        if (!(g instanceof FiguresGroup)) return false;

        FiguresGroup fg = (FiguresGroup) g;

        return included(fg) && fg.included(this);
    }

    public double area() {
        double area = 0;

        for (int i = 0; i < numF; i++) {
            area += figuresList[i].area();
        }

        return area;
    }
}