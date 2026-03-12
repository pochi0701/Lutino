// jss-mode.js - Ace Editor custom mode for JSS files
// JSS uses <? ?> delimiters for server-side JavaScript within HTML.
// This mode is loaded after ace.min.js and provides ace/mode/jss.

// Step 1: Define the highlight rules module
ace.define("ace/mode/jss_highlight_rules", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text_highlight_rules"], function(require, exports, module) {
    var oop = require("../lib/oop");
    var TextHighlightRules = require("./text_highlight_rules").TextHighlightRules;

    var JssHighlightRules = function() {
        // --- HTML states (outside <? ?>) ---
        // Simplified but functional HTML highlighting
        this.$rules = {
            "start": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",  // <?
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: "comment.start.xml",
                    regex: "<!--",
                    push: "html_comment"
                },
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<(?=\\s*script\\b)",
                    push: "html_script_tag"
                },
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<(?=\\s*style\\b)",
                    push: "html_style_tag"
                },
                {
                    token: "meta.tag.punctuation.tag-open.xml",  // <tag or </tag
                    regex: "<\\/?" ,
                    push: "html_tag"
                },
                {
                    token: "text.xml",
                    regex: "[^<]+"
                }
            ],

            // --- HTML comment ---
            "html_comment": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: "comment.end.xml",
                    regex: "-->",
                    next: "pop"
                },
                { defaultToken: "comment.xml" }
            ],

            // --- HTML tag attributes ---
            "html_tag": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: "meta.tag.punctuation.tag-close.xml",
                    regex: "\\/?>",
                    next: "pop"
                },
                {
                    token: "meta.tag.tag-name.xml",
                    regex: "[a-zA-Z][a-zA-Z0-9\\-]*"
                },
                {
                    token: "entity.other.attribute-name.xml",
                    regex: "[a-zA-Z_][a-zA-Z0-9_\\-]*"
                },
                {
                    token: "keyword.operator.attribute-equals.xml",
                    regex: "="
                },
                {
                    token: "string.attribute-value.xml",
                    regex: "'",
                    push: "html_attr_sq"
                },
                {
                    token: "string.attribute-value.xml",
                    regex: '"',
                    push: "html_attr_dq"
                },
                {
                    token: "text",
                    regex: "\\s+"
                }
            ],
            "html_attr_sq": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                { token: "string.attribute-value.xml", regex: "'", next: "pop" },
                { defaultToken: "string.attribute-value.xml" }
            ],
            "html_attr_dq": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                { token: "string.attribute-value.xml", regex: '"', next: "pop" },
                { defaultToken: "string.attribute-value.xml" }
            ],

            // --- <script> tag: parse attributes then inline JS ---
            "html_script_tag": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: "meta.tag.tag-name.xml",
                    regex: "script"
                },
                {
                    token: "meta.tag.punctuation.tag-close.xml",
                    regex: ">",
                    next: "html_script_body"
                },
                {
                    token: "meta.tag.punctuation.tag-close.xml",
                    regex: "/>",
                    next: "pop"
                },
                {
                    token: "entity.other.attribute-name.xml",
                    regex: "[a-zA-Z_][a-zA-Z0-9_\\-]*"
                },
                {
                    token: "keyword.operator.attribute-equals.xml",
                    regex: "="
                },
                {
                    token: "string.attribute-value.xml",
                    regex: "'[^']*'"
                },
                {
                    token: "string.attribute-value.xml",
                    regex: '"[^"]*"'
                },
                { token: "text", regex: "\\s+" }
            ],
            "html_script_body": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: ["meta.tag.punctuation.tag-open.xml", "meta.tag.tag-name.xml", "meta.tag.punctuation.tag-close.xml"],
                    regex: "(<\\/)(script)(>)",
                    next: "pop"
                },
                {
                    token: "keyword",
                    regex: "\\b(?:break|case|catch|continue|debugger|default|delete|do|else|finally|for|function|if|in|instanceof|new|return|switch|this|throw|try|typeof|var|void|while|with|class|const|export|extends|import|let|static|super|yield|async|await|of|get|set|null|undefined|true|false)\\b"
                },
                {
                    token: "support.function",
                    regex: "\\b(?:console|document|window|navigator|Math|JSON|Array|Object|String|Number|Date|RegExp|Error|Promise|Map|Set|Symbol)\\b"
                },
                {
                    token: "string",
                    regex: '"(?:[^"\\\\]|\\\\.)*?"'
                },
                {
                    token: "string",
                    regex: "'(?:[^'\\\\]|\\\\.)*?'"
                },
                {
                    token: "string",
                    regex: "`",
                    push: "inline_js_template"
                },
                {
                    token: "constant.numeric",
                    regex: "0[xX][0-9a-fA-F]+\\b|\\b\\d+(?:\\.\\d+)?(?:[eE][+-]?\\d+)?\\b"
                },
                {
                    token: "comment",
                    regex: "\\/\\/.*$"
                },
                {
                    token: "comment.start",
                    regex: "\\/\\*",
                    push: "inline_js_block_comment"
                },
                {
                    token: "keyword.operator",
                    regex: "===|!==|==|!=|<=|>=|&&|\\|\\||\\+\\+|--|=>|\\+=|-=|\\*=|/=|%=|[+\\-*/%&|^~<>!?:;]"
                },
                {
                    token: "paren.lparen",
                    regex: "[\\[\\(\\{]"
                },
                {
                    token: "paren.rparen",
                    regex: "[\\]\\)\\}]"
                },
                {
                    token: "text",
                    regex: "\\s+|[^<]"
                }
            ],
            "inline_js_block_comment": [
                { token: "comment.end", regex: "\\*\\/", next: "pop" },
                { defaultToken: "comment" }
            ],
            "inline_js_template": [
                { token: "string", regex: "`", next: "pop" },
                { token: "string", regex: "\\\\." },
                { defaultToken: "string" }
            ],

            // --- <style> tag ---
            "html_style_tag": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: "meta.tag.tag-name.xml",
                    regex: "style"
                },
                {
                    token: "meta.tag.punctuation.tag-close.xml",
                    regex: ">",
                    next: "html_style_body"
                },
                {
                    token: "meta.tag.punctuation.tag-close.xml",
                    regex: "/>",
                    next: "pop"
                },
                {
                    token: "entity.other.attribute-name.xml",
                    regex: "[a-zA-Z_][a-zA-Z0-9_\\-]*"
                },
                {
                    token: "keyword.operator.attribute-equals.xml",
                    regex: "="
                },
                {
                    token: "string.attribute-value.xml",
                    regex: "'[^']*'"
                },
                {
                    token: "string.attribute-value.xml",
                    regex: '"[^"]*"'
                },
                { token: "text", regex: "\\s+" }
            ],
            "html_style_body": [
                {
                    token: "meta.tag.punctuation.tag-open.xml",
                    regex: "<\\?(?:=)?",
                    push: "jss_js-start"
                },
                {
                    token: ["meta.tag.punctuation.tag-open.xml", "meta.tag.tag-name.xml", "meta.tag.punctuation.tag-close.xml"],
                    regex: "(<\\/)(style)(>)",
                    next: "pop"
                },
                {
                    token: "support.type",
                    regex: "[a-zA-Z_][a-zA-Z0-9_\\-]*(?=\\s*:)"
                },
                {
                    token: "constant.numeric",
                    regex: "#[0-9a-fA-F]{3,8}\\b|\\b\\d+(?:\\.\\d+)?(?:px|em|rem|%|vh|vw|pt|cm|mm|in|ex|ch|s|ms|deg|rad|grad|turn)?"
                },
                {
                    token: "comment",
                    regex: "\\/\\*",
                    push: "css_block_comment"
                },
                { defaultToken: "text" }
            ],
            "css_block_comment": [
                { token: "comment", regex: "\\*\\/", next: "pop" },
                { defaultToken: "comment" }
            ],

            // --- JSS JavaScript states (inside <? ?>) ---
            "jss_js-start": [
                {
                    token: "meta.tag.punctuation.tag-close.xml",  // ?>
                    regex: "\\?>",
                    next: "pop"
                },
                {
                    token: "keyword",
                    regex: "\\b(?:break|case|catch|continue|debugger|default|delete|do|else|finally|for|function|if|in|instanceof|new|return|switch|this|throw|try|typeof|var|void|while|with|class|const|export|extends|import|let|static|super|yield|async|await|of|get|set)\\b"
                },
                {
                    token: "constant.language",
                    regex: "\\b(?:null|undefined|NaN|Infinity|true|false)\\b"
                },
                {
                    token: "support.function",
                    regex: "\\b(?:print|include|require|saveToFile|loadFile|console|document|window|navigator|parseInt|parseFloat|isNaN|isFinite|encodeURI|decodeURI|encodeURIComponent|decodeURIComponent|eval|setTimeout|setInterval)\\b"
                },
                {
                    token: "support.class",
                    regex: "\\b(?:_SERVER|_GET|_POST|_COOKIE|_SESSION|Math|JSON|Array|Object|String|Number|Date|RegExp|Error|Promise|Map|Set|Symbol)\\b"
                },
                {
                    token: "storage.type",
                    regex: "\\b(?:var|let|const|function|class)\\b"
                },
                {
                    token: "string",
                    regex: '"',
                    push: "jss_js_string_dq"
                },
                {
                    token: "string",
                    regex: "'",
                    push: "jss_js_string_sq"
                },
                {
                    token: "string",
                    regex: "`",
                    push: "jss_js_template_string"
                },
                {
                    token: "constant.numeric",
                    regex: "0[xX][0-9a-fA-F]+\\b|\\b\\d+(?:\\.\\d+)?(?:[eE][+-]?\\d+)?\\b"
                },
                {
                    token: "comment",
                    regex: "\\/\\/.*$"
                },
                {
                    token: "comment.start",
                    regex: "\\/\\*",
                    push: "jss_js_block_comment"
                },
                {
                    token: "keyword.operator",
                    regex: "===|!==|==|!=|<=|>=|&&|\\|\\||\\+\\+|--|=>|\\+=|-=|\\*=|/=|%=|[+\\-*/%&|^~<>!?:;,]"
                },
                {
                    token: "paren.lparen",
                    regex: "[\\[\\(\\{]"
                },
                {
                    token: "paren.rparen",
                    regex: "[\\]\\)\\}]"
                },
                {
                    token: "punctuation.operator",
                    regex: "\\."
                },
                {
                    token: "identifier",
                    regex: "[a-zA-Z_$][a-zA-Z0-9_$]*"
                },
                {
                    token: "text",
                    regex: "\\s+"
                }
            ],
            "jss_js_string_dq": [
                { token: "constant.language.escape", regex: "\\\\." },
                { token: "string", regex: '"', next: "pop" },
                { defaultToken: "string" }
            ],
            "jss_js_string_sq": [
                { token: "constant.language.escape", regex: "\\\\." },
                { token: "string", regex: "'", next: "pop" },
                { defaultToken: "string" }
            ],
            "jss_js_template_string": [
                { token: "constant.language.escape", regex: "\\\\." },
                { token: "string", regex: "`", next: "pop" },
                { defaultToken: "string" }
            ],
            "jss_js_block_comment": [
                { token: "comment.end", regex: "\\*\\/", next: "pop" },
                { defaultToken: "comment" }
            ]
        };

        this.normalizeRules();
    };

    oop.inherits(JssHighlightRules, TextHighlightRules);
    exports.JssHighlightRules = JssHighlightRules;
});

