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

enum{
    ATACK = 0,
    DEFEND,
    HEAL
};

struct atributes{
    std::string name = "null";
    int atack = 0;
    float defense = 0;
    int healing = 0;

    int max_health = 0;
    int current_health = max_health;
    
    std::vector<int> warrior_strategy {0, 0, 0}; // atk | def | heal --> action chance

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

    int choose_action(){ // rolls dice and chooses warrior action
        dice dice;
        
        //std::cout << dice.roll(dice.make_warrior_dice(atr)) << std::endl;
        return dice.roll(dice.make_warrior_dice(atr));
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

class arena{
    public:

    std::vector<warrior> warrior_list;

    void add_warriors(){
        /*
        for(int i=0; i<2; i++){
            warrior warrior;
            warrior.get_atributes();
            warrior_list.emplace_back(warrior);
        }
        */

        warrior warrior1;
        warrior1.atr.name = "Spartacus";
        warrior1.atr.max_health = 40;
        warrior1.atr.current_health = warrior1.atr.max_health;
        warrior1.atr.atack = 5;
        warrior1.atr.defense = 0.3;
        warrior1.atr.healing = 10;
        warrior1.atr.warrior_strategy = {3,2,1};
        warrior_list.emplace_back(warrior1);

        warrior warrior2;
        warrior2.atr.name = "Flamma";
        warrior2.atr.max_health = 35;
        warrior2.atr.current_health = warrior2.atr.max_health;
        warrior2.atr.atack = 10;
        warrior2.atr.defense = 0.5;
        warrior2.atr.healing = 15;
        warrior2.atr.warrior_strategy = {3, 1 ,2};
        warrior_list.emplace_back(warrior2);
    }

    void next_event(){
        dice dice;
        std::vector<int> action_sequence {0,1};
        dice.shuffle_vector(action_sequence);


        for(int i=0; i<warrior_list.size(); i++){// warriors choose an action
            warrior_list[i].atr.current_action = warrior_list[i].choose_action();
        }

        warrior warrior_first = warrior_list[action_sequence[0]];
        warrior warrior_last = warrior_list[action_sequence[1]];
        
        switch(warrior_first.atr.current_action){

            case ATACK:
                warrior_first.atack(warrior_last);
                warrior_last.atr.current_state = check_if_alive(warrior_last);
                break;
            case HEAL:
                warrior_first.heal();
                break;
            case DEFEND:
                break;

            default:
                break;
        }

        warrior_list[action_sequence[1]] = warrior_last;
        
        if(warrior_last.atr.current_state == DEAD){
            warrior_last.atr.current_health = 0;

            render(warrior_list[0], warrior_list[1]);

            std::cout << warrior_first.atr.name << " " << "wins" << std::endl;
            exit(0);
        }

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

        warrior_list[action_sequence[0]] = warrior_first;

        if(warrior_first.atr.current_state == DEAD){
            warrior_first.atr.current_health = 0;
            render(warrior_list[0], warrior_list[1]);

            std::cout << warrior_last.atr.name << " " << "wins" << std::endl;
        }

        render(warrior_list[0], warrior_list[1]);


        if(warrior_first.atr.current_state == ALIVE){
            switch(warrior_first.atr.current_action){
                case ATACK:
                    std::cout << warrior_first.atr.name << " attacked " << warrior_last.atr.name << std::endl;
                    break;
                case HEAL:
                    std::cout<< warrior_first.atr.name << " healed his wounds " << std::endl;
                    break;
                case DEFEND:
                    std::cout<< warrior_first.atr.name << " is ready to defend" << std::endl;
                    break;

                default:
                    break;
            }
        }
        else{
            exit(0);
        }
        
        if(warrior_last.atr.current_state == ALIVE){
            switch(warrior_last.atr.current_action){
                case ATACK:
                    std::cout << warrior_last.atr.name << " attacked " << warrior_first.atr.name << std::endl;
                    break;
                case HEAL:
                    std::cout<< warrior_last.atr.name << " healed his wounds " << std::endl;
                    break;
                case DEFEND:
                    std::cout<< warrior_last.atr.name << " is ready to defend" << std::endl;
                    break;

                default:
                    break;
            }
        }
        else{
            exit(0);
        }
    }

    private:

    int check_if_alive(warrior warrior){ // checks if warrior is alive
        if(warrior.atr.current_health <= 0){
            return DEAD;
        }
        else{
            return ALIVE;
        }
    }

    int get_biggest_name(){
        int biggest = warrior_list[0].atr.name.size();
        for(auto element:warrior_list){
            if(element.atr.name.size() > biggest){
                biggest = element.atr.name.size();
            }
        }

        return biggest;
    }

    int get_biggest_health(){
        int biggest = std::to_string(warrior_list[0].atr.current_health).size();
        for(auto element:warrior_list){
            if(std::to_string(element.atr.current_health).size() > biggest){
               biggest = std::to_string(element.atr.current_health).size(); 
            }
        }

        return biggest;
    }

    void render(warrior warrior1, warrior warrior2){
        
        std::cout << std::string(30, '-') << std::endl;

        int biggest_health = get_biggest_health();
        int biggest_name = get_biggest_name();
    
        std::cout << std::left << std::setw(biggest_name+5) << "WARRIORS" 
                  << std::setw(biggest_health+5) << "HEALTH" << std::endl;

        std::cout << std::left << std::setw(biggest_name+5) << warrior1.atr.name 
                  << std::setw(biggest_health+5) << warrior1.atr.current_health;

        switch(warrior1.atr.current_state){
            case ALIVE:
                std::cout << std::left << std::setw(7) << "ALIVE" << std::endl; break;

            case DEAD:
                std::cout << std::left << std::setw(7) << "DEAD" << std::endl; break;
        }

        std::cout << std::left << std::setw(biggest_name+5) << warrior2.atr.name 
                  << std::setw(biggest_health+5) << warrior2.atr.current_health;

        switch(warrior2.atr.current_state){
            case ALIVE:
                std::cout << std::left << std::setw(7) << "ALIVE" << std::endl; break;

            case DEAD:
                std::cout << std::left << std::setw(7) << "DEAD" << std::endl; break;
        }
        
        std::cout << std::string(30, '-') << std::endl;
    }

};


int main(){

    arena coliseum;

    coliseum.add_warriors();
    while(true){
        coliseum.next_event();
    }

    return 0;
}