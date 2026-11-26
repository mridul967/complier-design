#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <string>
#include <cctype>
using namespace std;

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

int precedence(const string& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

vector<string> tokenize(const string& text) {
    vector<string> tokens;
    for (size_t i = 0; i < text.size();) {
        if (isspace(static_cast<unsigned char>(text[i]))) {
            ++i;
        } else if (isalnum(static_cast<unsigned char>(text[i])) || text[i] == '_') {
            size_t start = i;
            while (i < text.size() &&
                   (isalnum(static_cast<unsigned char>(text[i])) || text[i] == '_')) ++i;
            tokens.push_back(text.substr(start, i - start));
        } else {
            tokens.push_back(string(1, text[i++]));
        }
    }
    return tokens;
}

vector<string> toPostfix(const vector<string>& tokens) {
    vector<string> output;
    stack<string> operators;
    for (const string& token : tokens) {
        if (!isOperator(token) && token != "(" && token != ")") {
            output.push_back(token);
        } else if (token == "(") {
            operators.push(token);
        } else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                output.push_back(operators.top());
                operators.pop();
            }
            if (!operators.empty()) operators.pop();
        } else {
            while (!operators.empty() && isOperator(operators.top()) &&
                   precedence(operators.top()) >= precedence(token)) {
                output.push_back(operators.top());
                operators.pop();
            }
            operators.push(token);
        }
    }
    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }
    return output;
}

void generateCode(const string& expression) {
    size_t equalPos = expression.find('=');
    string target, rhs = expression;
    if (equalPos != string::npos) {
        target = expression.substr(0, equalPos);
        while (!target.empty() && isspace(static_cast<unsigned char>(target.back())))
            target.pop_back();
        rhs = expression.substr(equalPos + 1);
    }

    vector<string> postfix = toPostfix(tokenize(rhs));
    stack<string> operands;
    int tempCount = 1;
    cout << "Three-address code:\n";

    for (const string& token : postfix) {
        if (!isOperator(token)) {
            operands.push(token);
        } else {
            if (operands.size() < 2) {
                cout << "Invalid expression.\n";
                return;
            }
            string right = operands.top(); operands.pop();
            string left = operands.top(); operands.pop();
            string temp = "t" + to_string(tempCount++);
            cout << temp << " = " << left << " " << token << " " << right << '\n';
            operands.push(temp);
        }
    }

    if (operands.size() != 1) {
        cout << "Invalid expression.\n";
        return;
    }
    string result = operands.top();
    if (!target.empty()) cout << target << " = " << result << '\n';
    else cout << "Final result is stored in " << result << '\n';
}

int main() {
    cout << "Expression 1: x = (y + z) / k\n";
    generateCode("x = (y + z) / k");
    cout << "\nExpression 2: (a + b) * (c - d)\n";
    generateCode("(a + b) * (c - d)");
    return 0;
}
