#include "PlayerManager.h"
#include "Player.h"
#include "Scene.h"
#include "Game.h"
#include "Input.h"


/*
 * TimeNode
 */

TimeNode::TimeNode(Player *player)
	: 	prev(NULL),
		next(NULL),
		parent(NULL),
	  	left(NULL),
		right(NULL)
{
	position = player->GetPosition();
}

TimeNode::TimeNode(sf::Vector2f v) 
	: 	position(v),
	 	prev(NULL),
		next(NULL),
		parent(NULL),
	  	left(NULL),
		right(NULL)
{

}

void TimeNode::Apply(Player *player)
{
	player->SetPosition(position);
}




/*
 * TimeThread
 */

bool TimeThread::AreSiblings(TimeThread *t1, TimeThread *t2) 
{
	if (t1 == t2) {
		return false;
	}

	if (!t1->_splits.size() || !t2->_splits.size()) {
		return false;
	}

	return t1->_splits.back() == t2->_splits.back();
}

TimeThread::TimeThread(TimeNode *time, Scene *scene)
	:	_head(time),
		_tail(time),
		_current(time),
		_player(NULL)
{
	CreatePlayer(scene);
}

TimeThread::~TimeThread()
{
	if (_player) {
		_player->GetGame()->GetScene()->RemoveGameObject(_player);
		delete _player;
	}
}

bool TimeThread::NeedExpand()
{
	/* The TimeThread must expand when the PlayerManager is
	 * playing back recorded events, the thread has reached
	 * the tail, and the head has children.
	 */
	return (_current == _tail) && (_tail->left && _tail->right);
}

bool TimeThread::NeedReunion()
{
	/* Reunion is required when playing the time in reverse
	 * and the current node is the head.
	 */
	return (_current == _head);
}

void TimeThread::IncrementPlayback() 
{
	if (NeedExpand()) {
		printf("ERROR: SHOULD HAVE EXPANDED\n");
		return;
	}

	if (_current != _tail) {
		_current = _current->next;
		_current->Apply(_player);
	} else {
		Record();
	}
}

void TimeThread::DecrementPlayback()
{
	if (NeedReunion()) {
		printf("ERROR: SHOULD HAVE REUNITED\n");
		return;
	}
	
	_current = _current->prev;
	_current->Apply(_player);
}

void TimeThread::Record()
{
	if (_current != _tail) {
		printf("RECORDING BUT NOT AT END\n");
	}

	TimeNode *node = new TimeNode(_player);

	_tail->next = node;
	node->prev = _tail;
	_tail = node;

	_current = _tail;
}

void TimeThread::CreatePlayer(Scene *scene) 
{
	_player = new Player(scene->GetGame());
	scene->AddGameObject(_player);

	_head->Apply(_player);
}

void TimeThread::Expand(TimeThread *&t1, TimeThread *&t2, Scene *scene)
{
	Fork(t1, _tail->left, t2, _tail->right, scene);

	while (t1->_tail->next) 
		t1->_tail = t1->_tail->next;
	while (t2->_tail->next) 
		t2->_tail = t2->_tail->next;
}

TimeThread* TimeThread::Reunite(TimeThread *t1, TimeThread *t2,
								Scene *scene)
{
	if (!AreSiblings(t1, t2)) {
		printf("TimeThread::Reunite(): T1 and T2 are not siblings!\n");
		return NULL;
	}

	TimeNode *head = t1->_head->parent;
	TimeNode *tail = head;
	while (head->prev) { head = head->prev; }

	TimeThread *t = new TimeThread(head, scene);
	t->_head = head;
	t->_tail = tail;
	t->_current = tail;
	t->_current->Apply(t->_player);

	t->_splits = t1->_splits;
	t->_splits.pop_back();

	return t;
}

void TimeThread::Split(TimeThread *&cont, TimeThread *&plthread, 
					   Scene *scene)
{
	if (!_current->prev) {
		printf("Split called on thread at the first node\n");
		return;
	}

	TimeNode *orig = _current;
	TimeNode *copy = new TimeNode(_player);
	TimeNode *oldTail = _tail;
	
	// Update tail and current
	_tail = _current->prev;
	_tail->left = orig;
	_tail->right = copy;
	_tail->next = NULL;
	_current = _tail;

	// Create the new TimeThreads
	Fork(cont, orig, plthread, copy, scene);
	cont->_tail = oldTail;

	// Update the parents of the new TimeNodes
	orig->parent = _tail;
	copy->parent = _tail;
}

void TimeThread::EnableInput()
{
	_player->SetInputAvailable(true);
}

void TimeThread::DisableInput()
{
	_player->SetInputAvailable(false);
}

bool TimeThread::IsRootThread()
{
	return _splits.size() == 0;
}

TimeNode* TimeThread::GetHead()
{
	return _head;
}

void TimeThread::AssertSaneThreadStatus() 
{
	bool sane = true;

	if (_tail && _tail->next) {
		printf("Tail has a next node\n");
		sane = false;
	}
	
	if (_current != _tail && _current->next == NULL) {
		printf("Non-tail current does not have a next\n");
		sane = false;
	}

	if (!sane) {
		getchar();
	}
}

void TimeThread::Fork(TimeThread *&t1, TimeNode *n1,  
				  	  TimeThread *&t2, TimeNode *n2, Scene *scene)
{
	t1 = new TimeThread(n1, scene);
	t2 = new TimeThread(n2, scene);

	t1->_splits = _splits;
	t2->_splits = _splits;

	t1->_splits.push_back(_tail);
	t2->_splits.push_back(_tail);
}




