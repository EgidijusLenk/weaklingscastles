#include "pathfinder.hpp"

PathFinder::PathFinder()
{
}

PathFinder::~PathFinder()
{
}

// std::array<int, ((MAP_HEIGHT / PATH_CELL_SIZE) * (MAP_WIDTH / PATH_CELL_SIZE))> PathFinder::passable_area_;
std::array<int, ((MAP_HEIGHT / CELL_SIZE) * (MAP_WIDTH / CELL_SIZE))> PathFinder::passable_area_;

int PathFinder::getMap(int x, int y)
{
    if (x < 0 ||
        x >= MAP_WIDTH ||
        y < 0 ||
        y >= MAP_HEIGHT)
    {
        return 9;
    }

    // return passable_area_[(y * (MAP_WIDTH / PATH_CELL_SIZE)) + x];
    return passable_area_[(y * (MAP_WIDTH / CELL_SIZE)) + x];
}

std::vector<sf::Vector2f> PathFinder::findPath(sf::Vector2f start, sf::Vector2f end)
{
    auto duration_start = std::chrono::high_resolution_clock::now();

    AStarSearch<VectorNode> astarsearch;
    std::vector<sf::Vector2f> traces; // TODO instead of returning copy maybe I could move it or pass ref to param
    unsigned int SearchCount = 0;
    const unsigned int NumSearches = 1;

    while (SearchCount < NumSearches)
    {
        // log::ds("this run");
        // Create a start state
        VectorNode nodeStart;
        // nodeStart.x = std::round(start.x) / PATH_CELL_SIZE;
        // nodeStart.y = std::round(start.y) / PATH_CELL_SIZE;
        nodeStart.x = std::round(start.x) / CELL_SIZE;
        nodeStart.y = std::round(start.y) / CELL_SIZE;
        // nodeStart.x = rand() % MAP_WIDTH_astar;
        // nodeStart.y = rand() % MAP_HEIGHT_astar;

        // Define the goal state
        VectorNode nodeEnd;
        // nodeEnd.x = std::round(end.x) / PATH_CELL_SIZE;
        // nodeEnd.y = std::round(end.y) / PATH_CELL_SIZE;
        nodeEnd.x = std::round(end.x) / CELL_SIZE;
        nodeEnd.y = std::round(end.y) / CELL_SIZE;
        // nodeEnd.x = rand() % MAP_WIDTH_astar;
        // nodeEnd.y = rand() % MAP_HEIGHT_astar;

        // Set Start and goal states
        // VectorNode::nodes[0][0]
        astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);
        // astarsearch.SetStartAndGoalStates(nodeStart, nodeEnd);
        // log::ds("this run");
        unsigned int SearchState;
        unsigned int SearchSteps = 0;

        do
        {
            SearchState = astarsearch.SearchStep();
            // log::ds("this runxx");
            SearchSteps++;

        } while (SearchState == AStarSearch<VectorNode>::SEARCH_STATE_SEARCHING);
        // log::ds("this runaa");
        // switch (SearchState)
        // {
        // case AStarSearch<VectorNode>::SEARCH_STATE_NOT_INITIALISED:
        //     log::ds("SEARCH_STATE_NOT_INITIALISED");
        //     break;
        // case AStarSearch<VectorNode>::SEARCH_STATE_SEARCHING:
        //     log::ds("SEARCH_STATE_SEARCHING");
        //     break;
        // case AStarSearch<VectorNode>::SEARCH_STATE_SUCCEEDED:
        //     log::ds("SEARCH_STATE_SUCCEEDED");
        //     break;
        // case AStarSearch<VectorNode>::SEARCH_STATE_FAILED:
        //     log::ds("SEARCH_STATE_FAILED");
        //     break;
        // case AStarSearch<VectorNode>::SEARCH_STATE_OUT_OF_MEMORY:
        //     log::ds("SEARCH_STATE_OUT_OF_MEMORY");
        //     break;
        // case AStarSearch<VectorNode>::SEARCH_STATE_INVALID:
        //     log::ds("SEARCH_STATE_INVALID");
        //     break;

        // default:
        //     break;
        // }
        if (SearchState == AStarSearch<VectorNode>::SEARCH_STATE_SUCCEEDED)
        {
            // log::ds("SEARCH_STATE_SUCCEEDED");
            // cout << "Search found goal state\n";

            VectorNode *node = astarsearch.GetSolutionStart();
            int steps = 0;
            // traces.clear();
            // sf::RectangleShape temp_rect;
            // temp_rect.setSize({10.f, 10.f});
            sf::Vector2f temp_vect;
            // node->PrintNodeInfo();
            for (;;)
            {
                node = astarsearch.GetSolutionNext();

                if (!node)
                {
                    break;
                }
                // temp_vect.x = static_cast<float>(node->x * PATH_CELL_SIZE);
                // temp_vect.y = static_cast<float>(node->y * PATH_CELL_SIZE);
                temp_vect.x = static_cast<float>(node->x * CELL_SIZE);
                temp_vect.y = static_cast<float>(node->y * CELL_SIZE);
                // temp_rect.setPosition(static_cast<float>(node->x * CELL_SIZE), static_cast<float>(node->y * CELL_SIZE));
                traces.push_back(temp_vect);
                // node->PrintNodeInfo();
                steps++;
                // log::d(traces.size());
            };

            // cout << "Solution steps " << steps << endl;

            // Once you're done with the solution you can free the nodes up
            astarsearch.FreeSolutionNodes();
        }
        else if (SearchState == AStarSearch<VectorNode>::SEARCH_STATE_FAILED)
        {
            // std::cout << "Search terminated. Did not find goal state\n";
        }

        // Display the number of loops the search went through
        // cout << "SearchSteps : " << SearchSteps << "\n";

        SearchCount++;

        astarsearch.EnsureMemoryFreed();
    }
    auto duration_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = duration_end - duration_start;
    // std::cout << "time taken: " << duration.count() << " seconds" << std::endl;
    return traces;
};

