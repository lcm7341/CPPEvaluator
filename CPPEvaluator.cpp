#include "CPPEvaluator.h"
#include <string>
#include <vector>
#include <map>
#include <cmath>

double evalExpression(const std::vector<std::string>& tokens, std::map<std::string, double>& variables, int& index);

std::vector<std::string> tokenizeExpression(const std::string& expression)
{
    std::vector<std::string> tokens;
    std::string token;
    for (char c : expression)
    {
        if (isspace(c))
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (isalnum(c) || c == '.')
        {
            token += c;
        }
        else
        {
            if (!token.empty())
            {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        }
    }
    if (!token.empty())
    {
        tokens.push_back(token);
    }
    return tokens;
}

double evalVariable(const std::string& token, std::map<std::string, double>& variables)
{
    if (variables.find(token) != variables.end())
    {
        return variables[token];
    }
    else
    {
        std::cout << "Variable " << token << " is not defined. Enter a value for " << token << ": ";
        double value;
        std::cin >> value;
        variables[token] = value;
        return value;
    }
}

double evalFunction(const std::string& token, const std::vector<std::string>& tokens, std::map<std::string, double>& variables, int& index)
{
    if (token == "cos")
    {
        ++index;
        double result = evalExpression(tokens, variables, index);
        return cos(result);
    }
    else if (token == "sqrt")
    {
        ++index;
        double result = evalExpression(tokens, variables, index);
        return sqrt(result);
    }
    else
    {
        std::cout << "Unknown function: " << token << std::endl;
        return 0.0;
    }
}

double evalOperator(const std::string& token, double left, double right)
{
    if (token == "+")
    {
        return left + right;
    }
    else if (token == "-")
    {
        return left - right;
    }
    else if (token == "*")
    {
        return left * right;
    }
    else if (token == "/")
    {
        return left / right;
    }
    else if (token == "^")
    {
        return pow(left, right);
    }
    else
    {
        std::cout << "Unknown operator: " << token << std::endl;
        return 0.0;
    }
}

double evalExpression(const std::vector<std::string>& tokens, std::map<std::string, double>& variables, int& index)
{
    double result = 0.0;
    while (index < tokens.size())
    {
        const std::string& token = tokens[index];
        ++index;

        if (isdigit(token[0]))
        {
            result = std::stod(token);
        }
        else if (isalpha(token[0]))
        {
            result = evalVariable(token, variables);
        }
        else if (token == "(")
        {
            result = evalExpression(tokens, variables, index);
        }
        else if (token == ")")
        {
            return result;
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^")
        {
            double right = evalExpression(tokens, variables, index);
            result = evalOperator(token, result, right);
        }
        else if (token == "cos" || token == "sqrt")
        {
            result = evalFunction(token, tokens, variables, index);
        }
    }
    return result;
}

int main()
{
    std::string expression;
    std::cout << "Enter the expression: ";
    std::getline(std::cin, expression);

    std::vector<std::string> tokens = tokenizeExpression(expression);
    std::map<std::string, double> variables;

    char defineVariableChoice;
    do {
        std::string variableName;
        std::cout << "Enter variable name: ";
        std::cin >> variableName;

        if (variables.find(variableName) != variables.end()) {
            std::cout << "Variable " << variableName << " is already defined. Please enter a different name." << std::endl;
            continue;
        }

        double value;
        std::cout << "Enter the value for " << variableName << ": ";
        std::cin >> value;
        variables[variableName] = value;

        std::cout << "Do you want to define another variable? (y/n): ";
        std::cin >> defineVariableChoice;
    } while (defineVariableChoice == 'y' || defineVariableChoice == 'Y');

    int index = 0;
    double result = evalExpression(tokens, variables, index);

    std::cout << "Result: " << result << std::endl;

    system("pause");

    return 0;
}
