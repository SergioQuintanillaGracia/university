package exam1;

/**
 * Clase MyString: Exercise Lab Practice 2 - First Mid Term Exam Lab
 * 
 * @author PRG 
 * @version Academic year 2021-22
 */
public class MyString {    
    /** Hidden constructor, there are no objects of this class */
    private MyString() { }
    
    /**
     * Returns true if char c is a vowel and if not, it returns false .
     */
    private static boolean isVowel(char c) {
        final String VOWELS = "aeiouAEIOU";
        return VOWELS.indexOf(c) != -1;
    }
    
    /**
     * Returns an String resulting of remove all the characters of 
     * the String that are vowels
     */
    public static String removeVowels(String s) { 
        if (s.length() == 0) {
            return "";
        }
        if (isVowel(s.charAt(0))) {
            return removeVowels(s.substring(1));
        } else {
            return s.charAt(0) + removeVowels(s.substring(1));
        }
    } 
    
    /**
     *    You can use this main method to test some Strings as an Example, 
     *  but passing these examples does not guarantee the maximum mark.
     *   You can also check your code with different Strings.  
     */
    public static void main(String[] args) {        
        System.out.println("This method does Eight calls to removeVowels(String).");
        System.out.println("The first one is done with the empty String.");
        System.out.println("You can see as a result for each case the original String, the expected solution and your solution.\n");        
        
        String[] s = {"", "bcd", "abcd", "bcde", "aaa4uuu", "ctrl And Z", "aeiou", "Shine On Until Tomorrow"}; 
        String[] sol = {"", "bcd", "bcd", "bcd", "4", "ctrl nd Z", "", "Shn n ntl Tmrrw"};
        
        System.out.printf("%-30s %-25s %-15s\n", "      s", "removeVowels(s)", "Tu solución");
        for (int i = 0; i < s.length; i++) {
            System.out.printf("%-30s %-25s ", s[i], sol[i]);
            System.out.printf("%-15s\n", removeVowels(s[i]));
        }
    }
}