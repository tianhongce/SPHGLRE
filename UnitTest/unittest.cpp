#include "stdafx.h"
#include "CppUnitTest.h"
#include "../SPHGL/HashGrid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		HashGrid* hashGrid;
		UnitTest()
		{
			hashGrid = new HashGrid(1, 2 * 2 * 2, 2, 2, 2);
		}

		~UnitTest()
		{
			delete hashGrid;
		}


		TEST_METHOD(TestMethod)
		{
			// TODO:  在此输入测试代码
			/*
			Assert.AreEqual();	//测试指定的值是否相等，如果相等，则测试通过；
			Assert.Inconclusive();	//表示一个未验证的测试；
			Assert.IsTrue();		//测试指定的条件是否为True，如果为True，则测试通过；
			Assert.IsFalse();	//测试指定的条件是否为False，如果为False，则测试通过；
			Assert.IsNull();	//测试指定的对象是否为空引用，如果为空，则测试通过；
			Assert.IsNotNull();	//测试指定的对象是否为非空，如果不为空，则测试通过；
			*/
		}

		TEST_METHOD(GetGridIndexTest)
		{
			// TODO: 测试获取网格索引

			GridIndex gi = hashGrid->GetGridIndex(-2.0, -2.0, 3.0);
			int i = 0;
		}

		TEST_METHOD(SetHashCellTest)
		{
			// TODO: 测试添加哈希节点

		}

		TEST_METHOD(ConHashGridTest)
		{
			// TODO: 测试构造哈希网格
			for (double i = 0; i < 1.0; i += 0.1)
			{
				hashGrid->AddParticle(0.1 + i, 0.2 + i, 1.2, 5);
			}

			hashGrid->ClearParticle();

			int i = 0;

		}

		TEST_METHOD(GetGridCellTest)
		{
			// TODO: 测试获取网格单元
			hashGrid->AddParticle(1.1, 1.2, 2.2, 5);

			GridIndex gridIndex = hashGrid->GetGridIndex(1, 2, 2);

			GridCell* cell = NULL;

			cell = hashGrid->GetGridCell(gridIndex);

		}

	};
}