/*
PackFazupix KOTH by Touchofdeath Version 1.0
*/

class FYZUPIX_KOTH
{
	bool FYZUPIXK_LoggingEnabled = false;
	bool FYZUPIXK_RanFirstCleanup = false;
	
	ref array<ref FYZUPIX_KOTHEvent> FYZUPIXK_ActiveKoths = new ref array<ref FYZUPIX_KOTHEvent>;

	ref array<EntityAI> FYZUPIXK_ContainersEAIArray = new array<EntityAI>;
	ref array<int> FYZUPIXK_ContainersLifetimeArray = new array<int>;

	void FYZUPIX_KOTH()
	{
		Print("[PackFazupix KOTH] Loaded!");
	}

	void ~FYZUPIX_KOTH()
	{
		this.FYZUPIXK_LoggingEnabled = false;
		this.FYZUPIXK_RanFirstCleanup = false;

		if(this.FYZUPIXK_ActiveKoths) this.FYZUPIXK_ActiveKoths.Clear();
		this.FYZUPIXK_ActiveKoths = NULL;
		if(this.FYZUPIXK_ContainersEAIArray) this.FYZUPIXK_ContainersEAIArray.Clear();
		this.FYZUPIXK_ContainersEAIArray = NULL;
		if(this.FYZUPIXK_ContainersLifetimeArray) this.FYZUPIXK_ContainersLifetimeArray.Clear();
		this.FYZUPIXK_ContainersLifetimeArray = NULL;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartKOTH);
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(UpdateContainersLifetime);
	}
	
	//FOR YOUR SCRIPTED REWARDS, MAKE A SERVER SIDE OVERRIDE FOR THIS FUNCTION
	void ReleaseScriptedReward(EntityAI rewardEAI)
	{
		//rewardEAI.YourScriptedRewardReleaseFunction( ... );
	}
	
	//Override this function if you want to alter the rewards to be spawned, announcement style ETC
	void StartNewKOTH(int KOTH_ID, int newKothArrayID, string ContainerClassname, bool IsScripted)
	{
		//These 7 variables, you can adjust if you like or if you want to further customize how the KOTHs are started
		string	KOTH_Name = GetFYZUPIXKConfig().KOTHs.Get(KOTH_ID).GetName();
		vector	KOTH_Position = GetFYZUPIXKConfig().KOTHs.Get(KOTH_ID).GetPosition();
		vector	KOTH_Orientation = GetFYZUPIXKConfig().KOTHs.Get(KOTH_ID).GetOrientation();
		int		KOTH_CaptureRadius = GetFYZUPIXKConfig().KOTHs.Get(KOTH_ID).GetCaptureRadius();
		int		KOTH_EnemyCount = GetFYZUPIXKConfig().KOTHs.Get(KOTH_ID).GetEnemyCount();
		int		KOTH_LootCount = GetFYZUPIXKConfig().KOTHs.Get(KOTH_ID).GetRewardLootCount();
		string	KOTH_ContainerClassname = ContainerClassname;
		bool	KOTH_ContainerIsScripted = IsScripted;
		
		FYZUPIXK_ActiveKoths.Insert(new FYZUPIX_KOTHEvent(KOTH_ID, KOTH_Name, KOTH_Position, KOTH_Orientation, KOTH_CaptureRadius, KOTH_EnemyCount, KOTH_LootCount, KOTH_ContainerClassname, KOTH_ContainerIsScripted));
		
		SendNotification("New King of The Hill '" + KOTH_Name + "' starting!");
	}

	void OnInit()
	{
		FYZUPIXK_RanFirstCleanup = false;
		
		if(GetFYZUPIXKConfig().Settings.Get(0).LoggingEnabled())
		{
			FYZUPIXK_LoggingEnabled = true;
			
			if(GetFYZUPIXKConfig().Settings.Get(0).DebugQuickmode())
			{
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Start_Delay = 60;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Starting_Failed_Retry_Delay = 15;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Time_Between = 120;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Time_Limit = 120;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Capture_Time = 30;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Spawn_Start_Delay = 15;
				GetFYZUPIXKConfig().Settings.Get(0).Cleanup_Time_After_KOTH_Finished = 60;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Simultaneous_Stagger_Time_Min = 30;
				GetFYZUPIXKConfig().Settings.Get(0).KOTH_Simultaneous_Stagger_Time_Max = 60;
				
				Print("[PackFazupix KOTH] DEBUG QUICK MODE ENABLED.");
			}
			
		}
		
		Print("[PackFazupix KOTH] Initialized!");
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Waiting for " + GetFYZUPIXKConfig().Settings.Get(0).GetStartDelay() + " seconds before running cleanup script and attempting to start first KOTH.");

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartKOTH, GetFYZUPIXKConfig().Settings.Get(0).GetStartDelay() * 1000, false); //Start a KOTH after starting delay
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(UpdateContainersLifetime, 30 * 1000, true); //Script to monitor reward containers lifetime
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(KOTHCleanup, 30 * 1000, false, -1); //Do first cleanup after 30secs
	}

	int FindKOTHArrayID(int KOTHID)
	{
		for(int i=0; i<FYZUPIXK_ActiveKoths.Count(); i++)
		{
			if(FYZUPIXK_ActiveKoths.Get(i).GetKOTHID() == KOTHID) return i;
		}
		
		return -1;
	}

	//Function to clean up KOTH rewards and objects on server startup and prior to starting a new KOTH
	void KOTHCleanup(int KOTHID = -1)
	{
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script called!");
		
		EntityAI close_object;

		ref array<Object> close_objects = new array<Object>;
		ref array<CargoBase> close_objects_cargo = new array<CargoBase>;

		if(!FYZUPIXK_RanFirstCleanup)
		{
			if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Performing server startup cleanup!");
		
			for(int i=0; i<GetFYZUPIXKConfig().KOTHs.Count(); i++)
			{
				GetGame().GetObjectsAtPosition(GetFYZUPIXKConfig().KOTHs.Get(i).GetPosition(), GetFYZUPIXKConfig().Settings.Get(0).GetCleanupRadius(), close_objects, close_objects_cargo);
							
				for(int j=0; j<close_objects.Count(); j++)
				{
					close_object = EntityAI.Cast(close_objects.Get(j));
					for(int k=0; k<GetFYZUPIXKConfig().Reward_Containers.Count(); k++)
					{
						if(close_object && (close_object.GetType() == GetFYZUPIXKConfig().Reward_Containers.Get(k).GetClassname()))
						{
							if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script has located a KOTH reward container! Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ").");
							close_object.Delete();
						}
					}
					if(close_object && (close_object.GetType() == "fyzupix_KOTHflag"))
					{
						if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script has located a previous KOTH flag! Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ").");
						close_object.Delete();
					}
				}
			}
			FYZUPIXK_RanFirstCleanup = true;
		}
			
		if(KOTHID != -1)
		{
			int KothArrayID = FindKOTHArrayID(KOTHID);
			
			if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script is now cleaning up static objects and enemies from previous KOTH.");

			if(FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_ObjectsArray.Count() > 0)
			{
				for(int n=0; n<FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_ObjectsArray.Count(); n++)
				{
					close_object = EntityAI.Cast(FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_ObjectsArray.Get(n));
						
					if(close_object)
					{
						if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script has located previous KOTH static object. Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ").");

						close_object.Delete();
					}
				}
			}
			else
			{
				if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script could not find any previous KOTH static object(s) for cleanup.");
			}

			if(FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_EnemyAIArray.Count() > 0)
			{
				for(int l=0; l<FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_EnemyAIArray.Count(); l++)
				{
					close_object = EntityAI.Cast(FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_EnemyAIArray.Get(l));
						
					if(close_object)
					{
						if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script has located previous KOTH enemy. Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ").");

						close_object.SetHealth(0.0);
						close_object.SetLifetime(15);
					}
				}
			}
			else
			{
				if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script could not find any previous KOTH enemies(s) for cleanup.");
			}
			
			GetGame().GetObjectsAtPosition(FYZUPIXK_ActiveKoths.Get(KothArrayID).GetPosition(), GetFYZUPIXKConfig().Settings.Get(0).GetCleanupRadius(), close_objects, close_objects_cargo);
							
			for(int m=0; m<close_objects.Count(); m++)
			{
				close_object = EntityAI.Cast(close_objects.Get(m));
				
				if(close_object)
				{
					if(close_object.GetType() == "fyzupix_KOTHflag")
					{
						if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script has located previous KOTH's flag! Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ").");
						close_object.Delete();
					}
				}
			}

			FYZUPIXK_ActiveKoths.Remove(KothArrayID);

			if(GetFYZUPIXKConfig().Settings.Get(0).GetMaxSimultaneous() > 1)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartKOTH, GetFYZUPIXKConfig().Settings.Get(0).GetRandomStaggerTime() * 1000, false); //Start a KOTH after random stagger delay
			}
			else
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartKOTH, GetFYZUPIXKConfig().Settings.Get(0).GetTimeBetweenKOTHs() * 1000, false); //Start a KOTH after cooldown period between KOTHs
			}
		}
		
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH cleanup script successful!");
	}
	
	void KOTHCleanupPreStart(int KothID)
	{
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH pre-start cleanup script called!");

		ref array<Object> close_objects = new array<Object>;
		ref array<CargoBase> close_objects_cargo = new array<CargoBase>;
		
		GetGame().GetObjectsAtPosition(GetFYZUPIXKConfig().KOTHs.Get(KothID).GetPosition(), GetFYZUPIXKConfig().Settings.Get(0).GetCleanupRadius(), close_objects, close_objects_cargo);
							
		for(int i=0; i<close_objects.Count(); i++)
		{
			EntityAI close_object = EntityAI.Cast(close_objects.Get(i));
				
			if(close_object)
			{
				if(GetFYZUPIXKConfig().Reward_Containers.Count() > 0)
				{
					for(int j=0; j<GetFYZUPIXKConfig().Reward_Containers.Count(); j++)
					{
						if(close_object.GetType() == GetFYZUPIXKConfig().Reward_Containers.Get(j).GetClassname())
						{
							if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH pre-start cleanup script has located previous KOTH's container! Deleting '" + close_object.GetType() + "' at position (" + close_object.GetPosition() + ").");
							close_object.Delete();
						}
					}
				}
			}
		}
	}

	void UpdateContainersLifetime()
	{
		if(FYZUPIXK_ContainersEAIArray.Count() > 0)
		{
			for(int i=0; i<FYZUPIXK_ContainersEAIArray.Count(); i++)
			{
				if(FYZUPIXK_ContainersEAIArray.Get(i))
				{
					int tempLifetime = FYZUPIXK_ContainersLifetimeArray.Get(i);
					
					tempLifetime -= 30;
					
					if(tempLifetime <= 0)
					{
						FYZUPIXK_ContainersEAIArray.Remove(i);
						FYZUPIXK_ContainersLifetimeArray.Remove(i);
					}
					else
					{
						FYZUPIXK_ContainersLifetimeArray[i] = tempLifetime;
						
						FYZUPIXK_ContainersEAIArray.Get(i).SetLifetime(tempLifetime);
					}
				}
			}
		}
	}

	//Function for starting a new KOTH
	void StartKOTH()
	{
		if(GetFYZUPIXKConfig().KOTHs.Count() == 0)
		{
			Print("[PackFazupix KOTH] NO KOTHS DEFINED! KOTHS SCRIPT WILL NOT PROCEED");
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(StartKOTH);
			
			return;
		}
		
		int ProposedNewKOTHID = Math.RandomInt(0,GetFYZUPIXKConfig().KOTHs.Count());
		
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Attempting to start new KOTH ID '" + ProposedNewKOTHID + "'.");

		if(!CanStartKOTH(ProposedNewKOTHID))
		{
			if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Failed to start new KOTH ID '" + ProposedNewKOTHID + "'.");
			
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartKOTH, GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() * 1000, false); //Retry starting AI KOTH

			return;
		}

		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Setting up default parameters for new KOTH ID '" + ProposedNewKOTHID + "'.");
		
		if((FYZUPIXK_ActiveKoths.Count() + 1) < GetFYZUPIXKConfig().Settings.Get(0).GetMaxSimultaneous())
		{
			if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] We are not at KOTH limit, starting another KOTH after delay.");

			if(GetFYZUPIXKConfig().Settings.Get(0).DebugQuickmode()) GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartKOTH, GetFYZUPIXKConfig().Settings.Get(0).GetTimeBetweenKOTHs() * 1000, false);
			else GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(StartKOTH, GetFYZUPIXKConfig().Settings.Get(0).GetRandomStaggerTime() * 1000, false); //Start a KOTH after random stagger delay
		}
		
		string chosenContainerClassname = "";
		
		if(GetFYZUPIXKConfig().KOTHs.Get(ProposedNewKOTHID).GetContainerClassnames().Count() > 0)
		{
			chosenContainerClassname = GetFYZUPIXKConfig().KOTHs.Get(ProposedNewKOTHID).GetContainerClassnames().GetRandomElement();
		}
		else
		{
			chosenContainerClassname = GetFYZUPIXKConfig().Reward_Containers.Get(Math.RandomInt(0, GetFYZUPIXKConfig().Reward_Containers.Count())).GetClassname();
		}

		bool isScriptedContainer = GetFYZUPIXKConfig().Reward_Containers.Get(FindContainerID(chosenContainerClassname)).IsScriptedReward();

		int newKothArrayID = FYZUPIXK_ActiveKoths.Count();
		KOTHCleanupPreStart(ProposedNewKOTHID);
		StartNewKOTH(ProposedNewKOTHID, newKothArrayID, chosenContainerClassname, isScriptedContainer);
		SpawnKOTHObjects(ProposedNewKOTHID, newKothArrayID);
		SpawnKOTHEnemies(ProposedNewKOTHID, newKothArrayID);
	}
	
	int FindContainerID(string className)
	{
		if(className == "") return -1;
		
		for(int i=0; i<GetFYZUPIXKConfig().Reward_Containers.Count(); i++)
		{
			if(GetFYZUPIXKConfig().Reward_Containers.Get(i).GetClassname() == className) return i;
		}
		
		return -1;
	}

	//Function to spawn static objects for the KOTH.
	void SpawnKOTHObjects(int proposedKOTHID, int KothArrayID)
	{
		int objCount = 0;
		
		if(GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Count() > 0)
		{
			if(GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Count() > 0)
			{
				if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawning " + GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Count() + " objects for KOTH ID '" + proposedKOTHID + "'");

				for(int i=0; i<GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Count(); i++)
				{
					Object tempObject;
					string classname = GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Get(i).GetClassname();
					vector position = GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Get(i).GetPosition();
					vector rotation = GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Get(i).GetOrientation();
					float scale = GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).Static_Objects.Get(i).GetScale();
					
					if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawning object '" + classname + "' at position (" + position + "), rotation (" + rotation + ") and scale (" + scale + ").");

					if(Class.CastTo(tempObject, GetGame().CreateObject(classname, position)))
					{
						tempObject.SetOrientation(rotation);
						tempObject.SetOrientation(tempObject.GetOrientation());
						tempObject.SetScale(scale);
						tempObject.Update();
						tempObject.SetAffectPathgraph(true, false);
						
						if(tempObject.CanAffectPathgraph())
						{
							GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, tempObject);
						}
						
						FYZUPIXK_ActiveKoths.Get(KothArrayID).FYZUPIXK_ObjectsArray.Insert(tempObject);
						
						objCount ++;
					}
				}
			}
			else
			{
				if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH ID '" + proposedKOTHID + "' has no objects.");
			}
		}
		
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawned " + objCount + " objects.");
	}

	//Function to check if it's safe to start a new proposed KOTH ID
	//We don't want them starting if a KOTH is already running, if not defined, if server FPS is too low, under min player count or on top of a player
	bool CanStartKOTH(int proposedKOTHID)
	{
		if(FYZUPIXK_ActiveKoths.Count() >= GetFYZUPIXKConfig().Settings.Get(0).GetMaxSimultaneous())
		{
			Print("[PackFazupix KOTH] Unable to start new KOTH as we are already at maximum simultaneous KOTH limit. Retrying in " + GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");
			return false;
		}

		if(FYZUPIXK_ActiveKoths.Count() > 0)
		{
			foreach(FYZUPIX_KOTHEvent kothInstance: FYZUPIXK_ActiveKoths)
			{
				if(proposedKOTHID == kothInstance.GetKOTHID())
				{
					Print("[PackFazupix KOTH] Chosen KOTH ID (" + proposedKOTHID + ") is already running. Retrying in " + GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");
					return false;
				}
			}
		}
		
		if(GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).GetName() == "")
		{
			Print("[PackFazupix KOTH] KOTH name undefined, unable to start KOTH. Retrying in " + GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");
			return false;
		}

		if(GetGame().GetFps() > 2) // Server FPS too low
		{
			Print("[PackFazupix KOTH] Unable to start new KOTH as server FPS is too low. Retrying in " + GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");
			return false;
		}
		
		ref array<Man> all_players = new array<Man>;
		GetGame().GetPlayers(all_players);

		if(all_players.Count() < GetFYZUPIXKConfig().Settings.Get(0).GetMinPlayers())
		{
			Print("[PackFazupix KOTH] Not enough players to start KOTH. Retrying in " + GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");
			return false;
		}

		for(int i=0; i<all_players.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(all_players.Get(i));
			if(player)
			{
				if(vector.Distance(GetFYZUPIXKConfig().KOTHs.Get(proposedKOTHID).GetPosition(), player.GetPosition()) <= (GetFYZUPIXKConfig().Settings.Get(0).GetNoSpawnRange()))
				{
					Print("[PackFazupix KOTH] Player in spawn area, unable to start KOTH. Retrying in " + GetFYZUPIXKConfig().Settings.Get(0).GetFailStartDelay() + " seconds.");
					return false;
				}
			}
		}
		
		return true;
	}

	//Function to spawn the KOTH enemies
	void SpawnKOTHEnemies(int kothID, int kothArrayID)
	{
		int enemyCount = GetFYZUPIXKConfig().KOTHs.Get(kothID).GetEnemyCount();
		if(enemyCount == 0)
		{
			if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] KOTH ID '" + kothID + "' is configured to spawn no enemies.");
			return;
		}
		
		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawning in " + enemyCount + " KOTH Enemies");
		
		int failSafe = enemyCount * 2;
		int currentEnemies = 0;
		int currentAttempts = 0;
		
		vector position = GetFYZUPIXKConfig().KOTHs.Get(kothID).GetPosition();
		vector botSpawnPos = position;

		while(currentEnemies < enemyCount)
		{
			int chosenEnemyID = Math.RandomInt(0, GetFYZUPIXKConfig().Settings.Get(0).KOTH_Enemies.Count());
			
			if(Math.RandomIntInclusive(0,99) >= GetFYZUPIXKConfig().Settings.Get(0).KOTH_Enemies.Get(chosenEnemyID).GetRarity())
			{
				botSpawnPos = Vector(position[0] + Math.RandomInt(-5, 5), position[1], position[2] + Math.RandomInt(-5, 5));
				EntityAI newEnemy = EntityAI.Cast(GetGame().CreateObject(GetFYZUPIXKConfig().Settings.Get(0).KOTH_Enemies.Get(chosenEnemyID).GetClassname(), botSpawnPos, false, true));
				
				if(newEnemy)
				{
					newEnemy.SetOrientation(Vector(Math.RandomInt(0,360),0.0,0.0));
					FYZUPIXK_ActiveKoths.Get(kothArrayID).FYZUPIXK_EnemyAIArray.Insert(newEnemy);
					currentEnemies += 1;
				}
			}

			currentAttempts += 1;
			if(currentAttempts >= failSafe) currentEnemies = enemyCount;
		}

		if(FYZUPIXK_LoggingEnabled) Print("[PackFazupix KOTH] Spawned " + currentEnemies + " in " + currentAttempts + " attempts.");
	}
	
	static void SendNotification(string textInput)
	{
		if(!GetFYZUPIXKConfig().Settings.Get(0).NotificationsEnabled()) return;

		#ifdef EXPANSIONMOD
		ExpansionNotification("King of The Hill", textInput, "Territory", COLOR_EXPANSION_NOTIFICATION_MISSION, 15).Create();
		#endif

		//Vanilla NotificationSystem path. The "Notifications" Steam Workshop mod
		//(DaemonForge, id=2353998362) overrides this UI, so players running that
		//mod get the prettier toast automatically. Fires alongside Expansion so
		//we reach everyone regardless of which mod they have loaded.
		ref array<Man> all_players = new array<Man>;
		GetGame().GetPlayers(all_players);
		if(all_players.Count() > 0)
		{
			for(int i=0; i<all_players.Count(); i++)
			{
				PlayerBase player = PlayerBase.Cast(all_players.Get(i));

				if(player && player.IsAlive())
				{
					NotificationSystem.SendNotificationToPlayerExtended(player, 15, "King of The Hill", textInput, "set:dayz_inventory image:tf_flag");
				}
			}
		}
	}
}
