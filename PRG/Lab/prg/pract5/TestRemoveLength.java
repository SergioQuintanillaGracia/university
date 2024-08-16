package pract5;

import java.util.Scanner;
/**
 * Clase programa para facilitar la prueba del código, desarrollado por 
 * el alumno, para el método removeLength.
 * 
 * Se realizan 9 pruebas partiendo de diferentes conjuntos y eliminando 
 * mediante el método removeLength(int) las palabras de una longitud dada:
 * 
 * El main crea un conjunto con 5 palabras: "blue" "green" "red" "white" "yellow"
 * 
 * 1.- Llamar al método removeLength(2) para eliminar las palabras de longitud 2 y 
 *     comprobar que el conjunto no cambia porque no hay ninguna de dicha longitud. 
 * 2.- Llamar al método removeLength(3) para eliminar las palabras de longitud 3 y
 *     comprobar que se elimina la palabra "red" (la de enmedio).    
 * 3.- Llamar al método removeLength(4) para eliminar las palabras de longitud 4 y
 *     comprobar que se elimina la palabra "blue" (la primera) y es "green" la que 
 *     pasa a ser la primera.
 * 4.- Llamar al método removeLength(6) para eliminar las palabras de longitud 6 y
 *     comprobar que se elimina la palabra "yellow" (la última) y es "white" la que 
 *     pasa a ser la última.
 * 5.- Llamar al método removeLength(5) para eliminar las palabras de longitud 5 y
 *     comprobar que se eliminan las palabras "green" y "white" y el conjunto se 
 *     queda sin palabras.
 * 6.- Llamar al método removeLength(8) para eliminar las palabras de longitud 8 y
 *     comprobar que no ocurre nada porque el conjunto está vacío.
 * 
 * El main crea un conjunto con 5 palabras: "black" "gray" "green" "pink" "white"
 * 
 * 7.- Llamar al método removeLength(5) para eliminar las palabras de longitud 5 y
 *     comprobar que se eliminan las palabras "black", "green" y "white".
 *     
 * El main crea un conjunto con 5 palabras: "black" "gray" "green" "pink" "white" 
 * 
 * 8.- Llamar al método removeLength(4) para eliminar las palabras de longitud 4 y
 *     comprobar que se eliminan las palabras "gray" y "pink".
 * 
 * 9.- Llamar al método removeLength(5) para eliminar las palabras de longitud 5 y
 *     comprobar que se eliminan todas las palabras, quedando el conjunto vacío.
 *     
 * Esta clase es para uso particular del alumno. 
 * No se entrega. 
 * 
 * @author PRG
 * @version Curso 2023-24
 */
public class TestRemoveLength {
    private TestRemoveLength() { } // No se usan objetos de esta clase
    
    public static void main(String[] args)  {  
        SetStringExam sSE = new SetStringExam();
        sSE.add("blue"); sSE.add("green"); sSE.add("red"); sSE.add("white"); sSE.add("yellow"); 
        
        System.out.println("Se crea un conjunto con las 5 palabras que siguen \n"
            + "(que se muestran seguidas de su longitud):");
        System.out.println(listWithLength(sSE));
        
        System.out.println("** Prueba 1: llamada a removeLength(2) para eliminar las palabras de longitud 2.");              
        pressEnterToContinue();
        sSE.removeLength(2);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que el conjunto no ha cambiado porque no hay ninguna de dicha longitud.");
        pressEnterToContinue();
        
        System.out.println("** Prueba 2: llamada a removeLength(3) para eliminar las palabras de longitud 3.");              
        pressEnterToContinue();
        sSE.removeLength(3);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se ha eliminado la palabra \"red\" (la de enmedio).");
        pressEnterToContinue(); 
        
        System.out.println("** Prueba 3: llamada a removeLength(4) para eliminar las palabras de longitud 4.");                
        pressEnterToContinue();
        sSE.removeLength(4);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se ha eliminado la palabra \"blue\" (la primera)");
        System.out.println("y \"green\" ha pasado a ser la primera.");
        pressEnterToContinue();
        
        System.out.println("** Prueba 4: llamada a removeLength(6) para eliminar las palabras de longitud 6.");  
        pressEnterToContinue();
        sSE.removeLength(6);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se ha eliminado la palabra \"yellow\" (la última)");
        System.out.println("y \"white\" ha pasado a ser la última.");
        pressEnterToContinue();
        
        System.out.println("** Prueba 5: llamada a removeLength(5) para eliminar las palabras de longitud 5.");  
        pressEnterToContinue();
        sSE.removeLength(5);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se han eliminado las palabras \"green\" y \"white\""); 
        System.out.println("y el conjunto se ha quedado sin palabras.");
        pressEnterToContinue();
        
        System.out.println("** Prueba 6: llamada a removeLength(8) para eliminar las palabras de longitud 8.");                
        pressEnterToContinue();
        sSE.removeLength(8);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que no ha ocurrido nada porque el conjunto está vacío.");
        pressEnterToContinue();
        
        sSE = new SetStringExam();
        sSE.add("black"); sSE.add("gray"); sSE.add("green"); sSE.add("pink"); sSE.add("white"); 
        
        System.out.println("\nSe crea un conjunto con las 5 palabras que siguen \n"
            + "(que se muestran seguidas de su longitud):");
        System.out.println(listWithLength(sSE));
        
        System.out.println("** Prueba 7: llamada a removeLength(5) para eliminar las palabras de longitud 5.");  
        pressEnterToContinue();
        sSE.removeLength(5);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se han eliminado las palabras \"black\", \"green\" y \"white\"."); 
        pressEnterToContinue();
        
        sSE = new SetStringExam();
        sSE.add("black"); sSE.add("gray"); sSE.add("green"); sSE.add("pink"); sSE.add("white"); 
        
        System.out.println("Se crea un conjunto con las 5 palabras que siguen \n"
            + "(que se muestran seguidas de su longitud):");
        System.out.println(listWithLength(sSE));
        
        System.out.println("** Prueba 8: llamada a removeLength(4) para eliminar las palabras de longitud 4.");  
        pressEnterToContinue();
        sSE.removeLength(4);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se han eliminado las palabras \"gray\" y \"pink\"."); 
        pressEnterToContinue();
                
        System.out.println("** Prueba 9: llamada a removeLength(5) para eliminar las palabras de longitud 5.");  
        pressEnterToContinue();
        sSE.removeLength(5);
        System.out.println(listWithLength(sSE));
        System.out.println("...Comprueba que se han eliminado todas las palabras, quedando el conjunto vacío."); 
                
        System.out.println("\nFin de las pruebas!!!"); 
    }
    
    /**
     * Dado un SetStringExam s, devuelve un String con el listado de sus palabras 
     * seguidas de su longitud entre paréntesis y "\n". 
     * Si el conjunto está vacío, devuelve "empty set\n".
     */
    private static String listWithLength(SetStringExam s) {
        String[] list = s.toString().split("\n");
        if (list.length == 1 && list[0].equals("")) { return "empty set\n"; }
        String result = "";
        for (int i = 0; i < list.length; i++) { 
            result += String.format("%10s (%1d)\n", list[i], list[i].length());
        }        
        return result;
    }
    
    private static void pressEnterToContinue() {
        Scanner keyB = new Scanner(System.in);  
        System.out.println("Pulsa ENTER \u21B5 para continuar..."); 
        String s = keyB.nextLine();
    }
}
