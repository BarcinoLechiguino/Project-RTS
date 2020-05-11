#ifndef __DIALOGSYSTEM_H__
#define __DIALOGSYSTEM_H__

#include <string>
#include <vector>
#include <queue>
#include "Module.h"
#include "SDL\include\SDL_pixels.h"
#include "Timer.h"
#include "Point.h"

struct _TTF_Font;

//NPC phrase
class DialogBubble
{
public:
	DialogBubble();
	DialogBubble(const char text, int node_id);

public:
	std::vector<std::string> text; //Different lines in text
	int active_time;
	int bubble_id;

};

//Tree storing all the nodes and options
class Dialog
{
public:
	Dialog();

	void NextBubble();
public:
	std::vector <DialogBubble*> dialog_bubbles;
	iPoint position;
	int current_bubble;
	int dialog_id;
	int last_id;
};

class DialogSystem : public Module
{
public: 
	DialogSystem();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpddate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void StartDialog(int tree_id);
	void NextDialog();
	void DrawDialog();

	void NextBubbleCheck(); //checks if time or input to go to next dialog
	void LoadDialogTextures();

	bool LoadDialog();
	bool LoadTextBubbles(Dialog* dialog_tree, pugi::xml_node tree);

public:
	std::vector <Dialog*> dialogs; //Dialogs loaded available
	
	std::queue<Dialog*> dialog_queue; //queue storing pending dialogs
	Dialog* current_dialog;

	Timer* timer;

	SDL_Texture* border_texture;
	std::vector<SDL_Texture*> text_texture;
	SDL_Color dialog_color;
	_TTF_Font* dialog_font;
	int font_size;

	bool is_dialog_active;

public:

	pugi::xml_document dialog_file;
};
#endif // __DIALOGSYSTEM_H__