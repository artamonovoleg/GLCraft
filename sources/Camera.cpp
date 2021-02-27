#include "Camera.hpp"
#include "Engine.hpp"

void Camera::Update(float deltaTime)
{
    const auto& kb = Engine::GetEventSystem()->GetKeyboard();
    const auto& mouse = Engine::GetEventSystem()->GetMouse();

    if (kb.GetKey(GLFW_KEY_W))
        ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (kb.GetKey(GLFW_KEY_S))
        ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (kb.GetKey(GLFW_KEY_A))
        ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (kb.GetKey(GLFW_KEY_D))
        ProcessKeyboard(CameraMovement::RIGHT, deltaTime);


    static float lastX = 400;
    static float lastY = 300;
    static bool firstMouse = true;

    if (firstMouse)
    {
        lastX = mouse.GetPosition().x;
        lastY = mouse.GetPosition().y;
        firstMouse = false;
    }

    float xoffset = mouse.GetPosition().x - lastX;
    float yoffset = lastY - mouse.GetPosition().y;

    lastX = mouse.GetPosition().x;
    lastY = mouse.GetPosition().y;

    ProcessMouseMovement(xoffset, yoffset);
}