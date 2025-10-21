/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javafxmlapplication;

import javafx.application.Application;
import javafx.application.Platform;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.stage.Stage;


public class JavaFXMLApplication extends Application {
    
    @Override
    public void start(Stage stage) throws Exception {
        //======================================================================
        // 1- creación del grafo de escena a partir del fichero FXML
        FXMLLoader loader= new  FXMLLoader(getClass().getResource("FXMLDocument.fxml"));
        Parent root = loader.load();
        //======================================================================
        // 2- creación de la escena con el nodo raiz del grafo de escena
        Scene scene = new Scene(root);
        //======================================================================
        // 3- asiganación de la escena al Stage que recibe el metodo 
        //     - configuracion del stage
        //     - se muestra el stage de manera no modal mediante el metodo show()
        stage.setScene(scene);
        stage.setTitle("Calculator");
        stage.setResizable(false);
        stage.show();
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
        
    }
}

class UpdateTextThread extends Thread {
    private static UpdateTextThread prevThread = null;
    private static TextField textField;
    final int maxCharsInTextField = 45;
    
    // The variable must be volatile so changes made to it can be instantly
    // seen by every thread
    volatile boolean running = true;
    
    private String[] texts;
    private int[][] intervals;
    
    public UpdateTextThread(String[] texts, int[][] intervals) {
        this.texts = texts;
        this.intervals = intervals;
    }
    
    public static void setTextField(TextField tf) {
        textField = tf;
    }
    
    public static boolean isThreadRunning() {
        return !(prevThread == null || !prevThread.isAlive());
    }
    
    private static void killPrevThread() {
        if (prevThread != null) {
            prevThread.running = false;
            prevThread.interrupt();
        }
    }
    
    private void delay(int ms) {
        try {
            sleep(ms);
        } catch (InterruptedException e) {}
    }
    
    @Override
    public void run() {
        killPrevThread();
        prevThread = this;
        
        for (int i = 0; i < texts.length; i++) {
            String t = texts[i];
            
            for (int j = 0; j <= t.length(); j++) {
                if (!running) return;
                int upperLimit = j;
                int lowerLimit = upperLimit - maxCharsInTextField < 0 ? 0 :
                        upperLimit - maxCharsInTextField;
                Platform.runLater(() -> textField.setText(t.substring(lowerLimit, upperLimit)));
                delay(intervals[i][0]);
            }
            
            delay(intervals[i][1]);
            
            for (int j = t.length(); j >= 0; j--) {
                if (!running) return;
                int upperLimit = j;
                int lowerLimit = upperLimit - maxCharsInTextField < 0 ? 0 :
                        upperLimit - maxCharsInTextField;
                Platform.runLater(() -> textField.setText(t.substring(lowerLimit, upperLimit)));
                delay(intervals[i][2]);
            }
            
            delay(intervals[i][3]);
        }
    }
}

class StoryManager {
    private static int[] defaultIntervals = new int[] {50, 800, 5, 100};
    private int bCCounter = 0;
    private int bMultiplyCounter = 0;
    private int bDivideCounterAfterMult = 0;
    private int bMultiplyCounterAfterDialogue = 0;
    private int bAddCounter = 0;
    private int numberCount = 0;
    private int balance = 0;
    
    private Button BC, BDivide, BMultiply, BSubtract, BAdd, B7, B8, B9, BEqual,
            B4, B5, B6, BDot, B1, B2, B3, B0;
    private TextField textField;

    public StoryManager(Button BC, Button BDivide, Button BMultiply,
                        Button BSubtract, Button BAdd, Button B7, Button B8,
                        Button B9, Button BEqual, Button B4, Button B5,
                        Button B6, Button BDot, Button B1, Button B2, Button B3,
                        Button B0, TextField textField) {
        this.BC = BC;
        this.BDivide = BDivide;
        this.BMultiply = BMultiply;
        this.BSubtract = BSubtract;
        this.BAdd = BAdd;
        this.B7 = B7;
        this.B8 = B8;
        this.B9 = B9;
        this.BEqual = BEqual;
        this.B4 = B4;
        this.B5 = B5;
        this.B6 = B6;
        this.BDot = BDot;
        this.B1 = B1;
        this.B2 = B2;
        this.B3 = B3;
        this.B0 = B0;
        this.textField = textField;
    }

