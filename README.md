# plagDetectionSystem
Team Members: 
Isha Kumari - 2023csb1124 
Deepak Roat - 2023CSB1116 
Karan Verma	- 2023CSB1127 

Mentor - Seema Barda

PROJECT - Plagiarism detection system

This project detects plagiarism between two C code files by analysing token frequencies, N-gram sequences, 
and structural similarities through Abstract Syntax Trees (ASTs).
To compile the files, give the command 
"gcc main.c tokenizer.c file_util.c hash_table.c helper.c ngram.c plagiarism_check.c ast.c parser.c  -o plagiarism_detector" and then to find out the percentage similarities betweeen the input files, run the main.c file using the command - "./plagiarism_detector test1.c test2.c", where test1.c and test2.c are the files in which similarities is being calculated.
