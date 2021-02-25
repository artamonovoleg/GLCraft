#include <memory>
#include <iostream>
#include "Engine.hpp"

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });

    auto& keyboard = Engine::GetEventSystem()->GetKeyboard();
    auto& mouse = Engine::GetEventSystem()->GetMouse();

    while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
    {
        Engine::GetEventSystem()->Process();
        
        if (keyboard.GetKeyDown(GLFW_KEY_A))
            std::cout << "A down" << std::endl;
        if (keyboard.GetKeyUp(GLFW_KEY_D))
            std::cout << "D up" << std::endl;
        if (mouse.GetButtonDown(GLFW_MOUSE_BUTTON_LEFT))
            std::cout << "Mouse left down" << std::endl;
    }
}