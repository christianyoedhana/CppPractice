#pragma once
class Tank;
class Tiger;
class Sherman;

class Projectile
{
public:
	virtual void damage(Tank& target);
	virtual void damage(Tiger& target);
	virtual void damage(Sherman& target);
};

class Round76MM : public Projectile
{
public:
	virtual void damage(Tank& target) override;
	virtual void damage(Tiger& target) override;
};

class Round88MM : public Projectile
{
public:
	virtual void damage(Tank& target) override;
	virtual void damage(Sherman& target) override;
};