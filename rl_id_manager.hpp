#pragma once
/**
 * @note - id生成器
 */
#include <vector>
#include <algorithm>

class rl_id_manager
{
public:
	rl_id_manager() : _id(0){}
	void init()
	{
		_id = 0;
		_recycled_ids.clear();
	}

	rlId id()
	{
		if (_recycled_ids.size() != 0)
		{
			rlId __id = _recycled_ids[0];
			_recycled_ids.erase(_recycled_ids.begin());
			return __id;
		}

		return (++_id);
	}

	/** 确保回收的id唯一性，负责会打乱id分配秩序 */
	void recycle(rlId value)
	{
		if (value > _id || std::find(_recycled_ids.begin(), _recycled_ids.end(), value) != _recycled_ids.end())
			return;

		_recycled_ids.push_back(value);
	}

private:
	rlId _id;
	std::vector<rlId> _recycled_ids;
};