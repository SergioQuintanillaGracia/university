//
// Students can safely ignore this file.
// 
package utils;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

//
// Students can safely ignore this file.
// 

/**
 * <p>ProgramParameters utility class.
 * 
 * <p>This class allows a generic specification of parameters for command line programs.
 * <p>Once a ProgramParameters instance is created, user can call the "get" method to get a 
 * parameter value
 * 
 * @author Pablo Galdamez
 */
public class ProgramParameters {
    
    //
    // Information specified for each Parameter
    //
    private final Map<String, ParamInfo> paramInfos = new HashMap<>();

    //
    // Information specified for each Program
    //
    private final Map<String, ProgramInfo> programInfos = new HashMap<>();
    
    //
    // This program name, as we guess it using Java facilities.
    //
    private final String programName;

    //
    // Map as the result of parsing command line arguments
    // This map is empty until someone calls the "parse" method.
    //    
    private final Map<String, String> programParameters = new HashMap<>();

    
    /**
     * Build a new ProgramParameters instance
     * 
     * @param paramSpec Vector of parameter specifications. Each parameter speficiation is a String vector.
     * This specification contains 1,2 or 3 fields. First is the parameter name. The optional second field is
     * the informative text to show for this parameter when printing usage. The optional third flied is the parameter
     * default value.
     * 
     * @param programSpec Vector of program specifications. Each program specification is a String vector with 1 or
     * more fields. First field is the program name, the rest are parameter definitions. Each parameter definition has
     * the form of "parameterName[:required]". That is parameter name optionally followed by a colon and a required 
     * statement. Required statement can be 0,false, 1, true. It spefifies whether that parameter is required.
     */
    public ProgramParameters (String [][] paramSpec, String[][] programSpec) {
        
        //
        // Get program name, from classic sun property.
        //
        this.programName = System.getProperty("sun.java.command").split(" ")[0];

        //
        // Build paramInfo from the parameter specification
        //
        for (String [] param: paramSpec) {
            
            // paramenter name
            ParamInfo pinfo = new ParamInfo (param[0]);
            
            // inforative text
            if (param.length > 1) pinfo.text = param[1];
            
            // default value
            if (param.length > 2) pinfo.defaultValue = param[2];            
            
            // save the paramInfo.
            this.paramInfos.put (param[0].toLowerCase(), pinfo);
        }
        
        //
        // Build de programInfos from program specifications
        //
        for (String [] program: programSpec) {
            this.programInfos.put (program[0].toLowerCase(), new ProgramInfo (program));
        }                
    }
    
    /**
     * Add usage example to the named program
     * 
     * @param program program name
     * @param example line of text showing valid parameters assignment
     * 
     * @return this
     */
    public ProgramParameters addExample (String program, String example) {
        return addExample (program, example, "");
    }

    /**
     * Add usage example to the named program
     * 
     * @param program program name
     * @param example line of text showing valid parameters assignment
     * @param javaParams command line parameters for java. Classpath, etc.
     * 
     * @return this
     */
    public ProgramParameters addExample (String program, String example, String javaParams) {
        programInfos.get(program.toLowerCase()).addExample(example, javaParams);
        return this;
    }

    
    /**
     * Add a checker to validate a given paramenter.
     * <p> checkers are optional and help to stop at usage, if parameters are not valid.
     * 
     * @param parameter parameter name
     * @param checker Function to call as checker.
     * @return this
     */

    public ProgramParameters addChecker (String parameter, Consumer<String> checker) {
        paramInfos.get (parameter.toLowerCase()).checker = checker;
        return this;
    }
    
    
    /**
     * Get the value of a program parameter
     * 
     * <p>This is the simple method that finally we want to obtain from a ProgramParameters instance
     * 
     * @param paramName Parameter name
     * @return parameter value
     */

    public String get (String paramName) {
        return programParameters.get (paramName.toLowerCase());
    }

    /**
     * Returns true if argument is same as programName. False otherwise.
     * @param name other program name
     * @return true if the other program name is equal to our programName
     */
    public boolean isProgram(String name) {
        return name.equalsIgnoreCase(programName);
    }
    

    /**
     * Class ParamInfo -- parameters information
     * 
     * Class to hold information about a parameter: name, info, default value, and value checker
     */    
    private static class ParamInfo {
        //
        // Parameter name, case as specified by user
        //
        private final String name;
        
        // Line of text to show as informative text for the named parameter when printing usage
        //
        private String text;
        
        // Default value for the parameter
        //
        private String defaultValue;
        
        // Checker to test validity of parameters.
        // Checker should throw an exception if parameter value is not acceptable
        //
        private Consumer<String> checker;

        // Simple constructor        
        ParamInfo (String name) {this.name = name;}
    }

    /**
     * ProgramInfo -- Program usage information 
     * 
     * Class to hold information about a program: name, acceptable parameters,
     * required parameters and usage examples
     * 
     */
    private static class ProgramInfo {
        //
        // Program name, with case as specified by user
        //
        private final String name;
        
        // Acceptable parameters in lower case
        //
        private final List<String> params;
        
        // list of required parameters, in lower case
        //
        private final List<String> requiredParams = new LinkedList<>();
        
        // User specified examples of usage.
        //
        private final List<String[]> examples = new LinkedList<>();

        
        /**
         * Create a program Info from the Configuration String list for a program.
         * 
         * <p> first vector component is the program name.
         * 
         * <p> rest of the componets are parameter definitions of the form: parameter[:required]
         * 
         * Meaning parameter name, optionally followed by a colon and "required" statement.
         * Required statement can be 1, true, 0, false.
         * If required statement is not present, paramenter is considered not required.
         * 
         * @param program program parameters configuration string
         */
        
