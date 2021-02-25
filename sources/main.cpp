#include <memory>
#include <iostream>
#include "Engine.hpp"

int main()
{
    Engine::Init({ 800, 600, "Minecraft" });

    auto& keyboard = Engine::GetEventSystem()->GetKeyboard();

    while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
    {
        Engine::GetEventSystem()->Process();
        if (keyboard.GetKeyDown(GLFW_KEY_A))
            std::cout << "E" << std::endl;
    }
}