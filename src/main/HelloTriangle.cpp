#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Settings
const unsigned int WIDTH = 800;
const unsigned int HEINGT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
"}\n\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// Función que se llama cuando se redimensiona la ventana
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	if (window == NULL) {
		std::cout << "Error. Ventana no inicializada." << std::endl;
		return;
	}

	glViewport(0, 0, width, height);
}

// Función que cierra la ventana al presionar la tecal ESC
void processInput(GLFWwindow* window) {
	if (window == NULL) {
		std::cout << "Error. Ventana no inicializada." << std::endl;
		return;
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	// Inicialización del contexto de la ventana
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con el constructor y verificación de correcta inicialización
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEINGT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "Error al inicializar GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Carga de funciones OpenGL (glad)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Error al incializar glad." << std::endl;
		return -1;
	}

	// Construcción de Shaders Program
	// Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Verificación de compilación de vertex shader
	int sucess;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	// Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Verificación de compilación de fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	// Link Shader
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Verificación de errores del linker
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKED_FAILED\n" << infolog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Asignación de vertex data (y buffer(s)) y configuración de vertex attributes
	// float vertexs[] = {
	// 	 0.5f,  0.5f, 0.0f,	// Top right
	// 	 0.5f, -0.5f, 0.0f,	// Bottom right
	// 	-0.5f, -0.5f, 0.0f, // Bottom left
	// 	-0.5f,  0.5f, 0.0f	// Top left
	// };
	
	float vertexs[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};	

	// unsigned int indexs[] = {
	// 	0, 1, 3,	// Primer triángulo
	// 	1, 2, 3		// Segundo triángulo
	// };

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Primero vincule el objeto de matriz de vértices, luego vincule y configure los búferes de 
	// vértices y, por último, configure los atributos de vértices.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Tenga en cuenta que esto está permitido, la llamada a glVertexAttribPointer registró VBO 
	// como el objeto de búfer de vértices vinculado al atributo de vértices, por lo que después 
	// podemos desvincularlo de forma segura.
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Recuerda: NO desvincule el EBO mientras un VAO esté activo, ya que el objeto búfer del 
	// elemento vinculado SE almacena en el VAO; mantenga el EBO vinculado.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Puede desvincular el VAO posteriormente para que otras llamadas VAO no modifiquen 
	// accidentalmente este VAO, pero esto rara vez ocurre. Modificar otros
	// VAO requiere una llamada a glBindVertexArray de todos modos, por lo que generalmente no 
	// desvinculamos VAO (ni VBO) cuando no es directamente necesario.

	glBindVertexArray(0);

	// Descomenta esta llamada para dibujar polígonos en modo alámbrico.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Dibujar primer triángulo
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Intercambio de buffers y todos los eventos de entrada/salida (teclas presionadas/liberadas, movimiento de mouse, etc)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Opcional: deasignar todos los recursos una vez que hayan cumplido su propósito
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// Terminar, liberando todos los recursos previamente asignados
	glfwTerminate();

	return 0;
}