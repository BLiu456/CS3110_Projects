/*
Name: Benjamin Liu
Class: CS3110
Due Date: 4/13/2022

Project 1: Recognizing floating point numbers 
Used C++, Visual Studios 2019

Note:  C++ is accurate with floats for only about 6 to 7 digits.
*/
#include <iostream>
#include <vector>
using namespace std;

//Function prototypes 
float convertFloat(string); //What will recognize the string input as a float
int getDigit(char); //Helper functions for convertFloat
float posPowTen(int);
float negPowTen(int);
float calcWhole(vector<float>&);
float calcDecimal(vector<float>&);
int calcE(vector<int>&);

int main()
{  
    string userNum = "";
    bool again = true;
    while (again)
    {
        cout << "Input a floating point number or 'q' to quit: ";
        cin >> userNum;

        if (userNum != "q")
        {
            try
            {
                float fltNum = convertFloat(userNum);
                cout << "The number is recognized as: " << fltNum;
            }
            catch(const char* e)
            {
                cout << e;
            }
        }
        else
        {
            again = false;
        }

        cout << endl << endl; //Formating purposes
    }  
}

float convertFloat(string numStr)
{
    float totalNum = 0.0;
    int numDot = 0;
    int numE = 0;
    int numSpacer = 0; 
    bool hasF = false;
    bool inWholeState = true;
    bool inEState = false;
    bool eNeg = false;
    char prevChar = ' ';
    vector<float> wholePart;
    vector<float> decimalPart;
    vector<int> ePart;

    for (int i = 0; i < numStr.length(); i++)
    {
        switch (numStr[i]) //Examine character by character
        {
        case '0': //Digits 0 - 9
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (inWholeState == true)//Whole state
            {
                wholePart.push_back(getDigit(numStr[i]));  //Push numbers into vector. calculate total at end
            }
            else if (inWholeState == false && inEState == false)//Decimal state
            {
                decimalPart.push_back(getDigit(numStr[i]));
            }
            else if (inEState == true)//Exponent state
            {
                ePart.push_back(getDigit(numStr[i]));
            }

            numSpacer = 0; //Reset spacer count
            prevChar = numStr[i];
            break;

        case '.': //Transition to decimal 
            inWholeState = false;
            numDot++;
            if (numDot > 1) //If there are more than 1 '.', it is not valid
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (inEState) //No decimals for the e part
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == '_') //Can not procceed a '.'
            {
                throw "The input is not recognized as a valid floating point number";
            }
            prevChar = numStr[i];
            break;

        case 'e': //Transition to exponent 
        case 'E':
            inWholeState = false;
            inEState = true;
            numE++;
            if (numE > 1) //If more than one e then it is not valid
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == '_') //Can not procceed a '_'
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (i == 0) //e can not be the first character
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (i == numStr.length() - 1) //e can not be the last character
            {
                throw "The input is not recognized as a valid floating point number";
            }
            prevChar = numStr[i];
            break;

        case '-':
            if (prevChar == 'e' || prevChar == 'E') //Must procceed an exponent indicator
            {
                eNeg = true;
            }
            else
            {
                throw "The input is not recognized as a valid floating point number";
            }

            if (i == numStr.length() - 1) //Can't be the last character
            {
                throw "The input is not recognized as a valid floating point number";
            }
            prevChar = numStr[i];
            break;

        case '+':
            if (prevChar == 'e' || prevChar == 'E') //Must procceed an exponent indicator
            {
                eNeg = false;
            }
            else
            {
                throw "The input is not recognized as a valid floating point number";
            }

            if (i == numStr.length() - 1) //Can't be the last character
            {
                throw "The input is not recognized as a valid floating point number";
            }
            prevChar = numStr[i];
            break;

        case '_': //Is meant to seperate two numbers
            //Conditions where the spacer character '_' is not valid
            numSpacer++;
            if (i == 0) //'_' can't be the first character
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (i == numStr.length() - 1) //'_' can't be the last character
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == '.') //Can not be next to other special characters
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == 'e' || prevChar == 'E')
            {
                throw "The input is not recognized as a valid floating point number";
            }         
            else if (prevChar == '+' || prevChar == '-')
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (numSpacer > 2) //Only 2 consecutive spacers are allowed. Resets when a valid digit is read.
            {
                throw "The input is not recognized as a valid floating point number";
            }

            prevChar = numStr[i];
            break;

        case 'f': //float type suffix
        case 'F':
        case 'd':
        case 'D':
            hasF = true;
            if (i != numStr.length() - 1) //Must be the last character 
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == '_')
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == 'e' || prevChar == 'E')
            {
                throw "The input is not recognized as a valid floating point number";
            }
            else if (prevChar == '+' || prevChar == '-')
            {
                throw "The input is not recognized as a valid floating point number";
            }

            prevChar = numStr[i];
            break;

        default: //Any invalid character
            throw "The input is not recognized as a valid floating point number";
        }
    }

    //A number is in a valid floating point form, if it has at least a digit and either one of a '.', 'e', or 'f'
    //Correct usage of these special characters should of been checked in the switch statement above
    bool hasDigit = false;
    if (wholePart.size() != 0 || decimalPart.size() != 0) 
    {
        hasDigit = true;
    }

    bool validForm = false;
    if (numDot == 1 && hasDigit)
    {
        validForm = true;
    }
    else if (numE == 1 && hasDigit)
    {
        validForm = true;
    }
    else if (hasF && hasDigit)
    {
        validForm = true;
    }

    if (validForm)
    {
        //calculating total 
        totalNum = calcWhole(wholePart) + calcDecimal(decimalPart);
        if (eNeg)
        {
            totalNum *= negPowTen(calcE(ePart));
        }
        else
        {
            totalNum *= posPowTen(calcE(ePart));
        }
    }
    else
    {
        throw "The input is not recognized as a valid floating point number";
    }

    return totalNum; 
}

int getDigit(char num) //Returns a number based on the corresponding char
{
    switch (num)
    {
    case '1':
        return 1;

    case '2':
        return 2;

    case '3':
        return 3;

    case '4':
        return 4;

    case '5':
        return 5;

    case '6':
        return 6;

    case '7':
        return 7;

    case '8':
        return 8;

    case '9':
        return 9;

    case '0':
        return 0; 

    default:
        return -1; //Not a valid character 
    }
}

float posPowTen(int num)
{
    float power = 1.0;
    for (int i = 0; i < num; i++)
    {
        power *= 10.0;
    }
  
    return power;
}

float negPowTen(int num)
{
    float power = 1.0;
    for (int i = 0; i < num; i++)
    {
        power /= 10.0;
    }

    return power;
}

float calcWhole(vector<float>& whole)
{
    int numDigits = whole.size() - 1;
    float total = 0.0;

    for (int i = 0; i < whole.size(); i++)
    {
        float power = posPowTen(numDigits);
        numDigits--;
        total += whole[i] * power;
    }

    return total;
}

float calcDecimal(vector<float>& dec)
{
    int numDecimals = 0;
    float total = 0.0;

    for (int i = 0; i < dec.size(); i++)
    {
        numDecimals++;
        float power = negPowTen(numDecimals);
        total += dec[i] * power;
    }

    return total;
}

int calcE(vector<int>& e)
{
    int numDigits = e.size() - 1;
    int total = 0.0;

    for (int i = 0; i < e.size(); i++)
    {
        float power = posPowTen(numDigits);
        numDigits--;
        total += e[i] * power;
    }

    return total;
}
