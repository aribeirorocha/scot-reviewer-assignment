/**
 * @file Menu.cpp
 * @brief Implementação da interface de utilizador (Menu) do sistema.
 *
 * Contém a lógica para exibição de menus, tratamento de input e redirecionamento
 * de comandos para a lógica de negócio na classe App.
 */

#include "Menu.h"
#include <iostream>
#include <limits>
#include <string>

/**
 * @brief Construtor padrão do Menu.
 */
Menu::Menu() { }

/**
 * @brief Inicia o loop do menu interativo no terminal.
 * @details Complexidade Temporal: O(N * C), onde N é o número de interações do utilizador
 * e C é a complexidade da opção escolhida em cada iteração.
 */
void Menu::runInteractive() {
    bool running = true;
    while (running) {
        displayOptions();
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[ERRO] Input invalido. Por favor, insira um numero.\n" << std::endl;
            continue;
        }

        running = handleChoice(choice);
    }
    std::cout << "\nA sair... Ate logo!\n";
}

/**
 * @brief Exibe as opções textuais do menu.
 * @details Complexidade Temporal: O(1).
 */
void Menu::displayOptions() const {
    std::cout << "\n=========================================\n";
    std::cout << "  Scientific Conference Organization Tool  \n";
    std::cout << "=========================================\n";
    std::cout << "1. Carregar Dados (Parse CSV & Build Graph)\n";
    std::cout << "2. Listar Informacoes (Submissoes, Revisores e Params)\n";
    std::cout << "3. Gerar Atribuicoes de Revisao (Max Flow)\n";
    std::cout << "4. Executar Analise de Risco (K=1)\n";
    std::cout << "0. Sair\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "Escolha uma opcao: ";
}

/**
 * @brief Processa a escolha do utilizador e executa a ação correspondente.
 * @param choice Opção selecionada.
 * @return true se deve continuar, false se deve encerrar (opção 0).
 * @details Complexidade Temporal: variável conforme a opção escolhida.
 */
bool Menu::handleChoice(int choice) {
    switch (choice) {

        case 0:
            // Termina o programa
            return false;

        case 1: {
            std::string filename;
            std::cout << "\nDigite o caminho do ficheiro CSV: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::getline(std::cin, filename);
            if (filename.empty()) filename = "data/input.csv";

            if (app.loadData(filename)) {
                std::cout << "[SUCESSO] Ficheiro processado e Grafo inicializado.\n";
            } else {
                std::cout << "[ERRO] Falha ao carregar o ficheiro.\n";
            }
            break;
        }

        case 2: {
            const auto& submissions = app.getParser().getSubmissions();
            const auto& reviewers   = app.getParser().getReviewers();
            const auto& params      = app.getParser().getParams();

            if (submissions.empty()) {
                std::cout << "\n[AVISO] Nenhum dado carregado. Use a opcao 1 primeiro.\n";
                break;
            }

            std::cout << "\n--- ESTATISTICAS ---\n";
            std::cout << "Submissoes: " << submissions.size() << "\n";
            std::cout << "Revisores:  " << reviewers.size() << "\n";
            std::cout << "Vertices no Grafo: " << app.getGraph().getVertexSet().size() << "\n";

            std::cout << "\n--- PARAMETROS ---\n";
            std::cout << "MinReviewsPerSubmission: " << params.minReviewsPerSubmission << "\n";
            std::cout << "MaxReviewsPerReviewer:   " << params.maxReviewsPerReviewer << "\n";
            std::cout << "GenerateAssignments:     " << params.generateAssignments << "\n";
            std::cout << "RiskAnalysis:            " << params.riskAnalysis << "\n";
            std::cout << "OutputFileName:          " << params.outputFileName << "\n";

            std::cout << "\n--- SUBMISSOES ---\n";
            std::cout << "ID | Titulo                          | Dom.Pri | Dom.Sec\n";
            std::cout << "---+----------------------------------+---------+--------\n";
            for (const auto& s : submissions) {
                std::cout << s.id << "  | " << s.title.substr(0, 32);
                // padding
                for (int i = s.title.size(); i < 32; ++i) std::cout << ' ';
                std::cout << " | " << s.primaryDomain;
                std::cout << "       | ";
                if (s.secondaryDomain != -1) std::cout << s.secondaryDomain;
                else std::cout << "-";
                std::cout << "\n";
            }

            std::cout << "\n--- REVISORES ---\n";
            std::cout << "ID | Nome                     | Exp.Pri | Exp.Sec\n";
            std::cout << "---+--------------------------+---------+--------\n";
            for (const auto& r : reviewers) {
                std::cout << r.id << "  | " << r.name.substr(0, 24);
                for (int i = r.name.size(); i < 24; ++i) std::cout << ' ';
                std::cout << " | " << r.primaryExpertise;
                std::cout << "       | ";
                if (r.secondaryExpertise != -1) std::cout << r.secondaryExpertise;
                else std::cout << "-";
                std::cout << "\n";
            }
            break;
        }

        case 3: {
            if (app.getParser().getSubmissions().empty()) {
                std::cout << "\n[AVISO] Nenhum dado carregado. Use a opcao 1 primeiro.\n";
                break;
            }
            app.solveT21();
            break;
        }

        case 4: {
            if (app.getParser().getSubmissions().empty()) {
                std::cout << "\n[AVISO] Nenhum dado carregado. Use a opcao 1 primeiro.\n";
                break;
            }
            app.solveRisk1();
            break;
        }

        default:
            std::cout << "\n[ERRO] Opcao invalida.\n";
            break;
    }
    return true;
}

/**
 * @brief Executa o programa em modo batch.
 *
 * Lê o ficheiro de entrada, corre o Max Flow e a análise de risco conforme
 * os parâmetros do CSV, e escreve o output para o ficheiro configurado.
 * O segundo argumento (outputFile) sobrepõe o OutputFileName do CSV se fornecido.
 *
 * @param inputFile  Caminho do ficheiro CSV de entrada.
 * @param outputFile Caminho do ficheiro de saída (sobrepõe o do CSV se não vazio).
 * @details Complexidade Temporal: O(L * C + V * E^2 + R * V * E^2)
 */
void Menu::runBatch(const std::string& inputFile, const std::string& outputFile) {
    std::cout << "[Batch] A processar: " << inputFile << std::endl;

    if (!app.loadData(inputFile)) {
        std::cerr << "[Batch ERRO] Falha ao carregar o ficheiro." << std::endl;
        return;
    }

    // Se um ficheiro de saída foi passado como argumento, sobrepõe o do CSV
    if (!outputFile.empty()) {
        // Não há setter direto, mas podemos usar solveAutomatic que já usa o params
        // Para sobreposição simples: apenas informamos (a App usa outputFileName do CSV)
        std::cout << "[Batch] Ficheiro de saida: " << outputFile << std::endl;
    }

    app.solveAutomatic(outputFile);
}