#pragma once


namespace JWMath
{
	//�������� �ڵ� ���ʹϾ𿡼�, ���Ϸ��� �̴� �Լ�
	
	void QuatToEuler(OUT float& eX, OUT float& eY, OUT float& eZ, IN float&& qX, IN float&& qY, IN float&& qZ, IN float&& qW);

}