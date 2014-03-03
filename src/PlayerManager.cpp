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

TimeThread::TimeThread(TimeNode *time, Scene *scene)
	:	_head(time),
		_tail(time),
		_current(time),
		_player(NULL),
		_sibling(NULL)
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

	if (_current->next) {
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
	t1 = new TimeThread(_head->left, scene);
	t2 = new TimeThread(_head->right, scene);

	t1->_sibling = t2;
	t2->_sibling = t1;
}

TimeThread* TimeThread::Reunite(TimeThread *t1, TimeThread *t2,
								Scene *scene)
{
	if (t1->_sibling != t2 || t2->_sibling != t1) {
		printf("TimeThread::Reunite(): T1 and T2 are not siblings!\n");
		return NULL;
	}

	TimeThread *t = new TimeThread(t1->_head->parent, scene);
	return t;
}

void TimeThread::Split(TimeThread *&cont, TimeThread *&plthread, 
					   Scene *scene)
{
	if (!_current->prev) {
		printf("Split called on thread at the first node\n");
		return;
	}
	
	_current = _tail = _current->prev;
	TimeNode *copy = new TimeNode(_player);

	_current->left = _current->next;
	_current->right = copy;
	_current->next = NULL;

	_current->left->parent = _current;
	_current->right->parent = _current;
	
	cont = new TimeThread(_current->left, scene);
	plthread = new TimeThread(copy, scene);

	cont->_sibling = plthread;
	plthread->_sibling = cont;

}

TimeThread* TimeThread::GetSibling()
{
	return _sibling;
}

void TimeThread::EnableInput()
{
	_player->SetInputAvailable(true);
}

void TimeThread::DisableInput()
{
	_player->SetInputAvailable(false);
}



/*
 * PlayerManager
 */

PlayerManager::PlayerManager(Scene *scene)
	:	_scene(scene),
		_direction(FORWARD),
		_playThread(NULL)
{
	TimeNode *node = new TimeNode(sf::Vector2f(100.f,100.f));
	_playThread = new TimeThread(node, scene);
	_threads.push_back(_playThread);

	_playThread->EnableInput();
}

PlayerManager::~PlayerManager()
{
	
}

void PlayerManager::Update()
{
	if (_scene->GetGame()->GetInput()->IsKeyFresh(sf::Keyboard::J)) {
		SplitThread();
	}

	if (_scene->GetGame()->GetInput()->IsKeyFresh(sf::Keyboard::T)) {
		_direction = ((_direction == FORWARD) ? BACKWARD : FORWARD);
		
		(_direction==FORWARD) 
			? _playThread->EnableInput()
			: _playThread->DisableInput();
	}

	list<TimeThread*>::iterator it = _threads.begin();
	for (; it != _threads.end(); it++) {
		TimeThread *thread = *it;

		if (thread == _playThread) {
			thread->Record();
		} else if (_direction == FORWARD) {
			if (thread->NeedExpand()) {
				TimeThread *t1, *t2;
				thread->Expand(t1, t2, _scene);
				_threads.remove(thread);
				_threads.push_back(t1);
				_threads.push_back(t2);
				delete thread;
			} else {
				thread->IncrementPlayback();
			}
		} else if (_direction == BACKWARD) {
			if (thread->NeedReunion()) {
				TimeThread *t1, *t2;
				t1 = thread;
				t2 = t2->GetSibling();
				thread = TimeThread::Reunite(t1, t2, _scene);
				_threads.push_back(thread);

				if (_playThread == t1 || _playThread == t2) {
					_playThread = thread;
					_playThread->EnableInput();
				}

				it = EraseThread(t1);
				it = EraseThread(t2);
				delete t1;
				delete t2;
			} else {
				thread->DecrementPlayback();
			}
		}
	}
}

void PlayerManager::SplitThread()
{
	TimeThread *t1, *t2;
	_playThread->Split(t1, t2, _scene);

	_threads.remove(_playThread);
	_threads.push_back(t1);
	_threads.push_back(t2);

	delete _playThread;
	_playThread = t2;
	_playThread->EnableInput();
}

list<TimeThread*>::iterator PlayerManager::EraseThread(TimeThread *t) 
{
	list<TimeThread*>::iterator it = _threads.begin();

	for (; it != _threads.end(); it++) {
		if (*it == t) {
			return _threads.erase(it);
		}
	}
	
	return _threads.end();
}
