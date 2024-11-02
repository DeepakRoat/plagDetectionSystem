# plagDetectionSystem
Team Members: 
Isha Kumari - 2023csb1124 
Deepak Roat - 2023CSB1116 
Karan Verma	- 2023CSB1127 

Mentor - Seema Barda

PROJECT - Plagiarism detection system

This project detects plagiarism between two C code files by analysing token frequencies, N-gram sequences, 
and structural similarities through Abstract Syntax Trees (ASTs).
### HOW TO COMPILE AND RUN
To compile the files, give the command 
"gcc main.c tokenizer.c file_util.c hash_table.c helper.c ngram.c plagiarism_check.c ast.c parser.c  -o plagiarism_detector" and then to find out the percentage similarities betweeen the input files, run the main.c file using the command -
"./plagiarism_detector input1a.c input1b.c", where "input1a.c" and "input1b.c" are the files in which similarities is being calculated.
### WHAT TO EXPECT FROM OUTPUT 
Input files are two c code files and output will be plagiarism calculated via 3 different methods .
1. Frequency based : indicates surface-level similarities ,this only detects plagiarism involving minor changes like renaming variables .
2. N-Gram : captures pattern-based similarities , this indicates structural similarity between codes.
3. AST based :  A high AST similarity percentage indicates deep structural similarity, variable names may be changed this part indicated logical similarity between codes. This has max contribution to final plag.
4. Final plag percentage (includes all three factors)
