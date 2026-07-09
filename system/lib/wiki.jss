<?
/*  Wiki
 */
// setup some global storage
var config = { "PAWFALIKI_VERSION": "0.1.1" }; // Wiki version
//================
//================
// CONFIGURATION
//================
//================
// GENERAL: General configuration stuff
config.GENERAL = {
    "TITLE": "Lutino WIKI",                         // Title of the wiki
    "HOMEPAGE": "Lutino WIKI",                      // The title of the homepage
    "ADMIN": "webmaster at nowhere dot example",    // not used currently
    "CSS": "Wiki:wiki.css",                         // CSS file (title:filename)
    "PAGES_DIRECTORY": "./WikiPages/",              // Path to stored wiki pages
    "TEMP_DIRECTORY": "./WikiTemp/",                // Path to temporary directory for backups
    "MODTIME_FORMAT": "(%Y-%m-%d %H:%M:%S)",        // date() compatible format string for the pagelist
    "SHOW_CONTROLS": true,                          // show all the wiki controls - edit, save, PageList etc...
    "DEBUG": false,                                 // display debug information (pagegen time, uptime, load)
    "ENCODE": "utf-8"                               // "shift-jis","utf-8"...
};

// SYNTAX: Wiki editing syntax
config.SYNTAX = {
    "SHOW_BOX": true,                               // Display the wiki syntax box on edit page
    "WIKIWORDS": false,                             // Auto-generation of links from WikiWords
    "AUTOCREATE": true,                             // Display ? next to wiki pages that don't exist yet.
    "HTMLCODE": true                                // Allows raw html using %% tags
};

// BACKUP: Backup & Restore settings
config.BACKUP = {
    "ENABLE": true,                                 // Enable backup & restore
    "USE_ZLIB": true,                               // If available use the libz module to produce gzipped backups
    "MAX_SIZE": 3000000                             // maximum file size (in bytes) for uploading restore files
};

// RSS: RSS feed
config.RSS = {
    "ENABLE": true,                                 // Enable rss support (http://mywiki.example?format=rss)
    "ITEMS": 10,                                    // The number of items to display in rss feed (-1 for all).
    "TITLE_MODTIME": false,                         // Prints the modification time in the item title.
    "MODTIME_FORMAT": "(Y-m-d H:i:s T)"             // date() compatible format string
};

// CHANGES: email page changes
config.EMAIL = {
    "ENABLE": false,                                // do we email page changes?
    "CHANGES_TO": "admin@nowhere.example",          // if so, where to
    "CHANGES_FROM": "changes@nowhere.example",      // & where from
    "MODTIME_FORMAT": "%Y-%m-%d %H:%M:%S",          // date() compatible format string for the pagelist
    "SHOW_IP": false                                // show the modifiers ip in the email subject
};

// USERS: setup user passwords
config.USERS = {
    "admin2": "admin",                              // changing this would be a good idea!
    "group1": "group1password"                      // create a new user password
};

// RESTRICTED: give access to some users to edit restricted pages
config.RESTRICTED = {
    "RestoreWiki": ["admin2"]                       // only admin2 can restore wiki pages
};

// LOCALE: text for some titles, icons, etc - you can use wiki syntax in these for images etc...
config.LOCALE = {
    "EDIT_TITLE": "edit: ",                         // title prefix for edit pages
    "HOMEPAGE_LINK": "[[HomePage]]",                // link to the homepage
    "PAGELIST_LINK": "[[PageList]]",                // link to the pagelist
    "REQ_PASSWORD": "(locked)",                     // printed next to the edit btn on a locked page
    "PASSWORD_TEXT": "Password:"                    // printed next to the password entry box
};
// SPECIAL PAGES - reserved and unmodifiable by users
config.SPECIAL = {
    "PageList": 1,                                  // the page list
    "BackupWiki": 1,                                // the backup page
    "RestoreWiki": 1                                // the restore page
};
// MISC: Misc stuff
config.MISC = {
    "EXTERNALLINKS_NEWWINDOW": false,               // Open external links in a new window
    "REQ_PASSWORD_TEXT_IN_EDIT_BTN": false           // Include the req password text in the edit button
};
config.INTERNAL = { "VERBATIM": [], "ERRORS": [], "DATA": [] };

//===========================================================================
//===========================================================================
// initialise our style sheets
function isset(object) {
    return (!(object === undefined));
}
function css(pagename) {
    //global config;
    cssStr = config.GENERAL.CSS;   // fix: css という関数名への上書きを回避
    if (cssStr != "") {
        tokens = cssStr.split(":");
        title = tokens[0];
        path = tokens[1];//.remove(1).join(":");
        print("\t<link rel=\"stylesheet\" type=\"text/css\" href=\"" + path + "\" title=\"" + title + "\" />\n");
        if (config.RSS.ENABLE && pagename == "HomePage") {
            print("\t<link rel=\"alternate\" title=\"" + config.GENERAL.TITLE +
                " RSS\" href=\"" + _SERVER.SCRIPT_NAME +
                "?format=rss\" type=\"application/rss+xml\" />\n");
        }
    }
}

// emails page changes
function emailChanges(title, contents) {
    if (config.EMAIL.ENABLE) {
        dateStr = Date().toDateString(config.EMAIL.MODTIME_FORMAT);  // fix: date() → Date().toDateString()、変数名も dateStr に変更
        subject = title + " :: " + dateStr;
        if (config.EMAIL.SHOW_IP) {
            ipaddress = _SERVER.REMOTE_ADDR;
            subject += " :: IP " + ipaddress;
        }
        mail(config.EMAIL.CHANGES_TO, subject, contents, "From: " + config.EMAIL.CHANGES_FROM + "\r\n");
    }
}

// writes a file to disk
function writeFile(title, contents) {
    if (saveToFile(pagePath(title), contents) == 0) {
        error("Cannot write to server's file: " + pagePath(title));
        return 2;
    }

    // email page changes
    emailChanges(title, contents);
    return 0;
}

