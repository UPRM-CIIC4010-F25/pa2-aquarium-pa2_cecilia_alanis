#include "Aquarium.h"
#include <cstdlib>


string AquariumCreatureTypeToString(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return "BiggerFish";
        case AquariumCreatureType::NPCreature:
            return "BaseFish";
        case AquariumCreatureType::NemoFish:
            return "NemoFish";
        case AquariumCreatureType::DoryFish:
            return "DoryFish";
        default:
            return "UknownFish";
    }
}

// PlayerCreature Implementation
PlayerCreature::PlayerCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 10.0f, 1, sprite) {}


void PlayerCreature::setDirection(float dx, float dy) {
    m_dx = dx;
    m_dy = dy;
    normalize();
}

void PlayerCreature::move() {
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    this->bounce();
}

void PlayerCreature::reduceDamageDebounce() {
    if (m_damage_debounce > 0) {
        --m_damage_debounce;
    }
}
void PlayerCreature::bump(){
    
    m_dx = -m_dx;
    m_dy = -m_dy;

    //Move slightly in the new direction
    m_x += m_dx * m_speed * 2.0f;
    m_y += m_dy * m_speed * 2.0f;

   
}
//Magnet PowerUp setup
void PlayerCreature::activateMagnet(int duration){
    m_magnetActive = true;
    m_magnetTimer = duration;
}

void PlayerCreature::update() {
    this->reduceDamageDebounce();
    this->move();
    //Magnet PowerUp
    if (m_magnetActive){
        if(--m_magnetTimer <= 0){
            m_magnetActive = false;
        }

    }
}


void PlayerCreature::draw() const {
    
    ofLogVerbose() << "PlayerCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    if (this->m_damage_debounce > 0) {
        ofSetColor(ofColor::red); // Flash red if in damage debounce
    }
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
    ofSetColor(ofColor::white); // Reset color

}

void PlayerCreature::changeSpeed(int speed) {
    m_speed = speed;
}

void PlayerCreature::loseLife(int debounce) {
    if (m_damage_debounce <= 0) {
        if (m_lives > 0) this->m_lives -= 1;
        m_damage_debounce = debounce; // Set debounce frames
        ofLogNotice() << "Player lost a life! Lives remaining: " << m_lives << std::endl;
    }
    // If in debounce period, do nothing
    if (m_damage_debounce > 0) {
        ofLogVerbose() << "Player is in damage debounce period. Frames left: " << m_damage_debounce << std::endl;
    }
}


// NPCreature Implementation
NPCreature::NPCreature(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: Creature(x, y, speed, 30, 1, sprite) {
    m_dx = (rand() % 3 - 1); // -1, 0, or 1
    m_dy = (rand() % 3 - 1); // -1, 0, or 1
    normalize();

    m_creatureType = AquariumCreatureType::NPCreature;
}

void NPCreature::move() {
    // Simple AI movement logic (random direction)
    m_x += m_dx * m_speed;
    m_y += m_dy * m_speed;
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }
    bounce();
}

void NPCreature::draw() const {
    ofLogVerbose() << "NPCreature at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    ofSetColor(ofColor::white);
    if (m_sprite) {
        m_sprite->draw(m_x, m_y);
    }
}


BiggerFish::BiggerFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();

    setCollisionRadius(60); // Bigger fish have a larger collision radius
    m_value = 5; // Bigger fish have a higher value
    m_creatureType = AquariumCreatureType::BiggerFish;
}

void BiggerFish::move() {
    // Bigger fish might move slower or have different logic
    m_x += m_dx * (m_speed * 0.5); // Moves at half speed
    m_y += m_dy * (m_speed * 0.5);
    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }else {
        this->m_sprite->setFlipped(false);
    }

    bounce();
}

void BiggerFish::draw() const {
    ofLogVerbose() << "BiggerFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);
}

// NemoFish Implementation
NemoFish::NemoFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();
    setCollisionRadius(36);
    m_value = 3;
    m_creatureType = AquariumCreatureType::NemoFish;

}

void NemoFish::move() {
// Distinct movement behavior: Fast and Erractic

    // Moves two times faster
    m_x += m_dx * (m_speed * 2.0);
    m_y += m_dy * (m_speed * 2.0);

    // Erratic behavior: 25% chance of changing direction after each move
    if(rand() % 100 < 25) {
        m_dx = (rand() % 5 - 2);
        m_dy = (rand() % 5 - 2);
        normalize();

        //Sharp turns when changing direction (10% chance) 
        if(rand() % 10 == 0) {
            m_dx *= 3.0;
            m_dy *= 3.0;
            normalize();
        }
    }

    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }
    else {
        this->m_sprite->setFlipped(false);
    }
    bounce();
}

