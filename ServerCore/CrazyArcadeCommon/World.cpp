#include "Common.h"
#include "World.h"

void World::AddGameObject(std::shared_ptr<GameObject> obj)
{
	mGameObjects.push_back(obj);
	obj->SetIndex(mGameObjects.size() - 1);
}

void World::RemoveGameObject(std::shared_ptr<GameObject> obj)
{
	int index = obj->GetIndex();

	int lastIndex = mGameObjects.size() - 1;
	if (index != lastIndex)
	{
		mGameObjects[index] = mGameObjects[lastIndex];
		mGameObjects[index]->SetIndex(index);
	}

	obj->SetIndex(-1);

	mGameObjects.pop_back();
}

void World::Update(float deltaTime)
{
	for (int i = 0, c = mGameObjects.size(); i < c; ++i)
	{
		auto go = mGameObjects[i];


		if (!go->IsDie())
		{
			go->Update(deltaTime);
		}

		if (go->IsDie())
		{
			RemoveGameObject(go);
			go->HandleDying();
			--i;
			--c;
		}
	}
}

int World::getIndexOfGameObject(std::shared_ptr<GameObject> obj)
{
	return obj->GetIndex();
}
