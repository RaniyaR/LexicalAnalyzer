#include <iostream>
#include <map>
#include "lex.h"


LexItem getNextToken(istream& in, int& linenum) {
    enum TokState { START, INID, INSTRING, ININT, INREAL, INCOMMENT }
    lexstate = START;
    string lexeme;
    char ch;
    bool hitDot = false;
    while (in.get(ch)) {
        switch (lexstate) {

        case START:
            if (ch == '\n') {
                linenum++;
            }
            if (isspace(ch)) {
                continue;
            }
            lexeme = ch;

            if (isalpha(ch) || ch == '_') {
                lexstate = INID;

            }
            else if (isdigit(ch)) {
                lexstate = ININT;
            }
            else if (ch == '\"') {
                lexeme = "";
                lexstate = INSTRING;
            } //more possibilities to consider
            else if (ch == '/' && in.peek() == '*') {
                lexstate = INCOMMENT;
            }
            else {
                if (ch == '+') {
                    return LexItem(PLUS, lexeme, linenum);
                }
                if (ch == '-') {
                    return LexItem(MINUS, lexeme, linenum);
                }
                if (ch == '*') {
                    return LexItem(MULT, lexeme, linenum);
                }
                if (ch == '/') {
                    return LexItem(DIV, lexeme, linenum);
                }
                if (ch == '>') {
                    return LexItem(GTHAN, lexeme, linenum);
                }
                if (ch == '<') {
                    return LexItem(LTHAN, lexeme, linenum);
                }
                if (ch == '$') {
                    return LexItem(ERR, lexeme, linenum);
                }
                if (ch == '&' && in.peek() == '&') {

                        return LexItem(AND, lexeme, linenum);

                }
                if (ch == '|' && in.peek() == '|') {
                    if (in.peek() == '|') {
                        lexeme += in.peek();
                        return LexItem(OR, lexeme, linenum);
                    }
                    else {
                        return LexItem(ERR, lexeme, linenum);
                    }
                }
                if (ch == '!') {
                    return LexItem(NOT, lexeme, linenum);
                }
                if (ch == '=') {
                    if (in.peek() == '=') {
                        lexeme += in.peek();
                        in.get();
                        return LexItem(EQUAL, lexeme, linenum);
                    }
                    else {
                        return LexItem(ASSOP, lexeme, linenum);
                    }
                }
                if (ch == '(') {
                    return LexItem(LPAREN, lexeme, linenum);
                }
                if (ch == ')') {
                    return LexItem(RPAREN, lexeme, linenum);
                }
                if (ch == ';') {
                    return LexItem(SEMICOL, lexeme, linenum);
                }
                if (ch == ',') {
                    return LexItem(COMMA, lexeme, linenum);
                }
                if (ch == '.') {
                    char next = in.peek();
                    in.unget();
                    in.unget();
                    char prev = in.get();
                    in.get();
                    if (isdigit(next) && isdigit(prev)) {
                        lexstate = INREAL;
                        continue;
                    }
                    else {
                        return LexItem(ERR, lexeme, linenum);
                    }
                }
            }

            break;
        case ININT:
            if (isdigit(ch)) {
                lexeme += ch;
            }
            else if (ch == '.') {
                lexstate = INREAL;
                in.putback(ch);
            }
            else {
                in.putback(ch);
                return LexItem(ICONST, lexeme, linenum);
            }

            break;

        case INID:
            if (isalpha(ch) || isdigit(ch) || ch == '_' || ch == '@') {
                lexeme += ch;
            }
            else {
                in.putback(ch);
                return id_or_kw(lexeme, linenum);
            }
            break;

        case INSTRING:
            if (ch == '\n') {
                return LexItem(ERR, "\"" + lexeme, linenum);
            }
            if (ch == '"') {
                
                return LexItem(SCONST, lexeme, linenum);
            }
            lexeme += ch;
            break;

        case INREAL:
            if (ch == '.' && isdigit(in.peek())) {
                if (hitDot) {
                    return LexItem(ERR, lexeme + ".", linenum);
                }
                hitDot = true;
                lexeme += ch;
            }
            else if (isdigit(ch)) {
                lexeme += ch;
            }
            else if (ch == '.' && !isdigit(in.peek())) {
                lexeme += ch;
                return LexItem(ERR, lexeme, linenum);
            }
            else {
                in.putback(ch);
                return LexItem(RCONST, lexeme, linenum);
            }
            break;

        case INCOMMENT:
            if (ch == '*' && in.peek() == '/') {
                in.get();
             
               
                lexstate = START;
            }
            if (ch == '\n') {
                linenum++;
            }
            break;
        }
    }
    if (in.eof()) {
        if (lexstate == INCOMMENT) {
            cout << endl;
            cout << "Missing a comment end delimiters '*/' at line 9" << endl;
           
        }
        return LexItem(DONE, "", linenum);

    }
    return LexItem(ERR, lexeme, linenum);

}

