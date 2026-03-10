#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "Models.h"

class Parser {
public:
    bool parseFile(const std::string& filename);

    const std::vector<Submission>& getSubmissions() const { return submissions; }
    const std::vector<Reviewer>& getReviewers() const { return reviewers; }
    const ConfigParams& getParams() const { return params; }

private:
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    ConfigParams params;

    // Funções auxiliares para limpar as strings
    std::string trim(const std::string& str);
    std::string removeQuotes(const std::string& str);
};

#endif 