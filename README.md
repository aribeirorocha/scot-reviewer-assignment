# Scientific Conference Organization Tool (SCOT)
**Unidade Curricular:** Desenho de Algoritmos (DA) | 2º Ano, 2º Semestre
**Faculdade de Engenharia da Universidade do Porto (FEUP)**
**Ano Letivo:** 2025/2026

---

## 1. Descrição do Projeto
Este projeto consiste no desenvolvimento de uma ferramenta de apoio à organização de conferências científicas. O objetivo principal é automatizar e otimizar a atribuição de revisores a submissões de artigos (manuscritos), garantindo que:
* Cada artigo é revisto por peritos na área de especialidade correspondente.
* É respeitado o número mínimo de revisões por artigo (**MinReviewsPerSubmission**).
* É respeitada a carga máxima de trabalho de cada revisor (**MaxReviewsPerReviewer**).

A solução baseia-se na formulação do problema como uma **Rede de Fluxo Máximo (Max-Flow)**, utilizando algoritmos clássicos de grafos para encontrar a atribuição ótima e realizar análises de risco.



## 2. Estrutura do Projeto
A organização do código segue uma arquitetura modular para facilitar a manutenção e escalabilidade:

* `src/Models.h`: Definição das estruturas de dados base (`Submission`, `Reviewer`, `ConfigParams`).
* `src/Parser.h/.cpp`: Motor de leitura e processamento de ficheiros CSV com múltiplas secções.
* `src/Graph.h`, `Vertex.h`, `Edge.h`: Estrutura de grafos baseada nos materiais das aulas TP, adaptada para suportar fluxos residuais.
* `src/App.h/.cpp`: Classe gestora (Controller) que orquestra a lógica de negócio e os algoritmos.
* `src/Menu.h/.cpp`: Interface de utilizador via linha de comandos (CLI).

## 3. Algoritmos Implementados
* **Parsing e Limpeza:** Algoritmo de processamento de texto para lidar com comentários (`#`), aspas e delimitadores em CSV.
* **Edmonds-Karp (Max-Flow):** Implementação do algoritmo para resolver o problema de emparelhamento (matching) entre revisores e artigos.
* **BFS (Breadth-First Search):** Utilizado para encontrar caminhos aumentantes no grafo residual da rede de fluxo.

## 4. Compilação e Execução
O projeto utiliza o **CMake** para gestão de compilação.

### Pré-requisitos
* Compilador C++ com suporte a **C++17**.
* CMake 3.10 ou superior.

### Passo a Passo
1.  Gerar os ficheiros de build:
    ```bash
    cmake -B build
    ```
2.  Compilar o projeto:
    ```bash
    cmake --build build
    ```
3.  Executar o programa:
    * **Modo Interativo:** `./build/myProg`
    * **Modo Batch:** `./build/myProg -b data/input.csv risk_output.csv`

## 5. Documentação
A documentação técnica detalhada, incluindo diagramas de classes e análise de complexidade temporal, pode ser gerada via **Doxygen**:
```bash
doxygen Doxyfile
```

## 6.Autores
André Rocha - up202407636
<completar>
<completar>
