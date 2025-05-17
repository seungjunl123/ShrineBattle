// Fill out your copyright notice in the Description page of Project Settings.


#include "KwangGameplayTags.h"

namespace KwangGameplayTags
{
	// input tags
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
    UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Attack, "InputTag.Attack");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack, "InputTag.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll, "InputTag.Roll");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_R, "InputTag.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Pickup_Item, "InputTag.Pickup.Item");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld, "InputTag.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_MustBeHeld_Block, "InputTag.MustBeHeld.Block");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable, "InputTag.Toggleable");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggleable_TargetLock, "InputTag.Toggleable.TargetLock");

	// player tags
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack, "Player.Ability.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HeavyAttack, "Player.Ability.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitShake, "Player.Ability.HitShake");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll, "Player.Ability.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Block, "Player.Ability.Block");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, "Player.Ability.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_R, "Player.Ability.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_PickUp_Stone, "Player.Ability.PickUp.Stone");

	UE_DEFINE_GAMEPLAY_TAG(Player_State_HeavyAttack, "Player.State.HeavyAttack");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Roll, "Player.State.Roll");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_Blocking, "Player.State.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "Player.State.TargetLock");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitShake, "Player.Event.HitShake");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SuccessfulBlock, "Player.Event.SuccessfulBlock");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right, "Player.Event.SwitchTarget.Right");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left, "Player.Event.SwitchTarget.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_AOE, "Player.Event.AOE");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_ConsumeStones, "Player.Event.ConsumeStones");



	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy, "Player.SetByCaller.AttackType.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Skill_R, "Player.SetByCaller.Skill.R");

	UE_DEFINE_GAMEPLAY_TAG(Player_CoolDown_Skill_R, "Player.CoolDown.Skill.R");

	// enemy tags
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged, "Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SummonEnemies, "Enemy.Ability.SummonEnemies");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_DropStone, "Enemy.Ability.DropStone");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_Strafe, "Enemy.State.Strafe");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_UnderAttack, "Enemy.State.UnderAttack");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_State_Unblockable, "Enemy.State.Unblockable");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SummonEnemies, "Enemy.Event.SummonEnemies");
	
	// shared tags
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact, "Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death, "Shared.Ability.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_SpawnProjectile, "Shared.Ability.SpawnProjectile");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit, "Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact, "Shared.Event.HitReact");

	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage, "Shared.SetByCaller.BaseDamage");

	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Death, "Shared.Status.Death");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front, "Shared.Status.HitReact.Front");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left, "Shared.Status.HitReact.Left");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right, "Shared.Status.HitReact.Right");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back, "Shared.Status.HitReact.Back");

	// Game Data tags
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_SurvivalGameModeMap, "GameData.Level.SurvivalGameModeMap");
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level_MainMenuMap, "GameData.Level.MainMenuMap");

	UE_DEFINE_GAMEPLAY_TAG(GameData_SaveGame_Slot_1, "GameData.SaveGame.Slot.1");
}