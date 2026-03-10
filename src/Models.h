#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>


struct Submission {
    int id;
    std::string title;
    std::string authors;
    std::string email;
    int primaryDomain;
    int secondaryDomain = -1; // -1 indica que nao tem dominio secundario
};


struct Reviewer {
    int id;
    std::string name;
    std::string email;
    int primaryExpertise;
    int secondaryExpertise = -1; // -1 indica que nao tem especialidade secundaria
};


struct ConfigParams {
    int minReviewsPerSubmission = 0;
    int maxReviewsPerReviewer = 0;
    int primaryReviewerExpertise = 0;
    int secondaryReviewerExpertise = 0;
    int primarySubmissionDomain = 0;
    int secondarySubmissionDomain = 0;
    int generateAssignments = 0;
    int riskAnalysis = 0;
    std::string outputFileName = "output.csv";
};

#endif