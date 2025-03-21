package applications.library;

import libraries.dataStructures.scattered.HashTable;
import java.util.Scanner;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * EfficiencyTest: program to benchmark the efficiency of the Map index used
 * in the Search engine of this application. As aforementioned, the Map is
 * implemented with a Linked Hash Table with a default load factor of 0.75
 * (LF_STANDARD = 0.75). The factors that determine the Map's efficiency are
 * the ones that guarantee that the Table's real load factor, or the average
 * size of its buckets, is lower than 0.75. These factors are:
 ** (a) The hashCode() method implemented in the Term class,
 **     which acts as the Key of Map index: the better it is, the fewer
 **     the number of collisions produced will be.
 ** (b) The estimation of the maximum size of Map index, or, equivalently,
 **     the estimation of capacity of the Linked Hash Table that implements
 **     it (theArray.length), and thus the load factor is size / theArray.length
 *
 * To that end, indicating in the first argument of the main method the base
 * of the polynomial dispersion function that implements the hashCode() method
 * of the Term class ("31", "1" and "4") and, in the second argument, whether
 * the HashTable implements a Rehashing function ("YES" or "NO"), you can
 * obtain as a result the real load factors, standard deviations, average cost
 * of a key's lookup and the occupation histograms of 3 Linked Hash Tables
 * with maximum estimated sizes:
 * (1) 22310, the exact number of Terms contained in the books of lista10.txt
 * (2) 11155, half the previous size
 * (3)   112, a hundredth (approx.) of the previous size
 *
 * Analyzing said results, the most efficient parameters for
 * the Linked Hash Table implementation for Map index of lista10.txt
 * can be determined, i.e., which is the "best" hashCode() method
 * to be implemented in Term and which is the "best" size estimation
 * that it could have (as a maximum).
 * 
 * @author (EDA-QA) 
 * @version (Curso 2020-2021)
 */

public class EfficiencyTest {
    
    public static final String FILE_INPUT =  "lista10.txt";
    public static final String DIR_FILE_INPUT = LibrarySearch.bookListDir;
    public static final String BOOK_DIR = LibrarySearch.bookDir;
    
    public static final String DIR_OUTPUT = LibrarySearch.bookListDir + "res";
    public static final String FILE_OUTPUT = DIR_OUTPUT + File.separator + "histo"; 
    
    public static final int NUM_TERMS = 22310;
    public static final int[] MAX_TERMS = {NUM_TERMS,   // 22310
            (int) Math.round(NUM_TERMS / 2.0),          // 11155
            (int) Math.round(NUM_TERMS / (2.0 * 100))}; // 112
    
    public static void main(String[] args) {
 
        String hashCodeBase;
        String withRehashing;
        String rhCountStr = "";
        int rhCount = 0;
        if (args.length != 2) { 
            System.out.println("\tYou must indicate both:\n "
                               + "\t - The base used for Term's hashCode()\n"
                               + "\t - Whether the HashTable uses rehashing (YES/NO)");
        } else if (args[1].equalsIgnoreCase("yes") && !HashTable.REHASHING) {
            System.out.println("\tERROR: the version of HashTable you're using\n" +
                               "\t        DOES NOT IMPLEMENT REHASHING        \n" +
                               "\tTo run the program its 2nd parameter must be \"NO\"");
        } else if (args[1].equalsIgnoreCase("no") && HashTable.REHASHING) {
            System.out.println("\tERROR: the version of HashTable you're using\n" +
                               "\t          DOES IMPLEMENT REHASHING          \n" +
                               "\tTo run the program its 2nd parameter must be \"YES\"");
        } else {
            hashCodeBase = args[0];
            withRehashing = args[1].toLowerCase();
            if (withRehashing.equals("yes")) { withRehashing = "RH"; }
            else { withRehashing = ""; }
            try {
                int hcBase = Integer.parseInt(hashCodeBase);
                File dir = new File(DIR_OUTPUT); dir.mkdir();
                for (int i = 0; i < MAX_TERMS.length; i++) {

                    // 1.- Build the HashTable from the terms contained
                    // in the books of the list NOM_FILE_INPUT, using as
                    // hashCode a polynomial function with base hcBase,
                    // size NUM_TERMS, a maximum estimated size of MAX_TERMS[i]
                    // and initial load factor 0.75.
                    HashTable<Term, Term> tH = createTable(MAX_TERMS[i], hcBase);

                    // 2.- Show on screen all values of the tH Table built in
                    //     the i-th test:
                    // (a) Real load factor of the Table
                    // (b) Standard deviation of the buckets' sizes
                    // (c) Average cost to look up a key
                    System.out.println("\tMaximum Estimated Size = " + MAX_TERMS[i]);
                    System.out.printf("\tLoad Factor = %4.3f\n", tH.loadFactor());
                    System.out.printf("\tStandard Dev. = %4.3f\n", tH.standardDeviation());
                    
                    if (withRehashing.equals("RH")) {
                        rhCount = tH.numberOfRehashings();
                        rhCountStr = String.valueOf(rhCount);
                    }
                    System.out.printf("\tAverage cost to look up a key = %4.3f \n\n",
                                     tH.avgLookUpCost());

                    // 3.- Generate and save a .txt file with an occupation
                    // histogram of tH at DIR_OUTPUT subdirectory with the name:
                    PrintWriter pw = new PrintWriter(new File(FILE_OUTPUT 
                                                              + "B" + hashCodeBase
                                                              + "(" + MAX_TERMS[i] + ")"
                                                              + rhCountStr
                                                              + withRehashing + ".txt"));
                    pw.println(tH.histogram());
                    // graph: gnuplot> plot "histo...txt" using 1:2 with boxes
                    pw.close(); 
                }
                
            } catch (IOException e) {
                System.err.println("File not found " + FILE_INPUT);
            }
        }
    }

    // Creates a new Linked Hash Table with a maximum estimated size of maxTerms
    private static HashTable<Term, Term> createTable(int maxTerms, int hcBase)
        throws FileNotFoundException {

        HashTable<Term, Term> res = new HashTable<Term, Term>(maxTerms);
        boolean read = true;
        Scanner list = new Scanner(new File(DIR_FILE_INPUT + File.separator + FILE_INPUT));
        //System.out.println("Loading HashTable from..." + FILE_INPUT);

        while (list.hasNext()) {
            String bookName = list.next();
            read &= extractKeysFrom(BOOK_DIR + bookName, res, hcBase);
        }   
        if (!read) { throw new FileNotFoundException(); }
        return res;
    }

    // Updates HashTable t with the new terms that appear in bookName
    private static boolean extractKeysFrom(String bookName, HashTable<Term, Term> t, int hcBase) {
        boolean res = true;     
        try {            
            Scanner book = new Scanner(new File(bookName));
            //int sepIndex = bookName.lastIndexOf(File.separator);
            //String title = bookName.substring(sepIndex + 1);
            //System.out.println("... " + title.substring(0, title.indexOf(".txt")));
            int lineNum = 0;
            while (book.hasNext()) {
                String line = book.nextLine();
                String[] words = line.split(LibrarySearch.separators);
                for (int i = 0; i < words.length; i++) {
                    if (LibrarySearch.isTerm(words[i])) {
                        Term key = new Term(words[i].toLowerCase(), hcBase);
                        Term value = t.get(key);
                        if (value == null) {
                            t.put(key, key);
                        }
                    }
                }
            }
        } catch (FileNotFoundException e) {
            System.err.println("Error! Can't find file for book: " + bookName);
            res = false;     
        }
        return res;
    }
}