# ImmediateModeUI

A small immediate-mode UI demo built on openFrameworks. This project demonstrates a tiny, portable UI layer (`ui::UIManager`) that manages simple interactive controls (buttons, polygons) with rotation-aware rendering and hit-testing.

## Why
I often find myself in a situation where I need a few simple UI controls (buttons, toggles) for debugging, prototyping, or small tools. Full-featured UI libraries can be overkill and add complexity. This immediate-mode UI manager provides a lightweight solution that is easy to integrate and customize. It provides basic functionality like hit-testing, hover states, and action callbacks, while being flexible enough to extend.

**Contents**
- `src/ui/UIManager.h` / `src/ui/UIManager.cpp` — the reusable UI manager and `Button` type.
- `src/ofApp.*` — example integration showing how to add buttons and wire actions.

**Quick Build**
- From the project root you can use the provided VS Code tasks or run `make`.

```bash
# Debug build
make Debug -j

# Release build
make -j

# Run release (task provided)
make RunRelease
```

**Basic Concepts**
- `ui::Button` — represents a single control. Key fields:
  - `position` (`glm::vec2`) : top-left position in window coordinates
  - `size` (`glm::vec2`) : width/height (used for rectangles/ellipses and to center polygons)
  - `rotation` (`float`) : degrees, rotation around button center
  - `shape` (`std::string`) : `"rectangle"`, `"ellipse"`, or `"polygon"`
  - `points` (`std::vector<glm::vec2>`) : polygon points (relative coordinates)
  - `label` (`std::string`) : short on-screen label
  - `action` (`std::function<void()>`) : callable executed on click
  - `type` (`std::string`) : `"push"` or `"toggle"` (toggle flips `state`)

- `ui::UIManager` — stores buttons, draws them and handles hit/hover/click logic.
  - `addButton(...)` convenience overloads to create rectangle/ellipse buttons quickly.
  - `addPolygon(...)` overload that accepts a required point list for polygons.
  - `draw()` — render all UI elements.
  - `onMouseMoved(x,y)`, `onMousePressed(x,y)` — forward mouse events.
  - `registerAction(name, fn)` — optional action registry for name-based actions.

**Example Usage (in `ofApp::setup`)**
```cpp
// register named actions (optional)
uiManager.registerAction("jump", [this](){ doJump(); });

// add simple rectangle/ellipse buttons (convenience overload)
uiManager.addButton(100, 100, 80, 40, "Click me", "rectangle", 0.0f, [this](){ onClickA(); }, "push");
uiManager.addButton(200, 100, 60, 60, "Toggle", "ellipse", 0.0f, [this](){ onToggle(); }, "toggle");

// add polygon (requires points)
std::vector<glm::vec2> poly = {{0,0},{50,50},{50,0}}; // points relative to top-left of button
uiManager.addPolygon(300, 100, poly, "Poly", 0.0f, [](){ ofLog() << "Poly clicked"; }, "push");
```

**Event Forwarding (in `ofApp` event handlers)**
```cpp
void ofApp::draw(){
  uiManager.draw();
}

void ofApp::mouseMoved(int x, int y){
  uiManager.onMouseMoved(x, y);
}

void ofApp::mousePressed(int x, int y, int button){
  uiManager.onMousePressed(x, y);
}
```

**Notes & Tips**
- Rotation is handled around the button center for both rendering and hit-testing.
- Polygons must be supplied with a `points` vector; `addPolygon(...)` computes a bounding box so rotation and centering behave as with other shapes.
- Actions are stored as `std::function<void()>` — you can bind lambdas or member functions (capture `this`).
- For serialization-friendly setups, prefer `registerAction(name, fn)` and store `Button::name`; on load, `UIManager` will bind the registered function when possible.

## Minimal button & overloaded constructors

The UI manager provides convenience overloads so you can create UI elements with a minimal amount of code. Below are the rules and examples.

- Minimal visible button: you only need `x, y, width, height`. This creates a rectangle by default and does not require an action.

  ```cpp
  // visible-only button (no action)
  uiManager.addButton(10, 10, 80, 30);
  ```

- Minimal clickable: pass a lambda or assign an action afterwards.

  ```cpp
  // clickable inline
  uiManager.addButton(10, 50, 80, 30, "Click", "rectangle", 0.0f,
                      [](){ ofLog() << "Clicked"; });

  // or assign later
  ui::Button &b = uiManager.addButton(10, 90, 80, 30);
  b.action = [this](){ this->doSomething(); };
  ```

- `addButton(x,y,w,h, label, shape, rotation, action, type, name)` is the full convenience signature. All parameters except the first four are optional and have reasonable defaults (`label = ""`, `shape = "rectangle"`, `rotation = 0`, `action = nullptr`, `type = "push"`, `name = ""`).

- Polygons: if you specify `shape = "polygon"` you must provide a point list. Use `addPolygon(x, y, points, label, rotation, action, type, name)` which requires the `points` vector and computes an internal bounding box so rotation/centering behave consistently.

  ```cpp
  std::vector<glm::vec2> tri = {{0,0},{50,0},{25,40}};
  uiManager.addPolygon(200, 100, tri, "Tri", 0.0f, [](){ ofLog() << "Poly clicked"; });
  ```

**License / Attribution**

This code is a small demo for learning purposes. Use and adapt it in your projects.
