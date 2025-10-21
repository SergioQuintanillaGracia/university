package pract7;

import java.util.Scanner;
import pract5.DateTime;
/**
 * Clase Test7: clase programa que prueba las funcionalidades de las clases Event y Calendar.
 * 
 * @author IIP - Práctica 7
 * @version Curso 2023/24
 */
public class Test7 {
	/** Hidden constructor */
    private Test7() { } // there are no objects of this class
    
    public static void main(String[] args) {
        Scanner kbd = new Scanner(System.in);
        
        Calendar c = new Calendar();
                
        DateTime start = new DateTime(16, 3, 2024, 18, 0);
        DateTime end = new DateTime(20, 3, 2024, 21, 0);
        Event e = new Event(start, end, "CIRQUE DU SOLEIL", Event.SHOW);        
        boolean res = c.add(e);
        
        start = new DateTime(29, 11, 2024, 8, 30);
        end = new DateTime(29, 11, 2024, 13, 30);
        e = new Event(start, end, "JIDINF'24", Event.CONGRESS);        
        res = c.add(e);
        
        start = new DateTime(1, 1, 2024, 9, 0);
        end = new DateTime(6, 1, 2024, 20, 30);
        e = new Event(start, end, "PLAYMOBIL", Event.EXHIBITION);        
        res = c.add(e);
        
        System.out.println("\nThere are " + c.getNumEvents() + " events in the calendar");
        
        int opc;
        do {
            opc = menu(kbd);
            switch (opc) {
                case 1: 
                    addEvent(c, kbd); 
                    System.out.println("\nThere are " + c.getNumEvents() + " events in the calendar");
                    break;                        
                case 2: 
                    deleteEvent(c, kbd);
                    System.out.println("\nThere are " + c.getNumEvents() + " events in the calendar");
                    break; 
                case 3:    
                    showAllEvents(c, kbd);        
                    break;       
                case 4:
                    showEventLarger(c, kbd);
                    break;
                case 5:
                    showEventType(c, kbd);
                    break;    
                default: 
                    System.out.println("\n*** Bye ***");
            }
        } while (opc != 0); 
    }
    
    /**
     * Method that displays in the terminal window a menu with the 
     *  operations that can be performed on a calendar, and performs 
     *  the reading of a valid option from the keyboard.
     * @param t Scanner, the standard input (the keyboard).
     * @return int, the chosen option.
     */
    private static int menu(Scanner t) {
        int opc;
        do {
            System.out.println("\n*********** CALENDAR ***********");
            System.out.println(" 1. Add event");
            System.out.println(" 2. Delete event");
            System.out.println(" 3. Show all events");
            System.out.println(" 4. Show longest event");
            System.out.println(" 5. Show events of certain type");
            System.out.println(" 0. Finish");
            System.out.println("********************************");
            System.out.print("Choose an option: ");
            opc = t.nextInt();
            t.nextLine();
            if (opc < 0 || opc > 5) {
                System.out.println("\nIncorrect option.\n");
            }
        } while (opc < 0 || opc > 5);
        return opc;
    }
    
    /**
     * Method that displays in the terminal window a menu with the 
     *  types of events that can be found in a calendar, and performs 
	 *  the reading of a valid option from the keyboard.
     * @param t Scanner, the standard input (the keyboard).
     * @return int, the chosen option.
     */
    private static int typeMenu(Scanner t) {
        int opc;
        do {
            System.out.println("\nType:");
            System.out.println(" 1. Congress");
            System.out.println(" 2. Show");
            System.out.println(" 3. Exhibition");            
            System.out.print("Choose an option: ");
            opc = t.nextInt();
            t.nextLine();
            if (opc < 1 || opc > 3) {
                System.out.println("\nIncorrect option.\n");
            }
        } while (opc < 1 || opc > 3);
        return opc;
    }
    
    /**
     * Method that returns a String representing the given type event type, where 1 <= type <= 3.
     * @param type int, the type. Precondition: 1 <= type <= 3.
     * @return String, the type of event: congress, show or exposition.
     */
    private static String getStringType(int type) { 
        String res = "";
        switch (type) {
            case Event.CONGRESS: 
                res += "congress"; break;
            case Event.SHOW: 
                res += "show"; break; 
            default:
                res += "exhibition";
        }
        return res;    
    }
    
