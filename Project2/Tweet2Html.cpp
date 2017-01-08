/**
    Copyright (C) mygitacc50@gmail.com/

    A C++ program to convert the following features of a traditional
    Tweet into a formatted HTML for convenient display:
    
    1. Converts handles of the form @xyz to HTML anchors of the form:
       <a href="https://www.twitter.com/xxx>@xyz</a>

    2. Converts hash tags of the form #abc to HTML anchors of the form:
       <a href="https://www.twitter.com/hashtag/abc">#abc</a>

    3. Converts the following Emoticons to the image anchors shown below:

       ----------------------------------------------------------------
        Emoticon   IMG tag
       ----------------------------------------------------------------
          :-)     <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/smile.png">
          :-(     <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/frown.png">
          :-|     <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/normal.png">
          :++    <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/cpp.png">
         ----------------------------------------------------------------
*/

#include <iostream>
#include <string>
#include <fstream>

/** Convenience method to convert one or more blank spaces to HTML.

    This method encodes multiple blank spaces into "&nbsp;" HTML
    entities to ensure that entities are correctly translated.

    \param[in] spaces The sequence of spaces of be encoded.

    \return The HTML string with entities corresponding to the list of
    spaces.
*/
std::string spaces2html(std::string spaces) {
    std::string html, spc = " ";
    for (size_t i = 0; (i < spaces.size()); i++) {
        html += spc;
        spc   = "&nbsp;";
    }
    return html;
}

/** Convenience method to convert a Twitter handle to a HTML anchor
    tag.

    This is a convenience method to convert handles of the form @xyz
    to HTML anchors of the form: <a
    href="https://www.twitter.com/xxx>@xyz</a>

    \param[in] handle The handle to be converted.

    \return The HTML anchor tag corresponding to the given handle
*/
std::string handle2html(std::string handle) {
    std::string html = "<a href=\"https://www.twitter.com/";
    html += handle.substr(1) + "\">" + handle + "</a>";
    return html;
}

/** Convenience method to convert a Twitter hash tag to a HTML anchor
    tag.

    This is a convenience method to convert hash tags of the form #abc
    to HTML anchors of the form: <a
    href="https://www.twitter.com/hashtag/abc">#abc</a>

    \param[in] handle The handle to be converted.

    \return The HTML anchor tag for the given hash tag
*/
std::string tag2html(std::string tag) {
    std::string html = "<a href=\"https://www.twitter.com/hashtag/";
    html += tag.substr(1) + "\">" + tag + "</a>";
    return html;
}

/** Convenience method to convert a Twitter emoticon to image tag.

    ----------------------------------------------------------------
        Emoticon   IMG tag
       ----------------------------------------------------------------
          :-)     <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/smile.png">
          :-(     <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/frown.png">
          :-|     <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/normal.png">
          :++    <img src="http://ceclnx01.cec.miamiOH.edu/~raodm/emoticons/cpp.png">
		  
    \param[in] handle The handle to be converted.

    \return The HTML anchor tag for the given hash tag
*/
std::string emoticon2html(std::string tag) {
    // Each emoticon takes exactly 3 characters
    const std::string emoticons = ":-):-(:-|:++";
    // Each corresponding name takes exactly 7 characters
    const std::string names = "smile  frown  normal cpp";
    // Find index of emoticon/name
    int idx = emoticons.find(tag) / 3;
    // Using idx get name for the emoticon
    std::string name = names.substr(idx * 7, 7);
    // Strip out trailing blank spaces (if any)
    name = name.substr(0, name.find(' '));
    // Now use name to generate final URL
    std::string html = "<img src=\"http://ceclnx01.cec.miamiOH.edu/"
        "~raodm/hw3/";
    html += name + ".png\">";
    return html;
}

/** Method that converts tweets to html format.

    This method breaks a tweet into individual words and then
    translates each word depending on the first character of the word.
    This method preserves blank spaces between words to ensure they
    are formatted consistently.
*/
std::string tweet2html(std::string tweet) {
    std::string html = "<div class=\"tweet\">\n";  
    size_t start = 0;   // start index of a word.
    while (start < tweet.size()) {
        // Extract word or space(s) depending on character at start index
        const int wrdEnd = ((tweet[start] == ' ') ?
                            tweet.find_first_not_of(' ', start) :
                            tweet.find_first_of(' ', start));
        const std::string word = tweet.substr(start, wrdEnd - start);
        switch (word[0]) {
        case ' ': html += spaces2html(word);
            break;
        case '@': html += handle2html(word);
            break;
        case '#': html += tag2html(word);
            break;
        case ':': html += emoticon2html(word);
            break;
        default:
            html += word;
        }
        // Onto the next word
        start = wrdEnd;
    }
    // Finish the div tag
    html += "\n</div>";
    // Return the converted html back to the caller
    return html;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Specify input text file & output HTML file\n";
        return 1;
    }
    // Open input and output streams
    std::ifstream in(argv[1]);
    std::ofstream out(argv[2]);
    if (!in.good() || !out.good()) {
        std::cerr << "Error opening input or output file.\n";
        return 2;
    }
    // Print fixed HTML starting strings
    out << "<!DOCTYPE html>\n<html>\n<head>\n"
        << "<link type=\"text/css\" rel=\"stylesheet\" "
        << "href=\"http://ceclnx01.cec.miamiOH.edu/~raodm/"
        << "hw3/tweets.css\"/>\n</head>\n<body>\n";
    // Read and convert one tweet at a time.
    std::string tweet;
    while (std::getline(in, tweet)) {
        out << tweet2html(tweet) << std::endl;
    }
    // Finally wrap up the HTMl content
    out << "</body>\n</html>\n";
    return 0;
}