void NemoFish::draw() const {
    ofLogVerbose() << "NemoFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);

}

// DoryFish Implementation
DoryFish::DoryFish(float x, float y, int speed, std::shared_ptr<GameSprite> sprite)
: NPCreature(x, y, speed, sprite) {
    m_dx = (rand() % 3 - 1);
    m_dy = (rand() % 3 - 1);
    normalize();
    setCollisionRadius(48);
    m_value = 4;
    m_creatureType = AquariumCreatureType::DoryFish;
    m_movementTimer = 60 + rand() % 60;
}

void DoryFish::move() {
// Distinct movement behavior: Slow and Forgetful

    // Moves at half the speed
    m_x += m_dx * (m_speed * 0.5);
    m_y += m_dy * (m_speed * 0.5);

    if(m_movementTimer <= 0) {

        int action = rand() % 100; // Calculates a percentage to determine which of Dory's movements will be performed

        if(action < 40) {   // 40% chance of Stop and Pause
            m_dx = 0;
            m_dy = 0;
            m_movementTimer = 30 + rand() % 30;     // Dory pauses for 30 to 60 frames
        }

        else if(action < 80) {   // 40% chance of changing direction randomly
            m_dx = (rand() % 3 - 1);
            m_dy = (rand() % 3 - 1);
            normalize();
            m_movementTimer = 60 + rand() % 60;     // Keeps new direction for 1 to 2 seconds
        }
        else {  // 20% chance of rapid direction changes
            m_dx = (rand() % 3 - 1);
            m_dy = (rand() % 3 - 1);
            normalize();
            m_movementTimer = 10 + rand() % 20;
        }
    }

    else {
        m_movementTimer--;
    }

    if(m_dx < 0 ){
        this->m_sprite->setFlipped(true);
    }
    else {
        this->m_sprite->setFlipped(false);
    }
    bounce();
}

void DoryFish::draw() const {
    ofLogVerbose() << "DoryFish at (" << m_x << ", " << m_y << ") with speed " << m_speed << std::endl;
    this->m_sprite->draw(this->m_x, this->m_y);

}

// AquariumSpriteManager
AquariumSpriteManager::AquariumSpriteManager(){
    this->m_npc_fish = std::make_shared<GameSprite>("base-fish.png", 70,70);
    this->m_big_fish = std::make_shared<GameSprite>("bigger-fish.png", 120, 120);
    this->m_powerup = std::make_shared<GameSprite>("magnet_powerup.png", 64, 64);
    this->m_nemo_fish = std::make_shared<GameSprite>("Nemo.png", 85, 85);
    this->m_dory_fish = std::make_shared<GameSprite>("Dory.png", 100, 100);
}

std::shared_ptr<GameSprite> AquariumSpriteManager::GetSprite(AquariumCreatureType t){
    switch(t){
        case AquariumCreatureType::BiggerFish:
            return std::make_shared<GameSprite>(*this->m_big_fish);
            
        case AquariumCreatureType::NPCreature:
            return std::make_shared<GameSprite>(*this->m_npc_fish);

        case AquariumCreatureType::PowerUp: return m_powerup;

        case AquariumCreatureType::NemoFish:
            return std::make_shared<GameSprite>(*this->m_nemo_fish);

        case AquariumCreatureType::DoryFish:
            return std::make_shared<GameSprite>(*this->m_dory_fish);

        default:
            return nullptr;
    }
}


// Aquarium Implementation
Aquarium::Aquarium(int width, int height, std::shared_ptr<AquariumSpriteManager> spriteManager)
    : m_width(width), m_height(height) {
        m_sprite_manager =  spriteManager;
    }



void Aquarium::addCreature(std::shared_ptr<Creature> creature) {
    creature->setBounds(m_width - 20, m_height - 20);
    m_creatures.push_back(creature);
}

void Aquarium::addAquariumLevel(std::shared_ptr<AquariumLevel> level){
    if(level == nullptr){return;} // guard to not add noise
    this->m_aquariumlevels.push_back(level);
}

void Aquarium::update(std::shared_ptr<PlayerCreature> player) {
    for (auto& creature : m_creatures) {
        creature->move();
    }
    //Magnet PowerUp implementation
    if (player->isMagnetActive()){
        for (auto& creature : m_creatures){
            if(creature -> getValue() < player->getPower()){
                float dx = player->getX() - creature->getX();
                float dy = player->getY() - creature->getY();
                float distance = std::sqrt(dx*dx +dy*dy);
                if (distance > 1.0f){
                        dx /= distance;
                        dy /=distance;
                        creature -> setSpeed(4);
                        creature -> setDirection(dx,dy);
                    }
            }
        }

    }
    this->Repopulate();
}

