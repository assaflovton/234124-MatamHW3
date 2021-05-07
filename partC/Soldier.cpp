#include "Soldier.h"
#include <math.h>
using namespace mtm;
mtm::Soldier::Soldier(units_t health, units_t ammo, units_t range, units_t power, Team team) :
 ammo_per_load(3), max_step(3), ammo_per_attack(1), range_factor(3), power_factor(2)
{
    this->health = health, this->ammo = ammo, this->range = range, this->power = power, this->team = team;
}
bool mtm::Soldier::isValidMove(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    if (GridPoint::distance(src_coordinates, dst_coordinates) > ((*this).max_step)) 
    //distane messured as 'Manhetten distance'
    {
        throw(MoveTooFar());
    }
    return true;
}
bool mtm::Soldier::isValidAttack(const GridPoint &src_coordinates, const std::shared_ptr<Character> &attacked, 
const GridPoint &dst_coordinates)
{
    if (GridPoint::distance(src_coordinates, dst_coordinates) > ((*this).range)) 
    //not in the range of the player to perform the attack
    {
        throw(OutOfRange());
    }
    if ((*this).ammo < (*this).ammo_per_attack) //not enough ammo to perform the attack
    {
        throw(OutOfAmmo());
    }
    if((src_coordinates.row != dst_coordinates.row) && (src_coordinates.col != dst_coordinates.col))
    {
        throw(IllegalTarget());
    }
    return true;
}
//when the solider attacks he reduce the enemy heath in the dst coordinates by the power he has,
// and every character of the enemy team in the rang/range factor hit by power/power factor
//the soldier cannot hurt his team members, attack is possible in every state of the cell
//able to attack only characters in his row or colom (straigt line)
void mtm::Soldier::attack(Matrix<std::shared_ptr<Character>> game_board, const GridPoint &dst_coordinates)
{
    std::shared_ptr<Character> attacked_charachter = game_board(dst_coordinates.row, dst_coordinates.col);
    (*this).ammo -= (*this).ammo_per_attack;
    if (attacked_charachter != nullptr && (attacked_charachter)->getTeam() != (this)->team) 
    // there is a player in the cell
    {
        (*attacked_charachter).setHealth(-(*this).power); //attack the player in the dst cell
    }
    for (int i = 0; i < game_board.height(); i++) //attack the players in the range/range factor
    {
        for (int j = 0; j < game_board.width(); j++)
        {
            std::shared_ptr<Character> current_attacked = game_board(i, j);
            if (GridPoint::distance(dst_coordinates, GridPoint(i, j)) <= ceil(((double)(*this).range / 
            (double)(*this).range_factor)))
            {
                if (current_attacked != nullptr && (current_attacked)->getTeam() != (this)->team)
                 //attack only opponents
                {
                    current_attacked->setHealth(-ceil((double)this->power/(double)this->power_factor));
                }
            }
        }
    }
}
char Soldier::toString() const
{
    if ((*this).getTeam() == CPP)
        return 'S'; //for cpp
    return 's';     //for python
}
//add ammo to the soldier
void mtm::Soldier::reload()
{
    (*this).ammo += (*this).ammo_per_load;
}