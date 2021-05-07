#ifndef HW3_Soldier_H
#define HW3_Soldier_H
#include "Character.h"

namespace mtm{
class Soldier : public Character
{
private:
const units_t ammo_per_load;
const units_t max_step;
const units_t ammo_per_attack;
const int range_factor;
const int power_factor;
public:
    Soldier(units_t health,units_t ammo,units_t range,units_t power,Team team);
    ~Soldier() =default;
    virtual std::shared_ptr<Character> clone() const override
    {
            return std::shared_ptr<Soldier>(new Soldier(*this));
    }
    bool isValidMove(const GridPoint &src_coordinates, const GridPoint &dst_coordinates) override;
    bool isValidAttack(const GridPoint &src_coordinates,const std::shared_ptr<Character>& attacked
    ,const GridPoint &dst_coordinates) override;
    void attack(Matrix<std::shared_ptr<Character>> game_board,const GridPoint &dst_coordinates) override;
    void reload() override;
    char toString() const override;

};
}
#endif