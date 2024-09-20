 /* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Luisa Becker dos Santos
 * para a disciplina de Processamento Gráfico - Unisinos
 * Lista de Exercícios 1
 * Entrega em 14/09/2024
 *
 * RESPOSTAS:
 * 
 * 	1. GLSL (OpenGL Shading Language) é a linguagem de programação usada para escrever shaders na 
 *     OpenGL. É baseada em C e é utilizada para programar diretamente no pipeline gráfico da OpenGL, 
 *     permitindo controle sobre como os vértices e pixels são processados. Os dois princiais shaders são:
 *       - Vertex Shader: Processa cada vértice individualmente, transformando as coordenadas dos 
 *         vértices e aplicando cálculos relacionados a iluminação e projeção.
 *       - Fragment Shader: Processa cada fragmento (que é um candidato a pixel), calculando a cor, 
 *         textura e outros atributos que compõem o visual final do fragmento.
 * 
 * 
 *  2. Primitivas gráficas são as formas básicas usadas para compor uma cena gráfica. Elas incluem 
 *     pontos, linhas e triângulos. A partir dessas primitivas, podemos construir objetos mais complexos.
 *     O armazenamento dos vértices na OpenGL é feito em buffers como o Vertex Buffer Object (VBO), que 
 *     armazena os dados dos vértices (coordenadas, cores, normais, etc.). Esses vértices são então organizados 
 *     em arrays e enviados para a GPU para serem processados pelos shaders.
 * 
 * 
 *  3. VBO (Vertex Buffer Object): Um buffer que armazena os dados dos vértices (posição, cor, normais, etc.) 
 *     na memória da GPU. Ele é utilizado para enviar grandes quantidades de vértices para serem processados pela GPU.
 *     VAO (Vertex Array Object): Um objeto que armazena a configuração de como os vértices são lidos a partir dos VBOs,
 *     incluindo quais atributos do vértice (posição, cor, etc.) estão presentes, seus offsets e layouts. O VAO permite 
 *     que a configuração dos VBOs seja vinculada de forma fácil e reutilizada durante a renderização. 
 *     EBO (Element Buffer Object): Um buffer que armazena índices de vértices, permitindo a reutilização de vértices para
 *     formar primitivas gráficas (como triângulos). O EBO é usado em conjunto com VBOs para evitar a duplicação de dados 
 *     dos vértices.
 *     - Relação entre eles:
 *       [VAO] - Armazena configurações de atributos (posição, cor, etc.)
 *         |
 *         +-- [VBO] - Armazena os dados dos vértices (coordenadas, cor, etc.)
 *         |
 *         +-- [EBO] - Armazena os índices dos vértices para formar primitivas
 * 
 * 
 *  4. Shaders: O código define e utiliza vertex shaders e fragment shaders. O vertex shader processa os vértices 
 *     (posição, cor, etc.), enquanto o fragment shader processa a cor final de cada fragmento (pixel). Esses shaders 
 *     são programados em GLSL e compilados durante o tempo de execução.
 *     
 *     VBO (Vertex Buffer Object): No código, o VBO é utilizado para armazenar os dados dos vértices (coordenadas e 
 *     cores) e enviar essas informações para a GPU. O VBO permite que grandes conjuntos de vértices sejam processados 
 *     eficientemente. 
 * 
 *     VAO (Vertex Array Object): O VAO é configurado para armazenar as instruções de como a GPU deve interpretar os 
 *     dados contidos no VBO. Ele vincula o VBO e define o layout dos atributos dos vértices (ex. posição e cor) para 
 *     os shaders. Relação: O VAO armazena a configuração necessária para a GPU interpretar o VBO corretamente, enquanto os 
 *     shaders são responsáveis por processar esses dados e determinar a posição e a cor final de cada vértice e pixel.
 * 
 */

