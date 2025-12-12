#include "UIManager.h"

namespace ui {

Button & UIManager::addButton(Button b) {
    // if button references a named action, bind from registry
    if (!b.name.empty()) {
        auto it = registry.find(b.name);
        if (it != registry.end()) b.action = it->second;
    }
    buttons.push_back(std::move(b));
    return buttons.back();
}

Button & UIManager::addButton(float x, float y, float w, float h,
                             const std::string &label,
                             const std::string &shape,
                             float rotation,
                             std::function<void()> action,
                             const std::string &type,
                             const std::string &name) {
    Button b;
    b.position = {x, y};
    b.size = {w, h};
    b.rotation = rotation;
    b.label = label;
    b.shape = shape;
    b.type = type;
    b.name = name;
    if (!name.empty()) {
        auto it = registry.find(name);
        if (it != registry.end()) b.action = it->second;
        else b.action = action;
    } else {
        b.action = action;
    }
    buttons.push_back(std::move(b));
    return buttons.back();
}

Button & UIManager::addPolygon(float x, float y, const std::vector<glm::vec2> &points,
                              const std::string &label,
                              float rotation,
                              std::function<void()> action,
                              const std::string &type,
                              const std::string &name) {
    Button b;
    b.position = {x, y};
    // compute bounding box of points to set size so centering works
    if (!points.empty()) {
        float minX = points[0].x, minY = points[0].y;
        float maxX = points[0].x, maxY = points[0].y;
        for (const auto &p : points) {
            minX = std::min(minX, p.x);
            minY = std::min(minY, p.y);
            maxX = std::max(maxX, p.x);
            maxY = std::max(maxY, p.y);
        }
        b.size = { maxX - minX, maxY - minY };
    } else {
        b.size = {0,0};
    }
    b.rotation = rotation;
    b.label = label;
    b.shape = "polygon";
    b.type = type;
    b.points = points; // keep as provided (relative coordinates)
    b.name = name;
    if (!name.empty()) {
        auto it = registry.find(name);
        if (it != registry.end()) b.action = it->second;
        else b.action = action;
    } else {
        b.action = action;
    }
    buttons.push_back(std::move(b));
    return buttons.back();
}

void UIManager::draw() {
    for (const auto & button : buttons) {
        ofPushMatrix();
        ofPushStyle();
        glm::vec2 center = button.position + button.size * 0.5f;
        ofTranslate(center.x, center.y);
        ofRotateDeg(button.rotation);

        if (button.hovered) ofSetColor(ofColor::yellow);
        else if (button.shape == "rectangle") ofSetColor(ofColor::red);
        else if (button.shape == "ellipse") ofSetColor(ofColor::green);
        else ofSetColor(ofColor::blue);

        if (button.shape == "rectangle") {
            ofDrawRectangle(-button.size.x / 2.0f, -button.size.y / 2.0f, button.size.x, button.size.y);
        } else if (button.shape == "ellipse") {
            ofDrawEllipse(0, 0, button.size.x, button.size.y);
        } else if (button.shape == "polygon") {
            ofBeginShape();
            for (const auto & p : button.points) {
                glm::vec2 v = p - (button.size * 0.5f);
                ofVertex(v.x, v.y);
            }
            ofEndShape(true);
        }

        // draw label (simple)
        ofSetColor(ofColor::black);
        ofDrawBitmapString(button.label, -button.size.x/2.0f + 4, -button.size.y/2.0f + 14);

        ofPopStyle();
        ofPopMatrix();
    }
}

void UIManager::onMouseMoved(int x, int y) {
    for (auto & button : buttons) {
        glm::vec2 local = worldToLocalCentered((float)x, (float)y, button);
        bool isHover = false;
        if (button.shape == "rectangle") {
            float hw = button.size.x * 0.5f;
            float hh = button.size.y * 0.5f;
            isHover = (local.x >= -hw && local.x <= hw && local.y >= -hh && local.y <= hh);
        } else if (button.shape == "ellipse") {
            float rx = button.size.x * 0.5f;
            float ry = button.size.y * 0.5f;
            if (rx > 0 && ry > 0) isHover = ((local.x * local.x) / (rx * rx) + (local.y * local.y) / (ry * ry)) <= 1.0f;
        } else if (button.shape == "polygon") {
            std::vector<glm::vec2> poly;
            poly.reserve(button.points.size());
            for (const auto &p : button.points) poly.push_back(p - (button.size * 0.5f));
            isHover = pointInPolygon(local.x, local.y, poly);
        }
        button.hovered = isHover;
    }
}

void UIManager::onMousePressed(int x, int y) {
    for (auto & button : buttons) {
        glm::vec2 local = worldToLocalCentered((float)x, (float)y, button);
        bool isHit = false;
        if (button.shape == "rectangle") {
            float hw = button.size.x * 0.5f;
            float hh = button.size.y * 0.5f;
            isHit = (local.x >= -hw && local.x <= hw && local.y >= -hh && local.y <= hh);
        } else if (button.shape == "ellipse") {
            float rx = button.size.x * 0.5f;
            float ry = button.size.y * 0.5f;
            if (rx > 0 && ry > 0) isHit = ((local.x * local.x) / (rx * rx) + (local.y * local.y) / (ry * ry)) <= 1.0f;
        } else if (button.shape == "polygon") {
            std::vector<glm::vec2> poly;
            poly.reserve(button.points.size());
            for (const auto &p : button.points) poly.push_back(p - (button.size * 0.5f));
            isHit = pointInPolygon(local.x, local.y, poly);
        }

        if (isHit) {
            if (button.type == "push") {
                if (button.action) button.action();
            } else if (button.type == "toggle") {
                button.state = (button.state == "on") ? "off" : "on";
                if (button.action) button.action();
            }
        }
    }
}

void UIManager::registerAction(const std::string &name, std::function<void()> fn) {
    registry[name] = std::move(fn);
}

bool UIManager::hasAction(const std::string &name) const {
    return registry.find(name) != registry.end();
}

void UIManager::clear() {
    buttons.clear();
    registry.clear();
}

// Ray-casting point-in-polygon where polygon points are already in the local (centered) space
bool UIManager::pointInPolygon(float x, float y, const std::vector<glm::vec2> &poly) {
    bool inside = false;
    int n = (int)poly.size();
    if (n < 3) return false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        float xi = poly[i].x;
        float yi = poly[i].y;
        float xj = poly[j].x;
        float yj = poly[j].y;
        bool intersect = ((yi > y) != (yj > y)) && (x < (xj - xi) * (y - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
    }
    return inside;
}

glm::vec2 UIManager::worldToLocalCentered(float x, float y, const Button &button) {
    glm::vec2 center = button.position + button.size * 0.5f;
    glm::vec2 p = glm::vec2(x, y) - center;
    float angle = glm::radians(-button.rotation);
    float c = cosf(angle);
    float s = sinf(angle);
    return glm::vec2(p.x * c - p.y * s, p.x * s + p.y * c);
}

bool UIManager::getHoveredState(int x, int y) const {
    for (const auto &button : buttons) {
        if (button.hovered) {
            return true;
        }
    }
    return false;
}

} // namespace ui
