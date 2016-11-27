var text = `
Token Token::_EOF;
Token Token::ERROR;
Token Token::IDENTIFIER;
Token Token::BOOLEAN("boolean");
Token Token::CLASS("class");
Token Token::ELSE("else");
Token Token::FOR("for");
Token Token::IF("if");
Token Token::INT("int");
Token Token::NEW("new");
Token Token::PRIVATE("private");
Token Token::PUBLIC("public");
Token Token::RETURN("return");
Token Token::STATIC("static");
Token Token::THIS("this");
Token Token::VOID("void");

Token Token::STRINGLITERAL;

Token Token::INTLITERAL;

Token Token::CHARLITERAL;

Token Token::TRUE("true");
Token Token::FALSE("false");

Token Token::NULL_("null");
Token Token::LPAREN("(");
Token Token::RPAREN(")");
Token Token::LBRACE("{");
Token Token::RBRACE("}");
Token Token::LBRACKET("[");
Token Token::RBRACKET("]");

Token Token::SEMI(";");
Token Token::COMMA(",");
Token Token::DOT(".");
Token Token::ELLIPSIS("...");
Token Token::EQ("=");
Token Token::GT(">");
Token Token::LT("<");
Token Token::BANG("!");
Token Token::TILDE("~");
Token Token::QUES("?");
Token Token::COLON(":");
Token Token::EQEQ("==");
Token Token::LTEQ("<=");
Token Token::GTEQ(">=");
Token Token::BANGEQ("!=");
Token Token::AMPAMP("&&");
Token Token::BARBAR("||");
Token Token::PLUSPLUS("++");
Token Token::SUBSUB("--");
Token Token::PLUS("+");
Token Token::SUB("-");
Token Token::STAR("*");
Token Token::SLASH("/");
Token Token::AMP("&");
Token Token::BAR("|");
Token Token::CARET("^");
Token Token::PERCENT("%");
Token Token::LTLT("<<");
Token Token::GTGT(">>");
Token Token::GTGTGT(">>>");
Token Token::PLUSEQ("+=");
Token Token::SUBEQ("-=");
Token Token::STAREQ("*=");
Token Token::SLASHEQ("/=");
Token Token::AMPEQ("&=");
Token Token::BAREQ("|=");
Token Token::CARETEQ("^=");
Token Token::PERCENTEQ("%=");
Token Token::LTLTEQ("<<=");
Token Token::GTGTEQ(">>=");
Token Token::GTGTGTEQ(">>>=");
Token Token::MONKEYS_AT("@");
`

var array = text.split('\n');

function replacer(match, p1, p2, offset, string) {
    if (p2) {
        return `::${p1}(ID_${p1}, ${p2});`;
    } else {
        return `::${p1}(ID_${p1});`;
    }
}

var test = /::(\w*)(?:\((.*)\))?;/

for (var i = 0; i < array.length; i++) {

    var item = array[i];
    if (item.trim().length == 0) {
        continue;
    }


    array[i] = item.replace(test, replacer);
}

var fs = require('fs');
fs.writeFile("./output.txt", array.join('\n'), function (err) {
    if (err) {
        return console.log(err);
    }
    console.log("The file was saved!");
});
