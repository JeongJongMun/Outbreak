// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShake.h"

UCameraShake::UCameraShake() {
	// 카메라 쉐이크 설정
	OscillationDuration = 0.08f; // 반동 지속시간
	OscillationBlendInTime = 0.05f; // 반동 시작 블렌딩
	OscillationBlendOutTime = 0.05f; // 반동 종료 블렌딩

	RotOscillation.Pitch.Amplitude = 1.0f; // 위아래 반동 정도
	RotOscillation.Pitch.Frequency = 10.0f; // 반동 빈도

	RotOscillation.Yaw.Amplitude = 1.0f; // 좌우 반동 정도
	RotOscillation.Yaw.Frequency = 10.0f; // 반동 빈도
}