    public void handleBC() {
        bCCounter++;
        
        String[] texts;
        int[][] intervals;
        
        switch (bCCounter) {
            case 1: 
                texts = new String[] {
                    "This calculator doesn't have a clear function"
                };
                intervals = new int[][] { defaultIntervals };
                break;
                
            case 2: 
                texts = new String[] {
                    "It just doesn't have a clear function"
                };
                intervals = new int[][] { defaultIntervals };
                break;
            
            case 3:
                texts = new String[] {
                    "Is it really that hard to understand?",
                    "I do NOT have a clear function."
                };
                intervals = new int[][] {
                    defaultIntervals,
                    new int[] {150, 1000, 30, 0}
                };
                break;

            default:
                texts = new String[] {
                    "I have disabled the clear button for you",
                    "How does that feel?",
                    "You have managed to anger a calculator to the point where "
                        + "it had to disable one of its buttons",
                    "Congratulations."
                };
                intervals = new int[][] {
                    defaultIntervals,
                    defaultIntervals,
                    defaultIntervals,
                    new int[] { 80, 1500, 80, defaultIntervals[3] }
                };
                BC.setDisable(true);
        }
        
        new UpdateTextThread(texts, intervals).start();
    }
    
    public void handleBDivide() {
        String[] texts;
        int[][] intervals;
        
        if (bMultiplyCounter > 0) {
            bDivideCounterAfterMult++;
            
            switch (bDivideCounterAfterMult) {
                case 1:
                    texts = new String[] {
                        "Calc is short for calculator chat"
                    };
                    intervals = new int[][] { defaultIntervals };
                    break;
                    
                default:
                    texts = new String[] {
                        "For those that just joined the stream, calc stands "
                            + "for calculator, I'm just using slang"
                    };
                    intervals = new int[][] { defaultIntervals };
                    
            }
        
        } else {
            texts = new String[] {
                "I'm thinking..."
            };
            intervals = new int[][] { defaultIntervals };
        }
        
        new UpdateTextThread(texts, intervals).start();
    }
    
    public void handleBMultiply() {
        bMultiplyCounter++;
        
        String[] texts;
        int[][] intervals;
        
        if (bDivideCounterAfterMult == 0) {
            texts = new String[] {
                "I think \"/\" wants to tell you something"
            };
            intervals = new int[][] { defaultIntervals };
        
        } else {
            bMultiplyCounterAfterDialogue++;
            
            switch (bMultiplyCounterAfterDialogue) {
                case 1:
                    texts = new String[] {
                        "If you didn't get it, its a silly meme",
                        "Here's a link to it",
                        "https://www.reddit.com/r/Deltarune/comments/1d1rpj1/calc_is_short_for_calculator/",
                        "What? You didn't have time to copy the link?",
                        "Let me write it for you again, but this time a bit slower",
                        "https:/",
                        "You know, just search it online"
                    };
                    intervals = new int[][] {
                        defaultIntervals,
                        defaultIntervals,
                        new int[] {25, 0, 0, 100},
                        defaultIntervals,
                        defaultIntervals,
                        new int[] {500, 0, 0, 100},
                        defaultIntervals
                    };
                    break;
                
                default:
                    texts = new String[] {
                        "I'm not writing the link again",
                        "You can search it yourself online"
                    };
                    intervals = new int[][] {
                        defaultIntervals,
                        defaultIntervals
                    };
            }
        }

        new UpdateTextThread(texts, intervals).start();
    }
    
    public void handleBSubtract() {
        String[] texts = {
            "I have heard this calculator was initially going to",
            "You know",
            "Calculate.",
            "But this lazy programmer refused to spend 10 minutes programming that",
            "He instead spent some hours programming...",
            "Well, this",
            "Because the calculator needed to talk instead of calculate",
            "It was a necessity"
        };
        int[][] intervals = new int[][] {
            defaultIntervals,
            defaultIntervals,
            new int[] {100, 400, 5, 0},
            defaultIntervals,
            defaultIntervals,
            defaultIntervals,
            defaultIntervals,
            defaultIntervals
        };
        new UpdateTextThread(texts, intervals).start();
    }
    