// reads the contents of a file into a string (php<4.3.0 friendly)
function wikiReadFile(filename) {
    result = loadFromFile(filename);
    return result;
}

// returns the contents of a directory (php<4.3.0 friendly)
function wikiReadDir(path) {
    return eval(scandir(path));
}

// init the wiki if no pages exist
function initWiki(title) {
    contents = "Hello and welcome to Wiki!";
    writeFile(title, contents);
}

// get the title of a page
function getTitle() {
    page = "";
    if (!isset(_GET.page)) {
        page = "HomePage";
        if (!pageExists(page)) {
            initWiki(page);
        }
    }
    else {
        page = _GET.page;
        if (page.split("/").length > 1) {
            page = "HomePage";
        }
    }
    return page;
}

// get the current wiki 'mode'
function getMode() {
    mode = "";
    if (!isset(_POST.mode)) {
        mode = "display";
    }
    else {
        mode = _POST.mode;
    }
    return mode;
}

// check 
function authPassword(title, password) {
    auth = false;
    for (i = 0; i < config.RESTRICTED[title].length; i++) {
        user = config.RESTRICTED[title][i];
        if (config.USERS[user] == _POST.password)
            auth = true;
    }
    return auth;
}

// update the wiki - save/edit/backup/restore/cancel
function updateWiki(modes, title, config) {
    contents = "";
    backupEnabled = config.BACKUP.ENABLE;
    // cleanup any temp files
    if (backupEnabled) {
        cleanupTempFiles();
    }
    // backup the wiki
    if (title == "BackupWiki") {
        if (backupEnabled) {
            wikiname = config.GENERAL.TITLE;
            wikiname.replaceAll(" ", "_");
            //date = date( "Y-m-d_H-i-s" );
            //filename = tempDir()+wikiname+"_"+date+"+bkup";
            filename = tempDir() + wikiname + "_bkup";
            backupPages(filename);
            modes.mode = "backupwiki";
        }
        else {
            error("Backups have been disabled+");
        }
    }
    // restore from backup
    if (title == "RestoreWiki")
        if (backupEnabled) {
            if (modes.mode == "restorewiki" && isset(_POST.userfile) && isset(_POST.userfile.filename) && _POST.userfile.filename != "")
                restorePages();
            else
                modes.mode = "restorewiki";
        }
        else {
            error("Restore has been disabled+");
            modes.mode = "restorewiki";
        }

    // save page
    if (modes.mode == "save") {
        if (isset(_POST.contents)) {
            //contents = stripslashes( _POST.contents );
            contents = _POST.contents;

            // restricted access
            restricted = false;
            if (isLocked(title)) {
                // check if the password is correct
                restricted = !authPassword(title, _POST.password);
                if (restricted)
                    error("Wrong password. Try again+");
            }

            // write file    
            writeResult = 0;
            if (!isIpBlocked() && !restricted)
                writeResult = writeFile(title, contents);
        }
        modes.mode = "display";

        // go back if you can't write the data (avoid data loss)
        if ((restricted) || (writeResult != 0))
            modes.mode = "edit";
    }
    // cancel a page edit
    if (modes.mode == "cancel") {
        modes.mode = "display";
    }
    return contents;
}

