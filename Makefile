all:GPSHandler

CXXFLAGS=-g -I ./LLH -I ./include -I ./GPIOControl -std=c++11 -pthread

GPSHandler: objs/sdev.o objs/llh.o objs/gps.o objs/GPSHandler.o objs/BlackCore.o objs/BlackGPIO.o objs/lcd.o
	$(CXX) $(CXXFLAGS) objs/GPSHandler.o objs/gps.o objs/llh.o objs/sdev.o objs/BlackCore.o objs/BlackGPIO.o -o GPSHandler

objs/sdev.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/sdev.o LLH/sdev.cpp

objs/llh.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/llh.o LLH/llh.cpp

objs/gps.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/gps.o LLH/gps.cpp

objs/BlackCore.o:
	$(CXX) $(CXXFLAGS) -c -o objs/BlackCore.o GPIOControl/BlackCore.cpp

objs/BlackGPIO.o:
	$(CXX) $(CXXFLAGS) -c -o objs/BlackGPIO.o GPIOControl/BlackGPIO.cpp

objs/lcd.o:
	$(CXX) $(CXXFLAGS) -c -o objs/lcd.o include/lcd.h

objs/GPSHandler.o: 
	$(CXX) $(CXXFLAGS) -c -o objs/GPSHandler.o GPSHandler.cpp



clean:
	-rm objs/*.o GPSHandler
