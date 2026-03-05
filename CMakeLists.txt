cmake_minimum_required(VERSION 3.20)
project(ScientificConferenceTool)

# 1. Definir a versão do C++ (C++17 é o padrão moderno e estável)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Procurar automaticamente todos os ficheiros .cpp e .h na pasta src
# Isto evita que tenhas de editar este ficheiro cada vez que crias uma classe nova
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# 3. Incluir a pasta src para que o compilador encontre os teus headers (.h)
include_directories(src)

# 4. Criar o executável
add_executable(myProg ${SOURCES} ${HEADERS})

# 5. (Opcional) Se usares bibliotecas extra no futuro, adicionas aqui