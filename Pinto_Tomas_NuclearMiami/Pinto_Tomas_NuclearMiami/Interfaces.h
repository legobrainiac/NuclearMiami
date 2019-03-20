#ifndef INTERFACES_H
#define INTERFACES_H

class IDrawable
{
public:
	virtual void Render() const = 0;
};

class IUpdateable
{
public:
	virtual void Update(float dt) = 0;
};

#endif // !INTERFACES_H
