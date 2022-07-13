#include "LineLevelParser.h"

#include <iostream>

namespace MiniPython {

Lines stringToLines(const std::string &data) {
    Lines result;

    std::string current;
    for (const auto& ch: data) {
        if (ch == '\n') {
            result.push_back(current);
            current = {};
        }
        else {
            current += ch;
        }
    }

    result.push_back(current);

    return result;
}

Lines processLineContinuation(const Lines &lines) {
    Lines result;

    std::string prev_line;
    for (const auto& line: lines) {
        // line expects continuation
        if (line.size() > 0 && line[line.size() - 1] == '\\') {
            prev_line += line;
            prev_line.pop_back(); // remove trailing backslash
        }
        // normal line / continuation of previous line
        else {
            result.push_back(prev_line + line);
            prev_line = {};
        }
    }

    // last line expects continuation
    if (prev_line.size() > 0) {
        std::cerr << "Last line expects continuation (ends with '\\')\n";
        result.push_back(prev_line);
    }

    return result;
}

bool lineHasCode(const std::string &line) {
    for (const auto& ch: line) {
        if (ch == ' ' || ch == '\t') {
            continue;
        }
        else if (ch == '#') {
            return false;
        }
        else {
            return true;
        }
    }
    return false;
}

Lines removeLinesWithoutCode(const Lines &lines) {
    Lines result;

    for (const auto& line: lines) {
        if (lineHasCode(line)) {
            result.push_back(line);
        }
    }

    return result;
}

static bool isSubIndendation(const std::string &indentation, const std::string &parent_indentation) {
    if (indentation == parent_indentation) {
        return false;
    }

    if (indentation.rfind(parent_indentation, 0) == 0) {
        return true;
    }

    if (parent_indentation.rfind(indentation, 0) == 0) {
        return false;
    }

    throw std::runtime_error("Inconsistent indentation");
}

static std::pair<std::string, std::string> splitLineIntoIndentationAndContent(const std::string &str) {
    std::string indentation;
    std::string content;

    bool end_of_indent_found = false;

    for (char ch : str) {
        if (end_of_indent_found) {
            content += ch;
            continue;
        }

        if (ch == ' ' || ch == '\t') {
            indentation += ch;
            continue;
        }

        end_of_indent_found = true;
        content += ch;
    }

    return std::make_pair(indentation, content);
}

LineTree::LineTree(const Lines &lines) {
    Lines::const_iterator curr = lines.begin();
    while (curr < lines.end()) {
        children.push_back(std::make_shared<LineTree>(curr, lines.end(), ""));
    }
}

LineTree::LineTree(Lines::const_iterator &curr, const Lines::const_iterator &end, const std::string &parent_indentation) {
    auto pair = splitLineIntoIndentationAndContent(*curr);
    indentation = pair.first;
    value = pair.second;

    while (++curr < end) {
        auto ind_cont = splitLineIntoIndentationAndContent(*curr);
        auto child_indentation = ind_cont.first;
        if (isSubIndendation(child_indentation, indentation)) {
            children.push_back(std::make_shared<LineTree>(curr, end, indentation));
            curr--;
        }
        else {
            return;
        }
    }
}

} // namespace MiniPython
