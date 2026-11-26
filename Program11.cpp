#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct Node {
    string value;
    Node* left;
    Node* right;
    Node(string v, Node* l = nullptr, Node* r = nullptr)
        : value(v), left(l), right(r) {}
};

class Parser {
    string input;
    size_t pos = 0;

    Node* parseExpression() {
        Node* node = parseTerm();
        while (pos < input.size() && (input[pos] == '+' || input[pos] == '-')) {
            char op = input[pos++];
            Node* right = parseTerm();
            node = new Node(string(1, op), node, right);
        }
        return node;
    }

    Node* parseTerm() {
        Node* node = parseFactor();
        while (pos < input.size() && (input[pos] == '*' || input[pos] == '/')) {
            char op = input[pos++];
            Node* right = parseFactor();
            node = new Node(string(1, op), node, right);
        }
        return node;
    }

    Node* parseFactor() {
        if (pos >= input.size()) throw string("Unexpected end of expression");
        if (input[pos] == '(') {
            ++pos;
            Node* node = parseExpression();
            if (pos >= input.size() || input[pos] != ')')
                throw string("Missing closing parenthesis");
            ++pos;
            return node;
        }

        if (isalnum(static_cast<unsigned char>(input[pos])) || input[pos] == '_') {
            size_t start = pos;
            while (pos < input.size() &&
                   (isalnum(static_cast<unsigned char>(input[pos])) || input[pos] == '_'))
                ++pos;
            return new Node(input.substr(start, pos - start));
        }
        throw string("Invalid symbol in expression");
    }

public:
    explicit Parser(string text) {
        for (char c : text)
            if (!isspace(static_cast<unsigned char>(c))) input += c;
    }
    Node* parse() {
        Node* root = parseExpression();
        if (pos != input.size()) throw string("Unexpected symbol after expression");
        return root;
    }
};

void printTree(Node* node, const string& prefix = "", bool isLast = true, bool root = true) {
    if (!node) return;
    if (root) cout << node->value << '\n';
    else cout << prefix << (isLast ? "\\-- " : "|-- ") << node->value << '\n';

    string childPrefix = prefix;
    if (!root) childPrefix += (isLast ? "    " : "|   ");
    if (node->left)
        printTree(node->left, childPrefix, node->right == nullptr, false);
    if (node->right)
        printTree(node->right, childPrefix, true, false);
}

void deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

int main() {
    string expression;
    cout << "Enter expression: ";
    getline(cin, expression);

    try {
        Parser parser(expression);
        Node* root = parser.parse();
        cout << "\nSyntax tree:\n";
        printTree(root);
        deleteTree(root);
    } catch (const string& error) {
        cout << "Invalid expression: " << error << '\n';
        return 1;
    }
    return 0;
}