/*
 * PlayerManager
 */

PlayerManager::PlayerManager(Scene *scene)
	:	_scene(scene),
		_direction(FORWARD),
		_playThread(NULL)
{
	RestartAllThreads();
}

PlayerManager::~PlayerManager()
{
	
}

void PlayerManager::Update()
{
	if (_scene->GetGame()->GetInput()->IsKeyFresh(sf::Keyboard::T)) {
		SetTimeDirection((_direction==FORWARD)?BACKWARD:FORWARD);
	}

	list<TimeThread*>::iterator it = _threads.begin();
	for (; it != _threads.end(); it++) {
		TimeThread *thread = *it;
		thread->AssertSaneThreadStatus();

		 if (_direction == FORWARD) {
			if (thread == _playThread) {
				thread->Record();
			} else if (thread->NeedExpand()) {
				it = ExpandThread(thread);
			} else {
				thread->IncrementPlayback();
			}
		} else if (_direction == BACKWARD) {
			if (thread->NeedReunion()) {
				if (thread->IsRootThread()) {
					SetTimeDirection(FORWARD);
					return;
				} else {
					it = ReuniteThread(thread);
				}
			} else {
				thread->DecrementPlayback();
			}
		}
	}
}

void PlayerManager::RestartAllThreads()
{
	printf("Restarting all threads\n");

	TimeNode *rootNode = NULL;
	list<TimeThread*>::iterator it = _threads.begin();

	for (; it != _threads.end(); it++) {
		TimeThread *thread = *it;

		if (thread->IsRootThread()) {
			rootNode = thread->GetHead();
			it = EraseThread(thread);
			delete thread;
		} else {
			TimeThread *t1, *t2;
			thread = TimeThread::Reunite(thread, t2, _scene);
			_threads.push_back(thread);
			it = EraseThread(t1);
			it = EraseThread(t2);
			delete t1; 
			delete t2;
		}
	}

	if (rootNode) {
		DeleteSubtree(rootNode);
	}

	TimeNode *node = new TimeNode(sf::Vector2f(100.f,100.f));
	_playThread = new TimeThread(node, _scene);
	_threads.push_back(_playThread);

	_playThread->EnableInput();
}

void PlayerManager::SplitThread()
{
	printf("Splitting thread\n");

	TimeThread *t1, *t2;
	_playThread->Split(t1, t2, _scene);

	EraseThread(_playThread);
	_threads.push_back(t1);
	_threads.push_back(t2);

	delete _playThread;
	_playThread = t2;
	_playThread->EnableInput();
}

void PlayerManager::SetTimeDirection(Direction dir)
{
	if (dir == _direction) {
		return;
	}

	printf("Changing time direction: %i\n", (dir==FORWARD)?1:-1);

	_direction = dir;
	if (dir == FORWARD) {
		if (_playThread->NeedReunion() && _playThread->IsRootThread()) {
			RestartAllThreads();
		} else {
			SplitThread();
		}
		_playThread->EnableInput();
	} else if (dir == BACKWARD) {
		_playThread->DisableInput();
	}
}

TimeThread* PlayerManager::GetSibling(TimeThread *thread) const
{
	list<TimeThread*>::const_iterator it = _threads.begin();
	for (; it != _threads.end(); it++) {
		if (TimeThread::AreSiblings(*it, thread)) {
			return *it;
		}
	}

	return NULL;
}

list<TimeThread*>::iterator PlayerManager::EraseThread(TimeThread *t) 
{
	printf("Erasing thread\n");

	list<TimeThread*>::iterator it = _threads.begin();

	for (; it != _threads.end(); it++) {
		if (*it == t) {
			return _threads.erase(it);
		}
	}

	return _threads.end();
}

list<TimeThread*>::iterator PlayerManager::ExpandThread(TimeThread *t)
{
	printf("Expanding thread\n");

	TimeThread *t1, *t2;
	list<TimeThread*>::iterator it;

	t->Expand(t1, t2, _scene);
	it = EraseThread(t);
	delete t;

	_threads.push_back(t1);
	_threads.push_back(t2);

	return it;
}

list<TimeThread*>::iterator PlayerManager::ReuniteThread(TimeThread *t)
{
	TimeThread *t1, *t2;
	list<TimeThread*>::iterator it;

	t1 = t;
	t2 = GetSibling(t1);
	
	if (!t2) {
		printf("FAILED TO FIND SIBLING OF TIMETHREAD\n");
		getchar();
		return _threads.end();
	}

	t = TimeThread::Reunite(t1, t2, _scene);
	_threads.push_back(t);

	if (_playThread == t1 || _playThread == t2) {
		_playThread = t;
	}

	it = EraseThread(t1);
	it = EraseThread(t2);
	delete t1;
	delete t2;
	
	return it;
}

void PlayerManager::DeleteSubtree(TimeNode *tree) 
{
	list<TimeNode*> queue;
	queue.push_back(tree);

	while (queue.size()) {
		TimeNode *n = queue.front();
		queue.pop_front();

		while (n->next) {
			TimeNode *nxt = n->next;
			delete n;
			n = nxt;
		}
	
		if (n) {
			if (n->left) 
				queue.push_back(n->left);
			if (n->right) 
				queue.push_back(n->right);
			delete n;
		}
	}
}
