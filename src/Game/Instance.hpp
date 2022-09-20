#pragma once

#include <stdint.h>
#include <vector>

#include <list>


namespace game
{

	class Instance
	{
	public:
		Instance(int64_t _ID, bool _IsUnique = false);
		virtual ~Instance();

		int64_t get_id();

		virtual void update(int64_t _DeltaTime) = 0;
	private:
		int64_t mID;
		bool mIsUnique;
	};

	using InstancePtr = Instance*;
	using InstanceRef = Instance&;


	class IPack : public Instance
	{
	public:
		IPack(int64_t _ID, int64_t _MaxVolume = 64);
		virtual ~IPack();
		
		bool try_place(Instance& _Inst);
		
		virtual bool splice(IPack& _Other) = 0;
	protected:
		int64_t mVolume;
		int64_t mMaxVolume;
		std::vector<InstancePtr> mData;
	};

	template<class _Ty>
	class Pack : public IPack
	{
	public:
		Pack()
		{	}

		~Pack()
		{	}

		bool splice(IPack& _Other) override
		{

		}

	};
}