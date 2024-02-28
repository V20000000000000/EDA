1. 學號：
2. 姓名：
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++
5. 檔案壓縮方式: 
6. 各檔案說明：
	main.cpp		        : 主程式
   	AnalysisAndVisualization.h 	: header file
	AnalysisAndVisualization.cpp    : cpp file
	DataPreprocess.h 		: Declaration of data Preprocess 
    	DataPreprocess.cpp              : Definition of data preprocess 
	GeneratePlot.h  		: Declaration of generating the form of output 
	GeneratePlot.cpp  		: Definition of generating the form of output
	readme.txt		        : this file
	prefix.txt                      : prefix file
    	set_tics.txt                    : setting tics file
	suffix.txt                      : suffix file

7. 編譯方式說明：        	
    主程式：
        g++ -o main.exe main.cpp DataPreprocess.cpp GeneratePlot.cpp AnalysisAndVisualization.cpp
	
8. 執行、使用方式說明：
    主程式：
   	   
   	   執行檔的命令格式為 :
   	   ./main.exe input.txt output.gp

	   ex: ./main.exe input.txt output.gp
