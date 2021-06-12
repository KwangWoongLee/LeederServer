#pragma once
#include "Common.h"

class World : public Singleton<World>
{
public:
	void	AddGameObject(std::shared_ptr<GameObject> obj);
	void	RemoveGameObject(std::shared_ptr<GameObject> obj);

	void	Update();

	const std::vector< std::shared_ptr<GameObject> >& GetGameObjects()	const { return mGameObjects; }


private:
	int	GetIndexOfGameObject(std::shared_ptr<GameObject> obj);

	std::vector< std::shared_ptr<GameObject> >	mGameObjects;



};