void Aquarium::draw() const {
    for (const auto& creature : m_creatures) {
        creature->draw();
    }
}


void Aquarium::removeCreature(std::shared_ptr<Creature> creature) {
    auto it = std::find(m_creatures.begin(), m_creatures.end(), creature);
    if (it != m_creatures.end()) {
        ofLogVerbose() << "removing creature " << endl;
        int selectLvl = this->currentLevel % this->m_aquariumlevels.size();
        auto npcCreature = std::static_pointer_cast<NPCreature>(creature);
        this->m_aquariumlevels.at(selectLvl)->ConsumePopulation(npcCreature->GetType(), npcCreature->getValue());
        m_creatures.erase(it);
    }
}

void Aquarium::clearCreatures() {
    m_creatures.clear();
}

std::shared_ptr<Creature> Aquarium::getCreatureAt(int index) {
    if (index < 0 || size_t(index) >= m_creatures.size()) {
        return nullptr;
    }
    return m_creatures[index];
}



void Aquarium::SpawnCreature(AquariumCreatureType type) {
    int x = rand() % this->getWidth();
    int y = rand() % this->getHeight();
    int speed = 1 + rand() % 25; // Speed between 1 and 25

    switch (type) {
        case AquariumCreatureType::NPCreature:
            this->addCreature(std::make_shared<NPCreature>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::NPCreature)));
            break;
        case AquariumCreatureType::BiggerFish:
            this->addCreature(std::make_shared<BiggerFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::BiggerFish)));
            break;
        case AquariumCreatureType::NemoFish:
            this->addCreature(std::make_shared<NemoFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::NemoFish)));
            break;
        case AquariumCreatureType::DoryFish:
            this->addCreature(std::make_shared<DoryFish>(x, y, speed, this->m_sprite_manager->GetSprite(AquariumCreatureType::DoryFish)));
            break;
        default:
            ofLogError() << "Unknown creature type to spawn!";
            break;
    }

}


// repopulation will be called from the levl class
// it will compose into aquarium so eating eats frm the pool of NPCs in the lvl class
// once lvl criteria met, we move to new lvl through inner signal asking for new lvl
// which will mean incrementing the buffer and pointing to a new lvl index
void Aquarium::Repopulate() {
    ofLogVerbose("entering phase repopulation");
    // lets make the levels circular
    int selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
    ofLogVerbose() << "the current index: " << selectedLevelIdx << endl;
    std::shared_ptr<AquariumLevel> level = this->m_aquariumlevels.at(selectedLevelIdx);


    if(level->isCompleted()){
        level->levelReset();
        this->currentLevel += 1;
        selectedLevelIdx = this->currentLevel % this->m_aquariumlevels.size();
        ofLogNotice()<<"new level reached : " << selectedLevelIdx << std::endl;
        level = this->m_aquariumlevels.at(selectedLevelIdx);
        this->clearCreatures();
    }

    
    // now lets find how many to respawn if needed 
    std::vector<AquariumCreatureType> toRespawn = level->Repopulate();
    ofLogVerbose() << "amount to repopulate : " << toRespawn.size() << endl;
    if(toRespawn.size() <= 0 ){return;} // there is nothing for me to do here
    for(AquariumCreatureType newCreatureType : toRespawn){
        this->SpawnCreature(newCreatureType);
    }
}


// Aquarium collision detection
std::shared_ptr<GameEvent> DetectAquariumCollisions(std::shared_ptr<Aquarium> aquarium, std::shared_ptr<PlayerCreature> player) {
    for (int i = 0; i < aquarium->getCreatureCount(); i++){
        auto creature = aquarium->getCreatureAt(i);
        if(creature && creature != player && checkCollision(player, creature)){
            return std::make_shared<GameEvent>(GameEventType::COLLISION, player, creature);
    }
}
    if (!aquarium || !player) return nullptr;
    
    for (int i = 0; i < aquarium->getCreatureCount(); ++i) {
        std::shared_ptr<Creature> npc = aquarium->getCreatureAt(i);
        if (npc && checkCollision(player, npc)) {
            return std::make_shared<GameEvent>(GameEventType::COLLISION, player, npc);
        }
    }
    return nullptr;
};

//  Imlementation of the AquariumScene

