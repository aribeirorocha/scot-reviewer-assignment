/**
 * @file Parser.cpp
 * @brief Implementação da classe Parser responsável pelo processamento de ficheiros CSV.
 *
 * Contém a lógica para leitura linha-a-linha, filtragem de comentários e
 * conversão de strings para as estruturas de dados do domínio (Submissões, Revisores).
 */
#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <stdexcept>

/**
 * @brief Remove espaços em branco, tabulações e quebras de linha das extremidades de uma string.
 * @param str String original a ser limpa.
 * @return std::string String resultante sem espaços nas extremidades.
 * @details Complexidade Temporal: O(N), onde N é o comprimento da string.
 */
std::string Parser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * @brief Remove aspas duplas envolventes de uma string, se existirem.
 * @param str String a processar.
 * @return std::string String sem aspas.
 * @details Complexidade Temporal: O(N), onde N é o comprimento da string.
 */
std::string Parser::removeQuotes(const std::string& str) {
    std::string s = trim(str);
    if (s.length() >= 2 && s.front() == '"' && s.back() == '"') {
        return s.substr(1, s.length() - 2);
    }
    return s;
}

/**
 * @brief Motor principal de parsing que lê o ficheiro CSV e popula as listas de dados.
 *
 * Utiliza uma máquina de estados baseada em currentSection para distinguir
 * blocos de Submissões, Revisores, Parâmetros e Controlo.
 * Deteta IDs duplicados e rejeita o ficheiro com mensagem de erro.
 *
 * @param filename Caminho para o ficheiro CSV.
 * @return true Se o ficheiro foi processado com sucesso, false caso contrário.
 * @details Complexidade Temporal: O(L * C), onde L é o número de linhas e C o número médio de colunas.
 */
bool Parser::parseFile(const std::string& filename) {
    submissions.clear();
    reviewers.clear();
    params = ConfigParams(); // reset params

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Erro] Nao foi possivel abrir o ficheiro: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::string currentSection = "";
    std::set<int> subIds, revIds;
    bool hasError = false;

    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Detectar secções pelo '#'
        if (line[0] == '#') {
            if (line.find("Submissions") != std::string::npos) currentSection = "Submissions";
            else if (line.find("Reviewers") != std::string::npos) currentSection = "Reviewers";
            else if (line.find("Parameters") != std::string::npos) currentSection = "Parameters";
            else if (line.find("Control") != std::string::npos) currentSection = "Control";
            continue;
        }

        // Separar campos por vírgula, respeitando '#' como início de comentário inline
        std::vector<std::string> tokens;
        {
            std::stringstream ss(line);
            std::string token;
            while (std::getline(ss, token, ',')) {
                size_t hashPos = token.find('#');
                if (hashPos != std::string::npos) {
                    tokens.push_back(trim(token.substr(0, hashPos)));
                    break;
                }
                tokens.push_back(trim(token));
            }
        }

        try {
            if (currentSection == "Submissions" && tokens.size() >= 5) {
                Submission sub;
                sub.id = std::stoi(tokens[0]);
                sub.title = removeQuotes(tokens[1]);
                sub.authors = trim(tokens[2]);
                sub.email = trim(tokens[3]);
                sub.primaryDomain = std::stoi(tokens[4]);
                sub.secondaryDomain = -1;
                if (tokens.size() > 5 && !tokens[5].empty()) {
                    sub.secondaryDomain = std::stoi(tokens[5]);
                }
                // Validação de ID duplicado
                if (subIds.count(sub.id)) {
                    std::cerr << "[Erro] ID de submissao duplicado: " << sub.id << std::endl;
                    hasError = true;
                } else {
                    subIds.insert(sub.id);
                    submissions.push_back(sub);
                }
            }
            else if (currentSection == "Reviewers" && tokens.size() >= 4) {
                Reviewer rev;
                rev.id = std::stoi(tokens[0]);
                rev.name = trim(tokens[1]);
                rev.email = trim(tokens[2]);
                rev.primaryExpertise = std::stoi(tokens[3]);
                rev.secondaryExpertise = -1;
                if (tokens.size() > 4 && !tokens[4].empty()) {
                    rev.secondaryExpertise = std::stoi(tokens[4]);
                }
                // Validação de ID duplicado
                if (revIds.count(rev.id)) {
                    std::cerr << "[Erro] ID de revisor duplicado: " << rev.id << std::endl;
                    hasError = true;
                } else {
                    revIds.insert(rev.id);
                    reviewers.push_back(rev);
                }
            }
            else if (currentSection == "Parameters" || currentSection == "Control") {
                if (tokens.size() >= 2) {
                    std::string key = trim(tokens[0]);
                    std::string value = removeQuotes(tokens[1]);

                    if      (key == "MinReviewsPerSubmission")   params.minReviewsPerSubmission = std::stoi(value);
                    else if (key == "MaxReviewsPerReviewer")     params.maxReviewsPerReviewer = std::stoi(value);
                    else if (key == "PrimaryReviewerExpertise")  params.primaryReviewerExpertise = std::stoi(value);
                    else if (key == "SecondaryReviewerExpertise")params.secondaryReviewerExpertise = std::stoi(value);
                    else if (key == "PrimarySubmissionDomain")   params.primarySubmissionDomain = std::stoi(value);
                    else if (key == "SecondarySubmissionDomain") params.secondarySubmissionDomain = std::stoi(value);
                    else if (key == "GenerateAssignments")       params.generateAssignments = std::stoi(value);
                    else if (key == "RiskAnalysis")              params.riskAnalysis = std::stoi(value);
                    else if (key == "OutputFileName")            params.outputFileName = value;
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "[Aviso] Erro a converter dado na linha: " << line << std::endl;
        }
    }

    file.close();

    if (hasError) {
        std::cerr << "[Erro] Ficheiro rejeitado devido a IDs duplicados." << std::endl;
        return false;
    }

    return true;
}