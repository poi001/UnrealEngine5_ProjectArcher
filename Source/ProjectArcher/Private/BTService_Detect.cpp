// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "PA_Player_PlayerCharacter.h"
#include "PA_Enemy_Controller.h"
#include "PA_Enemy_Character.h"

#include "BehaviorTree/BlackboardComponent.h"		//������ ������Ʈ
#include "DrawDebugHelpers.h"						//��ο�����


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");		//��� �̸� ����
	Interval = 0.2f;				//Tick�ֱ� ����
	RandomDeviation = 0.0f;			//���� ����
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APA_Enemy_Character* ControllingPawn = Cast<APA_Enemy_Character>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn) return;
	auto ControllingPawnAI = Cast<APA_Enemy_Controller>(ControllingPawn->GetController());

	UWorld* World = ControllingPawn->GetWorld();			//���� ����
	FVector Center = ControllingPawn->GetActorLocation();	//���� ��ġ
	float DetectRadius = 1800.0f;							//Ž���ϴ� ���� ������ ����

	if (nullptr == World) return;
	if (ControllingPawn->DamagedPlayerCharacter == nullptr)
	{
		//�������� ���͵� �迭
		TArray<FOverlapResult> OverlapResults;
		//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü
		FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);	//1.�±״г�, 2. ���⼺����, 3. ������ ����
		bool bResult = World->OverlapMultiByChannel(		//Ž�� �����ȿ� ���Դ��� bool

			OverlapResults,									//Ž���� ����� ��Ƴ��� ����
			Center,											//������ ������ �߾� ��ġ
			FQuat::Identity,								//Ž���� ������ ȸ��
			ECollisionChannel::ECC_GameTraceChannel1,		//Ž���� ����� �ݸ��� ä�� ( �÷��̾� ���� )
			FCollisionShape::MakeSphere(DetectRadius),		//Ž���� ������ ���
			CollisionQueryParam								//Ž�� ����� ���� ���� ���� ��Ƶ� ����ü

		);
		if (bResult)
		{
			for (auto const& OverlapResult : OverlapResults)
			{
				//���� ��� for������ ������ ĳ���͵��� �迭�� ���ʷ� �÷��̾� ĳ�������� Ȯ��
				APA_Player_PlayerCharacter* Character = Cast<APA_Player_PlayerCharacter>(OverlapResult.GetActor());
				if (Character)
				{
					//�÷��̷��� �������� ��

					OwnerComp.GetBlackboardComponent()->SetValueAsObject(APA_Enemy_Controller::TargetKey, Character);
					//ControllingPawnAI->SetIsDetectedKey(true);		//�ִ��ν��Ͻ��� AI IsDetectedŰ�� true�� �����
					ControllingPawn->SetHPWidgetHidden(false);			//HPWidget�� �巯����

					//1.����, 2.������ġ, 3.������, 4.����, 5.��, 6.Ž�������� �����������, 7.Ž�������� ���ӵ� �ð�
					/*DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
					DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Green,
						false, 0.2f);*/
					return;
				}
			}
		}
		//�÷��̷��� �������� ������ ��
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APA_Enemy_Controller::TargetKey, nullptr);
		//ControllingPawnAI->SetIsDetectedKey(false);		//�ִ��ν��Ͻ��� AI IsDetectedŰ�� false�� �����
		ControllingPawn->SetHPWidgetHidden(true);			//HPWidget�� �����

		//1.����, 2.������ġ, 3.������, 4.����, 5.��, 6.Ž�������� �����������, 7.Ž�������� ���ӵ� �ð�
		//DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APA_Enemy_Controller::TargetKey, ControllingPawn->DamagedPlayerCharacter);
		ControllingPawn->SetHPWidgetHidden(false);			//HPWidget�� �巯����
	}
}