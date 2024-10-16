package practica3;

import java.util.*;


/**
 * class FiguresGroup.
 * 
 * @author LTP 
 * @version 2020-21
 */

public class FiguresGroup {
    private static final int NUM_FIGURES = 10;
    private Figure figuresList[] = new Figure[NUM_FIGURES];
    private int numF = 0;
    
    public List<Figure> orderedList() {
        List<Figure> a = new ArrayList<>();
        
        if (numF > 0) {
            a.add(figuresList[0]);
        }
        
        for (int i = 1; i < numF; i++) {
            Figure figToAdd = figuresList[i];
            boolean added = false;
            
            for (int j = a.size() - 1; j >= 0; j--) {
                Figure figToCompare = a.get(j);
                int compareRes = figToAdd.compareTo(figToCompare);
                
                if (compareRes >= 0) {
                    a.add(j + 1, figToAdd);
                    added = true;
                    break;
                }
            }
            
            if (!added)
                a.add(0, figuresList[i]);
        }
        
        return a;
    }
    
    public void add(Figure f) {
        figuresList[numF++] = f;
    }
    
    public String toString() {
        String s = "";
        for (int i = 0; i < numF; i++) {
            s += "\n" + figuresList[i];
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
    
    public boolean equals(Object o) {
        if (!(o instanceof FiguresGroup)) {
            return false;
        }
        
        FiguresGroup fg = (FiguresGroup) o;
        
        return included(fg) && fg.included(this);
    }
    
    public double area() {
        double a = 0;
        
        for (int i = 0; i < numF; i++) {
            a += figuresList[i].area();
        }
        
        return a;
    }
    
    public Figure greatestFigure() {
        double maxArea = -1;
        Figure maxAreaFig = null;
        
        for (int i = 0; i < numF; i++) {
            double currArea = figuresList[i].area();
            
            if (currArea > maxArea) {
                maxArea = currArea;
                maxAreaFig = figuresList[i];
            }
        }
        
        return maxAreaFig;
    }
}
