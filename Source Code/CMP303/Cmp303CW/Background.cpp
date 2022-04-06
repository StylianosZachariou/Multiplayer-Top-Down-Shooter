#include "Background.h"

int Backround::moveView(int viewCenter, int viewSize, int max)//This function moves the view
{
	if ((viewCenter + (viewSize / 2)) < max)//If view position is less than max
	{
		if (max - (viewCenter + (viewSize / 2)) >= 600)
		{
			return 600;//Move forward
		}
		else
		{
			return max - (viewCenter + (viewSize / 2));
		}
	}
	else if ((viewCenter + (viewSize / 2)) == max) // If view position is equal to max
	{
		return 0;//Stay still
	}
	else if ((viewCenter - (viewSize / 2)) > 0)//If view position more than 0
	{
		if ((viewCenter - (viewSize / 2)) >= 600)
		{
			return -600;//Move forward
		}
		else
		{
			return -(viewCenter - (viewSize / 2));
		}
	}

}