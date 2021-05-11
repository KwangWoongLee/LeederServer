#pragma once

namespace leeder
{
	class Thread
	{
	public:
		Thread(Thread&& rhs);
		Thread& operator=(Thread&& rhs);
		virtual ~Thread();

		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;
		

		template<class Callable, class ...Args>
		Thread(Callable&& func, Args&&... args);

		int		GetID() const { return mId; }


	private:
		int						mId;
		std::thread				mThread;


		static std::atomic<int> mThreadIDSeed;
	};

	extern thread_local Thread* thisThread;

	template<class Callable, class ...Args>
	inline Thread::Thread(Callable&& func, Args && ...args)
		: mId(mThreadIDSeed++)
		, mThread(std::forward<Callable>(func),std::forward<Args>(args)...)
	{
	}


}





//사용법
//std::function < void(int)> func = [](int i) {
	//	std::cout << "1!!!" << std::endl;
	//
	//    std::cout << "From Thread ID :" << std::this_thread::get_id() << "\n";
	//};
	//
	//std::vector<Thread> Threadpool;
	//
	//Thread t1(func, 1);
	//Threadpool.push_back(std::move(t1));
	//
	////threadpool 요소 변경
	//Threadpool[1] = std::move(t1);