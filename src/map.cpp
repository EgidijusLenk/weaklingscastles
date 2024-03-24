#include "map.hpp"

Map::Map(sf::RenderWindow &window) : window_(window)
{
    convertSketch();
}

Map::~Map()
{
}

void Map::convertSketch()
{

    for (unsigned char a = 0; a < (MAP_HEIGHT / CELL_SIZE); a++)
    {
        for (unsigned char b = 0; b < (MAP_WIDTH / CELL_SIZE); b++)
        {
            // By default, every cell is empty.
            map_[b][a] = Cell::Empty;

            switch (map_sketch_[a][b])
            {
            // #wall #obstacle #youcantgothroughme
            case '#':
            {
                map_[b][a] = Cell::Wall;

                break;
            }
            case '.':
            {
                map_[b][a] = Cell::Empty;

                break;
            }
            default:
            {
                break; // will be empty
            }
            }
        }
    }
    std::cout << "map converted" << std::endl;
};

void Map::drawMap()
{
    sf::Sprite sprite;

    sf::Texture texture;
    texture.loadFromFile("resources/Images/Map" + std::to_string(CELL_SIZE) + ".png");

    sprite.setTexture(texture);

    for (unsigned char a = 0; a < (MAP_WIDTH / CELL_SIZE); a++)
    {
        for (unsigned char b = 0; b < (MAP_HEIGHT / CELL_SIZE); b++)
        {
            sprite.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

            // We just crop out what we need from the texture.
            switch (map_[a][b])
            {
            case Cell::Wall:
            {
                bool down = 0;
                bool left = 0;
                bool right = 0;
                bool up = 0;

                if (b < (MAP_HEIGHT / CELL_SIZE) - 1)
                {
                    if (Cell::Wall == map_[a][1 + b])
                    {
                        down = 1;
                    }
                }
                // TODO following part is ripped, so figure it
                //  Since we have warp tunnels, we need to draw them as if they're connected.
                if (0 < a)
                {
                    if (Cell::Wall == map_[a - 1][b])
                    {
                        left = 1;
                    }
                }
                else
                {
                    left = 1;
                }

                if (a < (MAP_WIDTH / CELL_SIZE) - 1)
                {
                    if (Cell::Wall == map_[1 + a][b])
                    {
                        right = 1;
                    }
                }
                else
                {
                    right = 1;
                }

                if (0 < b)
                {
                    if (Cell::Wall == map_[a][b - 1])
                    {
                        up = 1;
                    }
                }

                sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

                window_.draw(sprite);
            }
            default:
            {
                break;
            }
            }
        }
    }
};

void Map::convertPassableArea()
{
    int pos = 0;
    for (unsigned char a = 0; a < (MAP_HEIGHT / CELL_SIZE); a++) // 45
    {
        for (unsigned char b = 0; b < (MAP_WIDTH / CELL_SIZE); b++) // 80
        {
            switch (map_sketch_[a][b])
            {
            // #wall
            case '#':
            {
                passable_area_[pos] = 9;

                break;
            }
            // .walkable
            case '.':
            {
                passable_area_[pos] = 0;
                break;
            }

            default:
            {
                // .walkable
                passable_area_[pos] = 1;
                break;
            }
            }
            pos++;
        }
    }
    // std::cout << "passable_area_ array built" << std::endl;
};