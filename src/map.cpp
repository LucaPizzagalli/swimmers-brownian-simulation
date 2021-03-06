#include "map.hpp"

Map::Map(double top, double bottom, double left, double right, double cell_size)
{
    this->isMapping = cell_size > 0;
    this->cell_size = cell_size;
    this->top = top - cell_size*2;
    this->left = left - cell_size*2;
    this->height = 5 + (int)((bottom - top) / cell_size);
    this->width = 5 + (int)((right - left) / cell_size);
    if (this->isMapping)
        this->cell = std::vector<std::set<Actor *>>(this->height * this->width);
    else
        this->cell = std::vector<std::set<Actor *>>(0);
}
void Map::depart(Actor *actor, Vector2D coord)
{
    if (this->isMapping)
    {
        int x = (int)((coord[0] - this->left) / cell_size);
        int y = (int)((coord[1] - this->top) / cell_size);
        cell[y * this->width + x].erase(actor);
    }
}
void Map::arrive(Actor *actor, Vector2D coord)
{
    if (this->isMapping)
    {
        int x = (int)((coord[0] - this->left) / cell_size);
        int y = (int)((coord[1] - this->top) / cell_size);
        cell[y * this->width + x].insert(actor);
    }
}
void Map::horizontal(Actor *actor, double yy)
{
    if (this->isMapping)
    {
        int y = (int)((yy - this->top) / cell_size);
        for (int x = 0; x < this->width; x++)
            cell[y * this->width + x].insert(actor);
    }
}
void Map::vertical(Actor *actor, double xx)
{
    if (this->isMapping)
    {
        int x = (int)((xx - this->left) / cell_size);
        for (int y = 0; y < this->height; y++)
            cell[y * this->width + x].insert(actor);
    }
}

std::set<Actor *> Map::check(Actor *actor, Vector2D coord)
{
    if (this->isMapping)
    {
        int x = (int)((coord[0] - this->left) / cell_size);
        int y = (int)((coord[1] - this->top) / cell_size);
        std::set<Actor *> merged = cell[y * this->width + x];
        merged.insert(cell[y * this->width + x + 1].begin(), cell[y * this->width + x + 1].end());
        merged.insert(cell[y * this->width + x - 1].begin(), cell[y * this->width + x - 1].end());
        merged.insert(cell[(y + 1) * this->width + x].begin(), cell[(y + 1) * this->width + x].end());
        merged.insert(cell[(y - 1) * this->width + x].begin(), cell[(y - 1) * this->width + x].end());
        merged.insert(cell[(y - 1) * this->width + x - 1].begin(), cell[(y - 1) * this->width + x - 1].end());
        merged.insert(cell[(y - 1) * this->width + x + 1].begin(), cell[(y - 1) * this->width + x + 1].end());
        merged.insert(cell[(y + 1) * this->width + x - 1].begin(), cell[(y + 1) * this->width + x - 1].end());
        merged.insert(cell[(y + 1) * this->width + x + 1].begin(), cell[(y + 1) * this->width + x + 1].end());
        merged.erase(actor);
        return merged;
    }
    else
        return std::set<Actor *>();
}

std::string Map::to_string() const
{
    std::stringstream strm;
    for (unsigned int i = 0; i < cell.size(); i++)
    {
        if (i % this->width == 0)
            strm << "\n";
        strm << cell[i].size() << " ";
    }
    strm << "\n";
    return strm.str();
}