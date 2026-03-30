# SCOT: Documentação Técnica de Engenharia (`src`)

Este diretório contém a implementação do motor algorítmico para o **Scientific Conference Organization Tool**. O foco desta documentação é detalhar a resolução das tarefas de alta complexidade **T2.3** e **T2.4** através de teoria de grafos avançada.

---

## 🔬 T2.3: Análise de Risco Multi-Agente ($K > 1$)

Embora a implementação base cubra o cenário de falha unitária ($K=1$) em `solveRisk1()`, a arquitetura foi desenhada para suportar a fundamentação teórica de falhas múltiplas.

### 1. Formulação Algorítmica e Estratégia de Procura
A abordagem para $K > 1$ utiliza um modelo de **Teste de Stress Combinatório**:
* **Identificação de Gargalos (Bottlenecks):** O sistema isola os "Revisores Ativos" (aqueles com fluxo $> 0$ na solução ótima inicial). Revisores com grau de saída zero no matching atual não impactam o corte mínimo imediato, sendo descartados para otimização do espaço de busca.
* **Explosão Combinatória:** O número de subproblemas de Max-Flow a resolver é definido por $\binom{R}{K}$. Para cada combinação, o sistema invalida os nós correspondentes no grafo residual e executa uma nova pesquisa de caminhos aumentantes (BFS).

### 2. Complexidade e Intratabilidade
* **Complexidade Assintótica:** $O\left(\binom{R}{K} \cdot V \cdot E^2\right)$.
* **Análise Crítica:** O impacto de $K > 1$ não é linear; a perda de dois revisores de domínios distintos pode ser irrelevante, enquanto a perda de dois especialistas no mesmo domínio pode atingir o **Corte Mínimo (Min-Cut)** daquele sub-grafo, tornando a atribuição impossível mesmo que existam outros revisores livres noutras áreas.

---

## 🧠 T2.4: General Formulation (Domínios Secundários)

A tarefa T2.4 expande a rede bipartida para um modelo de **Afinidade Multicamada**. Esta lógica está implementada no método `App::runFlow` através do `mode 3`.

### 1. Construção Dinâmica da Rede de Afinidade
Diferente da formulação básica, o sistema avalia quatro tipos de arcos de compatibilidade entre Submissões ($S_i$) e Revisores ($R_j$):
1.  **Match Perfeito:** $S_{primário} \equiv R_{primário}$
2.  **Apoio Especializado:** $S_{secundário} \equiv R_{primário}$
3.  **Extensão de Domínio:** $S_{primário} \equiv R_{secundário}$
4.  **Match de Recurso:** $S_{secundário} \equiv R_{secundário}$

### 2. Otimização: Do Max-Flow ao Min-Cost Max-Flow
Na implementação atual (Edmonds-Karp), todos os arcos acima têm capacidade unitária, focando-se na **viabilidade** da atribuição. 
* **Proposta de Engenharia:** Para atingir a "Atribuição Ótima" de qualidade, a infraestrutura está preparada para evoluir para **Min-Cost Max-Flow**. 
* **Modelo de Custos:** Ao atribuir custos diferenciais (ex: Primário-Primário = 0, Secundário-Secundário = 50), o algoritmo encontraria primeiro os caminhos de maior afinidade antes de recorrer a matches secundários, maximizando a qualidade científica da conferência.

---

## 🛠️ Detalhes de Implementação em `src`

* **`Graph.h`**: Implementação de **Edmonds-Karp** com complexidade $O(V \cdot E^2)$. O uso de `addBidirectionalEdge` garante que cada arco de atribuição tenha o seu correspondente residual para permitir o cancelamento de fluxo.
* **`App.cpp`**: Gere a construção do grafo garantindo que os IDs de submissões e revisores não colidam através de um mapeamento de *offset* (IDs negativos para revisores), criando um espaço de estados único no grafo.
* **`Parser.cpp`**: Valida a integridade dos dados, rejeitando IDs duplicados que poderiam corromper a estrutura da rede de fluxo.
* **`Menu.cpp`**: Implementa o modo batch (`-b`) para integração com scripts de avaliação, permitindo o redirecionamento flexível de ficheiros de output.

---
**Nota de Desempenho:** Todo o código foi documentado via **Doxygen**, incluindo as anotações de complexidade temporal para cada operação crítica, cumprindo os rigorosos requisitos de documentação técnica.