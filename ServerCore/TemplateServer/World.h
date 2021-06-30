#pragma once
#include "stdafx.h"

class World : public Singleton<World>
{
public:

	void	AddGameObject(std::shared_ptr<GameObject> obj);
	void	RemoveGameObject(std::shared_ptr<GameObject> obj);

	void	Update(float deltaTime);

	const std::vector< std::shared_ptr<GameObject> >& GetGameObjects()	const { return mGameObjects; }


private:
	int	getIndexOfGameObject(std::shared_ptr<GameObject> obj);


	std::vector< std::shared_ptr<GameObject> >	mGameObjects;



};

