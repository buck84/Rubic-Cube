/*
 * =====================================================================
 *
 * FileName:    global.h
 * Author:      Buck
 * Email:       Buck88@gmail.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: ȫ�ֱ���
 *
 * ======================================================================
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

/*
// ǰ������ �ϻ��°� ����Һ�
enum EnumFace
{
	EF_FRONT,
	EF_BACK,
	EF_UP,
	EF_DOWN,
	EF_LEFT,
	EF_RIGHT,
	FACE_NUM = 6,
};
*/

enum ColorState
{
	CS_BLUE,
	CS_GREEN,
	CS_YELLOW,
	CS_WHITE,
	CS_ORANGE,
	CS_RED,
	CS_BLACK,
	CS_NUM = 6,
};

enum RotateType
{
	RT_F,
	RT_B,
	RT_U,
	RT_D,
	RT_L,
	RT_R,
	RT_NUM,
};

// vertex buffer �������ƣ�����̫�߽ף����ϣ�������Ҫ���ӷ����buffer
#define RANK_MAX	12	// ħ������

#endif // _GLOBAL_H_