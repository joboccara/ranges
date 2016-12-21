joboccara/ranges

This project implements several range adaptors, that aim at behaving like those of boost ranges or range-v3.

The point of this project is to discover new range adaptors that can solve common problems.

During this research, I have implemented **cartesian\_product** that I believe doesn't exist in range libraries yet but has a real business case: producing the range of all the possible combinations between the elements of its input ranges. Look for testCartesianProduct in main.cpp to see how this looks like.

I have also implemented **adjacent**, which gives a view over pairs of consecutive elements in the input range. It seems that other people had the same need though, because it has just been added to the range-v3 library under the name of 'sliding' and that has more customization possible than my adjacent attempt. It's encouraging to see that I'm going in the right direction though.

I have also implemented some adaptors that already exist, like **transform** and **zip** for example, to see how new adaptors can be combined with these.

