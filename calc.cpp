#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>
#include <vector>

using namespace std;

// 演算子の優先順位を返す関数
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 演算を実行する関数
double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw runtime_error("Division by zero");
            return a / b;
    }
    return 0;
}

// 式を評価する関数
double evaluateExpression(const string& expression) {
    stack<double> values;
    stack<char> ops;
    vector<string> tokens;

    // トークン分割
    for (size_t i = 0; i < expression.length(); ) {
        // 空白をスキップ
        if (expression[i] == ' ') {
            i++;
            continue;
        }

        // 数字の場合
        if (isdigit(expression[i])) {
            string num;
            while (i < expression.length() && isdigit(expression[i])) {
                num += expression[i];
                i++;
            }
            tokens.push_back(num);
            continue;
        }

        // 演算子の場合
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            // 負の符号の判定
            if (expression[i] == '-' && (i == 0 || 
                (!tokens.empty() && (tokens.back() == "+" || tokens.back() == "-" || 
                 tokens.back() == "*" || tokens.back() == "/")))) {
                // 次の文字が数字なら負の符号として扱う
                if (i + 1 < expression.length() && isdigit(expression[i+1])) {
                    i++;
                    string num = "-";
                    while (i < expression.length() && isdigit(expression[i])) {
                        num += expression[i];
                        i++;
                    }
                    tokens.push_back(num);
                    continue;
                }
            }
            // 通常の演算子
            tokens.push_back(string(1, expression[i]));
            i++;
            continue;
        }

        throw runtime_error("Invalid character in expression");
    }

    // トークンを処理
    for (const auto& token : tokens) {
        if (token[0] == '-' && token.length() > 1) {
            // 負の数
            values.push(stod(token));
        } else if (isdigit(token[0])) {
            // 正の数
            values.push(stod(token));
        } else {
            // 演算子
            while (!ops.empty() && precedence(ops.top()) >= precedence(token[0])) {
                if (values.size() < 2) {
                    throw runtime_error("Invalid expression: not enough operands");
                }
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.push(token[0]);
        }
    }

    // 残りの演算を実行
    while (!ops.empty()) {
        if (values.size() < 2) {
            throw runtime_error("Invalid expression: not enough operands");
        }
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(val1, val2, op));
    }

    if (values.size() != 1) {
        throw runtime_error("Invalid expression: too many operands");
    }

    return values.top();
}

int main() {
    string expression;
    cout << "Enter an expression: ";
    getline(cin, expression);

    try {
        double result = evaluateExpression(expression);
        cout << "Result: " << result << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