// generate our html header
function htmlHeader(title, config) {
    origTitle = title;
    if (title == "HomePage") {
        title = config.GENERAL.HOMEPAGE;
    }
    print("<!doctype html>\n");
    print("<html lang=\"ja\">\n");
    print("<head>\n");
    print("  <meta charset=\"utf-8\">\n");
    print("  <title>\n");
    if (config.GENERAL.TITLE == title) {
        print(config.GENERAL.TITLE);
    } else {
        print(config.GENERAL.TITLE + ">" + title);
    }
    print("</title>\n");
    print("  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n");
    print("  <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-rbsA2VBKQhggwzxH7pPCaAqO46MgnOM80zW1RWuH61DGLwZJEdK2Kadq2F9CUG65\" crossorigin=\"anonymous\">");
    print("  <link rel =\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.1.1/css/all.min.css\">\n");
    print("  <script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-kenU1KFdBIe4zVF0s0G1M5b4hcpxyD9F7jL+jjXkk+Q2h455rYXK/7HAuoJl+0I4\" crossorigin=\"anonymous\"></script>");
    print("  <script src=\"http://code.jquery.com/jquery-1.12.0.min.js\"></script>\n");
    print("  <style>\n");
    print("    body { background: #eef1f5; font-family: 'Segoe UI', Meiryo, sans-serif; }\n");
    print("    .wiki-topnav {\n");
    print("      background: #2c3e50; color: #ecf0f1;\n");
    print("      display: flex; align-items: center; justify-content: space-between;\n");
    print("      padding: 0.6rem 1.2rem; border-radius: 0.5rem;\n");
    print("      margin-bottom: 1rem; box-shadow: 0 2px 6px rgba(0,0,0,0.2);\n");
    print("    }\n");
    print("    .wiki-topnav-brand {\n");
    print("      font-size: 1.2rem; font-weight: 700; letter-spacing: 1px;\n");
    print("      color: #ecf0f1; text-decoration: none;\n");
    print("    }\n");
    print("    .wiki-topnav-links a {\n");
    print("      color: #bdc3c7; text-decoration: none;\n");
    print("      margin-left: 1.1rem; font-size: 0.9rem; transition: color 0.2s;\n");
    print("    }\n");
    print("    .wiki-topnav-links a:hover { color: #ffffff; }\n");
    print("    .wiki-topnav-links a.home-link { font-size: 1.1rem; }\n");
    print("    .wiki-card {\n");
    print("      background: #ffffff; border-radius: 0.5rem;\n");
    print("      box-shadow: 0 1px 5px rgba(0,0,0,0.1); padding: 1.5rem 2rem;\n");
    print("      min-height: 200px; margin-bottom: 1rem;\n");
    print("    }\n");
    print("    .wiki_body { display: block; line-height: 1.8; }\n");
    print("    .wiki_body h1, .wiki_body .h1 { border-bottom: 2px solid #2c3e50; padding-bottom: 0.3rem; margin-bottom: 1rem; }\n");
    print("    .wiki_body a { color: #2980b9; }\n");
    print("    .wiki_body a:hover { color: #1a5276; }\n");
    print("    .wiki-controls { background: #f8f9fa; border-radius: 0.4rem; padding: 0.5rem 1rem; margin-bottom: 0.5rem; }\n");
    print("    .wiki-pagefooter { color: #888; font-size: 0.85rem; border-top: 1px solid #dee2e6; padding-top: 0.5rem; margin-top: 0.5rem; }\n");
    print("    .copyright { color: #aaa; font-size: 0.8rem; margin-top: 1rem; }\n");
    print("    .error { color: #c0392b; background: #fdecea; padding: 0.4rem 0.8rem; border-radius: 0.3rem; }\n");
    print("    #wiki-edit-form { display: none; }\n");
    print("    #wiki-edit-badge {\n");
    print("      position: fixed; bottom: 14px; right: 14px;\n");
    print("      background: #e74c3c; color: white;\n");
    print("      padding: 5px 13px; border-radius: 20px;\n");
    print("      font-size: 12px; display: none; z-index: 9999;\n");
    print("      cursor: pointer; box-shadow: 0 2px 6px rgba(0,0,0,0.3);\n");
    print("      user-select: none;\n");
    print("    }\n");
    print("    #wiki-edit-badge:hover { background: #c0392b; }\n");
    print("  </style>\n");
    print("  <script>\n");
    print("  (function() {\n");
    print("    var STORAGE_KEY = 'wiki_edit_mode';\n");
    print("    function setEditMode(on) {\n");
    print("      var form = document.getElementById('wiki-edit-form');\n");
    print("      var badge = document.getElementById('wiki-edit-badge');\n");
    print("      if (form) form.style.display = on ? 'block' : 'none';\n");
    print("      if (badge) badge.style.display = on ? 'block' : 'none';\n");
    print("      localStorage.setItem(STORAGE_KEY, on ? '1' : '0');\n");
    print("    }\n");
    print("    document.addEventListener('DOMContentLoaded', function() {\n");
    print("      var badge = document.createElement('div');\n");
    print("      badge.id = 'wiki-edit-badge';\n");
    print("      badge.innerHTML = '<i class=\"fas fa-pencil-alt\"></i> Edit ON';\n");
    print("      badge.title = 'クリックまたは / キーでトグル';\n");
    print("      badge.addEventListener('click', function() {\n");
    print("        var form = document.getElementById('wiki-edit-form');\n");
    print("        setEditMode(form && form.style.display === 'none');\n");
    print("      });\n");
    print("      document.body.appendChild(badge);\n");
    print("      setEditMode(localStorage.getItem(STORAGE_KEY) === '1');\n");
    print("      document.addEventListener('keydown', function(e) {\n");
    print("        var tag = document.activeElement ? document.activeElement.tagName.toLowerCase() : '';\n");
    print("        if (tag === 'input' || tag === 'textarea') return;\n");
    print("        if (e.key === '/') {\n");
    print("          var form = document.getElementById('wiki-edit-form');\n");
    print("          setEditMode(form && form.style.display === 'none');\n");
    print("          e.preventDefault();\n");
    print("        }\n");
    print("      });\n");
    print("    });\n");
    print("  })();\n");
    print("  </script>\n");
    print("</head>\n");
    print("<body>\n");
    print("  <div class=\"container mt-3\">\n");
    print("    <nav class=\"wiki-topnav\">\n");
    print("      <span class=\"wiki-topnav-brand\">Lutino Wiki</span>\n");
    print("      <div class=\"wiki-topnav-links\">\n");
    print("        <a class=\"home-link\" href=\"/\" title=\"Home\"><i class=\"fas fa-house\"></i></a>\n");
    print("        <a href=\"" + _SERVER.SCRIPT_NAME + "?page=HomePage\">HomePage</a>\n");
    print("        <a href=\"" + _SERVER.SCRIPT_NAME + "?page=PageList\">PageList</a>\n");
    print("      </div>\n");
    print("    </nav>\n");
    print("    <div class=\"wiki-card\">\n");
}
function htmlfooter2(title, config, mode) {
    origTitle = title;
    if (title == "HomePage") {
        title = config.GENERAL.HOMEPAGE;
    }
    // any errors?
    for (i = 0; i < config.INTERNAL.ERRORS.length; i++) {
        err = config.INTERNAL.ERRORS[i];
        print("<p class=\"error\">ERROR: " + err + "</p>");
    }
    print("<div class=\"wiki-pagefooter\">" + title + "</div>\n");
}

// generate our html footer
function htmlFooter() {
    //global config;
    if (config.GENERAL.DEBUG) {
        timeParts = microtime().split(" ");  // fix: PHP list() → 配列で受け取る
        usec = timeParts[0];
        sec = timeParts[1];
        end_time = (float)sec + (float)usec;
        duration = end_time - config.GENERAL.DEBUG_STARTTIME;("cut -d. -f1 /proc/uptime");
        load_ar = exec("cat /proc/loadavg").split(" ");  // fix: 余分な ) を除去
        load = load_ar[2];
        days = floor(uptime / 60 / 60 / 24);
        hours = uptime / 60 / 60 % 24;
        mins = uptime / 60 % 60;
        secs = uptime % 60;

        print("<hr><b><u>DEBUG</u></b><br>");
        print(wikiparse("~~#FF0000:PAGE GENERATION:~~ duration secs\n"));
        print(wikiparse("~~#FF0000:SERVER UPTIME:~~ days day(s) hours hour(s) mins minute(s) and secs second(s)\n"));
        print(wikiparse("~~#FF0000:SERVER LOAD:~~ load\n"));
    }
    print("    </div>\n");
    print("  </div>\n");
    print("    <!-- ===== copyright ===== -->\n");
    print("    <div class=\"copyright\">\n");
    print("      <p class=\"text-center\">\n");
    print("        Copyright(c) 2016-2026 <a href=\"https://www.birdland.co.jp\">Birdland Ltd.</a> All Rights Reserved.\n");
    print("      </p>\n");
    print("    </div>\n");
    print("  </body>\n");
    print("</html>\n");
}

