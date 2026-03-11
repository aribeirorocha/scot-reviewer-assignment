/**
 * @file Menu.cpp
 * @brief Implementação da interface de utilizador (Menu) do sistema.
 * * Contém a lógica para exibição de menus, tratamento de input e redirecionamento
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
 * * @details Complexidade Temporal: O(N * C), onde N é o número de interações do utilizador
 * e C é a complexidade da opção escolhida em cada iteração.
 */

void Menu::runInteractive() {
    bool running = true;
    while (running) {
        displayOptions();
        int choice;
        std::cin >> choice;

        // Tratamento de erros: se o utilizador inserir um caracter em vez de número
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\n[ERRO] Input invalido. Por favor, insira um numero.\n" << std::endl;
            continue;
        }

        running = handleChoice(choice);
    }
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
    std::cout << "3. Gerar Atribuicoes de Revisao (T2.1 - Max Flow)\n";
    std::cout << "4. Executar Analise de Risco (T2.2/T2.3)\n";
    std::cout << "0. Sair\n";
    std::cout << "-----------------------------------------\n";
    std::cout << "Escolha uma opcao: ";
}
/**
 * @brief Processa a escolha do utilizador e executa a ação correspondente.
 * * @param choice Inteiro que representa a opção selecionada.
 * @return true se o programa deve continuar, false se deve encerrar.
 * * @details Complexidade Temporal: Depende da opção:
 * - Caso 1: O(L * C + S + R) - Leitura de ficheiro e setup do grafo.
 * - Caso 2: O(S) - Listagem de submissões.
 * - Outros casos: Atualmente O(1).
 */


bool Menu::handleChoice(int choice) {
    switch (choice) {
        case 1: {
            std::string filename;
            std::cout << "\nDigite o caminho do ficheiro (ex: data/input.csv) ou Enter para padrao: ";
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
            std::getline(std::cin, filename);
            
            if (filename.empty()) filename = "data/input.csv";

            // Chama a App para carregar os dados e montar o grafo inicial (T1.2)
            if (app.loadData(filename)) {
                std::cout << "[SUCESSO] Ficheiro processado e Grafo inicializado.\n";
            }
            break;
        }
        case 2: {
            // Verifica se já existem dados carregados no parser da App
            const auto& submissions = app.getParser().getSubmissions();
            const auto& reviewers = app.getParser().getReviewers();
            const auto& params = app.getParser().getParams();

            if (submissions.empty()) {
                std::cout << "\n[AVISO] Nenhum dado carregado. Use a opcao 1 primeiro.\n";
                break;
            }
            
            std::cout << "\n--- ESTATISTICAS DOS DADOS ---\n";
            std::cout << "Submissoes: " << submissions.size() << "\n";
            std::cout << "Revisores:  " << reviewers.size() << "\n";
            std::cout << "Vertices no Grafo: " << app.getGraph().getVertexSet().size() << "\n";
            
            std::cout << "\n--- PARAMETROS CONFIGURADOS ---\n";
            std::cout << "Min Reviews/Submissao: " << params.minReviewsPerSubmission << "\n";
            std::cout << "Max Reviews/Revisor:   " << params.maxReviewsPerReviewer << "\n";
            std::cout << "Modo de Atribuicao:    " << params.generateAssignments << "\n";
            std::cout << "Ficheiro de Saida:     " << params.outputFileName << "\n";
            
            // Lista os IDs das submissões (exemplo de "request list of submissions")
            std::cout << "\nIDs das Submissoes: ";
            for(const auto& s : submissions) std::cout << s.id << " ";
            std::cout << "\n";
            break;
        }
        case 3:
            std::cout << "\n[T2.1] A calcular Max Flow... (A ser implementado)\n";
            break;
        case 4:
            std::cout << "\n[T2.2] A executar Analise de Risco... (A ser implementado)\n";
            break;
        case 0:
            std::cout << "\nA encerrar o programa. Ate breve!\n";
            return false;
        default:
            std::cerr << "\n[ERRO] Opcao invalida. Tente novamente.\n";
            break;
    }
    return true;
}

/**
 * @brief Executa o processamento automático sem intervenção do utilizador.
 * * @param inputFile Caminho do ficheiro CSV de entrada.
 * @param riskFile Caminho do ficheiro de saída para análise de risco (opcional).
 * * @details Complexidade Temporal: O(L * C + S + R) para carregar os dados.
 */


void Menu::runBatch(const std::string& inputFile, const std::string& riskFile) {
    std::cout << "[Batch Mode] A iniciar processamento de: " << inputFile << std::endl;
    
    if (app.loadData(inputFile)) {
        std::cout << "[Batch Mode] Dados carregados. Gerando outputs...\n";
        // Futuramente: app.executeMaxFlow();
    } else {
        std::cerr << "[Batch Mode ERRO] Falha ao processar o ficheiro." << std::endl;
    }
}