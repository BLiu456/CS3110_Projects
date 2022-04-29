# CS3110_Project2
# Name: Benjamin Liu
# Due Date: 5/04/2022
# Used C++, Visual Studios 2022

Type in an expression into the prompt. The program will first convert the inputted expression into a postfix expression, and
then evaluate the resulting postfix expression. Every number must be a floating point number.

Supported operators are +, -, *, /, and parentheses ().
Unary operations are not supported, so each operator will be treated as binary operators. 

The conversion to postfix expression and the evaluation were based off a PDA design, refer to the files "CS3110-Postfix PDA.pdf" and "CS3110-Eval PDA.pdf".

Spaces are only allowed between operands and operators or parentheses. 

The conversion of floating point numbers will utilize the code from project 1 and will be marked in comments. 