// Step 2: Define the mode module
ace.define("ace/mode/jss", ["require", "exports", "module", "ace/lib/oop", "ace/mode/text", "ace/mode/jss_highlight_rules", "ace/mode/matching_brace_outdent", "ace/mode/behaviour/cstyle", "ace/mode/folding/cstyle"], function(require, exports, module) {
    var oop = require("../lib/oop");
    var TextMode = require("./text").Mode;
    var JssHighlightRules = require("./jss_highlight_rules").JssHighlightRules;

    var Mode = function() {
        this.HighlightRules = JssHighlightRules;
        try {
            var MatchingBraceOutdent = require("./matching_brace_outdent").MatchingBraceOutdent;
            this.$outdent = new MatchingBraceOutdent();
        } catch(e) {}
        try {
            var CstyleBehaviour = require("./behaviour/cstyle").CstyleBehaviour;
            this.$behaviour = new CstyleBehaviour();
        } catch(e) {}
        try {
            var CStyleFoldMode = require("./folding/cstyle").FoldMode;
            this.foldingRules = new CStyleFoldMode();
        } catch(e) {}
    };
    oop.inherits(Mode, TextMode);

    (function() {
        this.$id = "ace/mode/jss";
    }).call(Mode.prototype);

    exports.Mode = Mode;
});
