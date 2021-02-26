#pragma once
namespace PureVirtualDemo {
	class Tank;
	class Tiger;
	class Sherman;

	class Projectile
	{
	public:
		virtual void damage(Tank& target) = 0;
		virtual ~Projectile() = 0;
	};

	class Round76MM : public Projectile
	{
	public:
		void damage(Tank& target) override;
	};

	class Round88MM : public Projectile
	{
	public:
		void damage(Tank& target) override;
	};

	class Tank
	{
	public:
		virtual void takeDamage(Projectile& round) = 0;
	};

	class Sherman : public Tank
	{
	public:
		void takeDamage(Projectile& round) override;
	};

	//Eh I will use this example to demonstrate visitor
	class Tiger : public Tank
	{
	public:
		void takeDamage(Projectile& round) override;
	};

	void pureVirtualDemo();
}
