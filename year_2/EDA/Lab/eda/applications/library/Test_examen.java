package applications.library;

import java.util.Arrays;
import java.io.FileNotFoundException;
import applications.library.LibrarySearch;

public class Test_examen {
    public static void main(String[] args) {
	LibrarySearch buscador;
	try {
	    buscador  = new LibrarySearch();
	    System.out.println("================");
	    System.out.println("Buscando \"disco\"");
	    System.out.println("================");

	    String res = buscador.condensedSearch("disco");
	    System.out.println(res);
	    if (test(res)) {
                System.out.println("CORRECT");
            }
	    else {
        	System.out.println("INCORRECT");
            }
    	} catch (FileNotFoundException err) {
	    System.err.println("Error: Could not find the book" );
        }
    }

    private static boolean test(String res){

        int [] vec_res = {144, 949, 978, 3552, 2136, 2158, 2305, 2307, 2311, 9910, 9910, 9914, 9935, 9936, 9946, 9954, 9955, 9957, 10004, 10077, 10078, 10173, 3080, 495, 498, 2386, 2433, 2498, 2608,1574};

        res = res.replaceAll("[a-zA-Z]", "");
	res = res.replaceAll("[-_\\[\\]\t]","");
	res = res.replace("3.0","");
	res = res.replaceAll(" ","");

	String[] res_vec_string = res.split("[,\n]");
	int[] res_vec_int = new int[res_vec_string.length];

        for (int i = 0; i < res_vec_string.length; i++) {
            res_vec_int[i] = Integer.parseInt(res_vec_string[i]);
        }

	Arrays.sort(vec_res);
	Arrays.sort(res_vec_int);

        if (res_vec_int.length != vec_res.length){
	    return false;
        }

	for (int i = 0; i < vec_res.length; i++) {
	    if (vec_res[i] != res_vec_int[i]) {
			return false;
        }
	}

	return true;
    }
}
