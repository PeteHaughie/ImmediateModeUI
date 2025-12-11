#pragma once

#include "ofMain.h"
#include <vector>
#include <string>
#include <functional>
#include <unordered_map>

namespace ui {

struct Button {
    glm::vec2 position{0,0}; // required
    glm::vec2 size{0,0}; // required
    float rotation = 0.0f; // degrees // optional
    std::string type = "push"; // e.g., "push", "toggle" // optional - default is push
    std::string shape = "rectangle"; // "rectangle", "ellipse", "polygon" // optional - default is rectangle
    std::vector<glm::vec2> points; // for polygon (relative coords) // optional
    std::string state = "off"; // optional - default is off
    bool hovered = false; // optional - default is false
    std::string label; // optional label for the button
    std::function<void()> action; // optional action callback
    std::string name; // optional serializable name / registry key
};

class UIManager {
public:
    UIManager() = default;

    // Add a button; returns reference to stored button so caller can modify
    Button & addButton(Button b);
    // Convenience overloads: minimal arguments
    Button & addButton(float x, float y, float w, float h,
                       const std::string &label = "",
                       const std::string &shape = "rectangle",
                       float rotation = 0.0f,
                       std::function<void()> action = nullptr,
                       const std::string &type = "push",
                       const std::string &name = "");

    // Polygon-specific convenience: requires a point list (relative coordinates)
    Button & addPolygon(float x, float y, const std::vector<glm::vec2> &points,
                        const std::string &label = "",
                        float rotation = 0.0f,
                        std::function<void()> action = nullptr,
                        const std::string &type = "push",
                        const std::string &name = "");

    // Draw all buttons
    void draw();

    // Mouse events
    void onMouseMoved(int x, int y);
    void onMousePressed(int x, int y);

    // Action registry: map names -> functions
    void registerAction(const std::string &name, std::function<void()> fn);
    bool hasAction(const std::string &name) const;

    void clear();

private:
    std::vector<Button> buttons;
    std::unordered_map<std::string, std::function<void()>> registry;

    // helpers
    static bool pointInPolygon(float x, float y, const std::vector<glm::vec2> &poly);
    static glm::vec2 worldToLocalCentered(float x, float y, const Button &button);
};

} // namespace ui
