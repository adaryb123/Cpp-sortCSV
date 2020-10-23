# Cpp-sortCSV
program for sorting .CSV files by given columns

Arguments are passed to main function in this order: [input_file_name] [output_file_name] [column_sequence].
For example: input.csv output.csv 2,4,3

Column sequence determines priority of columns when sorting the file. For example, 2,4,3 means the rows will at first be sorted by values in 2nd column, and if some rows have those values equal, they will be sorted by values in 4th column and then 3rd.