// the start of our wiki body
function htmlStartBlock() {
    //print("<hr>\n");
    //print("\t<hr />\n");
    //print("\t<table width=\"100%\" class=\"wiki_body_container\">\n");
    //print("\t<table class=\"wiki_body_container\">\n");
    //print("\t\t<tr>\n");
    //print("\t\t\t<td>\n");
    print("\n<!-- PAGE BODY -->\n");
}

// the end of our wiki body
function htmlEndBlock() {
    print("<!-- END OF PAGE BODY -->\n\n");
    //print("\t\t\t</td>\n");
    //print("\t\t</tr>\n");
    //print("\t</table>\n");
    //print("\t<hr />\n");
}

// link to another wiki page
function wikilink(title) {
    //global config;
    if (pageExists(title))
        return ("<a href=\"" + _SERVER.SCRIPT_NAME + "?page=" + title + "\">" + title + "</a>");
    else if (config.SYNTAX.AUTOCREATE)
        return (title + "<a href=\"" + _SERVER.SCRIPT_NAME + "?page=" + title + "\">?</a>");
    else
        return (title);
}

// link to another web page
function webpagelink(text) {
    results = text.split("|");
    size = results.length;
    if (size == 0)
        return text;

    // page link
    src = results[0];

    // link text
    desc = "";
    if (size > 1)
        desc = results[1];
    else
        desc = src;
    // is our text an image?
    patterns = "/\\{\\{([^{]*)\\}\\}/";    // fix: 壊れた正規表現を修正
    replacements = "\"+image( \"$1\" )+\"";
    cmd = ("desc = \"" + desc.preg_replace(patterns, replacements) + "\";");  // fix: \desc の \ を除去
    eval(cmd);

    // link target    
    window = "";
    if (size > 2)
        window = results[2];
    else
        if (config.MISC.EXTERNALLINKS_NEWWINDOW)
            window = "_blank";
        else
            window = "_self";

    // see whether it is a Wiki Link or not
    prefix = src.split("/");
    if ((prefix.length == 1)) // looks like a local file, an anchor link or a wikipage
    {
        if (pageExists(src)) // is it a wiki page
        {
            src = _SERVER.SCRIPT_NAME + "?page=" + src;
            window = "_self";
            resultstr = "<a href=\"" + src + "\" onclick=\"target='" + window + "';\">" + desc + "</a>";
        }
        else if (src[0] == "#") // maybe its an anchor link
        {
            window = "_self";
            resultstr = "<a href=\"" + src + "\" onclick=\"target='" + window + "';\">" + desc + "</a>";
        }
        else if (config.SYNTAX.AUTOCREATE) // maybe autolink
        {
            search_for_dot = src.indexOf("."); // don't support names with dots in - prevents creating executable scripts
            if (search_for_dot < 0)
                resultstr = (src + "<a href=\"" + _SERVER.SCRIPT_NAME + "?page=" + src + "\" onclick=\"target='" + window + "';\">?</a>");
            else
                resultstr = src;
        }
        else
            resultstr = desc;
    }
    else {
        resultstr = "<a href=\"" + src + "\" onclick=\"target='" + window + "';\">" + desc + "</a>";
    }
    return verbatim(resultstr);
}

// evaluate a chunk of text
function wikiEval(str) {
    result = "";
    cmd = "result = \"" + str + "\";";
    eval(cmd);
    return result;
}

// colour some text
function colouredtext(text) {
    results = text.split(":");
    size = results.length;
    if (size < 2) {
        return text;
    }
    colour = results[0];
    contents = wikiEval(implode(":", array_slice(results, 1)));
    resultstr = "<span style=\"color: #" + colour + ";\">" + contents + "</span>";
    return verbatim(resultstr);
}

// place an image
function image(text) {
    results = text.split("|");
    size = results.length;
    src = "";
    desc = "";
    width = "";
    height = "";
    align = "";
    valign = "";
    if (size >= 1)
        src = " src=\"" + results[0] + "\"";
    if (size >= 2)
        desc = " alt=\"" + results[1] + "\"";
    else
        desc = " alt=\"[img]\"";
    if (size >= 3)
        width += " width: " + results[2] + "px;";
    if (size >= 4)
        height += " height: " + results[3] + "px;";
    if (size >= 5)
        align = " float: " + results[4] + ";";
    if (size >= 6)
        valign = " vertical-align: " + results[5] + ";";
    resultstr = "";
    if (size > 0)
        resultstr = "<img" + src + width + height + align + valign + desc + " />";
    //resultstr = "<img" + src + " style=\"border:0pt none;" + width + height + align + valign + "\"" + desc + " />";
    return verbatim(resultstr);
}

// get some verbatim text
function getVerbatim(index) {
    //global config;
    verbat = config.INTERNAL.VERBATIM;
    return verbat[index];
}

// store some verbatim text
function verbatim(contents) {
    //global config;
    verbat = config.INTERNAL.VERBATIM;
    index = verbat.length;
    verbat[index] = contents;
    return "\"+getVerbatim(" + index + ")+\"";
}

// replace special chars with the appropriate html
function htmltag(contents) {
    // ' must be used for fields
    var result = contents;
    result = result.replaceAll("&lt;", "<");
    result = result.replaceAll("&gt;", ">");
    result = result.replaceAll("&quot;", "\\\"");
    return result;
}

