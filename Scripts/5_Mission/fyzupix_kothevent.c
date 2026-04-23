/*
PackFazupix KOTH by Touchofdeath Version 1.0
*/

class FYZUPIX_KOTHEvent
{
	ref array<PlayerBase> FYZUPIXK_PlayersArray = new array<PlayerBase>;
	ref array<EntityAI> FYZUPIXK_EnemyAIArray = new array<EntityAI>;
	ref array<Object> FYZUPIXK_ObjectsArray = new array<Object>;

	int FYZUPIXK_LBMarkerID;
	int FYZUPIXK_MarkerState;
	int FYZUPIXK_MarkerRadius;
	int FYZUPIXK_StartDelay;
	int FYZUPIXK_UpTime;
	float FYZUPIXK_CapturingTime;
	int FYZUPIXK_ID = -1;
	bool FYZUPIXK_InProgress = false;
	bool FYZUPIXK_Finished = false;
	bool FYZUPIXK_MarkerExists = false;
	
	string FYZUPIXK_Name;
	vector FYZUPIXK_Position;
	vector FYZUPIXK_Orientation;
	int FYZUPIXK_CaptureRadius;
	int FYZUPIXK_Enemies_Count;
	int FYZUPIXK_Reward_Loot_Count;
	string FYZUPIXK_Reward_Classname;
	bool FYZUPIXK_Reward_Scripted;
	
	EntityAI kothRewardEAI;
	fyzupix_KOTHflag kothFlagEntity;
	SmokeGrenadeBase kothWinSmokeEntity;

	#ifdef EXPANSIONMODNAVIGATION
	ExpansionMarkerModule m_KothMarkerEvent;
	ExpansionMarkerData FYZUPIXK_ExpMarker;
	#endif

	#ifdef LBmaster_Groups
	ref LBServerMarker FYZUPIXK_SVMarker;
	#endif

	void FYZUPIX_KOTHEvent(int kID, string kName, vector kPos, vector kRot, int kRad, int kEnc, int kRec, string krClass, bool krScript)
	{
		#ifdef EXPANSIONMODNAVIGATION
		CF_Modules<ExpansionMarkerModule>.Get(m_KothMarkerEvent);
		#endif

		FYZUPIXK_LBMarkerID = -1;
		FYZUPIXK_MarkerState = 0;
		FYZUPIXK_MarkerRadius = 0;
		FYZUPIXK_StartDelay = 0;
		FYZUPIXK_UpTime = 0;
		FYZUPIXK_CapturingTime = 0;
		FYZUPIXK_ID = kID;
		FYZUPIXK_InProgress = false;
		FYZUPIXK_Finished = false;
		FYZUPIXK_MarkerExists = false;
		
		FYZUPIXK_Name = kName;
		FYZUPIXK_Position = kPos;
		FYZUPIXK_Orientation = kRot;
		FYZUPIXK_CaptureRadius = kRad;
		FYZUPIXK_Enemies_Count = kEnc;
		FYZUPIXK_Reward_Loot_Count = kRec;
		FYZUPIXK_Reward_Classname = krClass;
		FYZUPIXK_Reward_Scripted = krScript;
		
		CreateMarker();
		
		SpawnFlag();

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateKOTH, 1000, true);
		
