#ifndef RENDER_HPP
#define RENDER_HPP

struct GLFWwindow;
class Shader;

void init_renderer(Shader* shader);

void render(GLFWwindow* window, Shader* shader);

#endif // RENDER_HPP
