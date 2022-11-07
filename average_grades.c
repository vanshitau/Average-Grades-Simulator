#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

int total_commas(FILE *f){
    int c;
    int count = 0;
    int quotation= 0;
    while ((c = fgetc(f)) != EOF) {
        putchar(c);
        //count the number of commas
        if(c == '"'){ 
            quotation++;
        }

        else{
            int is_even = (quotation % 2);
            if(c==','){
                if(is_even == 0){
                    count++;
                }
            }
        }
    }
    //rewind f to go to the begining of the file f
    rewind(f);
    return count;
}

//count the number of lines
int count_lines(FILE *f){
    int total_lines = 0;
    char buf[1000];
    while (fgets(buf, 1000, f)){
        total_lines++;  
    }
    //rewind the function
    rewind(f);
    return total_lines;
}

int *array_index(char *s, const int num_marks){
    //create an array of the size of commas found
    int quotation = 0;
    int j = 0;
    char ch = 0;
    int *arr = calloc((num_marks+2),sizeof(int));
    int i = 0;
    for(i = 0; i<strlen(s); i++){
        ch = s[i];
        if(ch == '"'){ // check if a string exists
            quotation++;
            if(quotation == 2){
                quotation = 0;
            }
        }
        else{
            if(ch ==','){
                if(quotation == 0){
                    //append to the list
                    arr[j] = i;
                    j++;
                }
            }
        }
        arr[j] = i+1;
   }
   return arr;
}

int substring(char *buf, int start, int finish, char *output){
    int i = 0;
    int j = 0;
    //Add the marks to a list
    for(i = start; i < finish; i++){
        output[j] = buf[i];
        j++;
    }
    output[j] = 0;
    return finish+1;
}
//calculate average for all columns of marks
//The marks represent the sum of all marks for each column
//The marks column represent all the students for that column
//which is the number of students per column
//num_marks is the number of marks in each row
void calculate_average(int num_marks, int marks[], int marks_column[]){
    for(int i =0; i < num_marks; i++){ 
        double average = (double) marks[i]/marks_column[i];
        printf("\nA%d: %.1lf\n", (i+1), average);
    }
}
void main(){
    //open the file in read mode
    const char *file1 = "grades2.txt";
    FILE *f = fopen(file1, "r");

    if (!f){
        fprintf(stderr, "The file could not be opened");
        exit(1);
    }

    int count_com = total_commas(f);
    //calculate marks per student

    //printf("Count_com: %d\n", count_com);
    int lines = count_lines(f);

    //need the total number of assignments (this will create the array)
    double num_marks = (count_com / lines) - 1;

    //array to store marks
    int *marks = calloc((num_marks), sizeof(int));
    char first_name[100];
    char last_name[100];

    char buf[1000];
    int num_students = 0;
    rewind(f);
    f = fopen(file1, "r");
    
    //make an array to store column totals
    int *marks_column = calloc((num_marks), sizeof(int));
    for(int i = 0; i < num_marks; i++){
        marks_column[i] = 0;
    }

    while (fgets(buf, 1000, f)){
       num_students++;

        int *arr = array_index(buf, num_marks);
        int index = 0;
        
        int start = 0;
        int finish = arr[index];
        index++; 

        int mark = 0;
        char output[100];
        
        //The first name and the last name is stated
        start = substring(buf,start,finish,first_name);

        //Only one name is stated (just the last name)
        finish = arr[index];
        index++;
        start = substring(buf, start, finish, last_name);

        int j = 0;
        for(int i = 0; i < num_marks; i++){
            finish = arr[index];
            index++;

            //For called for every assignment
            start = substring(buf,start,finish,output);

            //Convert the strings to an integer
            mark = atoi(output);
            if(mark != 0){
                marks_column[j]++;
            }
            
            //store marks for the column
            marks[j] += mark;
            j++;
        }
    }  
   
    calculate_average(num_marks, marks, marks_column);
        
    fclose(f);
}