VectorNode::VectorNode(int px, int py) : x(px), y(py)
{
}
VectorNode::VectorNode(){};
VectorNode::~VectorNode(){};

bool VectorNode::GetSuccessors(AStarSearch<VectorNode> *astarsearch, VectorNode *parent_node)
{

    int parent_x = -1;
    int parent_y = -1;

    if (parent_node)
    {
        parent_x = parent_node->x;
        parent_y = parent_node->y;
    }

    VectorNode NewNode;

    // push each possible move except allowing the search to go backwards
    // adjacent
    if ((PathFinder::getMap(x - 1, y) < 9) && !((parent_x == x - 1) && (parent_y == y)))
    {
        NewNode = VectorNode(x - 1, y);
        astarsearch->AddSuccessor(NewNode);
    }

    if ((PathFinder::getMap(x, y - 1) < 9) && !((parent_x == x) && (parent_y == y - 1)))
    {
        NewNode = VectorNode(x, y - 1);
        astarsearch->AddSuccessor(NewNode);
    }

    if ((PathFinder::getMap(x + 1, y) < 9) && !((parent_x == x + 1) && (parent_y == y)))
    {
        NewNode = VectorNode(x + 1, y);
        astarsearch->AddSuccessor(NewNode);
    }

    if ((PathFinder::getMap(x, y + 1) < 9) && !((parent_x == x) && (parent_y == y + 1)))
    {
        NewNode = VectorNode(x, y + 1);
        astarsearch->AddSuccessor(NewNode);
    }

    // diagonal movements
    if ((PathFinder::getMap(x - 1, y - 1) < 9) && !((parent_x == x - 1) && (parent_y == y - 1)))
    {
        NewNode = VectorNode(x - 1, y - 1);
        astarsearch->AddSuccessor(NewNode);
    }

    if ((PathFinder::getMap(x + 1, y - 1) < 9) && !((parent_x == x + 1) && (parent_y == y - 1)))
    {
        NewNode = VectorNode(x + 1, y - 1);
        astarsearch->AddSuccessor(NewNode);
    }

    if ((PathFinder::getMap(x - 1, y + 1) < 9) && !((parent_x == x - 1) && (parent_y == y + 1)))
    {
        NewNode = VectorNode(x - 1, y + 1);
        astarsearch->AddSuccessor(NewNode);
    }

    if ((PathFinder::getMap(x + 1, y + 1) < 9) && !((parent_x == x + 1) && (parent_y == y + 1)))
    {
        NewNode = VectorNode(x + 1, y + 1);
        astarsearch->AddSuccessor(NewNode);
    }

    return true;
}

float VectorNode::GoalDistanceEstimate(VectorNode &otherNode)
{
    float distance = std::sqrt(std::pow(otherNode.x - x, 2) + std::pow(otherNode.y - y, 2));
    return distance;
}

float VectorNode::GetCost(VectorNode &otherNode)
{
    float distance = std::sqrt(std::pow(otherNode.x - x, 2) + std::pow(otherNode.y - y, 2));
    return distance;
}

bool VectorNode::IsGoal(VectorNode &otherNode)
{

    if ((x == otherNode.x) &&
        (y == otherNode.y))
    {
        return true;
    }

    return false;
}

bool VectorNode::IsSameState(VectorNode &otherNode)
{

    if ((x == otherNode.x) &&
        (y == otherNode.y))
    {
        return true;
    }
    return false;
}
size_t VectorNode::Hash()
{
    size_t h1 = hash<float>{}(x);
    size_t h2 = hash<float>{}(y);
    return h1 ^ (h2 << 1);
}

void VectorNode::PrintNodeInfo()
{
    const int strSize = 100;
    char str[strSize];
    snprintf(str, strSize, "Node position : (%d,%d)\n", x, y);

    std::cout << str;
}