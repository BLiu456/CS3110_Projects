/*
Name: Benjamin Liu
Assignment: CS3110 Project 2
Language used is C++, and coded in Visual Studio 2022

Notes: A calculator for floating point numbers, each operator (+ - * /) will be considered binary
       Unary operations (such as + and -) are not supported
       Spaces are allowed in the expression but they must only be between an operand and an operator or parentheses
       There can be any number of spaces between the operator and operand 
       A vaild use of spaces would look like this: ( 1f +   1f)  * 2f - 1f  +2f
       An invalid use of spaces would be if it was between digits (i.e. 1  1.0 + 2 .0 would be invalid)
       Code from project 1 is marked with comments
*/

#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

string getPostfix(string);
float eval(string);

//---Function prototypes from project 1---
float convertFloat(string);
int getDigit(char);
float posPowTen(int);
float negPowTen(int);
float calcWhole(vector<float>&);
float calcDecimal(vector<float>&);
int calcE(vector<int>&);
//---End of function prototypes from project 1---

int main()
{
    string expression = "";
    float answer = 0.0;

    while (true)
    {
        cout << "Enter an expression or q to quit: ";
        getline(cin, expression);

        if (expression != "q")
        {
            try
            {
                answer = eval(getPostfix(expression));
                cout << "Result: " << answer << endl << endl;
            }
            catch (const char* e)
            {
                cout << e << endl << endl;
            }
        }
        else
        {
            break;
        }
    }
}

/*
    This function simply attempts to convert the infix expression into a postfix expression.
    The postfix expression will be returned as a string.
    This function was based off a PDA design with one stack. Refer to CS3310-Postfix PDA.pdf file for the design.
    Will not recognize expression if an invalid character was used or if a parentheses was not used correctly.
    Validity of floating point numbers and whether the expression was entered correctly will be checked in the eval() function.    
*/
string getPostfix(string infix)
{
	stack<char> operatorStack;
	string postfix = "";
	int parenthesesCount = 0;
    bool expectOperator = false; //expectOperator and prevIsOperator is used to help verify the usage of spaces
    bool prevIsOperator = true;  //Will set initially to true since it is okay to start off an expression with spaces

    //Based on character input, perform a certain task to mimic a PDA
	for (int i = 0; i < infix.length(); i++)
	{
        switch (infix[i])
        {
        case '+': //+ and - have the same precedence so handle these inputs the same
        case '-':
            if (i != 0)
            {
                if (infix[i - 1] == 'e' || infix[i - 1] == 'E') //Determines if the +- belongs to the e in float numbers
                {
                    postfix.insert(postfix.length(), 1, infix[i]);
                    break;
                }
            }

            postfix.append(" ");
            if (!operatorStack.empty())
            {
                if (operatorStack.top() == '(') //Can be pushed into stack if ( is on top
                {
                    operatorStack.push(infix[i]);
                }
                else if (operatorStack.top() == '*' || operatorStack.top() == '/') //Because +- has the lowest precedence pop the top operand into the expression 
                {                                                                  //and then push +- into stack
                    postfix.insert(postfix.length(), 1, operatorStack.top());
                    postfix.append(" ");

                    operatorStack.pop();
                    operatorStack.push(infix[i]);
                }
                else if (operatorStack.top() == '+' || operatorStack.top() == '-') //Same precedence, perform similar steps from the else if above 
                {
                    postfix.insert(postfix.length(), 1, operatorStack.top());
                    postfix.append(" ");

                    operatorStack.pop();
                    operatorStack.push(infix[i]);
                }
            }
            else
            {
                operatorStack.push(infix[i]);
            }

            prevIsOperator = true;
            break;

        case '*': //multiplication and division have the same precedence so handle these inputs the same
        case '/':
            postfix.append(" ");
            if (!operatorStack.empty())
            {
                if (operatorStack.top() == '(')
                {
                    operatorStack.push(infix[i]);
                }
                else if (operatorStack.top() == '*' || operatorStack.top() == '/') //Same precedence
                {
                    postfix.insert(postfix.length(), 1, operatorStack.top());
                    postfix.append(" ");

                    operatorStack.pop();
                    operatorStack.push(infix[i]);
                }
                else if (operatorStack.top() == '+' || operatorStack.top() == '-') //When a operator of lower precedence is on top, can simply push into stack
                {
                    operatorStack.push(infix[i]);
                }
            }
            else
            {
                operatorStack.push(infix[i]);
            }

            prevIsOperator = true;
            break;

        case '(': //Push ( into the stack, acts as a sort of precedence resetter
            operatorStack.push(infix[i]);
            parenthesesCount++;
            prevIsOperator = true;
            break;

        case ')':
            if (parenthesesCount > 0)
            {
                postfix.append(" ");
                if (!operatorStack.empty())
                {
                    while (operatorStack.top() != '(') //Pop all operators in stack until the ( is reached
                    {
                        postfix.insert(postfix.length(), 1, operatorStack.top());

                        operatorStack.pop();
                    }

                    operatorStack.pop(); //Pop the ( out the stack
                    parenthesesCount--;
                }
                else //the stack should not be empty if a ( is in the stack, so this check is more of a saftey net just in case 
                {
                    throw "Missing parentheses";
                }
            }
            else //If parenthesesCount <= 0 then there is a missing ( 
            {
                throw "Missing parentheses";
            }

            prevIsOperator = true;
            break;

        case ' ': //Spaces are accepted but must be between an operator and operand. If used validly then the program will simply do nothing with it.
            if (prevIsOperator) //If previous character was an operator then the next expected character is a digit
            {
                expectOperator = false;
            }
            else //If the previous char was not an operator then it was a digit, so the next char should be an operator
            {
                expectOperator = true;
            }
            break;

            //The valid characters used in floating-point numbers
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'd':
        case 'D':
        case 'e':
        case 'E':
        case 'f':
        case 'F':
        case '.':
        case '_':
            if (expectOperator == false)
            {
                postfix.insert(postfix.length(), 1, infix[i]);
                prevIsOperator = false;
            }
            else
            {
                throw "Invalid use of spaces";
            }
			break;

		default: //Any other character is invalid 
			throw "Invalid Character";
		}
	}

	if (parenthesesCount != 0) //parenteseCount > 0 means that a parentheses was never closed 
	{
		throw "Parentheses not closed";
	}

	while (!operatorStack.empty()) //Empty out the rest of the stack to complete the expression
	{
		postfix.append(" ");
		postfix.insert(postfix.length(), 1, operatorStack.top());
		operatorStack.pop();
	}

	return postfix;
}

