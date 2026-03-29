/**
 * @file Models.h
 * @brief Definição das estruturas de dados fundamentais do sistema SCOT.
 * Este ficheiro contém as estruturas (structs) que representam as entidades do domínio
 * do problema: Submissões (artigos), Revisores e Parâmetros de Configuração.
 * Sendo estruturas de dados simples (POD - Plain Old Data), a sua complexidade 
 * de acesso é O(1).
 */

#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>

/**
 * @struct Submission
 * @brief Representa um artigo científico submetido para revisão.
 */
struct Submission {
    int id;
    std::string title;
    std::string authors;
    std::string email;
    int primaryDomain;
    int secondaryDomain = -1; // -1 indica que nao tem dominio secundario
};

/**
 * @struct Reviewer
 * @brief Representa um perito (revisor) disponível para avaliar artigos.
 */
struct Reviewer {
    int id;
    std::string name;
    std::string email;
    int primaryExpertise;
    int secondaryExpertise = -1; // -1 indica que nao tem especialidade secundaria
};

/**
 * @struct ConfigParams
 * @brief Estrutura de controlo para os parâmetros de execução e algoritmos.
 * * Define os limites de carga para o grafo de fluxo e as flags de ativação
 * para as funcionalidades de atribuição e análise de risco.
 */

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