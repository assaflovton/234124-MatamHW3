#ifndef HW3_Sniper_H
#define HW3_Sniper_H
#include "Character.h"
namespace mtm
{
    class Sniper : public Character
    {
    private:
    const units_t ammo_per_load;
    const units_t max_step;
    const units_t ammo_per_attack;
    const int double_power;
    int attacks_count;
    const int range_factor;
    public:
        Sniper(units_t health,units_t ammo,units_t range,units_t power,Team team);
   virtual std::shared_ptr<Character> clone() const override
    {
            return std::shared_ptr<Sniper>(new Sniper(*this));
    }
    ~Sniper() =default;
        bool isValidMove(const GridPoint &src_coordinates, const GridPoint &dst_coordinates) override;
        bool isValidAttack(const GridPoint &src_coordinates,const std::shared_ptr<Character>& attacked
    ,const GridPoint &dst_coordinates) override;
        void attack(Matrix<std::shared_ptr<Character>> game_board,const GridPoint &dst_coordinates) override;
        void reload() override;
        char toString() const override;
    };
} // namespace mtm
#endif