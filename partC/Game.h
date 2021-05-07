#ifndef HW3_GAME_H
#define HW3_GAME_H
#include <memory>
#include "Character.h"
namespace mtm{
class Game
{
private:
int height;
int width;
Matrix<std::shared_ptr<Character>> game_board;
bool isNotInBoard(int row, int col)const;
bool isEmptyCell(int row, int col) const;

public:
    Game(int height, int width);
    Game(const Game &other);
    Game &operator=(const Game &other);
    void addCharacter(const GridPoint &coordinates, std::shared_ptr<Character> character);
    static std::shared_ptr<Character> makeCharacter(CharacterType type, Team team,
                                                    units_t health, units_t ammo, units_t range, units_t power);
    void move(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);
    void attack(const GridPoint &src_coordinates, const GridPoint &dst_coordinates);
    void reload(const GridPoint &coordinates);
    bool isOver(Team *winningTeam = NULL) const;
    friend std::ostream& operator<<(std::ostream& os, const Game& game)
    {
    char *game_array = new char[game.height * game.width];//create a one dimenstional array rpresentation of the game
    for (int i = 0; i < game.height; i++)
    {
        
        //std::cout<<(game.height);
        for (int j = 0; j < game.width; j++)
        {
            if (game.game_board(i, j)==nullptr)
            {
                game_array[(i * game.width) + j] = ' ';
            }
            else
            {
                game_array[i * (game.width) + j] = (game.game_board(i, j))->toString();
            }
        }
    }
    printGameBoard(os, game_array, game_array + game.height * game.width, game.width);
    delete[] game_array;
    return  os;
    }
};
}
#endif