		if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Successfully initialized new KOTH id " + kID + "! Monitoring script now running.");
	}

	void ~FYZUPIX_KOTHEvent()
	{
		if(this.FYZUPIXK_PlayersArray) this.FYZUPIXK_PlayersArray.Clear();
		this.FYZUPIXK_PlayersArray = NULL;
		if(this.FYZUPIXK_EnemyAIArray) this.FYZUPIXK_EnemyAIArray.Clear();
		this.FYZUPIXK_EnemyAIArray = NULL;
		if(this.FYZUPIXK_ObjectsArray) this.FYZUPIXK_ObjectsArray.Clear();
		this.FYZUPIXK_ObjectsArray = NULL;

		this.FYZUPIXK_LBMarkerID = 0;
		this.FYZUPIXK_MarkerState = 0;
		this.FYZUPIXK_MarkerRadius = 0;
		this.FYZUPIXK_UpTime = 0;
		this.FYZUPIXK_StartDelay = 0;
		this.FYZUPIXK_CapturingTime = 0;
		this.FYZUPIXK_ID = 0;
		this.FYZUPIXK_InProgress = false;
		this.FYZUPIXK_Finished = false;
		
		this.FYZUPIXK_Name = "";
		this.FYZUPIXK_Position = "0 0 0";
		this.FYZUPIXK_Orientation = "0 0 0";
		this.FYZUPIXK_CaptureRadius = 0;
		this.FYZUPIXK_Enemies_Count = 0;
		this.FYZUPIXK_Reward_Loot_Count = 0;
		this.FYZUPIXK_Reward_Classname = "";
		this.FYZUPIXK_Reward_Scripted = false;

		if(FYZUPIXK_MarkerExists) DeleteMarker();
		
		if(kothRewardEAI && GetFYZUPIXKConfig().Settings.Get(0).CleanupDeleteCrate()) kothRewardEAI.Delete();
		if(kothFlagEntity) kothFlagEntity.Delete();
		if(kothWinSmokeEntity) kothWinSmokeEntity.Delete();

		#ifdef EXPANSIONMODNAVIGATION
		this.m_KothMarkerEvent = NULL;
		this.FYZUPIXK_ExpMarker = NULL;
		#endif

		#ifdef LBmaster_Groups
		this.FYZUPIXK_SVMarker = NULL;
		#endif

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateKOTH);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.BuildFlag);
	}
	
	string GetName()
	{
		return FYZUPIXK_Name;
	}
	
	vector GetPosition()
	{
		return FYZUPIXK_Position;
	}
	
	vector GetOrientation()
	{
		return FYZUPIXK_Orientation;
	}
	
	int GetCaptureRadius()
	{
		return FYZUPIXK_CaptureRadius;
	}
	
	int GetEnemyCount()
	{
		return FYZUPIXK_Enemies_Count;
	}
	
	int GetRewardLootCount()
	{
		return FYZUPIXK_Reward_Loot_Count;
	}
	
	string GetRewardClassname()
	{
		return FYZUPIXK_Reward_Classname;
	}
	
	void SpawnFlag()
	{
		if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawning KOTH flag.");

		kothFlagEntity = fyzupix_KOTHflag.Cast(GetGame().CreateObjectEx("fyzupix_KOTHflag", FYZUPIXK_Position, ECE_PLACE_ON_SURFACE));
		if(kothFlagEntity) kothFlagEntity.SetOrientation("0.0 0.0 0.0");
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(BuildFlag, 100, false);
	}
	
	void BuildFlag()
	{
		if(kothFlagEntity)
		{
			kothFlagEntity.OnSpawnKOTHFlag(GetFYZUPIXKConfig().Settings.Get(0).GetRandomFlag());
			
			if(GetFYZUPIXKConfig().Settings.Get(0).GetDoSmoke()) kothFlagEntity.UpdateSmokeState(0);
			
			if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Successfully spawned and built KOTH flag.");
		}
	}
	
	void UpdateFlag(int playerCount)
	{
		if(kothFlagEntity)
		{
			int maxCaptureTime = GetFYZUPIXKConfig().Settings.Get(0).GetCaptureTime();
			
			float setHeight = Math.Clamp((FYZUPIXK_CapturingTime/maxCaptureTime), 0, 1);
			if(setHeight < 0) setHeight = 0;
			else if(setHeight > 1) setHeight = 1;

			kothFlagEntity.AnimateKOTHFlag(setHeight);
		}
	}
	
	void CreateMarker()
	{
		FYZUPIXK_MarkerRadius = GetFYZUPIXKConfig().Settings.Get(0).GetMarkerRadius();
		if(FYZUPIXK_MarkerRadius == -1) FYZUPIXK_MarkerRadius = FYZUPIXK_CaptureRadius;
		
		bool Is3DMarker = GetFYZUPIXKConfig().Settings.Get(0).Is3DMarkerEnabled();
		
		#ifdef LBmaster_Groups
			if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Creating Marker! LBMaster Groups detected.");
			
			#ifdef LBmaster_Rework
				FYZUPIXK_SVMarker = LBStaticMarkerManager.Get.AddTempServerMarker("King of The Hill", FYZUPIXK_Position + Vector(0.0, 3.0, 0.0), "LBmaster_Groups\\gui\\icons\\flag.paa", ARGB(255, 255, 255, 0), false, Is3DMarker, true, true);
			#endif
			#ifndef LBmaster_Rework
				FYZUPIXK_SVMarker = LBStaticMarkerManager.Get().AddTempServerMarker("King of The Hill", FYZUPIXK_Position + Vector(0.0, 3.0, 0.0), "LBmaster_Groups\\gui\\icons\\flag.paa", ARGB(255, 255, 255, 0), false, Is3DMarker, true, true);
			#endif
			
			if(FYZUPIXK_SVMarker)
			{
				FYZUPIXK_SVMarker.SetRadius(FYZUPIXK_MarkerRadius, ARGB(255, 255, 255, 0), true, Is3DMarker);
				FYZUPIXK_LBMarkerID = FYZUPIXK_SVMarker.uid;
			}
		#else
			#ifdef EXPANSIONMODNAVIGATION
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Creating Marker! Expansion Navigation detected.");
				FYZUPIXK_ExpMarker = m_KothMarkerEvent.CreateServerMarker("King of The Hill", "Territory", FYZUPIXK_Position + Vector(0.0, 3.0, 0.0), ARGB(255, 255, 0, 0), Is3DMarker);
			#endif
		#endif
		
		FYZUPIXK_MarkerState = 0;
		FYZUPIXK_MarkerExists = true;
	}
	
	void UpdateMarker(int state = 0)
	{
		if(FYZUPIXK_MarkerState == state) return; //Already done...
		
		#ifdef LBmaster_Groups
			if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Updating Marker Colour! LBMaster Groups detected.");
			
			if(FYZUPIXK_SVMarker == NULL) FYZUPIXK_SVMarker = LBStaticMarkerManager.Get.FindTempMarker(FYZUPIXK_LBMarkerID);
			
			if(FYZUPIXK_SVMarker)
			{
				if(state == 0)
				{
					FYZUPIXK_SVMarker.SetRadius(FYZUPIXK_MarkerRadius, ARGB(255, 255, 255, 0), true);
					FYZUPIXK_SVMarker.SetColorARGB(255, 255, 255, 0);
				}
				if(state == 1)
				{
					FYZUPIXK_SVMarker.SetRadius(FYZUPIXK_MarkerRadius, ARGB(255, 255, 255, 255), true);
					FYZUPIXK_SVMarker.SetColorARGB(255, 255, 255, 255);
				}
				if(state == 2)
				{
					FYZUPIXK_SVMarker.SetRadius(FYZUPIXK_MarkerRadius, ARGB(255, 255, 0, 0), true);
					FYZUPIXK_SVMarker.SetColorARGB(255, 255, 0, 0);
				}
				if(state == 3)
				{
					FYZUPIXK_SVMarker.SetRadius(FYZUPIXK_MarkerRadius, ARGB(255, 0, 0, 0), true);
					FYZUPIXK_SVMarker.SetColorARGB(255, 0, 0, 0);
				}
			}
		#else
			#ifdef EXPANSIONMODNAVIGATION
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Updating Marker Colour! Expansion Navigation detected.");
				
				if(FYZUPIXK_ExpMarker)
				{
					if(state == 0) FYZUPIXK_ExpMarker.SetColor(ARGB(255, 255, 255, 0));
					if(state == 1) FYZUPIXK_ExpMarker.SetColor(ARGB(255, 255, 255, 255));
					if(state == 2) FYZUPIXK_ExpMarker.SetColor(ARGB(255, 255, 0, 0));
					if(state == 3) FYZUPIXK_ExpMarker.SetColor(ARGB(255, 0, 0, 0));
				}
			#endif
		#endif
		
		FYZUPIXK_MarkerState = state;
	}
	
	void DeleteMarker()
	{
		#ifdef LBmaster_Groups
			if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Removing Marker! LBMaster Groups detected.");

			if(FYZUPIXK_SVMarker == NULL) FYZUPIXK_SVMarker = LBStaticMarkerManager.Get.FindTempMarker(FYZUPIXK_LBMarkerID);
			
			#ifdef LBmaster_Rework
				if(FYZUPIXK_SVMarker) LBStaticMarkerManager.Get.RemoveServerMarker(FYZUPIXK_SVMarker);
				//Trying to fix this null pointer... i have no idea whats causing it.
			#endif
			#ifndef LBmaster_Rework
				if(FYZUPIXK_SVMarker) LBStaticMarkerManager.Get().RemoveServerMarker(FYZUPIXK_SVMarker);
			#endif
		#else
			#ifdef EXPANSIONMODNAVIGATION
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Removing Marker! Expansion Navigation detected.");
				
				if(FYZUPIXK_ExpMarker)
				{
					if(m_KothMarkerEvent)
					{
						m_KothMarkerEvent.RemoveServerMarker(FYZUPIXK_ExpMarker.GetUID());
					}
					FYZUPIXK_ExpMarker = NULL;
				}
			#endif
		#endif
		
		FYZUPIXK_MarkerExists = false;
	}
	
	void UpdateKOTH()
	{
		if(FYZUPIXK_StartDelay < GetFYZUPIXKConfig().Settings.Get(0).GetSpawnStartDelay())
		{
			FYZUPIXK_StartDelay += 1;
			return;
		}
		
		FYZUPIXK_UpTime += 1;

		if(!FYZUPIXK_Finished)
		{
			int PlayersInCaptureArea = 0;

			FYZUPIXK_PlayersArray.Clear();
			
			bool IsDebugQuickmode = false;
			//GetFYZUPIXKConfig().Settings.Get(0).DebugQuickmode()

			ref array<Man> all_players = new array<Man>;
			GetGame().GetPlayers(all_players);
			if(all_players.Count() > 0)
			{
				//Check for players in KOTH capture area
				for(int i=0; i<all_players.Count(); i++)
				{
					PlayerBase player = PlayerBase.Cast(all_players.Get(i));
						
					if(player && player.IsAlive())
					{
						if(vector.Distance(FYZUPIXK_Position, player.GetPosition()) <= FYZUPIXK_CaptureRadius)
						{
							FYZUPIXK_PlayersArray.Insert(player);
						}
					}
				}
			}
			
			if((FYZUPIXK_PlayersArray.Count() > 0) || IsDebugQuickmode)
			{
				UpdateMarker(1); //Update marker colour
				if(GetFYZUPIXKConfig().Settings.Get(0).GetDoSmoke() && kothFlagEntity) kothFlagEntity.UpdateSmokeState(1); //Update smoke colour
				
				if(!FYZUPIXK_InProgress)
				{
					FYZUPIXK_InProgress = true;
					
					FYZUPIXKOTH.SendNotification("The fight for the hill '" + FYZUPIXK_Name + "' has started!");

				}
				
				if(FYZUPIXK_PlayersArray.Count() > 1)
				{
					float tempMultiplier = GetFYZUPIXKConfig().Settings.Get(0).GetExtraPlayersCaptureBonusPerSec() * (FYZUPIXK_PlayersArray.Count() - 1);
					
					if(tempMultiplier > GetFYZUPIXKConfig().Settings.Get(0).GetExtraPlayersCaptureBonusPerSecMax()) tempMultiplier = GetFYZUPIXKConfig().Settings.Get(0).GetExtraPlayersCaptureBonusPerSecMax();

					FYZUPIXK_CapturingTime += (1 + tempMultiplier);
				}
				else
				{
					FYZUPIXK_CapturingTime += 1;
				}
			}
			else
			{
				UpdateMarker(0); //Update marker colour
				
				if(GetFYZUPIXKConfig().Settings.Get(0).GetDoSmoke() && kothFlagEntity) kothFlagEntity.UpdateSmokeState(0); //Update smoke colour

				if((FYZUPIXK_CapturingTime >= 1.0) && (GetFYZUPIXKConfig().Settings.Get(0).GetLowerIfAbandoned())) FYZUPIXK_CapturingTime -= 1;
			}
			
			if((FYZUPIXK_UpTime >= GetFYZUPIXKConfig().Settings.Get(0).GetTimeLimit()) && (FYZUPIXK_CapturingTime < 1.0))
			{
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH failed!");

				UpdateMarker(3); //Update marker colour

				if(GetFYZUPIXKConfig().Settings.Get(0).GetDoSmoke() && kothFlagEntity) kothFlagEntity.UpdateSmokeState(3); //Update smoke colour

				FYZUPIXKOTH.SendNotification("You ran out of time to capture the hill '" + FYZUPIXK_Name + "'! Better luck next time!");
				
				FYZUPIXK_Finished = true;
			}

			if((FYZUPIXK_CapturingTime >= GetFYZUPIXKConfig().Settings.Get(0).GetCaptureTime()) && !FYZUPIXK_Finished) //Win
			{
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH has been won! Processing winners and reward loot!");

				UpdateMarker(2); //Update marker colour
				
				//if(GetFYZUPIXKConfig().Settings.Get(0).GetDoSmoke() && kothFlagEntity) kothFlagEntity.UpdateSmokeState(2); //Update smoke colour

				int firstPlayer = true;
				string tempPlayerNames = "NONE";
				
				for(int j=0; j<FYZUPIXK_PlayersArray.Count(); j++)
				{
					if(firstPlayer && (FYZUPIXK_PlayersArray.Count() > 1))
					{
						firstPlayer = false;
						tempPlayerNames = FYZUPIXK_PlayersArray.Get(j).GetIdentity().GetName();
					}
					else
					{
						if(FYZUPIXK_PlayersArray.Count() == 1) tempPlayerNames = FYZUPIXK_PlayersArray.Get(j).GetIdentity().GetName();
						else if(j == (FYZUPIXK_PlayersArray.Count() - 1)) tempPlayerNames = tempPlayerNames + " and " + FYZUPIXK_PlayersArray.Get(j).GetIdentity().GetName();
						else tempPlayerNames = tempPlayerNames + ", " + FYZUPIXK_PlayersArray.Get(j).GetIdentity().GetName();
					}
				}
				
				FYZUPIXKOTH.SendNotification("The hill '" + FYZUPIXK_Name + "' has been captured by " + tempPlayerNames + "!");

				if(GetFYZUPIXKConfig().Settings.Get(0).GetDoFireworksOnWin()) DoFireworks();
				
				KOTHRewardRelease();

				FYZUPIXK_UpTime = GetFYZUPIXKConfig().Settings.Get(0).GetTimeLimit();
				
				if(GetFYZUPIXKConfig().Settings.Get(0).GetDoSmoke())
				{
					kothWinSmokeEntity = SmokeGrenadeBase.Cast(GetGame().CreateObjectEx("M18SmokeGrenade_Green", FYZUPIXK_Position, ECE_PLACE_ON_SURFACE));
					if(kothWinSmokeEntity)
					{
						if(kothWinSmokeEntity.GetCompEM() && kothWinSmokeEntity.GetCompEM().CanWork())
						{
							kothWinSmokeEntity.GetCompEM().SwitchOn();
						}
					}
				}
				
				FYZUPIXK_Finished = true;
			}
			
			if(FYZUPIXK_Finished && kothFlagEntity) kothFlagEntity.DeleteSafe();
		}
		
		if(FYZUPIXK_Finished && (FYZUPIXK_UpTime >= (GetFYZUPIXKConfig().Settings.Get(0).GetTimeLimit() + GetFYZUPIXKConfig().Settings.Get(0).GetKOTHFinishedCleanupTime())))
		{
			if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH is over and has reached cleanup time! Performing clean-up!");
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.UpdateKOTH);
			
			DeleteMarker();
			
			FYZUPIXKOTH.KOTHCleanup(FYZUPIXK_ID);
		}

		UpdateFlag(FYZUPIXK_PlayersArray.Count());
	}

	//Function to finalize the reward when a KOTH is won
	void KOTHRewardRelease()
	{
		if(FYZUPIXK_Reward_Classname == "")
		{
			if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Reward classname is undefined. No reward spawned.");
			return;
		}
		
		if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Releasing KOTH reward now! Spawning " + FYZUPIXK_Reward_Classname + " at position (" + FYZUPIXK_Position + ")!");

		kothRewardEAI = EntityAI.Cast(GetGame().CreateObject(FYZUPIXK_Reward_Classname, FYZUPIXK_Position));
		
		if(kothRewardEAI) //Check it exists
		{
			kothRewardEAI.SetOrientation(FYZUPIXK_Orientation);
			
			kothRewardEAI.SetLifetime(GetFYZUPIXKConfig().Settings.Get(0).GetKOTHFinishedCleanupTime());
			
			if(FYZUPIXK_Reward_Scripted)
			{
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH reward container is special (scripted). Calling ReleaseScriptedReward function.");
				
				FYZUPIXKOTH.ReleaseScriptedReward(kothRewardEAI);
				
				return; // Scripted reward, we're done here.
			}
			else
			{
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawning reward loot into reward container!");
				
				ref array<int> EligibleRewards = new array<int>;
				ref array<int> SpawnedIDs = new array<int>;
				ref array<int> SpawnedQuantities = new array<int>;
				
				int tempChosenID;
				string tempClassName;
				string tempMagClassName;
				ref TStringArray tempAttachments;
				ref TStringArray tempExtraItems;
				ref TStringArray tempAlternatives;
				int tempMaxQty;
				int tempQuantity;
				
				for(int i=0; i<GetFYZUPIXKConfig().Loot.Count(); i++)
				{
					if(Math.RandomIntInclusive(1,100) <= GetFYZUPIXKConfig().Loot.Get(i).GetSpawnChance())
					{
						EligibleRewards.Insert(i);
					}
				}
				
				//Tiered roll: bucket eligible loot by Tier and pick a tier per slot
				//using LootTiers[].Weight. If LootTiers is empty or no loot matches
				//any configured tier, we fall back to the flat EligibleRewards pool.
				ref array<ref array<int>> EligibleByTier = new array<ref array<int>>;
				int totalTierWeight = 0;
				int tiersCount = 0;
				if(GetFYZUPIXKConfig().LootTiers) tiersCount = GetFYZUPIXKConfig().LootTiers.Count();
				for(int t=0; t<tiersCount; t++)
				{
					EligibleByTier.Insert(new array<int>);
				}
				for(int ei=0; ei<EligibleRewards.Count(); ei++)
				{
					int lootIdx = EligibleRewards.Get(ei);
					int itemTier = GetFYZUPIXKConfig().Loot.Get(lootIdx).GetTier();
					for(int ti=0; ti<tiersCount; ti++)
					{
						if(GetFYZUPIXKConfig().LootTiers.Get(ti).GetTier() == itemTier)
						{
							EligibleByTier.Get(ti).Insert(lootIdx);
							break;
						}
					}
				}
				for(int tw=0; tw<tiersCount; tw++)
				{
					if(EligibleByTier.Get(tw).Count() > 0)
					{
						totalTierWeight += GetFYZUPIXKConfig().LootTiers.Get(tw).GetWeight();
					}
				}
				bool useTieredRoll = (totalTierWeight > 0);
				
				if(EligibleRewards.Count() == 0)
				{
					if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] No eligible loot passed the Spawn_Chance roll - nothing to spawn.");
					return;
				}
				
				int failSafe = FYZUPIXK_Reward_Loot_Count * 2; //Set this as you wish. If the script cant spawn enough rewards for whatever reason, this is what will stop your server from locking up.
				int CurrentAttempts = 0;
				int CurrentSpawnedLoot = 0;
				
				EntityAI tempEAI;
				EntityAI tempEAI2;
				
				ref TStringArray spawnedItems = new TStringArray;
				
				while(CurrentSpawnedLoot < FYZUPIXK_Reward_Loot_Count)
				{
					if(useTieredRoll)
					{
						int roll = Math.RandomIntInclusive(1, totalTierWeight);
						int acc = 0;
						int chosenTierIdx = -1;
						for(int rt=0; rt<tiersCount; rt++)
						{
							if(EligibleByTier.Get(rt).Count() == 0) continue;
							acc += GetFYZUPIXKConfig().LootTiers.Get(rt).GetWeight();
							if(roll <= acc)
							{
								chosenTierIdx = rt;
								break;
							}
						}
						if(chosenTierIdx < 0)
						{
							//Fall back to flat pool if weights left nothing selectable
							tempChosenID = EligibleRewards.Get(Math.RandomInt(0,EligibleRewards.Count()));
						}
						else
						{
							ref array<int> tierPool = EligibleByTier.Get(chosenTierIdx);
							tempChosenID = tierPool.Get(Math.RandomInt(0, tierPool.Count()));
							if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Tier roll picked '" + GetFYZUPIXKConfig().LootTiers.Get(chosenTierIdx).GetName() + "' (T" + GetFYZUPIXKConfig().LootTiers.Get(chosenTierIdx).GetTier() + ").");
						}
					}
					else
					{
						tempChosenID = EligibleRewards.Get(Math.RandomInt(0,EligibleRewards.Count()));
					}
					tempClassName = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetClassname();
					tempMagClassName = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetMagClassname();
					tempAttachments = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetAttachments();
					tempExtraItems = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetExtraLoot();
					tempAlternatives = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetAlternatives();
					tempMaxQty = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetMaxSpawnable();
					tempQuantity = GetFYZUPIXKConfig().Loot.Get(tempChosenID).GetQuantity();
					
					if(tempMaxQty == -1) tempMaxQty = 999; //-1 should mean no limit
					
					tempAlternatives.Insert(tempClassName);
					string tempOrigClassname = tempClassName;
					
					if(tempAlternatives.Count() > 1) tempClassName = tempAlternatives.Get(Math.RandomInt(0,tempAlternatives.Count()));

					if(tempClassName != "")
					{
						if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Attempting to spawn in reward '" + tempClassName + "' into container inventory.");
						
						int foundqty = 0;
						foreach(string itemClass: spawnedItems)
						{
							if(itemClass == tempOrigClassname) foundqty += 1;
						}
						
						if(foundqty >= tempMaxQty)
						{
							if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Unable to spawn '" + tempClassName + "' as the spawn limit is reached.");
						}
						else
						{
							if(!Class.CastTo(tempEAI, kothRewardEAI.GetInventory().CreateInInventory(tempClassName)))
							{
								tempEAI = EntityAI.Cast(GetGame().CreateObjectEx(tempClassName, kothRewardEAI.GetPosition(), ECE_PLACE_ON_SURFACE));
								if(tempEAI)
								{
									InventoryLocation currentLocation = new InventoryLocation();
									tempEAI.GetInventory().GetCurrentInventoryLocation(currentLocation);
									InventoryLocation newLocation = new InventoryLocation();
									
									if(kothRewardEAI.GetInventory().FindFreeLocationFor(tempEAI, FindInventoryLocationType.CARGO, newLocation))
									{
										if(GameInventory.LocationCanMoveEntity(currentLocation, newLocation))
										{
											if(newLocation.GetParent().CanReceiveItemIntoCargo(tempEAI))
											{
												GetGame().RemoteObjectTreeCreate(tempEAI);
												if(!kothRewardEAI.GetInventory().TakeToDst(InventoryMode.SERVER, currentLocation, newLocation))
												{
													if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Failed to put " + tempClassName + " into inventory of item. Leaving on ground!");
												}
											}
										}
									}
								}
							}
							
							if(tempEAI)
							{
								spawnedItems.Insert(tempOrigClassname);
								
								ItemBase tempIB = ItemBase.Cast(tempEAI);
								
								if(tempIB)
								{
									if(tempQuantity == -1) tempIB.SetQuantity(tempIB.GetQuantityMax());
									else tempIB.SetQuantity(tempQuantity);
								}
								
								Weapon_Base weaponTemp;
								if(Weapon_Base.CastTo(weaponTemp, tempEAI))
								{
									if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Reward item detected as being a weapon, attempting to attach magazine and fill with ammo.");
									
									if(tempMagClassName != "")
									{
										weaponTemp.SpawnAmmo(tempMagClassName);
									}
								}
								if(tempAttachments.Count() > 0)
								{
									for(int j=0; j<tempAttachments.Count(); j++)
									{
										if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Attaching '" + tempAttachments.Get(j) + "' to reward.");

										if(Class.CastTo(tempEAI2, tempEAI.GetInventory().CreateInInventory(tempAttachments.Get(j))))
										{
											BatteryCheck(tempEAI2);
										}
									}
								}
								if(tempExtraItems.Count() > 0)
								{
									for(int k=0; k<tempExtraItems.Count(); k++)
									{
										if(Class.CastTo(tempEAI2, tempEAI.GetInventory().CreateInInventory(tempExtraItems.Get(k))))
										{
											if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawning extra item '" + tempExtraItems.Get(k) + "' into container inventory.");
												
											BatteryCheck(tempEAI2);
										}
									}
								}
							}
							else
							{
								if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Failed to spawn reward item " + tempClassName + "!");
							}
						}
					}

					CurrentAttempts += 1;
					
					if(CurrentAttempts >= failSafe) CurrentSpawnedLoot = FYZUPIXK_Reward_Loot_Count;
				}
				
				if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Finished spawning loot into reward container after " + CurrentAttempts + " attempts.");
			}
		}
		else if(FYZUPIXKOTH.FYZUPIXK_LoggingEnabled)
		{
			Print("[PackFazupix KOTH] Attempted to release rewards, but reward container not found! Unable to release reward.");
		}
	}
	
	void BatteryCheck(EntityAI tempEAI)
	{
		if(!tempEAI) return;
		
		if(tempEAI.HasEnergyManager()) tempEAI.GetInventory().CreateAttachment("Battery9V");
	}

	void DoFireworks()
	{
		FireworksLauncher fireWorks = FireworksLauncher.Cast(GetGame().CreateObjectEx("FireworksLauncher", FYZUPIXK_Position, ECE_PLACE_ON_SURFACE, RF_DEFAULT));
		
		EntityAI.Cast(fireWorks).OnIgnitedThis(NULL);
		
		fireWorks.SetSynchDirty();
	}
	
	int GetUpTime()
	{
		return FYZUPIXK_UpTime;
	}
	
	float GetCapturingTime()
	{
		return FYZUPIXK_CapturingTime;
	}
	
	int GetKOTHID()
	{
		return FYZUPIXK_ID;
	}
	
	bool IsInProgress()
	{
		return FYZUPIXK_InProgress;
	}
	
	bool IsFinished()
	{
		return FYZUPIXK_Finished;
	}
}
