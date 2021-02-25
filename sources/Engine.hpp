#pragma once

#include <memory>
#include "Window.hpp"
#include "EventSystem.hpp"

class Engine
{
    private:
    public:
        static void Init(const WindowProps& props);
        static void Shutdown();

        static std::unique_ptr<Window>&         GetWindow();
        static std::unique_ptr<EventSystem>&    GetEventSystem();
};