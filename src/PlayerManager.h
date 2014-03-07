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
	static bool AreSiblings(TimeThread *t1, TimeThread *t2);

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
	TimeNode* GetHead();

	void AssertSaneThreadStatus();

private:
	void CreatePlayer(Scene *scene);

	void Fork(TimeThread *&t1, TimeNode *n1,  
			  TimeThread *&t2, TimeNode *n2, Scene *scene);

	/* A list of the previous split-points upwards in the 
	 * tree. Sibling threads can be identified as the back of
	 * their _splits-list will be identical.
	 */
	list<TimeNode*> _splits;

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


private:
	void RestartAllThreads();

	void SplitThread();
	void SetTimeDirection(Direction dir);

	TimeThread* GetSibling(TimeThread *thread) const;

	list<TimeThread*>::iterator EraseThread(TimeThread *thread);
	list<TimeThread*>::iterator ExpandThread(TimeThread *thread);
	list<TimeThread*>::iterator ReuniteThread(TimeThread *thread);

	/* Delete the three rooted in "tree". */
	void DeleteSubtree(TimeNode *tree);

	Scene *_scene;

	TimeThread *_playThread;
	list<TimeThread*> _threads;

	Direction _direction;
};


