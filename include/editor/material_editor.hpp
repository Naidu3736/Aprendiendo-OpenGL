#ifndef UI_TERMINAL_HPP
#define UI_TERMINAL_HPP

#pragma once

#include "engine/core/material.hpp"
#include "engine/core/light.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <string>
#include <sstream>

namespace editor {
    using engine::core::Material;
    using engine::core::Light;

    class MaterialEditor {
    private:
        Material* m_material;
        Light* m_light;
        bool m_running;

    public:
        MaterialEditor(Material& material, Light& light)
            : m_material(&material), m_light(&light), m_running(true) {}

        void run()
        {
            while (m_running) {
                system("cls");
                showHeader();
                showCurrentValues();
                showMenu();
                handleInput();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        void stop()
        {
            m_running = false;
        }

    private:
        void showHeader() const
        {
            std::cout << "=========================================" << std::endl;
            std::cout << "       EDITOR DE MATERIALES - OPENGL" << std::endl;
            std::cout << "=========================================" << std::endl;
        }

        void showCurrentValues() 
        {
            std::cout << "\n--- MATERIAL ACTUAL ---" << std::endl;
            std::cout << "Ambient:  (" << m_material->ambient.r << ", " << m_material->ambient.g << ", " << m_material->ambient.b << ")" << std::endl;
            std::cout << "Diffuse:  (" << m_material->diffuse.r << ", " << m_material->diffuse.g << ", " << m_material->diffuse.b << ")" << std::endl;
            std::cout << "Specular: (" << m_material->specular.r << ", " << m_material->specular.g << ", " << m_material->specular.b << ")" << std::endl;
            std::cout << "Shininess: " << m_material->shininess << std::endl;
            
            std::cout << "\n--- LUZ ACTUAL ---" << std::endl;
            std::cout << "Position: (" << m_light->position.x << ", " << m_light->position.y << ", " << m_light->position.z << ")" << std::endl;
            std::cout << "Ambient:  (" << m_light->ambient.r << ", " << m_light->ambient.g << ", " << m_light->ambient.b << ")" << std::endl;
            std::cout << "Diffuse:  (" << m_light->diffuse.r << ", " << m_light->diffuse.g << ", " << m_light->diffuse.b << ")" << std::endl;
            std::cout << "Specular: (" << m_light->specular.r << ", " << m_light->specular.g << ", " << m_light->specular.b << ")" << std::endl;
        }

        void showMenu() const
        {
            std::cout << "\n=== OPCIONES ===" << std::endl;
            std::cout << "1. Editar Material Manualmente" << std::endl;
            std::cout << "2. Editar Luz" << std::endl;
            std::cout << "3. Mover Luz" << std::endl;
            std::cout << "4. Reset Todo" << std::endl;
            std::cout << "5. Salir del Editor" << std::endl;
            std::cout << "\nSelecciona una opcion: ";
        }

        void handleInput()
        {
            int choice;
            std::cin >> choice;

            switch (choice) {
                case 1: editMaterial(); break;
                case 2: editLight(); break;
                case 3: moveLight(); break;
                case 4: resetAll(); break;
                case 5: m_running = false; break;
                default: std::cout << "Opcion Invalida" << std::endl;
            }
        }

        // Función helper para leer valores con preservación
        template<typename T>
        bool readValueWithDefault(const std::string& prompt, T& currentValue) {
            std::cout << prompt << " [" << currentValue << "]: ";
            
            std::string input;
            std::getline(std::cin, input);
            
            // Si la entrada está vacía, mantener el valor actual
            if (input.empty()) {
                return false;
            }
            
            // Convertir la entrada al tipo deseado
            std::istringstream stream(input);
            T newValue;
            if (stream >> newValue) {
                currentValue = newValue;
                return true;
            }
            
            // Si la conversión falla, mantener el valor actual
            std::cout << "Entrada invalida. Manteniendo valor anterior." << std::endl;
            return false;
        }

        // Función helper para leer vec3 con preservación
        bool readVec3WithDefault(const std::string& prompt, glm::vec3& currentValue) {
            std::cout << prompt << " [" << currentValue.r << " " << currentValue.g << " " << currentValue.b << "]: ";
            
            std::string input;
            std::getline(std::cin, input);
            
            // Si la entrada está vacía, mantener el valor actual
            if (input.empty()) {
                return false;
            }
            
            // Convertir la entrada a vec3
            std::istringstream stream(input);
            float r, g, b;
            if (stream >> r >> g >> b) {
                currentValue.r = r;
                currentValue.g = g;
                currentValue.b = b;
                return true;
            }
            
            // Si la conversión falla, mantener el valor actual
            std::cout << "Entrada invalida. Use formato: R G B. Manteniendo valores anteriores." << std::endl;
            return false;
        }

        void editMaterial()
        {
            system("cls");
            std::cout << "=== EDITAR MATERIAL MANUALMENTE ===" << std::endl;
            std::cout << "(Presiona Enter para mantener el valor actual)" << std::endl << std::endl;
            
            // Limpiar el buffer del cin anterior
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Leer cada valor con posibilidad de preservar
            readVec3WithDefault("Ambient (R G B)", m_material->ambient);
            readVec3WithDefault("Diffuse (R G B)", m_material->diffuse);
            readVec3WithDefault("Specular (R G B)", m_material->specular);
            readValueWithDefault("Shininess", m_material->shininess);

            // Aplicar clamp a los valores
            m_material->ambient = glm::clamp(m_material->ambient, 0.0f, 1.0f);
            m_material->diffuse = glm::clamp(m_material->diffuse, 0.0f, 1.0f);
            m_material->specular = glm::clamp(m_material->specular, 0.0f, 1.0f);
            m_material->shininess = glm::clamp(m_material->shininess, 2.0f, 256.0f);
            
            std::cout << "\nMaterial actualizado!" << std::endl;
        }

        void editLight()
        {
            system("cls");
            std::cout << "=== EDITAR LUZ ===" << std::endl;
            std::cout << "(Presiona Enter para mantener el valor actual)" << std::endl << std::endl;
            
            // Limpiar el buffer del cin anterior
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Leer cada valor con posibilidad de preservar
            readVec3WithDefault("Ambient (R G B)", m_light->ambient);
            readVec3WithDefault("Diffuse (R G B)", m_light->diffuse);
            readVec3WithDefault("Specular (R G B)", m_light->specular);

            // Aplicar clamp a los valores
            m_light->ambient = glm::clamp(m_light->ambient, 0.0f, 1.0f);
            m_light->diffuse = glm::clamp(m_light->diffuse, 0.0f, 1.0f);
            m_light->specular = glm::clamp(m_light->specular, 0.0f, 1.0f);
            
            std::cout << "\nLuz actualizada!" << std::endl;
        }

        void moveLight()
        {
            system("cls");
            std::cout << "=== MOVER LUZ ===" << std::endl;
            std::cout << "(Presiona Enter para mantener la posicion actual)" << std::endl;
            std::cout << "Posicion actual: (" << m_light->position.x << ", " << m_light->position.y <<
                         ", " << m_light->position.z << ")" << std::endl;
            
            // Limpiar el buffer del cin anterior
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            std::cout << "Nueva posicion (X Y Z): ";
            std::string input;
            std::getline(std::cin, input);
            
            // Si la entrada no está vacía, procesarla
            if (!input.empty()) {
                std::istringstream stream(input);
                float x, y, z;
                if (stream >> x >> y >> z) {
                    m_light->position.x = x;
                    m_light->position.y = y;
                    m_light->position.z = z;
                    std::cout << "Luz movida!" << std::endl;
                } else {
                    std::cout << "Entrada invalida. Use formato: X Y Z. Posicion no cambiada." << std::endl;
                }
            } else {
                std::cout << "Posicion mantenida." << std::endl;
            }
        }

        void resetAll()
        {
            // Reset material
            m_material->ambient = glm::vec3(1.0f, 0.5f, 0.31f);
            m_material->diffuse = glm::vec3(1.0f, 0.5f, 0.31f);
            m_material->specular = glm::vec3(0.5f, 0.5f, 0.5f);
            m_material->shininess = 32.0f;
            
            // Reset light
            m_light->position = glm::vec3(1.2f, 1.0f, 2.0f);
            m_light->ambient = glm::vec3(0.2f, 0.2f, 0.2f);
            m_light->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            m_light->specular = glm::vec3(1.0f, 1.0f, 1.0f);
            
            std::cout << "Todo reseteado a valores por defecto!" << std::endl;
        }
    };
}

#endif // UI_TERMINAL_HPP