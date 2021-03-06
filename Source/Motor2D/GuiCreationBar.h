#ifndef __GUI_CREATION_BAR_H__
#define __GUI_CREATION_BAR_H__

#include "GuiElement.h"

//class Entity;
class StaticObject;

class GuiCreationBar : public GuiElement
{
public:
	GuiCreationBar(GUI_ELEMENT_TYPE type, int x, int y, bool is_visible, SDL_Rect* creation_bar, SDL_Rect* background, Entity* attached_entity);
	GuiCreationBar();

	bool Draw();

	void CheckInput();

	void CleanUp();

public:
	void UpdateCreationBarValue();							// Will update the creation bar value (creation_bar.w) according to how much time has passed since creation started.
	void SetNewCreationTime(float new_creation_time);		// Will set a new creation time and will reset the creation_finished to false.
	void ResetCreationBar();								// Will reset the creation bar back to 0. Will also reset creation_finished to false.

	float GetAccumulatedTime();							// Will return the current accumulated time.

public:
	bool creation_finished;									// Will track whether or not the creation process has finished or not.

private:
	SDL_Texture*	tex;

	SDL_Rect		background;								// Size and sprite section of the creation bar.
	SDL_Rect		creation_bar;							// Size and sprite section of the creation bar background. Can be nullptr.

	Entity*			attached_entity;						// Entity to which the creation bar will be attached to.

	float			creation_time;							// The time required to creation bar something (normally an entity).
	float			accumulated_time;						// The total amount of time that has elapsed since creation started.
};

#endif // !__GUI_CREATION_BAR_H