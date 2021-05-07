#ifndef HW3_Character_H
#define HW3_Character_H
#include "Auxiliaries.h"
#include "Matrix.h"
#include <memory>
#include "Exceptions.h"
namespace mtm
{
    class Character
    {
    protected:
        Team team;
        units_t health;
        units_t ammo;
        units_t range;
        units_t power;

    public:
        virtual ~Character(){}
        //used to copy the character when using smart pointer
        virtual std::shared_ptr<Character> clone() const =0;
        //return true if the move is within the character max_step
        virtual bool isValidMove(const GridPoint &src_coordinates, const GridPoint &dst_coordinates) = 0;
        //return true if the attack is valid, characters exist, within game borders, 
        //attacker is in src and defender in dst
        virtual bool isValidAttack(const GridPoint &src_coordinates, const std::shared_ptr<Character> &attacked,
         const GridPoint &dst_coordinates) = 0;
        //preform the attack according to the character type, attacker is in src and defender in dst
        virtual void attack(Matrix<std::shared_ptr<Character>> game_board,const GridPoint &dst_coordinates) = 0;
        //reload the ammo of the charachter in the given coordinates
        virtual void reload() = 0;
        //return a char representing the type of the characte, teams differ on the case of the alphabet
        virtual char toString() const = 0;
        Team getTeam() const
        {
            return this->team;
        }
        units_t getHealth() const
        {
            return this->health;
        }
        void setHealth(units_t val)
        {
            this->health += val;
        }
    };
} // namespace mtm
#endif