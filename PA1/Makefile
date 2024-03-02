genPlot: main.o AnalysisAndVisualization.o DataPreprocess.o GeneratePlot.o
	g++ main.o AnalysisAndVisualization.o DataPreprocess.o GeneratePlot.o -o genPlot

main.o : main.cpp
	g++ -c main.cpp

AnalysisAndVisualization.o : AnalysisAndVisualization.cpp AnalysisAndVisualization.h
	g++ -c AnalysisAndVisualization.cpp

DataPreprocess.o : DataPreprocess.cpp DataPreprocess.h
	g++ -c DataPreprocess.cpp

GeneratePlot.o : GeneratePlot.cpp GeneratePlot.h
	g++ -c GeneratePlot.cpp

clean : 
	rm *.o genPlot
