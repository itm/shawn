Shawn: The fast, highly customizable sensor network simulator
=====

Abstract
-------------
Shawn [1] is a discrete event simulator for sensor networks. Due to its high customizability, 
it is extremely fast but can be tuned to any accuracy that is required by the simulation or application. 

Shawn's licence terms are available in shawn-licence.txt

Documentation
-------------
[https://github.com/itm/shawn/wiki](https://github.com/itm/shawn/wiki)

Installation Quick start (non-authorative)
-------------
The following is a really quick start guide for the faint at heart using Linux or Cygwin on Windows. For a full installation guide, please refer to http://shawn.sourceforge.net

- Requirements: CMake (http://www.cmake.org), gcc and g++ (http://gcc.gnu.org)
- Navigate to /path/to/shawn/buildfiles
- Run "ccmake ../src"
- Type "c" and then "g"
- Run "make"
- Run "./shawn"

[1]	A. Kroeller, D. Pfisterer, C. Buschmann, S.P. Fekete, S. Fischer: "Shawn: A new approach to simulating wireless sensor networks", Proceedings of the Design, Analysis, and Simulation of Distributed Systems (DASD 05), San Diego, 2005.
