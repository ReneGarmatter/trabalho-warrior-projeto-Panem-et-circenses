/*!
 * @file main.cpp
 * @description
 * Panem et circenses is a program that simulates a gladiator battle
 * @author	Renê Garmatter
*/

/*
- atributes
    
    name
    atk
    def
    health

- actions

    atk
    def
    heal

- dependencies


    add warrior
    warrior list
    dice
    choosing actions
    game winner
*/

#include <iostream>
#include <vector>
#include <string>

// warrior atributes

struct atributes{
    std::string name = "null";
    int atack = 0;
    int defense = 0;
    int healing = 0;

    int current_health = 0;
    int max_health = 0;
    std::vector<int> warrior_strategy {0, 0, 0}; // str | def | heal chance
};

std::vector<atributes> warrior_list;

class warrior{
    public:

    atributes atr;

    void add_warrior(std::vector<atributes> &warrior_list){
        get_name();
        get_skills();
        warrior_list.push_back(atr);
    }

    void check_health(){ // checks if warrior is alive
        //TODO: implement check health
    }
    
    private:
    
    void get_name(){
        std::cout << "Warrior name: ";
        std::cin >> atr.name;

    }
    void get_skills(){
        std::cout << "Warrior atributes (ATK | DEF | HEALING | HEALTH): ";
        std::cin >> atr.atack >> atr.defense >> atr.healing >> atr.max_health;
    }
};

// warrior actions

enum{
    ATACK = 0,
    DEFEND,
    HEAL
};

class dice{
    public:

    int roll(std::vector<int> warrior_strategy){ // rolls dice based on warrior strategy
        //TODO: dice bias
        return 0;
    }
};

class arena{
    public:

    void action(atributes warrior, atributes enemy){
        dice dice;
        int current_action = dice.roll(warrior.warrior_strategy);

        switch(current_action){
            case ATACK:
                atack(warrior, enemy);
                break;
            case DEFEND:
                defend(warrior);
                break;
            case HEAL:
                heal(warrior);
                break;
            default:
                break;
        }
    }

    void render(){// renders game actions
        //TODO: implement render
    }

    private:

    //TODO: implement actionsPanem et circenses
    void atack(atributes warrior_atributes, atributes enemy){

    }
    void defend(atributes warrior_atributes){

    }
    void heal(atributes warrior_atributes){

    }
    
    
};

int main(){

    return 0;
}