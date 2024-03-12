// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "PA_Player_PlayerCharacter.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_Character.h"

#include "BehaviorTree/BlackboardComponent.h"		//블랙보드 컴포넌트
#include "DrawDebugHelpers.h"						//드로우디버그


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");		//노드 이름 설정
	Interval = 0.2f;				//Tick주기 설정
	RandomDeviation = 0.0f;			//랜덤 편차
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APA_Enemy_Character* ControllingPawn = Cast<APA_Enemy_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return;
	auto ControllingPawnAI = Cast<APA_Enemy_Controller>(ControllingPawn->GetController());

	UWorld* World = ControllingPawn->GetWorld();			//폰의 월드
	FVector Center = ControllingPawn->GetActorLocation();	//폰의 위치
	float DetectRadius = 1800.0f;							//탐지하는 원의 반지름 길이

	if (nullptr == World) return;
	if (ControllingPawn->DamagedPlayerCharacter == nullptr)
	{
		//오버랩된 액터들 배열
		TArray<FOverlapResult> OverlapResults;
		//탐색 방법에 대한 설정 값을 모아둔 구조체
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);	//1.태그닉넴, 2. 복잡성여부, 3. 제외할 액터
		bool bResult = World->OverlapMultiByChannel(		//탐지 범위안에 들어왔는지 bool

			OverlapResults,									//탐지할 방법을 모아놓은 변수
			Center,											//감지를 시작할 중앙 위치
			FQuat::Identity,								//탐색할 도형의 회전
			ECollisionChannel::ECC_GameTraceChannel1,		//탐색할 대상의 콜리전 채널 ( 플레이어 감지 )
			FCollisionShape::MakeSphere(DetectRadius),		//탐색할 도형의 모양
			CollisionQueryParam								//탐색 방법에 대한 설정 값을 모아둔 구조체

		);
		if (bResult)
		{
			for (auto const& OverlapResult : OverlapResults)
			{
				//범위 기반 for문으로 감지된 캐릭터들의 배열을 차례로 플레이어 캐릭터인지 확인
				APA_Player_PlayerCharacter* Character = Cast<APA_Player_PlayerCharacter>(OverlapResult.GetActor());
				if (Character)
				{
					//플레이러를 감지했을 때

					OwnerComp.GetBlackboardComponent()->SetValueAsObject(APA_Enemy_Controller::TargetKey, Character);
					//ControllingPawnAI->SetIsDetectedKey(true);		//애님인스턴스와 AI IsDetected키를 true로 만든다
					ControllingPawn->SetHPWidgetHidden(false);			//HPWidget을 드러낸다

					//1.월드, 2.시작위치, 3.반지름, 4.조각, 5.색, 6.탐지범위가 사라지지않음, 7.탐지범위가 지속될 시간
					/*DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
					DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Green,
						false, 0.2f);*/
					return;
				}
			}
		}
		//플레이러를 감지하지 못했을 때
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APA_Enemy_Controller::TargetKey, nullptr);
		//ControllingPawnAI->SetIsDetectedKey(false);		//애님인스턴스와 AI IsDetected키를 false로 만든다
		ControllingPawn->SetHPWidgetHidden(true);			//HPWidget을 숨긴다

		//1.월드, 2.시작위치, 3.반지름, 4.조각, 5.색, 6.탐지범위가 사라지지않음, 7.탐지범위가 지속될 시간
		//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APA_Enemy_Controller::TargetKey, ControllingPawn->DamagedPlayerCharacter);
		ControllingPawn->SetHPWidgetHidden(false);			//HPWidget을 드러낸다
	}
}