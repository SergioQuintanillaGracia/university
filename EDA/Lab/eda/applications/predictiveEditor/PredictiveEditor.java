package applications.predictiveEditor;

import libraries.dataStructures.hierarchical.BST;
import libraries.dataStructures.linear.LinkedListPOI;
import libraries.dataStructures.models.ListPOI;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.IOException;

/** Clase EditorPredictivo: es un Arbol Binario de Busqueda <br>
 * de elementos de tipo String, que representan las palabras <br>
 * de un idioma. Esta clase proporciona los metodos necesarios <br>
 * para la gestion de un editor predictivo. <br>
 *
 *  @version Febrero 2019
 */

public class PredictiveEditor extends BST<String> {

    /** Creates an empty PredictiveEditor */
    public PredictiveEditor() {
        super();
    }

    /**
     * Creates a PredictiveEditor based on a list of words, sorted
     * alphabetically, from a given file. The file contains a number
     * that represents the number of words in the first line, and then
     * one line per word.
     * @param fileName Name of the text file that contains the words
     *                 from which an editor will be created.
     */
    public PredictiveEditor(String fileName) {
        super();
        try {
            Scanner wordsFile = new Scanner(new File(fileName), "UTF-8");
            int size = wordsFile.nextInt(); wordsFile.nextLine();
            String[] words = new String[size];
            for (int lineNum = 0; wordsFile.hasNext() && lineNum < size; lineNum++)
                words[lineNum] = wordsFile.nextLine().toLowerCase().trim();
            wordsFile.close();
            this.root = buildBalanced(words, 0, size - 1);
        } catch (FileNotFoundException eChecked) {
            System.out.println("The file " + fileName + " cannot be read, check if it is in the right location");
        }
    }

    /** Adds the given word to a Predictive Editor.
     *  @param  word Word to be added to the Editor.
     */
    public void include(String word) {
        this.add(word.toLowerCase().trim());
    }

    /**
     * Writes to a file the number of words in this Editor and each
     * of the words in alphabetical order. Each element is in a separate line.
     * @param fileName Where the contents of the Editor should be saved.
     */
    public void save(String fileName) {
        try {
            PrintWriter wordsFile = new PrintWriter(fileName, "UTF-8");
            Object[] words = this.toArrayInOrder();
            wordsFile.println(words.length);
            for (int i = 0; i < words.length; i++) {
                wordsFile.println(words[i]);
            }
            wordsFile.close();
        } catch (IOException eChecked) {
            System.out.println("Error while saving to file " + fileName
                + ": " + eChecked);
        }
    }

    /**
     * Returns a ListPOI with, at most, the next n successors of a
     * given prefix; the first word of the list will be the prefix itself
     * if it is a valid word in the editor.
     * @param  prefix Prefix to search for.
     * @param  n      Maximum number of successors to return.
     * @return List with POI with the successors of prefix found.
     */
    public ListPOI<String> successors(String prefix, int n) {
        ListPOI<String> list = new LinkedListPOI<>();
        String curr = prefix;
        
        // If the prefix is in the tree, add it to the list
        if (get(prefix) != null) {
            System.out.println("OK correct word");
            list.add(prefix);
            n--;
        }
        
        // Add the right amount of successors to the prefix to the list
        while (n-- > 0) {
            curr = successor(curr);
            if (curr == null) break;
            if (!curr.startsWith(prefix)) break;
            list.add(curr);
            System.out.printf("Adding %s\n", curr);
        }
        
        System.out.print("Final list: ");
        System.out.println(list);
        
        return list;
    }
}
