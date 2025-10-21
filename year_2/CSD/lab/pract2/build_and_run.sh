#!/bin/bash

javac *.java
jar -cvfe ./build/PPhilo.jar PPhilo *.class
java -jar ./build/PPhilo.jar
