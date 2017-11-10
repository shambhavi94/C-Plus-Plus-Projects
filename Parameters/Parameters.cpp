#include <iostream>
using namespace std;
//Part 1
void fillArray(int data[], int size, int value){

    for(int i = 0; i < size; i++){

       data[i] = value;

    }

}
//Part 2
void calculateGrades(float scores[], char grades[], int size){


    for (int i = 0; i < size; i++){
        //for(int j=0; j < size; j++){
        if (scores[i] >= 90){
            char* grade = "A";
            //cout << "Grade is A" << endl;
            grades[i] = *grade;

        }
        else if (scores[i] >= 80 && scores[i] < 90){
            //cout << "Grade is B << endl;
            char* grade = "B";
            grades[i] = *grade;
        }
        else if (scores[i] >= 70 && scores[i] < 80){
            //cout << "Grade is C" << endl;
            char* grade = "C";
            grades[i]  = *grade;
        }
        else if (scores[i] >= 60 && scores[i] < 70){
            //cout << "Grade is D" << endl;
            char* grade = "D";

            grades[i] = *grade;
        }
        else{
            //cout << "Grade is F" << endl;
              char* grade = "F";
            grades[i] = *grade;
        }

    }

}
//Part 3
float getAverageScore(float scores[], int size){

    float sumAnswer = 0;
    float answer;

    for (int i = 0; i < size; i++){

        sumAnswer = sumAnswer + scores[i];
        }

        answer = sumAnswer/size;

        return answer;
}
//Part 4
float getMinScore(float scores[], int size){

    float minAnswer = scores[0];

    for (int i = 0; i < size; i++){

        if (scores[i] < minAnswer)
            minAnswer = scores[i];
    }
    return minAnswer;
}
//Part 5
float getMaxScore(float scores[], int size){

    float maxAnswer = scores[0];

    for (int i = 0; i < size; i++){

        if (scores[i] > maxAnswer)
            maxAnswer = scores[i];
    }
    return maxAnswer;
}
//Part 6
void sortScores(float scores[], int size){
    int j, flag = 1;
    int temp;
    for (int i = 1; i < size && flag; i++){

       flag = 0;
       for (j = 0; j < size - 1; j++){

        if (scores[j+1] < scores[j]){
            temp = scores[j];
            scores[j] = scores[j+1];
            scores[j+1] = temp;
            flag = 1;
        }
        }
}


}
//Part 7
float getMedian(float scores[], int size){
    float newArray[size];
    for (int k = 0 ; k < size; k++){
        newArray[k] = scores[k];
    }

    int j, flag = 1;
    float temp;
    for (int i = 1; i < size && flag; i++){
        flag = 0;
       for (j = 0; j < size - 1; j++){
            if (newArray[j+1] < newArray[j]){
                temp = newArray[j];
                newArray[j] = newArray[j+1];
                newArray[j+1] = temp;
                flag = 1;
            }
        }

    }
    int middle;
    float median;
    middle = size/2;
    if(size % 2 == 0){
        median = (newArray[middle-1]+newArray[middle])/2;
        return median;
    }
    else{
        median = newArray[middle];
        return median;
        }
}

//Part 8
int countGrade (char grades[], char grade, int size){

    int _count = 0;
    for (int i = 0; i < size; i++){
        if(grades[i] == grade){
            _count++;
        }
    }
    return _count;
}
int main(){
//Part1
    int size1 = 3;
    int _value = 7;
    int _data[size1];

    fillArray(_data, size1, _value);

//Part 2
    int size2 = 5;
    float scores2[size2] = {80, 90};
    char _grades [size2] = {'A', 'B', 'C', 'D', 'F'};
    calculateGrades(scores2, _grades, size2);

//Part 3

    int size3 = 3;
    float scores3[size3] = {3,2,3};
    float part3 = getAverageScore(scores3, size3);

    cout << "The answer is " << part3 << endl;

//Part 4
     int size4 = 4;
     float scores4[size4] = {3, 10, 20, 2};
     float part4 = getMinScore(scores4, size4);

     cout << "The answer is " << part4 << endl;
//Part 5

    int size5 = 4;
     float scores5[size5] = {3, 10, 20, 2};
     float part5 = getMaxScore(scores5, size5);

     cout << "The answer is " << part5 << endl;

    //part 7
    int _size = 8;
    float scores[_size] = {79.3, 100.7, 72.6, 60.8, 72.7, 94.1, 98.5, 88.9 };




    float _answer =  getMedian(scores, _size);
    cout << _answer << " " << endl;

//Part 8

    int size8 = 4;
    char grades[size8] = {'a', 'a', 'c', 'd'};
    char grade = 'a';

    int _answer_ = countGrade(grades, grade, size8);
    cout << _answer_ << " ";

}
