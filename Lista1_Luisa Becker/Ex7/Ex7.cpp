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

// Para desenhar a espiral
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupShader();
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Código fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Códifo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 400\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 1 - 7 -- Luisa Becker", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros das funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
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

	// Gerando um buffer simples, com a geometria de uma espiral
	GLuint VAO = setupGeometry();

	// Enviando a cor desejada (vec4) para o fragment shader
	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

	glUseProgram(shaderID);

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(2);

		glBindVertexArray(VAO); //Conectando ao buffer de geometria

		// Cor vermelha para a espiral
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f); // Enviando cor para variável uniform inputColor

		// Desenha a espiral - GL_LINE_STRIP
		const int numVertices = 1000; // Número de pontos para a espiral
		glDrawArrays(GL_LINE_STRIP, 0, numVertices); // Desenhar a espiral como uma linha conectada
		
		glBindVertexArray(0); // Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede para OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// Esta função está basntante hardcoded - objetivo é compilar e "buildar" um programa de
// shader simples e único neste exemplo de código
// O código fonte do vertex e fragment shader está nos arrays vertexShaderSource e
// fragmentShader source no iniçio deste arquivo
// A função retorna o identificador do programa de shader
int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
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
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

// Esta função está bastante hardcoded - objetivo é criar os buffers que armazenam a 
// geometria de uma espiral
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO

int setupGeometry()
{
	const int numVertices = 1000;  // Definimos um número elevado de vértices para suavizar a espiral
    const float a = 0.05f;         // Constante a, define a distância inicial
    const float b = 0.1f;          // Constante b, define a taxa de crescimento da espiral
    const float maxTheta = 10.0f * M_PI;  // Máximo valor de θ, 5 voltas completas

    GLfloat vertices[numVertices * 3]; // Cada vértice terá coordenadas (x, y, z)

    for (int i = 0; i < numVertices; ++i)
    {
        float theta = maxTheta * (i / static_cast<float>(numVertices - 1)); // θ variando de 0 até maxTheta
        float r = a + b * theta; // Definindo o raio de acordo com θ

        vertices[3 * i] = r * cos(theta); // x
        vertices[3 * i + 1] = r * sin(theta); // y
        vertices[3 * i + 2] = 0.0f; // z, como estamos desenhando no plano xy
    }

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    return VAO;
}