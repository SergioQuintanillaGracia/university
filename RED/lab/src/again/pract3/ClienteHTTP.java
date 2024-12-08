package again.pract3;

import java.net.*;
import java.io.*;

class ClienteHTTP {
    static String server_name;
    static int port;
    static Socket s;
    static ScannerRedes input;
    static PrintWriter output;


    public static void envia_peticion(String object) {
        output.printf("GET %s HTTP/1.1\r\nHost: %s:%d\r\nConnection: close\r\n\r\n",
                object, server_name, port);
        output.flush();
    }

    public static void lee_linea_estado() {
        System.out.println(">>>>>>>>>>>>>>> LINEA DE ESTADO <<<<<<<<<<<<<<<");
        System.out.println(input.nextLine());
    }

    public static void lee_cabeceras() {
        System.out.println(">>>>>>>>>>>>>>>    CABECERAS    <<<<<<<<<<<<<<<");
        while (input.hasNext()) {
            String line = input.nextLine();
            if (line.isEmpty()) break;
            System.out.println(line);
        }
    }

    public static void lee_cuerpo_texto() {
        System.out.println(">>>>>>>>>>>>>>>   CUERPO TEXTO  <<<<<<<<<<<<<<<");
        while (input.hasNext()) {
            System.out.println(input.nextLine());
        }
    }

    public static void lee_cuerpo_binario(String filename) throws IOException {
        System.out.println(">>>>>>>>>>>>>>>  CUERPO BINARIO <<<<<<<<<<<<<<<");
        FileOutputStream fos = new FileOutputStream(filename);
        int counter = 0;

        while (true) {
            int b = input.read();
            if (b == -1) break;
            fos.write(b);
            counter++;
        }

        System.out.printf("Read %d bytes\n", counter);

        fos.close();
    }



    public static void main(String args[]) throws Exception {
        server_name = "zoltar.redes.upv.es";
        port = 80;
        s = new Socket(server_name, port);
        input = new ScannerRedes(s.getInputStream());
        output = new PrintWriter(s.getOutputStream());

        envia_peticion("/");
        lee_linea_estado();
        lee_cabeceras();
        lee_cuerpo_texto();

        s.close();

        s = new Socket(server_name, port);
        input = new ScannerRedes(s.getInputStream());
        output = new PrintWriter(s.getOutputStream());

        envia_peticion("/oto1.jpg");
        lee_linea_estado();
        lee_cabeceras();
        lee_cuerpo_binario("oto1.jpg");

        s.close();

        s = new Socket(server_name, port);
        input = new ScannerRedes(s.getInputStream());
        output = new PrintWriter(s.getOutputStream());

        envia_peticion("/oto2.jpg");
        lee_linea_estado();
        lee_cabeceras();
        lee_cuerpo_binario("oto2.jpg");

        s.close();
    }
}
