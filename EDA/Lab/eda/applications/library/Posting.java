package applications.library;

/**
 * Posting: class that represents the information associated
 * to the appearance of a Term in a book's Index, i.e., the title
 * of the book and (the number of) the line where the Term appears.
 * 
 * @author (EDA) 
 * @version (Curso 2020-2021)
 */
public class Posting {
    
    protected String bookTitle;
    protected int lineNumber;

    /** Creates a Posting of a book's title bT and its line number lN. */
    public Posting(String bT,  int lN) {
        bookTitle = bT.substring(0, bT.indexOf(".txt"));
        lineNumber = lN;
    }

    /** Returns a String with a Posting in textual format. */
    public String toString() {
        return bookTitle + ", line " + lineNumber + "\n";
    }
}