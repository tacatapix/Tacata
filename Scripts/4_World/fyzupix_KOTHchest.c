class fyzupix_KOTHchest_Base extends Container_Base
{
	override bool CanPutInCargo(EntityAI parent)
	{
		return false; //no
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false; //no
	}
}
class fyzupix_KOTHchest_SupplyBox1 extends fyzupix_KOTHchest_Base {};
class fyzupix_KOTHchest_SupplyBox2 extends fyzupix_KOTHchest_Base {};
class fyzupix_KOTHchest_SupplyBox3 extends fyzupix_KOTHchest_Base {};
