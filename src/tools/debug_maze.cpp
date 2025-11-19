#include <iostream>
#include <vector>
#include <iomanip>
#include "../environnement/ModelGenerator.hpp"
#include "../environnement/Node.hpp"

void printAsciiMaze(const std::vector<Node*> &grid, int width, int height)
{
    int rows = height * 2 + 1;
    int cols = width * 2 + 1;
    std::vector<std::string> out(rows, std::string(cols, '#'));

    // place centers
    for (Node *n : grid)
    {
        int x = n->getxPos();
        int y = n->getyPos();
        int cr = y * 2 + 1;
        int cc = x * 2 + 1;
        out[cr][cc] = ' ';
        if (!n->top) out[cr-1][cc] = ' ';
        if (!n->bottom) out[cr+1][cc] = ' ';
        if (!n->left) out[cr][cc-1] = ' ';
        if (!n->right) out[cr][cc+1] = ' ';
    }

    for (auto &line : out)
        std::cout << line << std::endl;
}

int main()
{
    const int W = 7;
    const int H = 4;
    ModelGenerator mg(W,H);
    const auto &grid = mg.getGrid();

    std::cout << "Nodes (x,y) and walls:" << std::endl;
    for (Node *n : grid)
    {
        std::cout << "(" << n->getxPos() << "," << n->getyPos() << ") top=" << n->top << " bot=" << n->bottom << " left=" << n->left << " right=" << n->right << std::endl;
    }

    std::cout << "\nASCII maze (#+wall, space=open):\n";
    printAsciiMaze(grid, W, H);

        // Connectivity check: BFS using openings
        std::vector<std::vector<Node*>> byPos(H, std::vector<Node*>(W, nullptr));
        for (Node *n : grid) byPos[n->getyPos()][n->getxPos()] = n;

        std::vector<std::vector<bool>> seen(H, std::vector<bool>(W,false));
        std::vector<std::pair<int,int>> q;
        q.reserve(W*H);
        q.push_back({0,0});
        seen[0][0]=true;
        for (size_t qi=0; qi<q.size(); ++qi)
        {
            auto [y,x] = q[qi];
            Node *n = byPos[y][x];
            if (!n) continue;
            // move up
            if (!n->top && y-1>=0 && !seen[y-1][x]) { seen[y-1][x]=true; q.push_back({y-1,x}); }
            if (!n->bottom && y+1<H && !seen[y+1][x]) { seen[y+1][x]=true; q.push_back({y+1,x}); }
            if (!n->left && x-1>=0 && !seen[y][x-1]) { seen[y][x-1]=true; q.push_back({y,x-1}); }
            if (!n->right && x+1<W && !seen[y][x+1]) { seen[y][x+1]=true; q.push_back({y,x+1}); }
        }

        int reachable=0;
        for (int yy=0; yy<H; ++yy) for (int xx=0; xx<W; ++xx) if (seen[yy][xx]) ++reachable;
        std::cout << "\nConnectivity: reachable "<<reachable<<"/"<<(W*H)<<" nodes from (0,0)\n";

    return 0;
}

