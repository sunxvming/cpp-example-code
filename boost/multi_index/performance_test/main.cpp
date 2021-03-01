#include <cstdio>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/smart_ptr.hpp>
#include <iostream>
#include <boost/progress.hpp>
 
using boost::multi_index_container;
using namespace boost::multi_index;
using namespace std;
using namespace boost;
 
// 玩家信息
class CPlayer
{
public:
	CPlayer(int id, string name):id(id),name(name)
	{        
	}
 
	int		id;		// 唯一id
	string	name;	// 名字
};
 
struct id{};
struct name{};
 
// 定义player容器
typedef boost::multi_index_container<
	CPlayer,
	indexed_by
	<
		ordered_unique<tag<id>,			BOOST_MULTI_INDEX_MEMBER(CPlayer, int, id)>,
		ordered_non_unique<tag<name>,	BOOST_MULTI_INDEX_MEMBER(CPlayer, string, name)>
	>
> PlayerContainer;
 
typedef map<int,CPlayer*>		Map_Id_Player;
typedef map<string,CPlayer*>	Map_Name_Player;
typedef vector<CPlayer*>		Vec_Player;
 
int main() 
{
	PlayerContainer playerContiner;	// 容器
	Map_Id_Player	mapIdPlayer;	// map id->player
	Map_Name_Player mapNamePlayer;	// map name->player
 
	// 100万次测试
	int testNum = 1000000;
	Vec_Player vecPlayer;
 
	// 初始化
	for(int i=0;i<testNum;i++)
	{
		vecPlayer.push_back(new CPlayer(i,"player"));
	}
	// 随机打乱
	std::random_shuffle(vecPlayer.begin(), vecPlayer.end());
 
	cout<<"开始测试：执行次数:"<<testNum<<endl;
	/*	插入测试		*/
	{
		cout<<"multi_index插入耗时:";
		progress_timer t;
		for (Vec_Player::iterator iter=vecPlayer.begin();iter!=vecPlayer.end();iter++)
		{
			playerContiner.insert(**iter);
		}
		// progress_timer变量会在创建时计时，析构时自动打印出耗时，下同，不再解释
	}
	{
		cout<<"map插入耗时:";
		progress_timer t;
		for (Vec_Player::iterator iter=vecPlayer.begin();iter!=vecPlayer.end();iter++)
		{
			mapIdPlayer.insert(Map_Id_Player::value_type((*iter)->id,*iter));
			mapNamePlayer.insert(Map_Name_Player::value_type((*iter)->name,*iter));
		}
	}
 
	/*	查询测试		*/
	{
		cout<<"multi_index查询耗时:";
		progress_timer t;
		PlayerContainer::index<id>::type& player_ID = playerContiner.get<id>();
		PlayerContainer::index<name>::type& player_NAME = playerContiner.get<name>();
		for (Vec_Player::iterator iter=vecPlayer.begin();iter!=vecPlayer.end();iter++)
		{
			player_ID.find((*iter)->id);
			player_NAME.find((*iter)->name);
		}
	}
	{
		cout<<"map查询耗时:";
		progress_timer t;
		for (Vec_Player::iterator iter=vecPlayer.begin();iter!=vecPlayer.end();iter++)
		{
			mapIdPlayer.find((*iter)->id);
			mapNamePlayer.find((*iter)->name);
		}
	}
 
	/*	删除测试		*/
	{
		cout<<"multi_index删除耗时:";
		progress_timer t;
		PlayerContainer::index<id>::type& player_ID = playerContiner.get<id>();
		for (Vec_Player::iterator iter=vecPlayer.begin();iter!=vecPlayer.end();iter++)
		{
			player_ID.erase(player_ID.find((*iter)->id));
		}
	}
	{
		cout<<"map删除耗时:";
		progress_timer t;
		for (Vec_Player::iterator iter=vecPlayer.begin();iter!=vecPlayer.end();iter++)
		{
			Map_Id_Player::iterator result1 = mapIdPlayer.find((*iter)->id);
			if (result1 != mapIdPlayer.end())
			{
				mapIdPlayer.erase(result1);
			}
			
			Map_Name_Player::iterator result2 = mapNamePlayer.find((*iter)->name);
			if (result2 != mapNamePlayer.end())
			{
				mapNamePlayer.erase(result2);
			}
		}
	}
	cout<<"结束测试：执行次数:"<<testNum<<endl;
 
	getchar();
	return 0;
}