/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Luisa Becker dos Santos
 * para a disciplina de Processamento Gráfico - Unisinos
 * Lista de Exercícios 1
 * Entrega em 14/09/2024
 *
 * RESPOSTAS:
 * 
 * 	a) O VBO armazena os vértices e as cores dos pontos P1, P2 e P3. Como cada vértice possui uma posição 
 * em 2D (ou 3D se necessário) e uma cor, nosso VBO conterá esses dados intercalados. O VAO armazena as 
 * configurações necessárias para habilitar os atributos do VBO. Ele será responsável por armazenar as 
 * instruções para os atributos de posição e cor. Como o triângulo tem apenas três vértices, o uso de 
 * um EBO é opcional, no entanto, podemos aplicá-lo usando índices para economizar espaço ou reutilizar vértices.
 * 
 * 	b) Os atributos são passados para o vertex shader usando a palavra-chave layout. O layout location define 
 * a correspondência entre os atributos no shader e os atributos no buffer. O fragment shader receberia a cor 
 * do vértice e a aplicaria no fragmento. 
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
int setupGeometry();

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 1 - 8 -- Luisa Becker", nullptr, nullptr);
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

    // Gerando um buffer simples, com a geometria de um triângulo
    GLuint VAO = setupGeometry();

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
        glClear(GL_COLOR_BUFFER_BIT);

        // Usando o shader program
        glUseProgram(shaderID);

        // Bind do VAO (contém a geometria do triângulo)
        glBindVertexArray(VAO);

        // Chamada de desenho - drawcall
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Desvincula o VAO
        glBindVertexArray(0);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }

    // Pede para OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO);

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

// Função para configurar o VBO e VAO
int setupGeometry()
{
    // Definição dos vértices com suas posições e cores
    GLfloat vertices[] = {
        // Posições        // Cores
        0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // P1 (Vermelho)
       -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // P2 (Verde)
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f   // P3 (Azul)
    };

    GLuint VAO, VBO;

    // Gera os buffers e o VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Bind do VAO
    glBindVertexArray(VAO);

    // Bind do VBO e configuração do buffer de vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Atributo de posição (layout = 0 no vertex shader)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Atributo de cor (layout = 1 no vertex shader)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Desvincula o VBO e VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}