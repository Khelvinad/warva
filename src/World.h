#pragma once

#include "Iso.h"

#include <glm/glm.hpp>

#include <vector>

namespace warva {

class Input;

struct Character {
    int id = 0;
    int variant = 0; // 0 = staff, 1 = student, 2 = worker
    bool player = false;
    bool moving = false;
    glm::vec2 position{0.0f}; // continuous tile coordinates
    glm::vec2 target{0.0f};
};

struct Table {
    glm::vec2 position{0.0f}; // tile coordinates
};

struct Renderable {
    enum class Kind {
        Character,
        Table,
    };

    Kind kind = Kind::Character;
    glm::vec2 foot{0.0f};
    int variant = 0;
    int id = 0;
    float layer = 1.0f;
};

class World {
public:
    World();

    // Reads mouse state and applies the click command. Called once per frame,
    // before the fixed-step update, so a click is never lost.
    void handleInput(Input& input);
    void update(float dt);

    const std::vector<Character>& characters() const { return characters_; }
    const std::vector<Table>& tables() const { return tables_; }

    int gridWidth() const { return gridWidth_; }
    int gridHeight() const { return gridHeight_; }

    bool hoverValid() const { return hoverValid_; }
    bool hoverBlocked() const { return hoverBlocked_; }
    int hoverX() const { return hoverX_; }
    int hoverY() const { return hoverY_; }

    // Returns drawables sorted by foot screen Y, then layer, then id.
    std::vector<Renderable> renderables() const;

private:
    bool inBounds(int x, int y) const;
    bool occupied(int x, int y, int ignoreCharacterId) const;

    int gridWidth_ = 8;
    int gridHeight_ = 8;
    std::vector<Character> characters_;
    std::vector<Table> tables_;
    int hoverX_ = -1;
    int hoverY_ = -1;
    bool hoverValid_ = false;
    bool hoverBlocked_ = false;
    int nextId_ = 0;
};

} // namespace warva
