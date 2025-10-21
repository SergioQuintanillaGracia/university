package applications.library;

import libraries.dataStructures.models.Map;
import libraries.dataStructures.models.ListPOI;
import libraries.dataStructures.linear.LinkedListPOI;
import libraries.dataStructures.scattered.HashTable;
import java.util.Scanner;
import java.io.File;
import java.io.FileNotFoundException;

/**
 * LibrarySearch: class that represents an efficient search or lookup engine,
 * i.e., with an Index of a Digital Library. To this end, its basic functions
 * are...
 * 1.- INDEXING: create the library's Index, represented with a Map
 *     implemented through a Linked Hash Table.
 * 2.- SEARCHING: look up from the library's Index the information that
 *     exists in it about a given word or term (posting list).
 * 
 * @author (EDA-QB) 
 * @version (Curso 2021-2022)
 */
 
public class LibrarySearch {

    // A LibrarySearch HAS...

    // A String bookList, the text file that contains the names of
    // the (.txt files of the) books of a digital library. Its default value
    // is shown below, and can be changed if applicable.
    protected static String bookList =  "lista10.txt"; // "lista.txt"

    // A String bookListDir, the directory where the bookList file is located.
    // Its default value is shown below, and should only be changed when
    // absolutely necessary.
    protected static String bookListDir = "applications" + File.separator
                                             + "library" + File.separator;

    // A String bookDir, the directory where the (.txt files of the) books in
    // bookList are located. Its default value is shown below, and should
    // only be changed when absolutely necessary.
    protected static String bookDir = "applications" + File.separator
                                         + "library" + File.separator
                                             + "TXT" + File.separator;

    // A String separators, a regular expression that defines the word
    // separators that appear in the books from bookList.
    // Its default value is shown below, and can be changed if applicable.
    protected static String separators =
       "[[ ]*|[,]*|[\\.]*|[\t]*|[:]*|[;]*|[(]*|[)]*|[/]*|[!]*|[?]*|[¿]*|[“]*|[”]*|[+]*]+";

    // AN int maxTerms, the maximum number of different terms contained in
    // the books from bookList. Its default value is shown below, and should
    // be changed if applicable.
    protected static int maxTerms =  22310; // 105985 for "lista.txt"

    // A Map index, that represents the Index of a digital library.
    // It uses Term as its Key and ListPOI<Posting> as its Value
    protected Map<Term, ListPOI<Posting>> index;

    // A boolean verb, that enables/disables verbosity in the search engine.
    // Its default value is 'true' (enabled), and it can be disabled when desired.
    protected static boolean verb = true; // false;

    /**
     * Creates a Search engine for the Digital Library formed by the books
     * in bookList. In practice, it creates the index of the library with,
     * at most, maxTerms.
     * @throws FileNotFoundException If the list or any of the book's files
     *     are not where they are expected to be.
     */
    public LibrarySearch() throws FileNotFoundException {
        boolean res = true; 
        Scanner scan = new Scanner(new File(bookListDir + bookList));
        if (verb) {
            System.out.println("Creating the library's Index... " + bookList);
        }

        // Initialize the Map index in the Search engine using as dynamic
        // type the HashTable class.
        // NOTE: HashTable's constructor requires as its argument the number
        //       of Entries that the table will hold (at most).
        index = new HashTable<Term, ListPOI<Posting>>(maxTerms);
        while (scan.hasNext()) {
            String bookName = scan.next();
            String bookFile = bookDir + bookName;
            res &= indexBook(bookFile);
        }
        if (verb) {
            System.out.println("Terms Indexed (size of the Map) = " + index.size());
        }
        if (!res) { throw new FileNotFoundException(); }
    }