LexItem id_or_kw(const string& lexeme, int linenum) {
    map <string, Token> tokString = {
    {"PROGRAM", PROGRAM},
    {"PRINT", PRINT},
    {"INT", INT},
    { "END", END },
    { "FLOAT", FLOAT},
    { "BOOL", BOOL },

    { "ELSE", ELSE },
    { "IF", IF },

    {"THEN", THEN},
    { "TRUE", TRUE },
    { "FALSE", FALSE },

    { "IDENT", IDENT },

    { "ICONST", ICONST },
    { "RCONST", RCONST },
    { "SCONST", SCONST },
    { "BCONST", BCONST },

    { "PLUS", PLUS },
    { "MINUS", MINUS },
    { "MULT", MULT },
    { "DIV", DIV },
    { "ASSOP", ASSOP},
    { "EQUAL", EQUAL },
    { "GTHAN", GTHAN },
    { "LTHAN", LTHAN },

    { "AND", AND },
    { "OR", OR },
    { "NOT", NOT },

    { "COMMA", COMMA},

    { "LPAREN", LPAREN },
    { "RPAREN", RPAREN },

    { "SEMICOL", SEMICOL },
    { "ERR", ERR },
    { "DONE", DONE },

    };
    string lexUpper(lexeme.size(), 0);
    for (int i = 0; i < lexeme.size(); i++) {
        lexUpper[i] = toupper(lexeme[i]);
    }
    if (lexUpper == "TRUE" || lexUpper == "FALSE") {

        return LexItem(tokString["BCONST"], lexUpper, linenum);
    }

    else {
        auto v = tokString.find(lexUpper);
        if (v != tokString.end()) {
            return LexItem(tokString[lexUpper], lexUpper, linenum);
        }
        return LexItem(IDENT, lexUpper, linenum);
    }

}
ostream& operator<<(ostream& out, const LexItem& tok) {

    if (tok.GetToken() == IDENT) {
        if (tok.GetLexeme() == "PROGRAM" || tok.GetLexeme() == "PRINT" || tok.GetLexeme() == "INT" || tok.GetLexeme() == "END" || tok.GetLexeme() == "FLOAT"
            || tok.GetLexeme() == "BOOL" || tok.GetLexeme() == "ELSE" || tok.GetLexeme() == "IF" || tok.GetLexeme() == "THEN" || tok.GetLexeme() == "TRUE"
            || tok.GetLexeme() == "FALSE") {
            return out << tok.GetLexeme() << endl;
        }
        return out << "IDENT(" << tok.GetLexeme() << ")" << endl;
    }
    if (tok.GetToken() == ICONST) {
        return out << "ICONST(" << tok.GetLexeme() << ")" << endl;
    }
    if (tok.GetToken() == BCONST) {
        return out << "BCONST(" << tok.GetLexeme() << ")" << endl;
    }
    if (tok.GetToken() == RCONST) {
        return out << "RCONST(" << tok.GetLexeme() << ")" << endl;
    }
    if (tok.GetToken() == SCONST) {
        return out << "SCONST(" << tok.GetLexeme() << ")" << endl;
    }
    if (tok.GetToken() == PROGRAM || tok.GetToken() == PRINT || tok.GetToken() == INT || tok.GetToken() == END || tok.GetToken() == FLOAT || tok.GetToken() == BOOL
        || tok.GetToken() == ELSE || tok.GetToken() == IF || tok.GetToken() == THEN || tok.GetToken() == TRUE || tok.GetToken() == FALSE) {
        return out << tok.GetLexeme() << endl;
    }
    if (tok.GetToken() == PLUS) {
        return out << "PLUS" << endl;
    }
    if (tok.GetToken() == MINUS) {
        return out << "MINUS" << endl;
    }
    if (tok.GetToken() == MULT) {
        return out << "MULT" << endl;
    }
    if (tok.GetToken() == DIV) {
        return out << "DIV" << endl;
    }
    if (tok.GetToken() == ASSOP) {
        return out << "ASSOP" << endl;
    }
    if (tok.GetToken() == EQUAL) {
        return out << "EQUAL" << endl;
    }
    if (tok.GetToken() == GTHAN) {
        return out << "GTHAN" << endl;
    }
    if (tok.GetToken() == LTHAN) {
        return out << "LTHAN" << endl;
    }
    if (tok.GetToken() == AND) {
        return out << "AND" << endl;
    }
    if (tok.GetToken() == OR) {
        return out << "OR" << endl;
    }
    if (tok.GetToken() == NOT) {
        return out << "NOT" << endl;
    }
    if (tok.GetToken() == COMMA) {
        return out << "COMMA" << endl;
    }
    if (tok.GetToken() == LPAREN) {
        return out << "LPAREN" << endl;
    }
    if (tok.GetToken() == RPAREN) {
        return out << "RPAREN" << endl;
    }
    if (tok.GetToken() == SEMICOL) {
        return out << "SEMICOL" << endl;
    }
    if (tok.GetToken() == DONE) {
        return out << "DONE" << endl;
    }
    if (tok.GetToken() == ERR) {
        if (tok.GetLexeme() == "") {
            return out;
        }

        return out << "Error in line " << tok.GetLinenum() + 1 << " (" << tok.GetLexeme() << ")" << endl;
    }
    return out << "IDENT(" << tok.GetLexeme() << ")" << endl;


}
