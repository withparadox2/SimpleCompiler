var text = `
    static Token ERROR;
    static Token _EOF;

    static Token IDENTIFIER;
    static Token BOOLEAN;
    static Token CLASS;
    static Token ELSE;
    static Token FOR;
    static Token IF;
    static Token INT;
    static Token NEW;
    static Token PRIVATE;
    static Token PUBLIC;
    static Token RETURN;
    static Token STATIC;
    static Token THIS;
    static Token VOID;

    static Token STRINGLITERAL;
    static Token INTLITERAL;
    static Token CHARLITERAL;
    static Token TRUE;
    static Token FALSE;

    static Token NULL_;
    static Token LPAREN;
    static Token RPAREN;
    static Token LBRACE;
    static Token RBRACE;
    static Token LBRACKET;
    static Token RBRACKET;

    static Token SEMI;
    static Token COMMA;
    static Token DOT;
    static Token ELLIPSIS;
    static Token EQ;
    static Token GT;
    static Token LT;
    static Token BANG;
    static Token TILDE;
    static Token QUES;
    static Token COLON;
    static Token EQEQ;
    static Token LTEQ;
    static Token GTEQ;
    static Token BANGEQ;
    static Token AMPAMP;
    static Token BARBAR;
    static Token PLUSPLUS;
    static Token SUBSUB;
    static Token PLUS;
    static Token SUB;
    static Token STAR;
    static Token SLASH;
    static Token AMP;
    static Token BAR;
    static Token CARET;
    static Token PERCENT;
    static Token LTLT;
    static Token GTGT;
    static Token GTGTGT;
    static Token PLUSEQ;
    static Token SUBEQ;
    static Token STAREQ;
    static Token SLASHEQ;
    static Token AMPEQ;
    static Token BAREQ;
    static Token CARETEQ;
    static Token PERCENTEQ;
    static Token LTLTEQ;
    static Token GTGTEQ;
    static Token GTGTGTEQ;
    static Token MONKEYS_AT;
`

var array = text.split('\n');

var preName = '';
var ID_REGEX = /(ID.*?)=.*;/;
for (var i = 0; i < array.length; i++) {
    var item = array[i];
    if (item.trim().length == 0) {
        continue;
    }
    item = item.replace('static Token ', 'const static int ID_');

    item = item.replace(';', ` = ${preName} + 1;`);
    array[i] = item;

    console.log(item)

    preName = item.match(ID_REGEX)[1];
}

var fs = require('fs');
fs.writeFile("./output.txt", array.join('\n'), function (err) {
    if (err) {
        return console.log(err);
    }
    console.log("The file was saved!");
});