    /** Returns true after updating the Search engine of a Digital Library with
     *  the terms contained in bookFile (a file.txt). As a consequence, the library's
     *  index is updated with the terms in bookFile.
     *  The method returns false if bookFile does not exist.
     */
    public boolean indexBook(String bookFile) {
        boolean res = true;
        try {
            Scanner book = new Scanner(new File(bookFile));
            int sepIndex = bookFile.lastIndexOf(File.separator);
            String title = bookFile.substring(sepIndex + 1);
            if (verb) {
                System.out.println("Indexing book... "
                                  + title.substring(0, title.indexOf(".txt")));
            }
            int lineNum = 0;
            while (book.hasNext()) {
                String line = book.nextLine();
                lineNum++;
                String[] words = line.split(separators);
                Posting p = new Posting(title, lineNum);
                for (int i = 0; i < words.length; i++) {
                    String word = words[i];
                    if (isTerm(word)) {
                        // Update the Map index, associating Posting p to a Key
                        Term key = new Term(word.toLowerCase());
                        ListPOI<Posting> valor = index.get(key);
                        if (valor == null) {
                            valor = new LinkedListPOI<Posting>();
                        }
                        valor.add(p);
                        index.put(key, valor);
                    }
                }
            }
        } catch (FileNotFoundException e) {
            System.err.println("Error! book file not found: " + bookFile);
            res = false;
        }
        return res;
    }

    /**
     * Returns in text format (String) the result of searching the term associated
     * to aWord in a Digital Library's Index (frequency of the term in the library,
     * and list of book titles and lines where the term appears in the library, i.e.,
     * its Posting list).
     */
    public String search(String aWord) {
        String res = "";
        Term key = new Term(aWord.toLowerCase());
        ListPOI<Posting> value = index.get(key);
        if (value == null) {
            res += "The word \"" + aWord + "\" doesn't appear in any book on this library";
        } else {
            res += "Found " + value.size() + "  instances of the word \"" + aWord
                   + "\" in...\n" + value;
        }
        return res;
    }
    
    public String condensedSearch(String word) {
        // Get the bucket corresponding to the passed word
        Term wordTerm = new Term(word);
        ListPOI<Posting> bucket = index.get(wordTerm);
        
        // Create a map that will store the occurrences for the word
        // in each book
        Map<String, ListPOI<Integer>> cases = new HashTable<>(bucket.size());
        
        // Iterate through the bucket and count occurrences
        bucket.begin();
        while (!bucket.isEnd()) {
            Posting p = bucket.get();
            if (cases.get(p.bookTitle) == null) {
                // The book doesn't yet have an entry in the map
                ListPOI<Integer> list = new LinkedListPOI<>();
                list.add(p.lineNumber);
                // Add the entry in the map with just the current occurrence
                cases.put(p.bookTitle, list);
            } else {
                // The book has an entry in the map, we just have to update
                // the ListPOI
                ListPOI<Integer> list = cases.get(p.bookTitle);
                list.end();
                list.add(p.lineNumber);
                cases.put(p.bookTitle, list);
            }
            
            bucket.next();
        }
        
        // Create a string with the correct format for the result
        StringBuilder sb = new StringBuilder("");
        
        ListPOI<String> keys = cases.keys();
        keys.begin();
        
        // Iterate through every key in the map
        while (!keys.isEnd()) {
            sb.append("%s %s\n".formatted(keys.get(), cases.get(keys.get())));
            keys.next();
        }
        
        // Return the resulting text
        return sb.toString();
    }

    /** Checks whether aWord is a term in a Digital Library's Index, i.e.,
     *  whether aWord is non-empty and contains only letters. */
    protected static boolean isTerm(String aWord) {
        if (aWord.length() == 0)
            return false;
        for (int i = 0; i < aWord.length(); i++)
            if (!Character.isLetter(aWord.charAt(i)))
                return false;
        return true;
    }
    
    /** Returns a ListPOI with the terms of the Digital Library's index that
     *  appear n times in its books, or null if none exist.
     */
    public ListPOI<Term> termsRepeated(int n) {  
        ListPOI<Term> terms = new LinkedListPOI<>();
        ListPOI<Term> keys = index.keys();
        
        keys.begin();
        while (!keys.isEnd()) {
            if (index.get(keys.get()).size() == n) {
                terms.add(keys.get());
            }
            keys.next();
        }
        
        if (terms.size() == 0) return null;
        return terms;
    }

    /** Returns a ListPOI with the terms of the Digital Library's Index that
     *  appear only once in its books, i.e., the so-called "hapax legomena"
     *  of the Digital Library, or null if none exist.
     */
    public ListPOI<Term> hapax() {
        return termsRepeated(1);
    }
}    