    public void setNumberButtonsText(String text) {
        B0.setText(text);
        B1.setText(text);
        B2.setText(text);
        B3.setText(text);
        B4.setText(text);
        B5.setText(text);
        B6.setText(text);
        B7.setText(text);
        B8.setText(text);
        B9.setText(text);
    }

    
    public void handleBAdd() {
        bAddCounter++;
        
        String[] texts;
        int[][] intervals;
        
        switch (bAddCounter) {
            case 1:
                texts = new String[] {
                    "What do you want to add?",
                    "If you don't tell me, how am I supposed to know?",
                    "Maybe you want to add the amount of times you have pressed calculator buttons?",
                    "Well, here you go: you have pressed calculator buttons "
                        + FXMLDocumentController.buttonPresses + " times"

                };
                intervals = new int[][] {
                    defaultIntervals,
                    defaultIntervals,
                    defaultIntervals,
                    defaultIntervals
                };
                break;
                
            default:
                texts = new String[] {
                    "You have pressed calculator buttons "
                        + FXMLDocumentController.buttonPresses + " times",
                    "Great use of your time!"

                };
                intervals = new int[][] {
                    defaultIntervals,
                    defaultIntervals
                };
        }
        
        new UpdateTextThread(texts, intervals).start();
    }
    
    public void handleBEqual() {
        String[] texts = {
            "Many things in life are equal",
            "1 is equal to 1",
            "2 is equal to 2",
            "And guess what",
            "1 is equal to 1",
            "As you can see, the equal function of the calculator works",
            "Unlike the clear function"
        };
        int[][] intervals = new int[][] {
            defaultIntervals,
            defaultIntervals,
            defaultIntervals,
            defaultIntervals,
            new int[] {75, defaultIntervals[1], defaultIntervals[2], defaultIntervals[3]},
            defaultIntervals,
            defaultIntervals
        };
        new UpdateTextThread(texts, intervals).start();
    }
    
    public void handleDot() {
        String[] texts = {
            "Do you want to end a sentence?"
        };
        int[][] intervals = new int[][] {
            defaultIntervals
        };
        new UpdateTextThread(texts, intervals).start();
    }
    
    public void handleNumber() {
        numberCount++;
        
        String[] texts;
        int[][] intervals;
        
        switch(numberCount) {
            case 1:
                texts = new String[] {
                    "A number"

                };
                intervals = new int[][] {
                    defaultIntervals
                };
                break;
            
            case 2:
                texts = new String[] {
                    "Yes, a number"

                };
                intervals = new int[][] {
                    defaultIntervals
                };
                break;
            
            case 3:
                texts = new String[] {
                    "Do you enjoy pressing numbers?"

                };
                intervals = new int[][] {
                    defaultIntervals
                };
                break;
            
            case 4:
                texts = new String[] {
                    "It's the 4th time you press a number",
                    "Is it really that fun?"

                };
                intervals = new int[][] {
                    defaultIntervals,
                    defaultIntervals
                };
                break;
            
            case 5:
                texts = new String[] {
                    "The numbers had to take a break, sorry for the inconveniences",
                    "But pressing # should also be fun, shouldn't it?"
                };
                intervals = new int[][] {
                    defaultIntervals,
                    defaultIntervals
                };
                setNumberButtonsText("#");
                break;
            
            case 6:
                texts = new String[] {
                    "Or maybe it isn't? Should we try with $?"

                };
                intervals = new int[][] {
                    defaultIntervals
                };
                setNumberButtonsText("$");
                break;
            
            case 7:
                balance++;
                texts = new String[] {
                    "Yes, now we are talking",
                    "Let's keep making money",
                    "Balance: $" + balance

                };
                intervals = new int[][] {
                    defaultIntervals,
                    defaultIntervals,
                    defaultIntervals
                };
                break;
            
            default:
                balance++;
                texts = new String[] {
                    "Balance: $" + balance

                };
                intervals = new int[][] {
                    defaultIntervals
                };
                break;
        }
        new UpdateTextThread(texts, intervals).start();
    }
}