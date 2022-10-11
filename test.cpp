#include "pch.h"
#include <gtest/gtest.h>
#include <assert.h>

typedef unsigned long DWORD;
typedef long long INT_PTR;
typedef unsigned char BYTE;

class CDWordArray
{
public:
	CDWordArray();
	INT_PTR GetSize() const;
	DWORD GetAt(INT_PTR nIndex) const;
	INT_PTR Add(DWORD newELement);
	void RemoveAll();

	void SetAtGrow(INT_PTR nIndex, DWORD newElement);
	void SetSize(INT_PTR nNewSize, INT_PTR nGrowBy = -1);

	INT_PTR min(INT_PTR a, INT_PTR b);
	INT_PTR max(INT_PTR a, INT_PTR b);

protected:
	DWORD* m_pData;
	INT_PTR m_nSize;
	INT_PTR m_nMaxSize;
	INT_PTR m_nGrowBy;
};

CDWordArray::CDWordArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

//void CDWordArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy)
//{
//	if (nGrowBy >= 0)
//		m_nGrowBy = nGrowBy;
//
//	if (nNewSize < 0)
//		return;
//
//	
//	else if (m_pData == NULL)
//	{
//		m_pData = (DWORD*) new BYTE[nNewSize * sizeof(DWORD)];
//		memset(m_pData, 0, nNewSize * sizeof(DWORD));
//		m_nSize = nNewSize;
//	}
//
//	else if (nNewSize == 0)
//	{
//		delete[] m_pData;
//		m_pData = NULL;
//		m_nSize = m_nMaxSize = 0;
//	}
//
//	else if (nNewSize < m_nMaxSize)
//	{
//		if (nNewSize > m_nSize)
//		{
//			memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(DWORD));
//			m_nSize = nNewSize;
//		}
//
//		else
//			return;
//	}
//
//	else
//	{
//		INT_PTR nGrowArrayBy = m_nGrowBy;
//
//		if (nGrowArrayBy == 0)
//		{
//			nGrowArrayBy = min(1024, max(4, m_nSize / 8));
//		}
//
//		INT_PTR nNewMax = max(nNewSize, nGrowArrayBy + m_nMaxSize);
//
//		DWORD* pNewData = (DWORD*) new BYTE[nNewMax * sizeof(DWORD)];
//		memcpy(pNewData, m_pData, m_nSize * sizeof(DWORD));
//		memset(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(DWORD));
//
//		delete[] m_pData;
//		m_pData = pNewData;
//		m_nSize = nNewSize;
//		m_nMaxSize = nNewMax;
//		
//	}
//}
void CDWordArray::SetSize(INT_PTR nNewSize, INT_PTR nGrowBy)
{
	if (nNewSize < 0)
		return;

	if (nGrowBy >= 0)

		m_nGrowBy = nGrowBy;

	if (nNewSize == 0)
	{
		delete[] m_pData;
		m_pData = NULL;
		m_nSize = m_nMaxSize = 0;
	}

	else if (m_pData == NULL)
	{
		m_pData = (DWORD*) new BYTE[nNewSize * sizeof(DWORD)];
		memset(m_pData, 0, nNewSize * sizeof(DWORD));
		m_nSize = m_nMaxSize = nNewSize;
	}

	else if (nNewSize <= m_nMaxSize)
	{
		if (nNewSize > m_nSize)
		{
			memset(&m_pData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(DWORD));
			m_nSize = nNewSize;
		}
		else
			return;

	}

	else
	{
		INT_PTR nGrowArrayBy = m_nGrowBy;
		if (nGrowArrayBy == 0)
		{
			nGrowArrayBy = min(1024, max(4, m_nSize / 8));
		}

		INT_PTR nNewMax;
		if (nNewSize < m_nMaxSize + nGrowArrayBy)
			nNewMax = m_nMaxSize + nGrowArrayBy;
		else
			nNewMax = nNewSize;


		DWORD* pNewData = (DWORD*) new BYTE[nNewMax * sizeof(DWORD)];

		memcpy(pNewData, m_pData, m_nSize * sizeof(DWORD));
		memset(&pNewData[m_nSize], 0, (nNewSize - m_nSize) * sizeof(DWORD));

		delete[] m_pData;
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

void CDWordArray::SetAtGrow(INT_PTR nIndex, DWORD newElement)
{
	assert(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex + 1);
	m_pData[nIndex] = newElement;
}

INT_PTR CDWordArray::Add(DWORD newElement) 
{
	INT_PTR nIndex = m_nSize;
	SetAtGrow(nIndex, newElement);
	return nIndex;
}

INT_PTR CDWordArray::GetSize() const
{
	return m_nSize;
}

DWORD CDWordArray::GetAt(INT_PTR nIndex) const
{
	assert(nIndex >= 0 || nIndex < m_nSize);
	return m_pData[nIndex];
}

void CDWordArray::RemoveAll()
{
	SetSize(0);
}

INT_PTR CDWordArray::min(INT_PTR a, INT_PTR b)
{
	return a <= b ? a : b;
}

INT_PTR CDWordArray::max(INT_PTR a, INT_PTR b)
{
	return a >= b ? a : b;
}

TEST(CDWordArray, SetSizeLT0)
{
	CDWordArray cd;
	cd.SetSize(-5);
	EXPECT_EQ(0, cd.GetSize());
}

TEST(CDWordArray, SetSizeDATANULL)
{
	CDWordArray cd;
	cd.SetSize(10);
	EXPECT_EQ(10, cd.GetSize());
}

TEST(CDWordArray, SetSizeEQ0)
{
	CDWordArray cd;
	cd.SetSize(10);
	cd.SetSize(0);
	EXPECT_EQ(0, cd.GetSize());
}

TEST(CDWordArray, SetSizeLTMaxSize)
{
	CDWordArray cd;
	cd.SetSize(10, 4);
	cd.SetSize(8);
	EXPECT_EQ(10, cd.GetSize());
	/*cd.SetSize(12);
	EXPECT_EQ(12, cd.GetSize());*/
	cd.Add(15);
	cd.Add(13);
	
	cd.SetSize(13);
	EXPECT_EQ(15, cd.GetAt(10));
	EXPECT_EQ(13, cd.GetSize());
}

TEST(CDWordArray, SetSizeGTMaxSize)
{
	CDWordArray cd;
	cd.SetSize(10, 4);
	cd.Add(11);
	cd.Add(12);
	
	cd.SetSize(17);
	EXPECT_EQ(0, cd.GetAt(15));
	EXPECT_EQ(11, cd.GetAt(10));
	EXPECT_EQ(17, cd.GetSize());
}

TEST(CDWordArray, setAtGrowLTESize)
{
	CDWordArray cd;
	cd.SetSize(7);
	cd.SetAtGrow(6, 8); // index < size
	EXPECT_EQ(8, cd.GetAt(6));
	EXPECT_EQ(7, cd.GetSize());

}

TEST(CDWordArray, setAtGrowGTSize)
{
	CDWordArray cd;
	cd.SetSize(7);
	cd.SetAtGrow(10, 9);// index >= size
	EXPECT_EQ(0, cd.GetAt(8));
	EXPECT_EQ(9, cd.GetAt(10));
}

TEST(CDWordArray, Add)
{
	CDWordArray cd;
	cd.Add(10);
	cd.Add(5);
	cd.Add(20);
	EXPECT_EQ(3, cd.Add(15));
}

TEST(CDWordArray, getSize)
{
	CDWordArray cd;
	EXPECT_EQ(0, cd.GetSize()); // Constructor
	cd.Add(10);
	cd.Add(5);
	cd.Add(20);
	cd.SetSize(6);
	//std::cout << cwa.GetAt(0) << std::endl;
	EXPECT_EQ(10, cd.GetAt(0));
	EXPECT_EQ(0, cd.GetAt(4));
	EXPECT_EQ(6, cd.GetSize());
}

TEST(CDWordArray, removeAll)
{
	CDWordArray cd;
	cd.Add(10);
	cd.Add(5);
	cd.Add(20);
	cd.RemoveAll();
	EXPECT_EQ(0, cd.GetSize());
}


int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}