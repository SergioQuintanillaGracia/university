#!/bin/bash

javac *.java
jar -cvfe ./build/Ants.jar Ants *.class
java -jar ./build/Ants.jar "$@"
