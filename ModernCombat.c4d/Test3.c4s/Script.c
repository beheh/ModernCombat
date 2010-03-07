/*-- CMC Test --*/

#strict
#include CSTD

func ChooserRuleConfig()
{
  return [FDMG,NOFF,NOAM];
}

func Initialize()
{
  DrawCircle("BackWall-Concrete2",1450,320,70,70,90,180);
  DrawCircle("BackWall-Concrete2",880,390,40,40,180,270,10,10);
  DrawCircle("BackWall-Concrete2",1000,390,40,40,90,180,10,10);
  DrawRamp("Wall-Concrete2",1260,390,50,-30);
  DrawRamp("Wall-Concrete2",1150,390,-50,-30);
}

public func SetUpStore(pStore)
{
  pStore->AddWares("IsWeapon", -1);
  pStore->AddWares("IsAmmoPacket", -1);
  pStore->AddWares("IsUpgrade", -1);
  pStore->AddWares("IsEquipment", -1);
  pStore->SortWare("IsWeapon","IsAmmoPacket","IsUpgrade","IsEquipment");
}