void AquariumGameScene::Update(){
    std::shared_ptr<GameEvent> event;

    this->m_player->update();

    if (this->updateControl.tick()) {
        event = DetectAquariumCollisions(this->m_aquarium, this->m_player);
        if (event != nullptr && event->isCollisionEvent()) {
            ofLogVerbose() << "Collision detected between player and NPC!" << std::endl;
            if(event->creatureB != nullptr){
                event->print();
                //Magnet PowerUp collision check
                if (auto power = std::dynamic_pointer_cast<PowerUp>(event->creatureB)){
                    if(power->getPowerUpType()==PowerUpType::MAGNET_CORE){
                        this->m_player->activateMagnet(10*60);
                        this->m_aquarium->removeCreature(power);
                        ofLogNotice()<<"Magnet Core activated!";
                        return;
                    }
                }
                if(this->m_player->getPower() < event->creatureB->getValue()){
                    ofLogNotice() << "Player is too weak to eat the creature!" << std::endl;
                    this->m_player->loseLife(3*60); // 3 frames debounce, 3 seconds at 60fps
                    this->m_player->bump();
                    if(this->m_player->getLives() <= 0){
                        this->m_lastEvent = std::make_shared<GameEvent>(GameEventType::GAME_OVER, this->m_player, nullptr);
                        return;
                    }
                    
                }
                else{
                    this->m_aquarium->removeCreature(event->creatureB);
                    this->m_player->addToScore(1, event->creatureB->getValue());
                    if (this->m_player->getScore() % 25 == 0){
                        this->m_player->increasePower(1);
                        ofLogNotice() << "Player power increased to " << this->m_player->getPower() << "!" << std::endl;
                    }
                
                //Magnet Core PowerUp to spawn
                if(this->m_player->getScore()>=30 && this->m_player->getScore() % 30==0){
                    auto magnet = std::make_shared<PowerUp>(
                        rand() % this-> m_aquarium->getWidth(),
                        rand() % this-> m_aquarium->getHeight(),
                        this->m_aquarium->getSpriteManager()->GetSprite(AquariumCreatureType::PowerUp),
                        PowerUpType::MAGNET_CORE
                    );
                
                this->m_aquarium->addCreature(magnet);
                ofLogNotice() << "Magnet Core spawned";
                }
            }
                

            } else {
                ofLogError() << "Error: creatureB is null in collision event." << std::endl;
            }
        }
        this->m_aquarium->update(m_player);
    }

}

void AquariumGameScene::Draw() {
    this->m_player->draw();
    this->m_aquarium->draw();
    this->paintAquariumHUD();

}


void AquariumGameScene::paintAquariumHUD(){
    float panelWidth = ofGetWindowWidth() - 150;
    ofDrawBitmapString("Score: " + std::to_string(this->m_player->getScore()), panelWidth, 20);
    ofDrawBitmapString("Power: " + std::to_string(this->m_player->getPower()), panelWidth, 30);
    ofDrawBitmapString("Lives: " + std::to_string(this->m_player->getLives()), panelWidth, 40);
    for (int i = 0; i < this->m_player->getLives(); ++i) {
        ofSetColor(ofColor::red);
        ofDrawCircle(panelWidth + i * 20, 50, 5);
    }
    ofSetColor(ofColor::white); // Reset color to white for other drawings
}

void AquariumLevel::populationReset(){
    for(auto node: this->m_levelPopulation){
        node->currentPopulation = 0; // need to reset the population to ensure they are made a new in the next level
    }
}

void AquariumLevel::ConsumePopulation(AquariumCreatureType creatureType, int power){
    for(std::shared_ptr<AquariumLevelPopulationNode> node: this->m_levelPopulation){
        ofLogVerbose() << "consuming from this level creatures" << endl;
        if(node->creatureType == creatureType){
            ofLogVerbose() << "-cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            if(node->currentPopulation == 0){
                return;
            } 
            node->currentPopulation -= 1;
            ofLogVerbose() << "+cosuming from type: " << AquariumCreatureTypeToString(node->creatureType) <<" , currPop: " << node->currentPopulation << endl;
            this->m_level_score += power;
            return;
        }
    }
}

bool AquariumLevel::isCompleted(){
    return this->m_level_score >= this->m_targetScore;
}

std::vector<AquariumCreatureType> Level_0::Repopulate() {
    return RepopulateTemplate();
}

std::vector<AquariumCreatureType> Level_1::Repopulate() {
    return RepopulateTemplate();
}

std::vector<AquariumCreatureType> Level_2::Repopulate() {
    return RepopulateTemplate();
}

std::vector<AquariumCreatureType> Level_3::Repopulate() {
    return RepopulateTemplate();
}

std::vector<AquariumCreatureType> Level_4::Repopulate() {
    return RepopulateTemplate();
}

std::vector<AquariumCreatureType> Level_5::Repopulate() {
    return RepopulateTemplate();
}