// parse wiki code & replace with html
function wikiparse(contents) {
    //global config;
    //patterns = [];
    //replacements = [];
    patterns = [];
    replacements = [];
    //contents = htmlspecialchars(contents, ENT_COMPAT, "UTF-8");
    contents = htmlspecialchars(contents.addSlashes());
    // webpage links
    patterns[0] = "/\\[\\[([^\\[]*)\\]\\]/";
    replacements[0] = "\"+webpagelink( \"$1\" )+\"";

    // images
    patterns[1] = "/\\{\\{([^{]*)\\}\\}/";
    replacements[1] = "\"+image( \"$1\" )+\"";

    // coloured text
    patterns[2] = "/~~#([^~]*)~~/";
    replacements[2] = "\"+colouredtext( \"$1\" )+\"";

    patterns[3] = '/\\$/';
    replacements[3] = "&DOLLAR;";

    // verbatim text
    patterns[4] = "/\\~\\~\\~(.*)\\~\\~\\~/";
    replacements[4] = "\"+verbatim( \"$1\" )+\"";
    //if ( config.SYNTAX.HTMLCODE )
    //{
    patterns[5] = "/\\%\\%(.*)\\%\\%/";
    replacements[5] = "\"+htmltag( \"$1\" )+\"";
    //}

    // substitute complex expressions
    contents = wikiEval(contents.preg_replace(patterns, replacements));
    //contents = contents.preg_replace( patterns, replacements );
    //contents = wikiEval( contents );
    patterns = [];//[];
    replacements = [];//array();

    // h1
    patterns[0] = "/==([^=]*[^=]*)==/";
    replacements[0] = "<span class=\\\"h1\\\">$1</span>";

    // italic
    patterns[1] = "/''([^']*[^']*)''/";
    replacements[1] = "<i>$1</i>";

    // bold
    patterns[2] = "/\\*\\*([^\\*]*[^\\*]*)\\*\\*/";
    replacements[2] = "<b>$1</b>";

    // underline
    patterns[3] = "/__([^_]*[^_]*)__/";
    replacements[3] = "<span style=\\\"text-decoration: underline;\\\">$1</span>";

    // html shortcuts
    patterns[4] = "/@@([^@]*)@@/";
    replacements[4] = "<a name=\\\"$1\\\"></a>";

    // wiki words    
    if (config.SYNTAX.WIKIWORDS) {
        patterns[5] = "/([A-Z][a-z0-9]+[A-Z][A-Za-z0-9]+)/";
        replacements[5] = "\"+wikilink( \"$1\" )+\"";
    }

    // substitute simple expressions & final expansion
    contents = wikiEval(contents.preg_replace(patterns, replacements));
    //contents = contents.preg_replace( patterns, replacements );
    patterns = [];//array();
    replacements = [];//array();

    // replace some whitespace bits & bobs  
    patterns[0] = "/\t/";
    replacements[0] = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    patterns[1] = "/  /";
    replacements[1] = "&nbsp;&nbsp;";
    patterns[2] = "/&DOLLAR;/";
    replacements[2] = "$";
    patterns[3] = "/\n/";
    replacements[3] = "<br>\n";
    contents = contents.preg_replace(patterns, replacements);
    return contents;
}

// returns the directory where the wiki pages are stored
function pageDir() {
    //global config;
    return (config.GENERAL.PAGES_DIRECTORY);
}

// returns the directory where the temporary backups are stored
function tempDir() {
    //global config;
    return (config.GENERAL.TEMP_DIRECTORY);
}

// returns the full path to a page
function pagePath(title) {
    return (pageDir() + title);//.nkfconv("Ws"));
}

// clean up the temp directory
function cleanupTempFiles() {
    files = wikiReadDir(tempDir());
    for (i = 0; i < files.length; i++) {
        mtime = filedate(files[i]);  // fix: file → files
        //now = date("U");
        //if ( now-mtime>300 ) // delete any files that are older than 5 minutes
        //    unlink( file ); 
    }
}

// is this page 'special'?
function isSpecial(title) {
    //global config;
    return (isset(config.SPECIAL[title]));
}

// is this page 'locked'?
function isLocked(title) {
    //global config;
    return (isset(config.RESTRICTED[title]));
}



// add an error to our error buffer
function error(str) {
    //global config;
    config.INTERNAL.ERRORS[config.INTERNAL.ERRORS.length] = str;  // fix: PHP構文 [] = を修正
}

// are there any errors so far?
function anyErrors() {
    //global config;
    if (config.INTERNAL.ERRORS.length == 0)
        return false;
    else
        return true;
}

// is this ip address blocked?
function isIpBlocked() {
    //global config;
    result = false;
    ipaddress = _SERVER.REMOTE_ADDR;
    //foreach (config.BLOCKED_IPS as ip)
    //{
    //    if (preg_match( "/"+ip+"/", ipaddress ))
    //    {
    //        error( "Your ip address has been blocked from making changes!" );
    //        result = true;
    //        break;
    //    }    
    //}
    return result;
}

// does a given page exist yet?
function pageExists(title) {
    if (file_exists(pagePath(title)) || isSpecial(title)) {
        return true;
    } else {
        return false;
    }
}

// returns a list of pages
function pageList() {
    //global config;
    contents = "";
    files = wikiReadDir(pageDir());
    details = [];
    for (i = 0; i < files.length; i++) {
        file = files[i];
        details[file] = filedate(file);
    }
    //arsort(details);
    //reset(details);
    s = Object.keys(details);
    for (i = 0; i < s.length; i++) {
        dd = details[s[i]];
        ff = basename(s[i]);//.nkfconv("Sw");
        //contents += "[[" + basename(s[i]).nkfconv("Sw") + "]]\t" + details[s[i]].toDateString(config.GENERAL.MODTIME_FORMAT) + "\n";
        contents += "[[" + basename(s[i]) + "]]\t" + details[s[i]].toDateString(config.GENERAL.MODTIME_FORMAT) + "\n";
    }
    return contents;
}

