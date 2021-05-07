#include "Medic.h"

using namespace mtm;
mtm::Medic::Medic(units_t health, units_t ammo, units_t range, units_t power, Team team) : 
ammo_per_load(5), max_step(5), ammo_per_attack_team(0), ammo_per_attack_enemy(1)
{
   this->health = health, this->ammo = ammo, this->range = range, this->power = power, this->team = team;
}

bool mtm::Medic::isValidMove(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    if (GridPoint::distance(src_coordinates, dst_coordinates) > this->max_step)
    {
        throw(MoveTooFar());
    }
    return true;
}
bool mtm::Medic::isValidAttack(const GridPoint &src_coordinates, const std::shared_ptr<Character> &attacked, 
const GridPoint &dst_coordinates)
{
    if (GridPoint::distance(src_coordinates, dst_coordinates) > this->range)
    {
        throw(OutOfRange());
    }
    if (attacked==nullptr)
    {
        throw(IllegalTarget()); //medic cannot attack empty cell
    }
    if (dst_coordinates.row == src_coordinates.row && dst_coordinates.col == src_coordinates.col)
    {
        throw(IllegalTarget()); //medic cannot attack himself
    }
    if (this->ammo < this->ammo_per_attack_enemy)
    {
        throw(OutOfAmmo());
    }
    return true;
}
//when the medic attack he hill his team members and hurt the enemy in the dst in the amamount of the power he has
//can attack in all directions, cannot attack himself and cannot attack an empty cell
void mtm::Medic::attack(Matrix<std::shared_ptr<Character>> game_board, const GridPoint &dst_coordinates)
{
    std::shared_ptr<Character> attacked_charachter = game_board(dst_coordinates.row, dst_coordinates.col);
    if (attacked_charachter != nullptr) //there is a player in the cell
    {
        if ((*attacked_charachter).getTeam() == (*this).getTeam()) 
        //the attacked player is in the same group as the medic
        {
            (*attacked_charachter).setHealth(this->power);
            this->ammo -= this->ammo_per_attack_team;
        }
        else // the attacked player is an enemy
        {
            (*attacked_charachter).setHealth(-this->power);
            this->ammo -= this->ammo_per_attack_enemy;
        }
    }
}
void mtm::Medic::reload()
{
    this->ammo += this->ammo_per_load;
}
char mtm::Medic::toString() const
{
    if ((*this).getTeam() == CPP)
    {
        return 'M'; //for cpp
    }
    return 'm'; //for pyhton
}