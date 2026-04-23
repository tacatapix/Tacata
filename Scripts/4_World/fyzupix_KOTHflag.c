class fyzupix_KOTHflag extends BaseBuildingBase
{
	protected float m_RaisedAmount;
	protected float m_RaisedAmountLocal;

	//State = 0 Not Started, State = 1 In Progress, State = 2 Win, State = 3 Fail
	protected int m_KOTHSmokeState;
	protected int m_KOTHSmokeStateLocal;

	protected Particle m_KOTHSmoke;

	int SMOKE_EMPTY = ParticleList.GRENADE_M18_YELLOW_LOOP;
	int SMOKE_ACTIVE = ParticleList.GRENADE_M18_WHITE_LOOP;
	int SMOKE_FAIL = ParticleList.GRENADE_M18_RED_LOOP;
	int SMOKE_WIN = ParticleList.GRENADE_M18_GREEN_LOOP;

	void fyzupix_KOTHflag()
	{
		m_RaisedAmount = 0;
		m_RaisedAmountLocal = m_RaisedAmount;
		
		m_KOTHSmokeState = -1;
		m_KOTHSmokeStateLocal = m_KOTHSmokeState;
		
		RegisterNetSyncVariableInt("m_KOTHSmokeState");
		RegisterNetSyncVariableFloat("m_RaisedAmount");

		AddProxyPhysics("Base");
		AddProxyPhysics("Support");
		AddProxyPhysics("Pole");

		UpdateNavmesh();
	}

	void ~fyzupix_KOTHflag()
	{
	}
	
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if(!GetGame().IsServer())
		{
			if(m_KOTHSmoke) m_KOTHSmoke.Stop();
		}
	}
	
	void UpdateSmokeState(int newColourState)
	{
		if(newColourState != m_KOTHSmokeState)
		{
			m_KOTHSmokeState = newColourState;
			
			SetSynchDirty();
		}
	}

	void UpdateSmokeColour()
	{
		if(m_KOTHSmoke) m_KOTHSmoke.Stop();
		
		if(m_KOTHSmokeStateLocal != -1) m_KOTHSmoke = ParticleManager.GetInstance().PlayOnObject(GetCurrentSmokeParticleID(), this, vector.Zero, vector.Zero, true);
	}

	override void OnVariablesSynchronized()
	{
		if(m_RaisedAmountLocal != m_RaisedAmount)
		{
			m_RaisedAmountLocal = m_RaisedAmount;
		}

		if(m_KOTHSmokeStateLocal != m_KOTHSmokeState)
		{
			m_KOTHSmokeStateLocal = m_KOTHSmokeState;

			UpdateSmokeColour();
		}

		UpdateVisuals();
	}

    void OnSpawnKOTHFlag(string FlagClassName) 
	{
		UpdateVisuals();
		
		UpdateNavmesh();

		GetInventory().CreateAttachment(FlagClassName);
		
		if(m_KOTHSmokeStateLocal != -1) m_KOTHSmoke = ParticleManager.GetInstance().PlayOnObject(GetCurrentSmokeParticleID(), this, vector.Zero, vector.Zero, true);
    }
	
	int GetCurrentSmokeParticleID()
	{
		if(m_KOTHSmokeStateLocal == 1) return SMOKE_ACTIVE;
		else if(m_KOTHSmokeStateLocal == 2) return SMOKE_FAIL;
		else if(m_KOTHSmokeStateLocal == 3) return SMOKE_WIN;
		return SMOKE_EMPTY;
	}

	void AnimateKOTHFlag(float setHeight)
	{
		if(setHeight < 0.0) setHeight = 0.0;
		else if(setHeight > 1.0) setHeight = 1.0;

		SetAnimationPhase("flag_mast", 1 - setHeight);
		
		m_RaisedAmount = setHeight;

		if(GetGame().IsServer()) SetSynchDirty();
	}
	
	override void SetPartsAfterStoreLoad()
	{
		//nada
	}

	override void UpdateVisuals()
	{
		if(m_RaisedAmountLocal < 0.0) m_RaisedAmountLocal = 0.0;
		else if(m_RaisedAmountLocal > 1.0) m_RaisedAmountLocal = 1.0;
		
		SetAnimationPhase("flag_mast", 1 - m_RaisedAmountLocal);
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;	
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		return false;
	}
}