// returns the pageList in RSS2.0 format
function rssFeed() {
    print("<?xml version=\"1.0\"?>\n");
    print("<rss version=\"2.0\">\n");
    print("\t<channel>\n");
    title = config.GENERAL.TITLE;
    print("\t\t<title>" + title + "</title>\n");   // fix: 変数を文字列リテラルにしていたのを修正
    url = "http://" + _SERVER.SERVER_NAME + _SERVER.SCRIPT_NAME;  // fix: . → + で文字列結合
    print("\t\t<link>" + url + "</link>\n");
    print("\t\t<description>Recently changed pages on the " + title + " wiki.</description>\n");  // fix: 変数埋め込み
    print("\t\t<generator>Wiki v" + config.PAWFALIKI_VERSION + "</generator>\n");
    files = wikiReadDir(pageDir());
    details = [];
    for (i = 0; i < files.length; i++) {
        file = files[i];
        details[file] = filedate(file);
    }
    //arsort(details);
    //reset(details);
    item = 0;
    numItems = config.RSS.ITEMS;
    rssKeys = Object.keys(details);  // fix: PHP list()/each() → Object.keys() で反復
    for (rssIdx = 0; rssIdx < rssKeys.length; rssIdx++) {
        key = rssKeys[rssIdx];
        val = details[key];
        title = basename(key);
        modtime = ("" + val).toDateString("%Y-%m-%d %H:%M:%S");  // fix: PHP date() → toDateString()（strftime形式）
        description = title + " " + modtime;
        print("\t\t<item>\n");
        if (config.RSS.TITLE_MODTIME)
            print("\t\t\t<title>" + description + "</title>\n");   // fix: 変数を文字列リテラルにしていたのを修正
        else
            print("\t\t\t<title>" + title + "</title>\n");         // fix: 同上
        print("\t\t\t<link>" + url + "?page=" + title + "</link>\n");  // fix: "title" → + title +
        print("\t\t\t<description>" + description + "</description>\n");
        print("\t\t</item>\n");
        item++;
        if (numItems != -1 && item >= numItems)
            break;
    }
    print("\t</channel>\n");
    print("</rss>\n");
}

// backup all the wiki pages to a file
function backupPages(filename) {
    files = wikiReadDir(pageDir());
    details = [];
    for (i = 0; i < files.length; i++) {
        file = files[i];
        details[file] = filedate(file);
    }
    //arsort(details);
    //reset(details);    
    pages = [];
    pos = 0;
    bkKeys = Object.keys(details);  // fix: PHP list()/each() → Object.keys() で反復
    for (bkIdx = 0; bkIdx < bkKeys.length; bkIdx++) {
        key = bkKeys[bkIdx];
        val = details[key];
        pages[pos] = [];
        pages[pos].title = basename(key);
        pages[pos].datestring = "" + val;  // fix: date("U", val) → val はfiledate()が返すUnixタイムスタンプそのまま
        pos = pos + 1;
    }
    numpages = pages.length;
    if (numpages == 0) // must have at least 1 page for a backup
    {
        error("No pages to backup yet!");
        return;
    }
    // fix: extension_loaded/gzopen/fopen等のPHP関数をsaveToFileで置き換え
    backupContent = numpages + "\n";
    for (i = 0; i < pages.length; i++) {
        page = pages[i];
        lines = wikiReadFile(pagePath(page.title)).rtrim();
        numlines = lines.split("\n").length;
        if (numlines == 0) // no lines?! weird - we must have at least 1 line for restore
        {
            numlines = 1;
            lines += "\n";
        }
        backupContent += page.title + "\n" + page.datestring + "\n" + numlines + "\n" + lines + "\n";
    }
    if (saveToFile(filename, backupContent) == 0) {
        error("Cannot write backup file: " + filename);
    }
    return 0;
}

// restore all the wiki pages from a file
function restorePages() {
    //global config, _POST;
    config.INTERNAL.DATA.RESTORED = undefined;  // fix: PHP unset() → undefinedへ代入
    if (!authPassword("RestoreWiki", _POST.password)) {
        error("Wrong password. Try again+");
        return;
    }

    if (!isset(_POST.userfile) || !isset(_POST.userfile.filebody) || _POST.userfile.length == 0) {
        error("No file was uploaded!<BR>Maybe the filesize exceeded the maximum upload size of " + config.BACKUP.MAX_SIZE + "bytes+");
        return;
    }

    // fix: _FILES/_POST.userfile.filebody はbase64エンコード済み → atob()でデコード
    fileContent = atob(_POST.userfile.filebody);
    if (!fileContent) {
        error("Could not read uploaded file content!");
        return;
    }
    fileLines = fileContent.split("\n");
    lineIdx = 0;
    fileerror = "NO ERROR";

    // sanity check on file
    numPages = Integer.parseInt(fileLines[lineIdx].trim());
    lineIdx++;
    if (numPages > 0) // must be at least 1 page
    {
        for (i = 0; i < numPages; i++) {
            if (lineIdx >= fileLines.length) { fileerror = "Invalid title on page " + i + "!"; break; }
            lineIdx++; // title
            if (lineIdx >= fileLines.length) { fileerror = "Invalid mod time on page " + i + "!"; break; }
            lineIdx++; // modtime
            if (lineIdx >= fileLines.length) { fileerror = "Invalid numlines on page " + i + "!"; break; }
            numLines = Integer.parseInt(fileLines[lineIdx].trim());
            lineIdx++;
            if (numLines > 0) // must have at least 1 line
            {
                for (j = 0; j < numLines; j++) {
                    if (lineIdx >= fileLines.length && i != numPages - 1) { fileerror = "Invalid line read on page " + i + "!"; }
                    lineIdx++;
                }
            }
            else {
                fileerror = "Invalid number of page lines on page " + i + "!";
            }
        }
    }
    else {
        fileerror = "Invalid number of backup pages!";
    }
    if (fileerror != "NO ERROR") {
        error("This does not appear to be a valid backup file! (" + fileerror + ")");
        return;
    }

    // if we got here the file is OK - restore the pages!!
    config.INTERNAL.DATA.RESTORED = [];
    restored = config.INTERNAL.DATA.RESTORED;
    fileLines = fileContent.split("\n");
    lineIdx = 0;
    numPages = Integer.parseInt(fileLines[lineIdx].trim());
    lineIdx++;
    for (i = 0; i < numPages; i++) {
        title = fileLines[lineIdx].trim();
        lineIdx++;
        modtime = fileLines[lineIdx].trim();
        lineIdx++;
        numLines = Integer.parseInt(fileLines[lineIdx].trim());
        lineIdx++;
        contents = "";
        for (j = 0; j < numLines; j++) {
            contents += fileLines[lineIdx] + "\n";
            lineIdx++;
        }
        if (!writeFile(title, contents)) {
            restored[restored.length] = title;
        }
    }
}

