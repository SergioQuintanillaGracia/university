package again.pract3;

import java.io.*;

public class ScannerRedes {

   InputStream Stream;
   int ultimaLectura;
   boolean usarUltimaLectura = false;



   public ScannerRedes(InputStream istr) {
      Stream = istr; 
   }

	public int read() throws IOException {  return Stream.read(); }


   public String nextLine() {
     String cadena = "";
     int byte1;
     try{
       if (usarUltimaLectura == true) 
          {byte1= ultimaLectura;
           usarUltimaLectura = false;
       }
       else {
           byte1 = Stream.read();
           if (byte1 == -1) return cadena;
       }

       int byte2 = Stream.read();
       if (byte2 == -1) {
           // byte1 puede ser /n o ser diferente de /n, pero en ambos casos
           // no se ha completado una nueva linea y por tanto devolvermos 
           //una cadena vacia
           return cadena;
         }
       
       while (byte2 != 10) { //el codigo 10 es /n
         cadena = cadena + Character.toString((char)byte1);
         byte1 = byte2;
         byte2 = Stream.read();
         if (byte2 == -1) {return "";} //no se ha completado una linea
       }
       if (byte1 != 13) {cadena = cadena + Character.toString((char)byte1);} //el codigo 13 es /r
     }
     catch (IOException e) {System.out.println("Error procesando la linea");} 
     return cadena;
   }



   public boolean hasNext() {
     if (usarUltimaLectura == true) return true;
     try {
       ultimaLectura = Stream.read();
     }  
     catch (IOException e) {System.out.println("Error en hasNext");} 
     if (ultimaLectura != -1)
        {usarUltimaLectura = true;
         return true;
        } 
     return false;
   }


}
