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
float getDigit(char); //Helper functions for convertFloat
bool spacerValidity(string, int, int, int);
float posPowTen(int);
float negPowTen(int);
float calcWhole(vector<float>&);
float calcDecimal(vector<float>&);

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

float convertFloat(string num)
{
    //Check first character to see if it is positive or negative, not sign in the front will be treated as positive
    bool isNegative = false;
    int startIndex = 0;
    if (num[0] == '-')
    {
        isNegative = true;
        startIndex = 1;
    }
    else if (num[0] == '+')
    {
        startIndex = 1;
    }
    
    //User can use 'f' or 'F' at end of string to indicate this number is suppose to be a floating point number
    //If the character is there then the string will be recognized as a float and no need to account for it in calculations 
    int numLength = num.length();
    if (num[numLength - 1] == 'f' || num[numLength - 1] == 'F')
    {
        numLength--;
    }

    int totalDot = 0; //Can only have a certain number of '.' so keep track of it
    int eindex = -1; //e to indicate scientific notation
    bool isWhole = true; //Flag variable for when calculating the whole number part or the decimal part
    vector<float> wholePart;
    vector<float> decimalPart;

    for (int i = startIndex; i < numLength; i++)
    {
        //Checking for special characters like '_', '.', and 'e'
        if (num[i] == '_') //'_' is spacer character
        {
            //Making sure if the usage of '_' is valid
            if (spacerValidity(num, startIndex, numLength, i))
            {
                continue; //If valid just continue with calculations
            }
            else
            {
                throw "The input is not recognized as a floating point number"; //Rejecting string
            }
            
        }
        else if (num[i] == '.')
        {
            isWhole = false;
            totalDot++;
            
            if (totalDot <= 1)
            {
                continue;
            }
            else //Can not have more than one decimal point, if more reject the string
            {
                throw "The input is not recognized as a floating point number";
            }
        }
        else if (num[i] == 'e' || num[i] == 'E')
        {
            eindex = i;
            break;
        }

        float value = getDigit(num[i]); //Get the number variation of character if possible
        
        if (value == -1.0) //Was not a valid number
        {
            throw "The input is not recognized as a floating point number";
        }

        //Calculating whole and decimal part seperately 
        if (isWhole)
        {
            wholePart.push_back(value);
        }
        else
        {
            decimalPart.push_back(value);
        }
    }

    float totalNum = calcWhole(wholePart) + calcDecimal(decimalPart);

    if (eindex != -1) //Calculating scientific notation
    {
        //Check to see if the usage of e is valid
        if (eindex == startIndex) //e can not be the first character
        {
            throw "The input is not recognized as a floating point number";
        }
        else if (eindex == numLength - 1) //e can not be the last character and must have numbers after it
        {
            throw "The input is not recognized as a floating point number";
        }

        bool eNeg = false;
        int eStart = eindex + 1;
        if (num[eStart] == '-')
        {
            eNeg = true;
            eStart++;
        }
        else if (num[eStart] == '+')
        {
            eStart++;
        }

        float ePart = 0;
        int eDigits = numLength - eStart - 1;
        for (int i = eStart; i < numLength; i++)
        {
            if (num[i] == '_') //'_' can be used in scientific notations similar to the before part
            {
                if (spacerValidity(num, eStart, numLength, i))
                {
                    eDigits--;
                    continue; 
                }
                else
                {
                    throw "The input is not recognized as a floating point number"; 
                }
            }

            float value = getDigit(num[i]);

            if (value == -1)
            {
                throw "The input is not recognized as a floating point number";
            }

            float power = posPowTen(eDigits);
            eDigits--;
            ePart += value * power;
        }

        if (eNeg)
        {
            totalNum *= negPowTen(ePart);
        }
        else
        {
            totalNum *= posPowTen(ePart);
        }

    }

    if (isNegative)
    {
        totalNum *= -1.0;
    }

    return totalNum; 
}

float getDigit(char num)
{
    switch (num)
    {
    case '1':
        return 1.0;

    case '2':
        return 2.0;

    case '3':
        return 3.0;

    case '4':
        return 4.0;

    case '5':
        return 5.0;

    case '6':
        return 6.0;

    case '7':
        return 7.0;

    case '8':
        return 8.0;

    case '9':
        return 9.0;

    case '0':
        return 0.0; 

    default:
        return -1.0; //Not a valid character 
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

bool spacerValidity(string num, int startIndex, int len, int i)
{
    //Conditions where the spacer character '_' is not valid
    if (i == startIndex) //'_' can't be the first character
    {
        return false;
    }
    else if (i == len - 1) //'_' can't be the last character
    {
        return false;
    }
    else if (num[i - 1] == '.' || num[i + 1] == '.') //Can not be next to other special characters
    {
        return false;
    }
    else if (num[i - 1] == 'e' || num[i + 1] == 'e')
    {
        return false;
    }
    else if (num[i - 1] == 'E' || num[i + 1] == 'E')
    {
        return false;
    }

    return true;
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
