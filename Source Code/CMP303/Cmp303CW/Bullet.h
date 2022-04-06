#pragma once

#include "Framework/GameObject.h"
#include "Player.h"

struct PositionMessagesType;
enum class BulletType { NORMAL, HOMING };

class Bullet : public GameObject
{
public:
	Bullet(int id,BulletType bulletType,sf::Vector2f velocity, sf::Vector2f position,float gameTime);
	~Bullet();

	bool update(float dt, sf::Vector2f mousePos,int id, float gameTime);

	int getId();
	void setId(int newId);

	BulletType getType();
	int getDamage();

	void addMessage(PositionMessagesType ppmt);

	sf::Vector2f predict(float time);

	float getTimeSpawned();
private:
	int id = -1;
	int damage = 0;
	float counter = 0;
	float timeSpawned;
	sf::Texture texture;
	BulletType type;

	std::vector<PositionMessagesType> homingPositionMessages;
	std::vector<PositionMessagesType> homingPredictions;
};

