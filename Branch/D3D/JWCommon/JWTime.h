#pragma once

class Time
{
private:
	__int64 ticksPerSecond;//�ʴ� ƽ
	__int64 curTicks;//���� ƽ
	__int64 lastTicks;//���� üũ ������ ƽ
	__int64 lastFPSUpdate;

	unsigned int frameCount;
	float runningTime;//Time ��ü �������� �ð��� �󸶳� �귶�� ����
	float framePerSecond;//�ʴ� ������

	float secondsPerTick;//1ƽ�� �� ��
	float timeElapsed;//1�����Ӵ� �� ��

	static Time* instance;
	Time();
	~Time();

public:
	static Time* Get() { return instance; }
	static void Create() { instance = new Time(); }
	static void Delete() { delete instance; }

	void Update();


	float Delta() { return timeElapsed; }
	float FPS() { return framePerSecond; }
	float Running() { return runningTime; }
};