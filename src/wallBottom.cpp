#include "wallBottom.hpp"
#include <algorithm>
#include <sstream>

WallBottom::WallBottom(nlohmann::json physics_parameters, Map *map)
{
    this->y = physics_parameters["y"].get<double>();
    this->y2 = this->y + physics_parameters["thickness"].get<double>();
    this->hardness = physics_parameters["wallInteraction"]["hardness"].get<double>();
    if (physics_parameters["thickness"].get<double>() > 0)
        map->horizontal(this, this->y);
}

double WallBottom::get_y() const
{
    return this->y;
}

double WallBottom::get_hardness() const
{
    return this->hardness;
}

std::string WallBottom::state_to_string(int time_step) const
{
    std::stringstream strm;
    strm << "time-step: " << time_step << "\n";
    strm << "y: " << this->y << "\n";
    return strm.str();
}

void WallBottom::draw(int time_step, Camera *camera) const
{
    double middle_y = ((this->y + this->y2) / 2 - camera->coord[1]) * camera->zoom;
    double thickness = (this->y2 - this->y) / 2 * camera->zoom;
    for (int x = 0; x < SCREEN_WIDTH; x++)
        for (int y = (int)(middle_y - thickness); y <= (int)(middle_y + thickness) + 1; y++)
            if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
            {
                double fading = std::max(1 - (y - middle_y) * (y - middle_y) / (thickness * thickness), 0.);
                camera->pixels[y][x][2] = int(camera->pixels[y][x][2] * (1 - fading) + 255 * fading);
                camera->pixels[y][x][1] = int(camera->pixels[y][x][1] * (1 - fading) + 255 * fading);
            }
}

CellForce WallBottom::interaction(Cell *cell, int now)
{
    CellInstance cellInstance = cell->get_instance(now);

    double distance;
    double force_body_modulus;
    double force_flagella_modulus;

    // force on body
    distance = this->y - cellInstance.coord[1];

    if (distance <= 0)
        force_body_modulus = 10000.;
    else if (distance < cell->get_body_radius() * 1.122462) // 2^(1/6)
    {
        double rad_6 = pow(cell->get_body_radius(), 6.);
        double dist_6 = pow(distance, 6.);
        force_body_modulus = 24 * this->hardness * (2 * rad_6 * rad_6 / (dist_6 * dist_6 * distance) - rad_6 / (dist_6 * distance));
    }
    else
        force_body_modulus = 0;

    // force on flagella
    distance = this->y - cell->get_flagella_coord(cellInstance)[1];
    if (distance <= 0)
        force_flagella_modulus = 10000.;
    else if (distance < cell->get_flagella_radius() * 1.122462) // 2^(1/6)
    {
        double rad_6 = pow(cell->get_flagella_radius(), 6.);
        double dist_6 = pow(distance, 6.);
        force_flagella_modulus = 24 * this->hardness * (2 * rad_6 * rad_6 / (dist_6 * dist_6 * distance) - rad_6 / (dist_6 * distance));
    }
    else
        force_flagella_modulus = 0;

    return CellForce(Vector2D{0., -force_body_modulus}, Vector2D{0., -force_flagella_modulus});
}