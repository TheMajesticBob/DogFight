#include "ai_testingtask.h"

void TestingTask::run()
{
	BTTask::run();

	_currentTime = _time;
	// std::cout << "Task " << _name << " started";
}

void TestingTask::update(const double& dt)
{
	_currentTime -= dt;

	// std::cout << _currentTime << std::endl;

	if (_currentTime <= 0.0f)
	{
		onFinished(true);
	}
}
