/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Luisa Becker dos Santos
 * para a disciplina de Processamento Gráfico - Unisinos
 * Lista de Exercícios 1
 * Entrega em 14/09/2024
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
void setupHouse(GLuint &VAO1, GLuint &VAO2, GLuint &VAO3, GLuint &VAO4, GLuint &VAO5, GLuint &VAOGrid);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 400 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"out vec3 vertexColor;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(position, 1.0);\n"
"    vertexColor = color;\n"
"}\0";

// Código fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 400 core\n"
"in vec3 vertexColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"    color = vec4(vertexColor, 1.0);\n"
"}\n\0";

// Função MAIN
int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 1 - 9 -- Luisa Becker", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros das funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Obtendo as informações de versão
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Compilando e buildando o programa de shader
    GLuint shaderID = setupShader();

    // Gerar e configurar os VAOs
    GLuint VAO1, VAO2, VAO3, VAO4, VAO5, VAOGrid;
    setupHouse(VAO1, VAO2, VAO3, VAO4, VAO5, VAOGrid);

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Cor de fundo (branco)
        glClear(GL_COLOR_BUFFER_BIT);

        // Usando o shader program
        glUseProgram(shaderID);

        // Desenha a grade quadriculada
        glBindVertexArray(VAOGrid);
        glDrawArrays(GL_LINES, 0, 44); // Desenha as linhas da grade

        // Desenha a casa
        glBindVertexArray(VAO1); // Desenho do telhado
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO2); // Desenho das paredes
        glDrawArrays(GL_LINES, 0, 4);

        glBindVertexArray(VAO3); // Desenho da porta
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glBindVertexArray(VAO4); // Desenho da janela
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_LINES, 4, 8); // Cruz da janela

        glBindVertexArray(VAO5); // Desenho da base
        glDrawArrays(GL_LINES, 0, 2);

        // Desvincula o VAO
        glBindVertexArray(0);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }

    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

// Função de callback de teclado - É chamada sempre que uma tecla for pressionada ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// Função que compila e linka o Vertex e Fragment Shader em um Shader Program
int setupShader()
{
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Checando erros de compilação
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Checando erros de compilação
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Linkando os shaders e criando o programa de shader
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Checando por erros de linkagem
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Limpando os shaders, já que não são mais necessários após o linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// Função para configurar os VAOs da casa e a grade quadriculada
void setupHouse(GLuint &VAO1, GLuint &VAO2, GLuint &VAO3, GLuint &VAO4, GLuint &VAO5, GLuint &VAOGrid)
{
    // Definição dos vértices e cores para cada parte da casa

    // Telhado (vermelho)
    GLfloat roof[] = {
        0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // Ponto superior
       -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // Inferior esquerdo
        0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f   // Inferior direito
    };

    // Paredes (preto)
    GLfloat walls[] = {
       -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    // Porta (marrom)
    GLfloat door[] = {
       -0.15f, -0.5f, 0.0f, 0.5f, 0.25f, 0.0f,
       -0.15f, -0.3f, 0.0f, 0.5f, 0.25f, 0.0f,
        0.15f, -0.5f, 0.0f, 0.5f, 0.25f, 0.0f,
        0.15f, -0.3f, 0.0f, 0.5f, 0.25f, 0.0f
    };

    // Janela (amarelo e cruz preta)
    GLfloat window[] = {
       -0.25f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Inferior esquerdo
       -0.25f,  0.2f, 0.0f, 1.0f, 1.0f, 0.0f,  // Superior esquerdo
        0.05f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // Inferior direito
        0.05f,  0.2f, 0.0f, 1.0f, 1.0f, 0.0f,  // Superior direito
       -0.25f,  0.1f, 0.0f, 0.0f, 0.0f, 0.0f,  // Linha horizontal
        0.05f,  0.1f, 0.0f, 0.0f, 0.0f, 0.0f,
       -0.10f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // Linha vertical
       -0.10f,  0.2f, 0.0f, 0.0f, 0.0f, 0.0f
    };

    // Base (laranja)
    GLfloat base[] = {
       -1.0f, -0.5f, 0.0f, 1.0f, 0.5f, 0.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 0.5f, 0.0f
    };

    // Configuração da grade
    GLfloat gridLines[44 * 6]; // 44 linhas, 6 componentes (x, y, z, r, g, b)
    int index = 0;
    float step = 0.1f;
    for (float x = -1.0f; x <= 1.0f; x += step) {
        gridLines[index++] = x;
        gridLines[index++] = -1.0f;
        gridLines[index++] = 0.0f;
        gridLines[index++] = 0.9f; // Cinza claro
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = x;
        gridLines[index++] = 1.0f;
        gridLines[index++] = 0.0f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
    }
    for (float y = -1.0f; y <= 1.0f; y += step) {
        gridLines[index++] = -1.0f;
        gridLines[index++] = y;
        gridLines[index++] = 0.0f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 1.0f;
        gridLines[index++] = y;
        gridLines[index++] = 0.0f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
        gridLines[index++] = 0.9f;
    }

    // Gerar buffers e configurar VAOs para cada parte
    GLuint VBO1, VBO2, VBO3, VBO4, VBO5, VBOGrid;

    // Telhado
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roof), roof, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Paredes
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(walls), walls, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Porta
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(door), door, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Janela
    glGenVertexArrays(1, &VAO4);
    glGenBuffers(1, &VBO4);
    glBindVertexArray(VAO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(window), window, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Base
    glGenVertexArrays(1, &VAO5);
    glGenBuffers(1, &VBO5);
    glBindVertexArray(VAO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(base), base, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Grade quadriculada
    glGenVertexArrays(1, &VAOGrid);
    glGenBuffers(1, &VBOGrid);
    glBindVertexArray(VAOGrid);
    glBindBuffer(GL_ARRAY_BUFFER, VBOGrid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gridLines), gridLines, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
}