/*
    eval() will take in a postfix expression, and will attempt to calculate it. 
    Returns the result as a floating point number.
    Based off a PDA design with one stack. Refer to CS3310-Eval PDA.pdf file for the design.
*/
float eval(string postfix)
{
	stack<float> numStack;
	string numTemp = "";
	float total = 0.0;
	float val1 = 0;
	float val2 = 0;
    bool digitState = false; //State when reading in a digit to from the float number

	for (int i = 0; i < postfix.length(); i++)
	{
		switch (postfix[i])
		{
		case '0': //All valid characters used in floating point numbers
		case '1': //Until a space is read, append these characters into a string
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case 'd':
		case 'D':
		case 'e':
		case 'E':
		case 'f':
		case 'F':
		case '.':
		case '_':
			numTemp.insert(numTemp.length(), 1, postfix[i]);
            digitState = true;
			break;

		case ' ': //Takes the numTemp string and convert it to a floating point number.
			if (numTemp.length() != 0)
			{
				numStack.push(convertFloat(numTemp)); //Utilizes project1 DFA, and this is where floating point validity is tested
				numTemp = "";
                digitState = false;
			}

			break;

		case '+':
            if (digitState) //Determines if the + is used for e
            {
                numTemp.insert(numTemp.length(), 1, postfix[i]);
                break;
            }

            if (numStack.size() >= 2) //Takes the first 2 number in the stack and adds them
            {
                val1 = numStack.top();
			    numStack.pop();
			    val2 = numStack.top();
			    numStack.pop();

			    numStack.push(val2 + val1); //Push result into stack
            }
            else  //If at least two numbers are not in the stack when an operator is read, then the expression was entered incorrectly.
            {
                throw "Invalid Expression";
            }
			
			break;

		case '-':
            if (digitState) //Determines if the + is used for e
            {
                numTemp.insert(numTemp.length(), 1, postfix[i]);
                break;
            }

            if (numStack.size() >= 2) //Takes the first 2 number in the stack and subtracts them
            {
                val1 = numStack.top();
                numStack.pop();
                val2 = numStack.top();
                numStack.pop();

                numStack.push(val2 - val1);
            }
            else
            {
                throw "Invalid Expression";
            }

			break;

		case '*':
            if (numStack.size() >= 2) //Takes the first 2 number in the stack and multiplys them
            {
                val1 = numStack.top();
                numStack.pop();
                val2 = numStack.top();
                numStack.pop();

                numStack.push(val2 * val1);
            }
            else
            {
                throw "Invalid Expression";
            }
			break;

		case '/':
            if (numStack.size() >= 2) //Takes the first 2 number in the stack and divides them
            {
                val1 = numStack.top();
                numStack.pop();
                val2 = numStack.top();
                numStack.pop();

                if (val1 == 0.0) //Handling zero by division error
                {
                    throw "Divison by Zero error";
                }

                numStack.push(val2 / val1);
            }
            else
            {
                throw "Invalid Expression";
            }
			break;

		default: //Any other character is invalid
			throw "Invalid Character";
		}
	}

    if (numStack.size() == 1) //The top and only number left in the stack is the result of the expression.
    {
        return numStack.top(); 
    }
    else
    {
        throw "Not a valid expression";
    }
}


//---Below is my code from project 1---
//Code below should work the same as it did in the 1st project. 
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
