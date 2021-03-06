#define BOOST_TEST_MODULE test_ff

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include "ff.h"
#include "common/log.h"

using namespace ff;
#define FF_TEST_TIME 100000


BOOST_AUTO_TEST_SUITE(minimal_test)


int inc(int t)
{
	return t + 1;
}
double inc(double t)
{
	return t + 1;
}

/*
int fib(int n)
{
	if(n <=2)
		return 1;
	ff::para<int> a, b;
	a([&n]()->int{return fib(n - 1);});
	b([&n]()->int{return fib(n - 2);});
	return (a && b).then([](int x, int y){return x + y;});
}*/
void ff_test_para()
{
	int num = 10;
	para<int> a;
	a([&num](){return inc(num);}).then([&num](int x){
		BOOST_CHECK(x == inc(num));
	});
	ff::para<> b;
	int b_res;
	b[a]([&num, &a, &b_res](){b_res = inc(num + a.get());}).then([&num, &a, &b, &b_res](){
		BOOST_CHECK(b_res == inc(num + a.get()));
	});
	
	ff::para<int> f1;
	ff::para<double> f2;
	f1([](){return inc(1);});
	f2([](){return inc(2.2);});
	(f1 || f2).then([](int index, std::tuple<int, double> res){
		if(index == 0)
			BOOST_CHECK_MESSAGE(std::get<0>(res) == inc(1), "res:"<<std::get<0>(res)<<", should be "<<inc(1));
		else if(index == 1)
			BOOST_CHECK_MESSAGE(std::get<1>(res) == inc(2.2), "res:"<<std::get<1>(res)<<", should be "<<inc(2.2));
	});
	
	ff::para<> f3;
	f3([](){return inc(3);});
	ff::para<double> f4;
	f4[f3]([](){return inc(4.5);}).then([](double x){
		BOOST_CHECK(x == inc(4.5));
	});
	(f3 && f4).then([](double x){
		BOOST_CHECK(x == inc(4.5));
	});	
}

void ff_test_paragroup()
{
	std::vector<int> s;
	s.push_back(10);
	s.push_back(11);
	s.push_back(15);
	s.push_back(9);
	
	int ssum = 0;
	std::for_each(s.begin(), s.end(), [&ssum](int x){ssum += x;});
	
	ff::accumulator<int> sum(0, [](const int & x, const int& y){return x + y;});
	ff::paragroup pg1;
	pg1.for_each(s.begin(), s.end(), [&sum](int x){
	  sum.increase(x);
	});
	ff_wait(all(pg1));
	
	BOOST_CHECK(ssum == sum.get());
	//std::cout<<"fff"<<std::endl;
	/*
	ff::paragroup pg2;
	ff::single_assign<int> first;
	pg2.for_each(s.begin(), s.end(), [&first](int x){first = x;});
	std::cout<<"pg2 first: "<<first.get()<<std::endl;
	*/
}
BOOST_AUTO_TEST_CASE(test_para)
{
	for(int i = 0; i < FF_TEST_TIME; ++i)
	{
		ff_test_para();
		ff_test_paragroup();
	}
	//std::cout<<"xxx"<<std::endl;
}

BOOST_AUTO_TEST_SUITE_END()
#if 0
int main(int argc, char *argv[])
{	
	_DEBUG(ff::log<>::init(ff::INFO, "log.txt"))
	_DEBUG(LOG_INFO(main)<<"main start")
	ff::para<int> a;
	int num = 10;
	a([&num](){return foo(num);}).then([](int x){std::cout <<"got x:"<<x<<std::endl;});
	ff::para<> b;
	b[a]([&num, &a](){foo(num + a.get());}).then([](){std::cout<<"do anything here!"<<std::endl;});
	
	
	int fib_res = fib(num);
	std::cout<<"fib( "<<num<<" )="<<fib_res<<std::endl;
	
	ff::para<int> f1;
	ff::para<double> f2;
	f1([](){return foo(1);});
	f2([](){return bar(2.2);});
	(f1 || f2).then([](int index, std::tuple<int, double> res){
		if(index == 0)
			std::cout<<"(f1||f2).then 0 "<<std::get<0>(res)<<std::endl;
		else if(index == 1)
			std::cout<<"(f1||f2).then 1 "<<std::get<1>(res)<<std::endl;
	});
	
	ff::para<> f3;
	f3([](){return foo(3);});
	ff::para<double> f4;
	f4[f3]([](){return bar(4.5);}).then([](double x){std::cout<<"f4.then "<<x<<std::endl;});
	(f3 && f4).then([](double x){std::cout<<"f3&&f4.then "<<x<<std::endl;});
	
	
	std::vector<int> s;
	s.push_back(10);
	s.push_back(11);
	s.push_back(15);
	s.push_back(9);
	ff::paragroup pg;
	pg.for_each(s.begin(), s.end(), [](int x){std::cout<<"para group: "<<x<<std::endl;});
	
	ff::accumulator<int> sum(0, [](const int & x, const int& y){return x + y;});
	ff::paragroup pg1;
	pg1[all(pg)].for_each(s.begin(), s.end(), [&sum](int x){
	  sum.increase(x);
	});
	ff_wait(all(pg1));
	std::cout<<"pg1 sum: "<<sum.get()<<std::endl;
	/*
	ff::paragroup pg2;
	ff::single_assign<int> first;
	pg2.for_each(s.begin(), s.end(), [&first](int x){first = x;});
	std::cout<<"pg2 first: "<<first.get()<<std::endl;
	*/
	//LOG_INFO(main)<<"main quit!";
	return 0;
}
#endif