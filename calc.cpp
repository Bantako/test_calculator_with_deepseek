#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>

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

    // 入力文字列のバリデーション
    for (size_t i = 0; i < expression.length(); i++) {
        char ch = expression[i];
        if (!isdigit(ch)) {
            // 負の符号の場合は次の文字が数字であることを確認
            if (ch == '-' && i + 1 < expression.length() && isdigit(expression[i+1])) {
                continue;
            }
            if (ch != ' ' && ch != '+' && ch != '-' && ch != '*' && ch != '/') {
                throw runtime_error("Invalid character in expression");
            }
        }
    }

    for (size_t i = 0; i < expression.length(); i++) {
        // 空白をスキップ
        if (expression[i] == ' ') continue;

        // 数字または負の数の場合
        if (isdigit(expression[i]) || (expression[i] == '-' && i + 1 < expression.length() && isdigit(expression[i+1]))) {
            bool isNegative = false;
            if (expression[i] == '-') {
                isNegative = true;
                i++;
            }
            
            double val = 0;
            while (i < expression.length() && isdigit(expression[i])) {
                val = (val * 10) + (expression[i] - '0');
                i++;
            }
            if (isNegative) {
                val = -val;
            }
            values.push(val);
            i--;
        }
        // 演算子の場合
        else {
            // 負の符号ではなく演算子のマイナスの場合
            if (expression[i] == '-' && (i == 0 || expression[i-1] == '(' || 
                expression[i-1] == '+' || expression[i-1] == '-' || 
                expression[i-1] == '*' || expression[i-1] == '/')) {
                // これは負の符号なので次の文字を処理
                i++;
                double val = 0;
                while (i < expression.length() && isdigit(expression[i])) {
                    val = (val * 10) + (expression[i] - '0');
                    i++;
                }
                values.push(-val);
                i--;
                continue;
            }
            
            // 演算子の前に数字がない場合
            if (values.empty()) {
                throw runtime_error("Invalid expression: operator without preceding number");
            }

            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                if (values.size() < 2) {
                    throw runtime_error("Invalid expression: not enough operands");
                }
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.push(expression[i]);
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

