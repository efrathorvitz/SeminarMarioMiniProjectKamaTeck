#include "EntitiesPool.h"

void EntitiesPool::insert(EntityPtr whom)
{
	_pool.push_back(whom);
	_isInUse.push_back(false);
}

EntityPtr EntitiesPool::getNext()
{
	for (int i = 0; i < _isInUse.size(); i++)
		if (!_isInUse[i]) {
			_isInUse[i] = true;
			return _pool[i];
		}
	throw exception("there is not avvilible entity");
}

EntityPtr EntitiesPool::getByIndex(int index)
{
	return _isInUse[index] ? _pool[index] : nullptr;
}

void EntitiesPool::returnEntity(EntityPtr toFree)
{
	auto findEntity = find(_pool.begin(), _pool.end(), toFree);
	if (findEntity == _pool.end())
		throw exception("there isnt valid entity");
	_isInUse[findEntity - _pool.begin()] = false;
}
