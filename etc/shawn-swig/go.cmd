:: Wrapper Code aus interface Datei generieren
swig -c++ -java -package de.swarmnet.shawn -outdir de/swarmnet/shawn -o output/shawn_wrap.cpp shawn.i

:: Compile java code and generate jar file
javac de/swarmnet/shawn/*.java
jar cf output/jshawn.jar de/swarmnet/shawn/*.class
