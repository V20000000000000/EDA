1. 學號：b11032055
2. 姓名：康閎茗
3. 使用之程式語言：C++
4. 使用之編譯平台：Linux GNU g++
5. 檔案壓縮方式: tar zcvf b11032055-p1.tgz b11032055-p1
6. 各檔案說明：
	b11032055-p1/main.cpp		                : 主程式
    b11032055-p1/AnalysisAndVisualization.h 	: header file
	b11032055-p1/AnalysisAndVisualization.cpp   : cpp file
	b11032055-p1/DataPreprocess.h 		        : Declaration of data Preprocess 
    b11032055-p1/DataPreprocess.cpp             : Definition of data preprocess 
	b11032055-p1/GeneratePlot.h  		        : Declaration of generating the form of output 
	b11032055-p1/GeneratePlot.cpp  		        : Definition of generating the form of output
	b11032055-p1/readme.txt		                : this file
	b11032055-p1/prefix.txt                     : prefix file
    b11032055-p1/set_tics.txt                   : setting tics file
	b11032055-p1/suffix.txt                     : suffix file

7. 編譯方式說明：        	
    主程式：
        g++ -o main.exe main.cpp DataPreprocess.cpp GeneratePlot.cpp AnalysisAndVisualization.cpp
	
8. 執行、使用方式說明：
    主程式：
   	   
   	   執行檔的命令格式為 :
   	   ./main.exe input.txt output.gp

	   ex: ./main.exe input.txt output.gp