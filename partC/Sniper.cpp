#include "Sniper.h"
#include <math.h>
using namespace mtm;
mtm::Sniper::Sniper(units_t health, units_t ammo, units_t range, units_t power,Team team):ammo_per_load(2),max_step(4),
ammo_per_attack(1),double_power(3),attacks_count(0),range_factor(2)
{
  this->health = health, this->ammo = ammo, this->range = range, this->power = power, this->team = team;
   }

bool mtm::Sniper::isValidMove(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    if (GridPoint::distance(src_coordinates, dst_coordinates) > (this->max_step))
    {
        throw(MoveTooFar());
    }
    return true;
}
bool mtm::Sniper::isValidAttack(const GridPoint &src_coordinates, const std::shared_ptr<Character> &attacked, 
const GridPoint &dst_coordinates)
{
    if (GridPoint::distance(src_coordinates, dst_coordinates) < (ceil((double)this->range / (double)range_factor)) || 
    GridPoint::distance(src_coordinates, dst_coordinates) > this->range)
    {
        throw(OutOfRange());
    }
    if (this->ammo < this->ammo_per_attack)
    {
        throw(OutOfAmmo());
    }
    if (attacked == nullptr || (*attacked).getTeam() == this->team) //can aim only enemies
    {
        throw(IllegalTarget());
    }
    return true;
}
//when the sniper attacks he can damage only enemies,can attack in al directions,every third attack double its power,
// only attacks that succsseded count
void mtm::Sniper::attack(Matrix<std::shared_ptr<Character>> game_board, const GridPoint &dst_coordinates)
{
    std::shared_ptr<Character> attacked_charachter = game_board(dst_coordinates.row, dst_coordinates.col);
    this->ammo -= this->ammo_per_attack; //update the amount of ammo left
    (this->attacks_count)++; //uupdate the count of the attacks, if steped into this function the  attack succsseded
    units_t power_add = 0;
    if ((this->attacks_count) % (this->double_power) == 0)
    {
        power_add = this->power;
    }
    (*attacked_charachter).setHealth(- (this->power + power_add)) ;
}
char Sniper::toString() const
{
    if ((this->getTeam() == CPP))
        return 'N';
    return 'n';
}
void mtm::Sniper::reload()
{
    this->ammo += this->ammo_per_load;
}