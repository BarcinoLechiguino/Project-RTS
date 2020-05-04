#ifndef __ENEMY_TOWNHALL_H__
#define __ENEMY_TOWNHALL_H__

#include "StaticObject.h"

class EnemyTownHall : public StaticObject
{
public:

	EnemyTownHall(int x, int y, ENTITY_TYPE type, int level);

	virtual bool Awake(pugi::xml_node&);

	virtual bool PreUpdate();

	virtual bool Update(float dt, bool doLogic);

	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual void Draw();

public:
	void InitEntity();
	void AttachHealthbarToEntity();
	void AttachCreationBarToEntity();

	void StartUnitCreation();

	void GenerateUnit(ENTITY_TYPE type, int level);

	void LevelChanges();

public:
	SDL_Rect	hall_rect;
	SDL_Rect	hall_rect_1;
	SDL_Rect	hall_rect_2;

	int			unit_level;

	float		enemy_gatherer_creation_time;

	ENTITY_TYPE created_unit_type;
	bool		creating_unit;
};

#endif // !__ENEMY_TOWNHALL_H__
