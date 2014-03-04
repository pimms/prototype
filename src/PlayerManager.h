#pragma once

#include <SFML/Graphics.hpp>
#include <list>
using std::list;

#include "GameObject.h"

class Player;
class Scene;


struct TimeNode {
public:
	TimeNode(Player *player);
	TimeNode(sf::Vector2f v);
	void Apply(Player *player);

	TimeNode *prev;
	TimeNode *next;
	TimeNode *parent;
	TimeNode *left;
	TimeNode *right;

	sf::Vector2f position;
};

class TimeThread {
public:
	TimeThread(TimeNode *time, Scene *scene);
	~TimeThread();

	bool NeedExpand();
	bool NeedReunion();

	void IncrementPlayback();
	void DecrementPlayback();
	void Record();

	void Expand(TimeThread *&t1, TimeThread *&t2, Scene *scene);
	static TimeThread* Reunite(TimeThread*,TimeThread*,Scene*);

	/* The current story is continued in "cont", and "playerthr" 
	 * starts off with a blank sleet.
	 */
	void Split(TimeThread *&cont, TimeThread *&playerthr, Scene *scene);

	TimeThread* GetSibling();
	void EnableInput();
	void DisableInput();

	bool IsRootThread();

private:
	void CreatePlayer(Scene *scene);

	TimeThread *_sibling;

	TimeNode *_head;
	TimeNode *_tail;
	TimeNode *_current;
	Player *_player;
};

class PlayerManager {
public:
	/* Which direction are we currently moving
	 * in time?
	 */
	enum Direction {
		FORWARD,
		BACKWARD,
	};


	PlayerManager(Scene *scene);
	~PlayerManager();

	/* Update must be called AFTER user input has been 
	 * handled and threads been split.
	 */
	void Update();

	void SplitThread();

private:
	list<TimeThread*>::iterator EraseThread(TimeThread *thread);

	Scene *_scene;

	TimeThread *_playThread;
	list<TimeThread*> _threads;

	Direction _direction;
};


