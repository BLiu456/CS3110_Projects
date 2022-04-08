/*
Name: Benjamin Liu
Class: CS3110
Due Date: 4/11/2022

Project 1: Recognizing floating point numbers 
Used C++, Visual Studios 2019

Note: Depending on compiler/system, the floating point number will be rounded after a certain amount of digits. On my system it rounds after 7 digits. 
      Not really sure how to fix it, but a valid floating point number is still recognized so the program still works as intended.  
*/
#include <iostream>
#include <vector>

using namespace std;

//Function prototypes 
float convertFloat(string); //What will recognize the string input as a float
float getDigit(char); //Helper functions for convertFloat
float posPowTen(int);
float negPowTen(int);
float calcWhole(vector<float>&);
float calcDecimal(vector<float>&);

int main()
{
    string userNum = "";
    cout << "Input a floating point number: ";
    cin >> userNum;

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
    //If the character is there then the string is recognized and no need to account for it in calculations 
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
        if (num[i] == '.')
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
        else if (num[i] == 'e')
        {
            eindex = i;
            break;
        }

        float value = getDigit(num[i]);
        
        if (value == -1.0)
        {
            throw "The input is not recognized as a floating point number";
        }

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

    if (eindex != -1)
    {
        //Check to see if the usage of e is valid
        if (eindex == 0) //e can not be the first character
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

    return totalNum; //If the input is recognized as a float, then return the floating point number
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