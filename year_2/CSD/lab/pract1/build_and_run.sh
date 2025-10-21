#!/bin/bash

javac *.java
jar -cvfe ./build/PPool.jar PPool *.class
java -jar ./build/PPool.jar