// print a little wiki syntax box
function printWikiSyntax() {
    print("\t<div class=\"wikisyntax\">\n");
    print("\t<table>\n");
    print("\t\t<tr>\n");
    print("\t\t\t<td colspan=3>");
    print(wikiparse("**__Syntax__** ") + "<span class=\"optionalvalue\">(optional values)</span><br>");
    print("\t\t\t</td>\n");
    print("\t\t</tr>\n");
    print("\t\t<tr>\n");
    print("\t\t\t<td align=\"right\">");
    print("bold text: <br>");
    print("italic text: <br>");
    print("underlined text: <br>");
    print("verbatim(無効) text: <br>");
    print("link: <br>");
    if (config.SYNTAX.WIKIWORDS)
        print("wiki link: <br>");
    print("image: <br>");
    print("hex-coloured text: <br>");
    if (config.SYNTAX.HTMLCODE) {
        print("html code: <br>");
    }
    print("anchor link: <br>");
    print("\t\t\t</td>\n");
    print("\t\t\t<td>");
    print("**abc**<br>");
    print("''abc''<br>");
    print("__abc__<br>");
    print("~~~abc~~~<br>");
    print("[[url|<span class=\"optionalvalue\">description</span>|<span class=\"optionalvalue\">target</span>]]<br>");
    if (config.SYNTAX.WIKIWORDS)
        print("WikiWord<br>");
    print("{{url|<span class=\"optionalvalue\">alt</span>|<span class=\"optionalvalue\">width</span>|<span class=\"optionalvalue\">height</span>|<span class=\"optionalvalue\">align</span>|<span class=\"optionalvalue\">vertical-align</span>}}<br>");
    print("~~#AAAAAA:grey~~<br>");
    if (config.SYNTAX.HTMLCODE)
        print("%%html code%%<br>");
    print("@@name@@<br>");
    print("\t\t\t</td>\n");
    print("\t\t</tr>\n");
    print("\t</table>\n");
    print("\t</div>\n");
}

// display a wiki page
//function displayPage( title, mode, contents="" )
function displayPage(title, mode, contents) {
    //global config;
    // handle special pages 
    if (title == "PageList") {
        contents = pageList();
    } else if (title == "RestoreWiki") {
        if (!isset(config.INTERNAL.DATA.RESTORED)) {
            contents += "<b>WARNING: Restoring wiki pages will overwrite any existing pages with the same name!</b><br><br>";
            contents += "Backup File: ";
            contents += "<input type=\"hidden\" name=\"MAX_FILE_SIZE\" value=\"" + config.BACKUP.MAX_SIZE + "\" /><br>";
            contents += "<input name=\"userfile\" type=\"file\" class=\"fileupload\" size=\"32\" /><br><br>";
            contents += "Enter the password below and click <b>restore</b>+";
        }
        else {
            contents = wikiparse("Restored **" + config.INTERNAL.DATA.RESTORED.length + "** wiki pages:\n");
            for (i = 0; i < config.INTERNAL.DATA.RESTORED.length; i++) {  // fix: i< が抜けていた無限ループを修正
                page = config.INTERNAL.DATA.RESTORED[i];
                contents += wikiparse("-> [[" + page + "]]\n");
            }
        }
    } else if (title == "BackupWiki") {
        if (!anyErrors()) {
            wikiname = config.GENERAL.TITLE;
            wikiname.replaceAll(" ", "_");
            files = wikiReadDir(pageDir());
            backups = wikiReadDir(tempDir());
            contents = "Backed up " + files.length + " pages+\n\nRight-click on the link below and \"Save Link to Disk...\".\n";
        }
    } else {
        if (pageExists(title)) {
            if (!((mode == "edit") && (contents != ""))) {
                contents = wikiReadFile(pagePath(title));
            }
        }
        else {
            contents = "This is the page for " + title + "!";
            mode = "editnew";
        }
    }

    if (mode == "display") {
        print("<span class=\"wiki_body\">\n");
        print(wikiparse(contents));
        print("</span>\n");
    } else if (mode == "backupwiki") {
        print("<span class=\"wiki_body\">\n");
        print(wikiparse(contents));
        print("</span>\n");
    } else if (mode == "restorewiki") {
        print("<span class=\"wiki_body\">\n");
        print(contents);
        print("</span>\n");
    } else if (mode == "edit" || mode == "editnew") {
        print("<form action=\"" + _SERVER.SCRIPT_NAME + "?page=" + title + "\" method=\"post\">\n");
        print("<textarea name=\"contents\" cols=\"100\" rows=\"24\">" + contents + "</textarea>\n");
    }
    return mode;
}

