#include "Game.h"
#include "Character.h"
#include "Soldier.h"
#include "Medic.h"
#include "Sniper.h"
#include "stdio.h"
#include <iostream>
using namespace mtm;
//create a game in the the given size initilized with nullptr
Game::Game(int height, int width) : height(height), width(width),
game_board((height <= 0 || width <= 0) ? throw IllegalArgument() : 
Matrix<std::shared_ptr<Character>>(Dimensions(height, width), nullptr))
{
}
//create a copy of the game, copy each character by value using clone method
Game::Game(const Game &other) : height(other.height), width(other.width),
                                game_board(other.game_board)
{
    for (int i = 0; i < (this->height); i++)
    {
        for (int j = 0; j < (this->width); j++)
        {
            if (other.game_board(i, j) != nullptr)
            {
                    (game_board(i, j)) = (other.game_board(i, j))->clone();
            }
        }
    }
}
//overload the assgiment operator, copy by value from the given Game
Game &Game::operator=(const Game &other)
{
    if (this == &other) //protect from self assigment
    {
        return *this;
    }
    (this->game_board) = Matrix<std::shared_ptr<Character>>(Dimensions(other.game_board.height(),
                                                                       other.game_board.width()),
                                                            nullptr);
    this->height = other.height;
    this->width = other.width;
    for (int i = 0; i < game_board.height(); i++)
    {
        for (int j = 0; j < game_board.width(); j++)
        {
            if (other.game_board(i, j) != nullptr)
                game_board(i, j) = ((other.game_board(i, j))->clone()); //using clone to copy each character
        }
    }
    return *this;
}
//get a smart pointer to a character and add it in the specified coordinates, upte by refernce using the pointer
void Game::addCharacter(const GridPoint &coordinates, std::shared_ptr<Character> character)
{
    int row = coordinates.row;
    int col = coordinates.col;
    if (isNotInBoard(row, col))
    {
        throw IllegalCell();
    }
    if (game_board(row, col) != nullptr)
    {
        throw CellOccupied();
    }
    game_board(row, col) = character; //assign the pointer
}
//create a new character by using the right con' using the specifiec character type, teturn a smart pointer to it
std::shared_ptr<Character> Game::makeCharacter(CharacterType type, Team team,
                                               units_t health, units_t ammo, units_t range, units_t power)
{
    if (health <= 0 || ammo < 0 || power < 0 || range < 0)
    {
        throw IllegalArgument();
    }
    if (type == SOLDIER)
    {
        return std::shared_ptr<Character>(new Soldier(health, ammo, range, power, team));
    }
    else if (type == MEDIC)
    {
        return std::shared_ptr<Character>(new Medic(health, ammo, range, power, team));
    }
    return std::shared_ptr<Character>(new Sniper(health, ammo, range, power, team));
}
//move a character from the src to the dst coordinates
void Game::move(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{

    if (isNotInBoard(src_coordinates.row, src_coordinates.col) || 
    isNotInBoard(dst_coordinates.row, dst_coordinates.col))
    {
        throw IllegalCell();
    }
    if (isEmptyCell(src_coordinates.row, src_coordinates.col))
    {
        throw CellEmpty();
    }
    bool check = (game_board(src_coordinates.row, src_coordinates.col))
                     ->isValidMove(src_coordinates, dst_coordinates);
    if (!isEmptyCell(dst_coordinates.row, dst_coordinates.col))
    {
        throw CellOccupied();
    }
    if (check) //check validty of the charachter movement
    {
        (*this).game_board(dst_coordinates.row, dst_coordinates.col) =
            (*this).game_board(src_coordinates.row, src_coordinates.col); //copy the player form dst to src point

        (this)->game_board(src_coordinates.row, src_coordinates.col) = nullptr; //free the last cell
    }
}
//return true if the cell is empty
bool Game::isEmptyCell(int row, int col) const
{
    if (((*this).game_board)(row, col) == nullptr)
    {
        return true;
    }
    return false;
}
//return true if the coordinates are not inside the board game
bool Game::isNotInBoard(int row, int col) const
{
    if (height <= row || row < 0 || width <= col || col < 0)
    {
        return true;
    }
    return false;
}
//attack the charachter if the dst coordinates, calls the aproprate attack function according to 
//the the character type in the src coordinates
//if a charcacter died as a result of the attack the coordinates become nullptr
void Game::attack(const GridPoint &src_coordinates, const GridPoint &dst_coordinates)
{
    if (isNotInBoard(src_coordinates.row, src_coordinates.col) || 
    isNotInBoard(dst_coordinates.row, dst_coordinates.col))
    {
        throw IllegalCell();
    }
    if (isEmptyCell(src_coordinates.row, src_coordinates.col))
    {
        throw CellEmpty();
    }
    if ((game_board(src_coordinates.row, src_coordinates.col))
    ->isValidAttack(src_coordinates, game_board(dst_coordinates.row, dst_coordinates.col), dst_coordinates)) 
    //check ofor the validty of the attack
    {
        (game_board(src_coordinates.row, src_coordinates.col))->attack(game_board, dst_coordinates);
        for (std::shared_ptr<Character> &character : game_board)
        {
            if (character != nullptr && character->getHealth() <= 0) //character died=no health
            {
                character = std::shared_ptr<Character>(nullptr);
            }
        }
    }
}
//add ammo to the charachter in the coordinates accordingly to their type
void Game::reload(const GridPoint &coordinates)
{
    if (isNotInBoard(coordinates.row, coordinates.col))
    {
        throw IllegalCell();
    }
    if (isEmptyCell(coordinates.row, coordinates.col))
    {
        throw CellEmpty();
    }
    game_board(coordinates.row, coordinates.col)->reload();
}
//check if the the game is over, if there are only charchters of one team on the board there is a winning team, 
//return true,if there are no characters return false, wriet the results to the given pinter
bool Game::isOver(Team *winningTeam) const
{

    int cpp_count = 0, python_count = 0;

    for (std::shared_ptr<Character> character : game_board)
    {
        if (character == nullptr) //empty cell
        {
            continue;
        }
        if (character->getTeam() == PYTHON)
        {
            python_count++; //count the members of python team
        }
        if (character->getTeam() == CPP)
        {
            cpp_count++; //count the members of cpp team
        }
    }
    if (cpp_count != 0 && python_count == 0) // if there only cpp members left, cpp won
    {
        if (winningTeam != nullptr)
        {
            *winningTeam = CPP;
        }
        return true;
    }
    else if (cpp_count == 0 && python_count != 0) //python won
    {
        if (winningTeam != nullptr)
        {
            *winningTeam = PYTHON;
        }
        return true;
    }
    return false;
}
//print the game, for cpp characters uppercase letters and for python lowercase letters
//' '=empty, S for soldier , N for sniper, and M for medic