    /**
     * Method that adds an event to the given calendar, reading the event data from the keyboard.
     * @param c Calendar, the calendar.
     * @param kdb Scanner, the standard input (keyboard).
     */ 
    private static void addEvent(Calendar c, Scanner kbd) {
        System.out.print("\nTitle? ");
        String title = kbd.nextLine().toUpperCase();
        
        int type = typeMenu(kbd);
        
        DateTime start = new DateTime(4, 7, 2023), end = new DateTime(4, 7, 2023); 
        boolean isCorrect, isPosterior;
        System.out.println("\nStart date:");
        do {
            System.out.print("Day? ");
            int day = kbd.nextInt();
            System.out.print("Month? ");
            int month = kbd.nextInt();
            System.out.print("Year? ");
            int year = kbd.nextInt();
            System.out.print("Hours? ");
            int hours = kbd.nextInt();
            System.out.print("Minutes? ");
            int minutes = kbd.nextInt();
            kbd.nextLine();    
            isCorrect = DateTime.isCorrect(hours, minutes, day, month, year);
            if (isCorrect) { start = new DateTime(day, month, year, hours, minutes); }
            else { System.out.println("\nStart date is incorrect.");}
        } while (!isCorrect);
        
        System.out.println("\nEnd date (later to " + start + "):");
        do {
            System.out.print("Day? ");
            int day = kbd.nextInt();
            System.out.print("Month? ");
            int month = kbd.nextInt();
            System.out.print("Year? ");
            int year = kbd.nextInt();
            System.out.print("Hours? ");
            int hours = kbd.nextInt();
            System.out.print("Minutes? ");
            int minutes = kbd.nextInt();
            kbd.nextLine();    
            isCorrect = DateTime.isCorrect(hours, minutes, day, month, year);
            end = new DateTime(day, month, year, hours, minutes);
            isPosterior = start.compareTo(end) < 0;
            if (!isCorrect || !isPosterior) { 
                System.out.println("\nEnd date is incorrect or not later than start date.");
            }
        } while (!isCorrect || !isPosterior);
        
        Event e = new Event(start, end, title, type);        
        if (c.add(e)) { System.out.println("\nAdded event."); }
        else { System.out.println("\nFailed to add."); }
    }
    
    /**
     * Method that deletes an event, whose title it gets from the keyboard, 
     *  from the calendar given as parameter.
     * @param c Calendar, the calendar.
     * @param kdb Scanner, the standard input (keyboard).
     */ 
    private static void deleteEvent(Calendar c, Scanner kbd) {
        System.out.print("\nTitle? ");
        String title = kbd.nextLine().toUpperCase();             
        if (c.delete(title)) { System.out.println("\nDeleted event."); }
        else { System.out.println("\nFailed to delete."); }
    }  
    
    /**
     * Method that displays in the terminal window all the events of the given calendar.
     * @param c Calendar, the calendar.
     * @param kdb Scanner, the standard input (keyboard).
     */ 
    private static void showAllEvents(Calendar c, Scanner kbd) {
        String res = c.toString();
        if (res.equals("")) { System.out.println("\nThere are no events."); }
        else { System.out.println("\n" + res); }
    }
    
    /**
     * Method that displays the longest event of a given calendar.
     * @param c Calendar, the calendar.
     * @param kdb Scanner, the standard input (keyboard).
     */ 
    private static void showEventLarger(Calendar c, Scanner kbd) {  
        if (c.getNumEvents() == 0) { System.out.println("\nThere are no events."); }
        else {
            Event res = c.eventLarger();
            System.out.println("\n" + res.toString());            
        }
    }
    
    /**
     * Method that displays the events of a given type of the given calendar.
     * @param c Calendar, the calendar.
     * @param kdb Scanner, the standard input (keyboard).
     */ 
    private static void showEventType(Calendar c, Scanner kbd) {
        int type = typeMenu(kbd);
        Event[] aux = c.filterByType(type);
        if (aux.length == 0) { System.out.println("\nThere are no " + getStringType(type) + "-type events."); }
        else {
            System.out.println();
            for (int i = 0; i < aux.length; i++) {
                System.out.println(aux[i].toString()); 
            }
        }
    }
}
