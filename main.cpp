#include <memory>
#include "Engine.hpp"

int main()
{

    Engine::Init({ 800, 600, "Minecraft" });

    auto& keyboard = Engine::GetEventSystem()->GetKeyboard();

    while (!keyboard.GetKey(GLFW_KEY_ESCAPE))
    {
        Engine::GetEventSystem()->Process();
    }
}