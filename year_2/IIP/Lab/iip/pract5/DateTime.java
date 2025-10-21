package pract5;

import pract4.TimeInstant;
/**
 * Class {@code DateTime} allows to represent a timeStamp of a precise date (day, month and year).
 * 
 * @author IIP 
 * @version Academic Year 2023-24
 */
public class DateTime { 
    // Attributes
    /** private attribute to represent the day of the current DateTime. */
    private int day;  
    /** private attribute to represent the month of the current DateTime. */
    private int month;
    /** private attribute to represent the year of the current DateTime. */
    private int year;
    /** private attribute to represent the instant day of the current DateTime. */
    private TimeInstant instant;
   
    /**
     * Create a new DateTime with the given values of day, month, year, hours, 
     * and minutes, (assuming they are correct)
     * @param d int day of the new DateTime.
     * @param m int month of the new DateTime.
     * @param y int year of the new DateTime.
     * @param hour int hours of the new DateTime.
     * @param min int minutes of the new DateTime.
     */
    public DateTime(int d, int m, int y, int hour, int min) {
        day = d;
        month = m;
        year = y;
        instant = new TimeInstant(hour, min);
    }
    
    /** 
     * Create a new DateTime with the given values of day, month, and year 
     * (assuming they are correct) and the current UTC instant. 
     * @param d int día del DateTime.
     * @param m int mes del DateTime.
     * @param y int anyo del DateTime.
     */
    public DateTime(int d, int m, int y) {
        day = d;
        month = m;
        year = y;
        instant = new TimeInstant();
    }
    
    /**
     * Returns ths value of attribute day of the current DateTime.
     * @return {@code int}, the day.
     */
    public int getDay() { return day; }
   
    /**
     * Returns ths value of attribute month of the current DateTime.
     * @return {@code int}, the month.
     */
    public int getMonth() { return month; }
   
    /**
     * Returns ths value of attribute year of the current DateTime.
     * @return {@code int}, the year.
     */
    public int getYear() { return year; }
    
    /**
     * Returns ths value of attribute instant of the current DateTime..
     * @return {@link TimeInstant}, the instant.
     */
    public TimeInstant getInstant() { return instant; }
   
    /**
     * updates the attribute day of the current DateTime.
     * @param d int el nuevo día.
     */
    public void setDay(int d) { day = d; }
   
    /**
     * updates the attribute month of the current DateTime.
     * @param m int the new month.
     */
    public void setMonth(int m) { month = m; }
   
    /**
     * updates the attribute year of the current DateTime.
     * @param y int the new year.
     */
    public void setYear(int y) { year = y; }
   
    /**
     * updates the attribute instant of the current DateTime.
     * @param h int the new hours.
     * @param m int the new minutes.
     */
    public void setInstant(int h, int m) { 
        instant.setHours(h);
        instant.setMinutes(m);
    }
         
    /** 
     *  Return a String representation of the current DateTime 
     *  in the format "hh:mm dd/mm/yyyy".
     *  @return {@link String}, DateTime in format "hh:mm dd/mm/yyyy".
     */
    public String toString() {
        // Version 1:
        // String dd = "0" + day;
        // dd = dd.substring(dd.length() - 2);
        // String mm = "0" + month;
        // mm = mm.substring(mm.length() - 2);
        // String yyyy = "000" + year;
        // yyyy = yyyy.substring(yyyy.length() - 4);
        
        // Version 2:
        // String dd = day / 10 + "" + day % 10;
        // String mm = month / 10 + "" + month % 10;
        // String yyyy = year / 1000 + "" 
                    // + (year % 1000) / 100 + ""
                    // + ((year % 1000) % 100) / 10 + ""
                    // + ((year % 1000) % 100) % 10;
                     
        // Versión 3: CHANGE THE ASSIGNMENTS TO FULFILL THE SPECIFICATION
        String dd = "%02d".formatted(day);
        // TO COMPLETE dd with a zero if it is necessary
        String mm = "%02d".formatted(month);
        // TO COMPLETE dd with a zero if it is necessary
        String yyyy = "%04d".formatted(year);
        // TO COMPLETE dd with one or more zeros if it is necessary
        return instant + " " + dd + "/" + mm + "/" + yyyy;
    } 
   
    /**
     * Check the equality of the current DateTime with the given Object o.
     * @param o Object.
     * @return {@code boolean}, {@code true} if o is a Date time and instant, 
     *   day, month, and day are the same of the current DateTime.
     * {@code false} otherwise.
     */
    public boolean equals(Object o) {
        // TO COMPLETE, CHANGE THE RETURN TO FULFILL THE SPECIFICATION
        if (o instanceof DateTime) {
            DateTime obj = (DateTime) o;
            
            return obj.day == day && obj.month == month
            && obj.year == year && obj.instant.equals(instant);
        }
        
        return false;
    } 
  
    /** 
     * Chronologically compares the current DateTime with a given DateTime.
     * @param d reference to an object of class {@link DateTime} 
     * to compare with the current DateTime.
     * @return {@code int},returns a negative, positive or zero value if the 
     * current DateTime is, respectively, earlier, later or equal to the given DateTime.
     */
    public int compareTo(DateTime d) {
        // Versión 1:
        /*
        int res = 1;
        if (year < d.year) { res = -1; }
        else if (year == d.year) {
            if (month < d.month) { res = -1; }
            else if (month == d.month) {
                if (day < d.day) { res = -1; }
                else if (day == d.day) {
                    res = instant.chronoComparison(d.instant);
                }
            }
        }
        return res;
        */
        
        // Version 2:
        // COMMENT THE VERSION 1 AND COMPLETE THE VERSION 2 AS IS
        // DESCRIBED IN THE STATEMENT OF LAB ACTIVITY 5 (PDF)
        if (this.year != d.year) { return this.year - d.year; }
        else if (this.month != d.month) { return this.month - d.month; }
        else if (this.day != d.day) { return this.instant.chronoComparison(d.instant); }
        
        return 0;
    }    
    
    /**
     * Checks if a given year is a leap year.
     * @param year int the year.
     * @return {@code boolean}, returns {@code true} if the year is a leap year. 
     * and {@code false} otherwise.
     */   
    public static boolean isLeap(int year) { 
        // Leap year is the year divisible by 4, unless it is 
        // secular year - the last year of each century, ending in "00" -,  
        // in which case it must also be divisible by 400.
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0); 
    }    
   
    /** 
     * Returns the length of a month of a given year in days.
     * @param month int the month.
     * @param year int the year.
     * @return {@code int}, the days of the month of the year.
     */
    public static int lengthOfMonth(int month, int year) {
        int numberOfDays = 31;
        if (month == 2) { 
            numberOfDays = 28;
            if (isLeap(year)) {
                numberOfDays = 29;
            }
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11) { numberOfDays = 30; }
        return numberOfDays;
    }
    
    /** 
     * Checks whether the given hours, minutes, day, month and year data 
     * correspond to a correct time and date.
     * @param hours int the hours.
     * @param min int the minutes.
     * @param day int the day.
     * @param month int the month.
     * @param year int the year.
     * @return {@code boolean}, returns {@code true} if the data is correct. 
     * and {@code false} otherwise.
     */
    public static boolean isCorrect(int hours, int min, int day, int month, int year) {
        boolean correct = false;
        if (0 <= hours && hours <= 23 && 0 <= min && min <= 59) {
            if (year > 0 && (month >= 1 && month <= 12) && (day >= 1 && day <= 31)) {
                int numberOfDays = lengthOfMonth(month, year);
                if (day <= numberOfDays) { correct = true; }
            }
        }
        return correct;
    }
}
