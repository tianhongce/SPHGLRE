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
			// TODO:  �ڴ�������Դ���
			/*
			Assert.AreEqual();	//����ָ����ֵ�Ƿ���ȣ������ȣ������ͨ����
			Assert.Inconclusive();	//��ʾһ��δ��֤�Ĳ��ԣ�
			Assert.IsTrue();		//����ָ���������Ƿ�ΪTrue�����ΪTrue�������ͨ����
			Assert.IsFalse();	//����ָ���������Ƿ�ΪFalse�����ΪFalse�������ͨ����
			Assert.IsNull();	//����ָ���Ķ����Ƿ�Ϊ�����ã����Ϊ�գ������ͨ����
			Assert.IsNotNull();	//����ָ���Ķ����Ƿ�Ϊ�ǿգ������Ϊ�գ������ͨ����
			*/
		}

		TEST_METHOD(GetGridIndexTest)
		{
			// TODO: ���Ի�ȡ��������

			GridIndex gi = hashGrid->GetGridIndex(-2.0, -2.0, 3.0);
			int i = 0;
		}

		TEST_METHOD(SetHashCellTest)
		{
			// TODO: ������ӹ�ϣ�ڵ�

		}

		TEST_METHOD(ConHashGridTest)
		{
			// TODO: ���Թ����ϣ����
			for (double i = 0; i < 1.0; i += 0.1)
			{
				hashGrid->AddParticle(0.1 + i, 0.2 + i, 1.2, 5);
			}

			hashGrid->ClearParticle();

			int i = 0;

		}

		TEST_METHOD(GetGridCellTest)
		{
			// TODO: ���Ի�ȡ����Ԫ
			hashGrid->AddParticle(1.1, 1.2, 2.2, 5);

			GridIndex gridIndex = hashGrid->GetGridIndex(1, 2, 2);

			GridCell* cell = NULL;

			cell = hashGrid->GetGridCell(gridIndex);

		}

	};
}