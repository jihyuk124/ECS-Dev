#pragma once
//�޸� ���� ã��
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//new ������. �޸� ������ ���� ������ ã�Ƴ��� ���ؼ� ���
#ifdef _DEBUG 
#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 
#endif 
#endif  // _DEBUG