// display the wiki controls
function displayControls(title, mode) {
    //global config;
    print("<div class=\"wiki-controls\">\n");
    if (config.GENERAL.SHOW_CONTROLS) {
        if (mode == "display") {
            if (!(isSpecial(title))) {
                print("<form id=\"wiki-edit-form\" action=\"" + _SERVER.SCRIPT_NAME + "?page=" + title + "\" method=\"post\">\n");
                if (config.MISC.REQ_PASSWORD_TEXT_IN_EDIT_BTN) {
                    print("<input name=\"mode\" value=\"edit\" type=\"hidden\" />");
                    print("<input value=\"edit ");
                    if (isLocked(title)) { print(config.LOCALE.REQ_PASSWORD); }
                    print("\" type=\"submit\" />");
                } else {
                    print("<input name=\"mode\" class=\"btn btn-sm btn-secondary\" value=\"edit\" type=\"submit\" />");
                    if (isLocked(title)) print(wikiparse(config.LOCALE.REQ_PASSWORD));
                }
                print("</form>\n");
            }
            if (title == "PageList" && config.BACKUP.ENABLE) {
                print("<form action=\"" + _SERVER.SCRIPT_NAME + "?page=" + title + "\" method=\"post\">\n");
                print("<input name=\"mode\" class=\"btn btn-sm btn-secondary\" value=\"backup\" type=\"submit\" />");
                print(" <input name=\"mode\" class=\"btn btn-sm btn-secondary\" value=\"restore\" type=\"submit\" />");
                print("</form>\n");
            }
        } else if (mode == "backupwiki") {
            if (!anyErrors()) {
                wikiname = config.GENERAL.TITLE;
                wikiname.replaceAll(" ", "_");
                files = wikiReadDir(pageDir());
                backups = wikiReadDir(tempDir());
                details = [];
                for (i = 0; i < backups.length; i++) {
                    backup = backups[i];
                    details[backup] = filedate(backup);
                }
                dcKeys = Object.keys(details);
                for (dcIdx = 0; dcIdx < dcKeys.length; dcIdx++) {
                    key = dcKeys[dcIdx];
                    statResult = file_stat(key);
                    size = (statResult != "undefined") ? eval(statResult).size : 0;
                    print(wikiparse("[[" + key + "|" + basename(key) + "]] (" + size + " bytes)\n"));
                }
            }
        } else if (mode == "restorewiki") {
            if (!isset(config.INTERNAL.DATA.RESTORED)) {
                print(wikiparse(" " + config.LOCALE.PASSWORD_TEXT));
                print("<input name=\"password\" type=\"password\" class=\"form-control d-inline-block\" style=\"width:auto\" />");
                print(" <input name=\"mode\" value=\"restorewiki\" type=\"hidden\" />\n");
                print("<input class=\"btn btn-sm btn-warning\" value=\"restore\" type=\"submit\" />\n");
            }
        } else if (mode == "edit") {
            if (isLocked(title)) {
                print(wikiparse(config.LOCALE.PASSWORD_TEXT));
                print("<input name=\"password\" type=\"password\" class=\"pass\" size=\"17\" />");
            }
            print("<input name=\"mode\" class=\"btn btn-sm btn-success\" value=\"save\" TYPE=\"submit\" />\n");
            print(" <input name=\"mode\" class=\"btn btn-sm btn-secondary\" value=\"cancel\" TYPE=\"submit\" />\n");
            print("</form>\n");
        } else if (mode == "editnew") {
            if (isLocked(title)) {
                print(wikiparse(config.LOCALE.PASSWORD_TEXT));
                print("<input name=\"password\" type=\"password\" class=\"pass\" size=\"17\" />");
            }
            print("<input name=\"mode\" class=\"btn btn-sm btn-success\" value=\"save\" type=\"submit\" />");
            print("</form>\n");
        }
    }
    print("</div>\n");
    if (mode == "restorewiki")
        print("</form>\n");
    if ((mode == "edit" || mode == "editnew") && config.SYNTAX.SHOW_BOX && title != "RestoreWiki")
        printWikiSyntax();
}
// パッチ
function mybasename(str) {
    pos = str.indexOf("/");  // fix: 引数が逆だったのを修正（str.indexOf(str, "/") → str.indexOf("/")）
    if (pos < 0) {
        return str;
    } else {
        return substr(str, pos + 1, strlen(str) - pos - 1);
    }
}
//==============
//==============
// MAIN BLOCK!
//==============
//==============

// by defining PAWFALIKI_FUNCTIONS_ONLY and including this file we can use all
// the wiki functions without actually displaying a wiki.
if (!isset(PAWFALIKI_FUNCTIONS_ONLY)) {
    if (config.GENERAL.DEBUG) {
        timeParts = microtime().split(" ");  // fix: PHP list() → 配列で受け取る
        usec = timeParts[0];
        sec = timeParts[1];
        config.GENERAL.DEBUG_STARTTIME = (float)sec + (float)usec;
    }

    // stop the page from being cached
    header("Cache-Control: no-store, no-cache, must-revalidate");

    // find out what wiki 'mode' we're in
    mode = getMode();
    format = _GET.format;
    if (format == "rss" && config.RSS.ENABLE) {
        rssFeed();
        exit();
    }

    // get the page title
    title = getTitle();

    if (mode == "backup")
        title = "BackupWiki";
    if (mode == "restore")
        title = "RestoreWiki";

    // get the page contents
    modes = {};
    modes.mode = mode;
    contents = updateWiki(modes, title, config);
    mode = modes.mode;

    // page header
    if (mode == "edit")
        htmlHeader(wikiparse(config.LOCALE.EDIT_TITLE) + title, config);
    else
        htmlHeader(title, config);

    // page contents
    // restorewiki フォームはファイルアップロードを含むため、displayPage より前に開く必要がある
    if (mode == "restorewiki" && !isset(config.INTERNAL.DATA.RESTORED))
        print("\t<form enctype=\"multipart/form-data\" action=\"" + _SERVER.SCRIPT_NAME + "?page=" + title + "\" method=\"post\">\n");
    htmlStartBlock();
    mode = displayPage(title, mode, contents);
    htmlEndBlock();

    // page controls
    displayControls(title, mode);
    if (mode == "edit") {
        htmlfooter2(wikiparse(config.LOCALE.EDIT_TITLE) + title, config, mode);
    } else {
        htmlfooter2(title, config, mode);
    }

    // page footer
    htmlFooter();
}
?>
