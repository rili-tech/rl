#pragma once

class rlOperation
{
public:
	void start()
	{
		if (will_begin() == false)
			return;
		beginning();
		ended();
	}

protected:
	virtual bool will_begin(){ return true; }
	virtual void beginning(){ }
	virtual void ended(){}
};

