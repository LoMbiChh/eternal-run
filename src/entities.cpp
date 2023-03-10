#include <cstdlib>
#include <cmath>
#include <ncursesw/ncurses.h>

#include "entities.hpp"

/* Classes */

// Methods of class Entity

Entity::Entity() {};

Entity::Entity(int y, int x, int direction) {
    this->y = y;
    this->x = x;
    this->direction = direction;
}

void Entity::set_y(int y) {
    this->y = y;
}

int Entity::get_y() {
    return this->y; 
}

void Entity::set_x(int x) {
    this->x = x;
}

int Entity::get_x() {
    return this->x; 
}

void Entity::set_direction(int direction) {
    this->direction = direction;
}

int Entity::get_direction() {
    return this->direction; 
}

// Methods of class Player

Player::Player() {};

Player::Player(
    int y, int x, int direction, bool is_moving,
    int life, int shield
):Entity(y, x, direction) {
    this->is_moving = is_moving; 

    this->life = life;
    this->shield = shield;

    this->has_pistol = false;
    this->faster_bullet_speed = false;
    this->bullet_damage = 1;
}

void Player::set_is_moving(bool is_moving) {
    this->is_moving = is_moving;
}

bool Player::get_is_moving() {
    return this->is_moving;
}

void Player::set_life(int life) {
    this->life = life;
}

int Player::get_life() {
    return this->life;
}

void Player::set_shield(int shield) {
    this->shield = shield; 
}

int Player::get_shield() {
    return this->shield; 
}

void Player::set_has_pistol() {
    this->has_pistol = true;
}

int Player::get_has_pistol() {
    return this->has_pistol;
}

void Player::set_faster_bullet_speed() {
    this->faster_bullet_speed = true;
}

int Player::get_faster_bullet_speed() {
    return this->faster_bullet_speed;
}

void Player::set_bullet_damage(int bullet_damage) {
    this->bullet_damage = bullet_damage;
}

int Player::get_bullet_damage() {
    return this->bullet_damage;
}

void Player::increment_life() {
    this->life++;
}

void Player::decrease_life() {
    this->life--; 
}

void Player::increment_shield() {
    this->shield++;
}

void Player::decrease_shield() {
    this->shield--; 
}

void Player::increment_bullet_damage() {
    this->bullet_damage++;
}

// Methods of class Enemy

Enemy::Enemy() {};

Enemy::Enemy(
    int y, int x, int direction, int level
):Entity(y, x, direction) {
    this->level = level;
    this->blocks_traveled = 0;
};

int Enemy::get_level() {
    return this->level;
}

int Enemy::get_blocks_traveled() {
    return this->blocks_traveled;
}

void Enemy::decrease_level(int level_decrease) {
    this->level -= level_decrease;
}

void Enemy::increment_blocks_traveled() {
    this->blocks_traveled++;
}

void Enemy::reset_blocks_traveled() {
    this->blocks_traveled = 0;
}

/* Struct enemy methods */

void add_enemy(enemy_node* &head, Enemy new_enemy) {
    enemy_node *new_head = new enemy_node; 
    new_head->current_enemy = new_enemy;
    new_head->next = head; 
    head = new_head;
}

void delete_enemy_list(enemy_node* &head) {
    // delete all the nodes of the enemy list
    enemy_node *iterator = head; 

    while (iterator != NULL) {
        enemy_node *tmp = iterator; 
        iterator = iterator->next; 
        delete tmp; 
    }

    head = NULL; 
}

void get_random_enemy_position(map *map, Player player, int &y, int &x) {
    // generate random numbers until
    // the position is valid in the map
    bool valid = false;

    while (!valid) {
        y = (rand() % 19) + 1;
        x = (rand() % 59) + 1;

        if (
            map->blocks[y][x] != WALL_BLOCK 
            && (abs(player.get_y() - y)>=10 
            || abs(player.get_x() - x) >= 10)
        ) valid = true; 
    }
}

int get_random_enemy_direction(map *map, int y, int x) {
    // get random enemy direction based
    // on the position of walls
    bool valid = false;
    int new_direction = (rand() + 1) % 4;

    while (!valid) {
        new_direction = (new_direction + 1) % 4;

        // check if next block in this direction is a wall
        int new_y = y;
        int new_x = x;

        switch (new_direction) {
            case DOWN:
                new_y++;
                break;
            case UP:
                new_y--;
                break;
            case RIGHT:
                new_x++;
                break;
            case LEFT:
                new_x--;
                break;
        }

        if (map->blocks[new_y][new_x] != WALL_BLOCK
            && new_y >= 0 && new_y < 20 && new_x >= 0 && new_x < 60)
            valid = true;
    }

    return new_direction;
}

Enemy get_new_enemy(map *map, Player player, int enemy_level) {
    // create and return a new enemy
    int y, x;
    get_random_enemy_position(map, player, y, x);

    return Enemy(
        y, x, 
        get_random_enemy_direction(map, y, x), 
        enemy_level
    );
}

void create_enemy_list(map *map, Player player, enemy_node* &head, int level) {
    if (head != NULL) delete_enemy_list(head);

    // add enemies based on current level
    int max_enemy_level = 1;
    for (int i = 1; i <= level; i++) {
        int enemy_level = i/2;
        
        if (enemy_level < 1) enemy_level = 1;
        if (enemy_level > 9) enemy_level = 9;

        if (enemy_level > max_enemy_level)
            max_enemy_level = enemy_level;
        
        add_enemy(head, get_new_enemy(map, player, enemy_level));
    }

    // add more enemies based on upgrades
    if (player.get_has_pistol())
        add_enemy(head, get_new_enemy(map, player, max_enemy_level));
    if (player.get_faster_bullet_speed())
        add_enemy(head, get_new_enemy(map, player, max_enemy_level));
    if (player.get_bullet_damage() > 1)
        add_enemy(head, get_new_enemy(map, player, max_enemy_level));
}

/* Struct bullet methods */

void add_bullet(bullet_node* &head, Entity new_bullet) {
    bullet_node *new_head = new bullet_node;
    new_head->current_bullet = new_bullet;
    new_head->next = head;
    head = new_head;
}
