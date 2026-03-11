/**
 * @file Parser.h
 * @brief Definição da classe Parser para processamento de ficheiros de configuração.
 * * Esta classe é responsável por ler, validar e armazenar os dados contidos
 * nos ficheiros CSV, organizando-os em estruturas utilizáveis pelo resto do sistema.
 */
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "Models.h"

/**
 * @class Parser
 * @brief Classe que converte ficheiros de texto CSV em objetos do domínio.
 * * Gere o armazenamento temporário de submissões, revisores e parâmetros de 
 * configuração lidos durante a execução.
 */
class Parser {
public:
    /**
     * @brief Lê e processa o conteúdo de um ficheiro CSV.
     * @param filename Caminho para o ficheiro a ser lido.
     * @return true se o processamento foi concluído sem erros fatais, false caso contrário.
     * @details Complexidade Temporal: O(L * C), onde L é o número de linhas e C o número médio de caracteres.
     */
    bool parseFile(const std::string& filename);
    /**
     * @brief Retorna a lista de submissões carregadas.
     * @return Referência constante para o vetor de Submission.
     * @details Complexidade Temporal: O(1).
     */
    const std::vector<Submission>& getSubmissions() const { return submissions; }
    /**
     * @brief Retorna a lista de revisores carregados.
     * @return Referência constante para o vetor de Reviewer.
     * @details Complexidade Temporal: O(1).
     */
    const std::vector<Reviewer>& getReviewers() const { return reviewers; }
    /**
     * @brief Retorna os parâmetros de configuração e controlo.
     * @return Referência constante para a estrutura ConfigParams.
     * @details Complexidade Temporal: O(1).
     */
    const ConfigParams& getParams() const { return params; }

private:
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    ConfigParams params;

    /**
     * @brief Remove espaços em branco e caracteres de controlo das extremidades.
     * @param str String a limpar.
     * @return String limpa.
     * @details Complexidade Temporal: O(N).
     */
    std::string trim(const std::string& str);
    /**
     * @brief Remove aspas duplas envolventes de campos de texto.
     * @param str String a processar.
     * @return String sem aspas.
     * @details Complexidade Temporal: O(N).
     */
    std::string removeQuotes(const std::string& str);
};

#endif 