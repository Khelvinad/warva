#include "World.h"

#include "Input.h"

#include <algorithm>
#include <cmath>

namespace warva {

World::World() {
    Character player;
    player.id = nextId_++;
    player.variant = 0;
    player.player = true;
    player.position = {3.0f, 3.0f};
    player.target = player.position;
    characters_.push_back(player);

    Character student;
    student.id = nextId_++;
    student.variant = 1;
    student.position = {2.0f, 5.0f};
    student.target = student.position;
    characters_.push_back(student);

    Character worker;
    worker.id = nextId_++;
    worker.variant = 2;
    worker.position = {6.0f, 2.0f};
    worker.target = worker.position;
    characters_.push_back(worker);

    tables_.push_back({{5.0f, 5.0f}});
    tables_.push_back({{1.0f, 2.0f}});
}

void World::handleInput(Input& input) {
    const ScreenPoint mouse = input.mouseVirtual();
    int tileX = -1;
    int tileY = -1;
    if (insideVirtualScreen(mouse.x, mouse.y)) {
        screenToTile(mouse.x, mouse.y, tileX, tileY);
    }

    hoverValid_ = inBounds(tileX, tileY);
    hoverX_ = tileX;
    hoverY_ = tileY;

    Character& player = characters_.front();
    hoverBlocked_ = hoverValid_ && occupied(tileX, tileY, player.id);

    if (input.consumeClick() && hoverValid_ && !hoverBlocked_) {
        player.target = {static_cast<float>(tileX), static_cast<float>(tileY)};
        player.moving = true;
    }
}

void World::update(float dt) {
    Character& player = characters_.front();
    if (!player.moving) return;

    const glm::vec2 delta = player.target - player.position;
    const float distance = glm::length(delta);
    constexpr float kSpeedTilesPerSecond = 3.5f;
    const float step = kSpeedTilesPerSecond * dt;

    if (distance <= step || distance < 0.0001f) {
        player.position = player.target;
        player.moving = false;
        return;
    }
    player.position += (delta / distance) * step;
}

std::vector<Renderable> World::renderables() const {
    std::vector<Renderable> result;
    result.reserve(characters_.size() + tables_.size());

    for (const Character& character : characters_) {
        result.push_back({Renderable::Kind::Character, character.position,
                          character.variant, character.id, 1.0f});
    }
    for (std::size_t i = 0; i < tables_.size(); ++i) {
        result.push_back({Renderable::Kind::Table, tables_[i].position, 0,
                          static_cast<int>(i), 1.0f});
    }

    // Painter's algorithm: objects lower on screen are drawn later. Layer and
    // id keep the order stable when two objects share the same foot line.
    std::sort(result.begin(), result.end(), [](const Renderable& a, const Renderable& b) {
        const ScreenPoint aFoot = tileToScreen(a.foot.x, a.foot.y);
        const ScreenPoint bFoot = tileToScreen(b.foot.x, b.foot.y);
        if (aFoot.y != bFoot.y) return aFoot.y < bFoot.y;
        if (a.layer != b.layer) return a.layer < b.layer;
        return a.id < b.id;
    });
    return result;
}

bool World::inBounds(int x, int y) const {
    return x >= 0 && x < gridWidth_ && y >= 0 && y < gridHeight_;
}

bool World::occupied(int x, int y, int ignoreCharacterId) const {
    for (const Table& table : tables_) {
        if (static_cast<int>(table.position.x) == x &&
            static_cast<int>(table.position.y) == y) {
            return true;
        }
    }
    for (const Character& character : characters_) {
        if (character.id == ignoreCharacterId) continue;
        if (static_cast<int>(std::round(character.position.x)) == x &&
            static_cast<int>(std::round(character.position.y)) == y) {
            return true;
        }
    }
    return false;
}

} // namespace warva
