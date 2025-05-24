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
#include <random>
#include <algorithm>
#include <iomanip>

// warrior atributes

enum{
    ALIVE = 0,
    DEAD
};

struct atributes{
    std::string name = "null";
    int atack = 0;
    int defense = 0;
    int healing = 0;

    int max_health = 0;
    int current_health = max_health;
    
    std::vector<int> warrior_strategy {0, 0, 0}; // str | def | heal --> action chance

    char current_action;

    int current_state = ALIVE;
};

class dice{
    public:

    int roll(std::vector<int> dice_faces){ // rolls dice based on warrior strategy

        std::random_device random;
        std::mt19937 random_generator(random());

        std::shuffle(dice_faces.begin(), dice_faces.end(), random_generator);

        return dice_faces[0];
    }

    void shuffle_vector(std::vector<int> &vec){

        std::random_device random;
        std::mt19937 random_generator(random());

        std::shuffle(vec.begin(), vec.end(), random_generator);
    }

    std::vector<int> make_warrior_dice(atributes atr){
        
        std::vector<int> dice_faces;
        for(int i=0; i<atr.warrior_strategy.size(); i++){

            for(int j=0; j<atr.warrior_strategy[i]; j++){
                switch(i){
                    case 0:
                        dice_faces.emplace_back(ATACK);
                        break;
                    case 1:
                        dice_faces.emplace_back(DEFEND);
                        break;
                    case 2:
                        dice_faces.emplace_back(HEAL);
                        break;
                }
            }
        }

        return dice_faces;
    }
};

class warrior{
    public:

    atributes atr;

    void get_atributes(){
        get_name();
        get_skills();
    }

    void choose_action(){ // rolls dice and chooses warrior action
        dice dice;

        atr.current_action = dice.roll(dice.make_warrior_dice(atr));
    }

    void atack(warrior &enemy){ // atack action

        if(enemy.atr.current_action == DEFEND){
            enemy.atr.current_health -= (atr.atack * enemy.atr.defense);
            return;
        }
        else{
            enemy.atr.current_health -= atr.atack;
            return;
        }
    }
    void heal(){ // heal action
        
        atr.current_health += atr.healing;

        if(atr.current_health > atr.max_health){ 
            atr.current_health = atr.max_health;
        }
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

class arena{
    public:

    std::vector<warrior> warrior_list;

    void add_warriors(){
        for(int i=0; i<2; i++){
            warrior warrior;
            warrior.get_atributes();
            warrior_list.emplace_back(warrior);
        }
    }

    void next_event(){
        dice dice;
        std::vector<int> action_sequence {0,1};
        dice.shuffle_vector(action_sequence);

        warrior warrior_first = warrior_list[action_sequence[0]];
        warrior warrior_last = warrior_list[action_sequence[1]];

        for(auto warrior:warrior_list){// warriors choose an action
            warrior.choose_action();
        }

        switch(warrior_first.atr.current_action){

            case ATACK:
                warrior_first.atack(warrior_last);
                warrior_first.atr.current_state = check_if_alive(warrior_last);
                break;
            case HEAL:
                warrior_first.heal();
                break;
            case DEFEND:
                break;

            default:
                break;
        }

        warrior_list[action_sequence[0]] = warrior_first;


        switch(warrior_last.atr.current_action){

            case ATACK:
                warrior_last.atack(warrior_first);
                warrior_first.atr.current_state = check_if_alive(warrior_first);
                break;
            case HEAL:
                warrior_last.heal();
                break;
            case DEFEND:
                break;

            default:
                break;
        }  
    }

    private:

    int check_if_alive(warrior warrior){ // checks if warrior is alive
        if(warrior.atr.current_health <= 0){
            return DEAD;
        }
        else if(warrior.atr.current_health > 0){
            return ALIVE;
        }
    }

    void render(warrior warrior1, warrior warrior2){
        
        std::cout << std::string(112, '-') << std::endl;

        
    }

};


int main(){

    arena coliseum;

    coliseum.add_warriors();

    for(auto element:coliseum.warrior_list){
        std::cout << element.atr.name << std::endl;
    }

    return 0;
}