        private ProgramInfo (String[] program) {
            this.name = program[0];            
            List<String> paramDefs = Arrays.asList(program).subList(1, program.length);            
            
            //
            // create param list same size as specified param definitions.
            //
            this.params = new ArrayList<>(paramDefs.size());
            
            //
            // loop param definitions, and extract the 2 parts that build definition ==> name:required
            // If there is a ":" and something else after, we take it always as required, except if value is "0" or "false".
            //
            for (String paramDef: paramDefs) {
                String [] parts = paramDef.split(":");
                params.add (parts[0].toLowerCase()); // Save parameter as lowercase
                if (parts.length > 1) {
                    String req = parts[1].toLowerCase().trim();
                    if (!req.equals("0") && !req.equals("false")) requiredParams.add(parts[0].toLowerCase());
                }
            }
        }

        public String getProgramName () {return name;}
        public List<String> parameters () {return params;}
        public List<String> requiredParameters () {return requiredParams;}
        
        public ProgramInfo addExample (String ex, String jparams) {
            this.examples.add (new String []{jparams, ex});
            return this;
        }
    }

         
    /**
     * Print usage information on System.out
     * 
     */
    public void usage () {
       ProgramInfo programInfo = programInfos.get (programName.toLowerCase());

       System.out.println ();

       System.out.println ("USAGE HELP");
       System.out.println ();
       System.out.print ("    java " + programName + " ");
       for (String param: programInfo.parameters()) {
           ParamInfo pinfo = paramInfos.get(param.toLowerCase());
           if (programInfo.requiredParameters().contains(param.toLowerCase())) 
               System.out.print (pinfo.name + "=... ");
           else 
               System.out.print ("[" + pinfo.name + "=...] ");           
       }              
       
       System.out.println ();
       System.out.println ();
       System.out.println ();
       System.out.println ("USAGE -- valid arguments:");
       System.out.println ();
       
       for (String param: programInfo.params) {
           ParamInfo pinfo = paramInfos.get (param.toLowerCase());
           System.out.println ("   " + pinfo.name + " = " + pinfo.text);           
       }

       // Show examples section 
       System.out.println ();
       if (!programInfo.examples.isEmpty()) {
           System.out.println ();
           if (programInfo.examples.size() == 1) System.out.println ("Example:");
           else System.out.println ("Examples:");
           for (String[] example: programInfo.examples) {           
               String javap = example[0].isEmpty() ? " ": " " + example[0] + " ";
               System.out.println ("   java" + javap + programName + " " + example[1]);
           }
       }
       System.out.println ();
       System.exit(1);        
    }
    
    /**
     * Parse command line arguments.
     * 
     * <p> on success, user can call the "get" method to get program parameters.
     * 
     * <p> on failure, print usage and terminate program.
     * 
     * @param args command line parameters
     */
    
    public void parse (String args[]) {
        try {
            parseEx (args);
        } catch (Exception e) {
            usage ();
        }
    }

    /**
     * Parse command line arguments.
     * 
     * <p> on success, user can call the "get" method to get program parameters.
     * 
     * <p> on failure, throws exception
     * 
     * @param args command line parameters
     * 
     * @throws java.lang.Exception Cannot parse exception
     */

    public void parseEx (String args[]) throws Exception {

        ProgramInfo programInfo = programInfos.get(programName.toLowerCase());        
        Map<String, String> argMap = mapOf (args);

        //
        // Check parameters for validity
        //
        for (String key: argMap.keySet()) {
            ParamInfo pinfo = paramInfos.get(key.toLowerCase());
            if (pinfo.checker != null) {
                pinfo.checker.accept(argMap.get(key));
            }
        }
        
        //
        // Add default values to explicit parameters
        //
        for (String param: programInfo.params) {
            if (!argMap.containsKey (param.toLowerCase())) {
                ParamInfo paramInfo = paramInfos.get (param.toLowerCase());                
                if (paramInfo.defaultValue != null) argMap.put(param.toLowerCase(), paramInfo.defaultValue);
            }
        }
        
        //
        // Check we don't provide extra argments, other than specifed
        //
        for (String param: argMap.keySet()) {
            if (!programInfo.params.contains (param.toLowerCase())) {
                throw new Exception ("Unknow parameter found " + param);
            }
        }
        
        //
        // Check required parameters exist
        //
        for (String param: programInfo.requiredParameters()) {
            if (!argMap.containsKey (param.toLowerCase())) {
                throw new Exception ("Required parameter " + param + " not specified");
            }
        }
        
        //
        // Finally this is what we want to have: the map of program parameters
        //
        programParameters.putAll(argMap);
    }

    /**
     * Return a map for the arguments, with key in lowercase, and value as unchanged string.
     * 
     * @param args Command line arguments, format should be "key1 = val key2=val2 ..."
     * @return The map
     * @throws Exception parse errors, for instance even number of paramenters.
     */    
    public Map<String,String> mapOf (String [] args) throws Exception {
        List<String> flatArgs = new ArrayList<>();
        for (String str:args) {
            String [] parts = str.split("=");
            if (parts.length > 2) throw new Exception ("Usage error");
            for (String part:parts) if (!part.isEmpty()) flatArgs.add(part);
        }
       if (!flatArgs.isEmpty() && flatArgs.size() % 2 != 0) throw new Exception ("Usage error");
       
       Map<String, String> argMap = new HashMap<>();
       for (int i=0; i<flatArgs.size(); i+=2) {
           argMap.put(flatArgs.get(i).toLowerCase(), flatArgs.get(i+1));
       }
       
       return argMap;
    }
    
    
}
